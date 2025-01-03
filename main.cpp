#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include "lowpasscoefs.cpp"
#include "FIRFilter.cpp"

//just reminders for goals: 8 bit -> 4th factor reduction, 16 bit -> 2nd factor reduction, 12 bit -> 4th factor reduction, 
// 4 bit -> 8th factor reduction, 2 bit -> 16th factor reduction

//helper function to quantize a sample to any-bit precision
short quantize_to_bit(float sample, int bit_depth) {
    int max_value = (1 << (bit_depth - 1)) - 1; // max positive value for the bit depth, 2^(bit_depth-1) - 1
    int quantized = (int)(sample * max_value);  // map to range [-max_value, max_value]
    if (quantized > max_value) quantized = max_value;
    if (quantized < -max_value - 1) quantized = -max_value - 1;
    return (short)(quantized * (32767 / max_value)); // scale back to 16-bit range
}

//helper function to process audio in chunks for different factor reductions
SNDFILE* process_audio_chunks(SNDFILE *sndfile_in, SNDFILE *sndfile_out, float *input_buffer, short *output_buffer,
                              size_t chunk_frames, int sample_rate_reduction_factor, int bit_depth, int input_channels,
                              FilterInput& filterInput, std::vector<float>& coefficients) {
    
    filterInput.c = coefficients.data();
    filterInput.filterLength = coefficients.size();

    // Allocate buffers
    float* filtered_buffer = (float*)malloc(chunk_frames * input_channels * sizeof(float));
    float* history_buffer = (float*)calloc((filterInput.filterLength - 1) * input_channels, sizeof(float));
    if (!filtered_buffer || !history_buffer) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(filtered_buffer);
        free(history_buffer);
        return NULL;
    }

    sf_count_t frames_read;

    while ((frames_read = sf_readf_float(sndfile_in, input_buffer, chunk_frames)) > 0) {
        sf_count_t output_frames = 0;

        //apply the FIR filter
        for (int ch = 0; ch < input_channels; ++ch) {
            float* channel_input = (float*)malloc((frames_read + filterInput.filterLength - 1) * sizeof(float));
            if (!channel_input) {
                fprintf(stderr, "Memory allocation failed.\n");
                free(filtered_buffer);
                free(history_buffer);
                return NULL;
            }

            //combine history and current chunk
            memcpy(channel_input, history_buffer + ch * (filterInput.filterLength - 1), (filterInput.filterLength - 1) * sizeof(float));
            for (sf_count_t i = 0; i < frames_read; ++i) {
                channel_input[i + (filterInput.filterLength - 1)] = input_buffer[i * input_channels + ch];
            }

            // FIR filter
            filterInput.x = channel_input;
            filterInput.inputLength = frames_read + filterInput.filterLength - 1;
            filterInput.y = filtered_buffer + ch * chunk_frames;
            filterInput.outputLength = frames_read;

            firFilter(filterInput);

            // Update history buffer
            memcpy(history_buffer + ch * (filterInput.filterLength - 1), channel_input + frames_read, (filterInput.filterLength - 1) * sizeof(float));
            free(channel_input);
        }

        // Quantize, dither, and downsample!!
        for (sf_count_t i = 0; i < frames_read; i += sample_rate_reduction_factor) {
            for (int ch = 0; ch < input_channels; ++ch) {
                float sample = filtered_buffer[i * input_channels + ch];
                float dither = ((float)rand() / RAND_MAX) - 0.5f;
                sample += dither / (1 << (bit_depth - 1));
                output_buffer[output_frames * input_channels + ch] = quantize_to_bit(sample, bit_depth);
            }
            output_frames++;
        }

        // Write processed data!
        sf_writef_short(sndfile_out, output_buffer, output_frames);
    }

    free(filtered_buffer);
    free(history_buffer);
    return sndfile_out;
}




int main() {
    int bit_depth = 6; // Quantization bit depth
    int sample_rate_reduction_factor = 4; //Partner for 8-bit selection


    SF_INFO sfinfo_in;
    SNDFILE *sndfile_in = sf_open("test.wav", SFM_READ, &sfinfo_in);
    if (!sndfile_in) {
        fprintf(stderr, "Error opening input file: %s\n", sf_strerror(NULL));
        return 1;
    }

    //calculate low pass filter coefficients
    float originalSampleRate = sfinfo_in.samplerate;
    float newSampleRate = originalSampleRate / sample_rate_reduction_factor;
    float cutoffFrequency = (newSampleRate / 2) / originalSampleRate; // Nyquist frequency
    size_t filterLength = 51; // Number of coefficients (odd for symmetry)
    std::vector<float> coefficients = calculateLowPassCoefficients(cutoffFrequency, filterLength);

    // Configure output file
    SF_INFO sfinfo_out = sfinfo_in;
    sfinfo_out.samplerate /= sample_rate_reduction_factor; //Fixes sample rate and duration for the output file
    SNDFILE *sndfile_out = sf_open("output.wav", SFM_WRITE, &sfinfo_out);
    if (!sndfile_out) {
        fprintf(stderr, "Error opening output file: %s\n", sf_strerror(NULL));
        sf_close(sndfile_in);
        return 1;
    }

    size_t chunk_frames = 1024; // Number of frames to process per chunk
    size_t input_buffer_size = chunk_frames * sfinfo_in.channels;
    size_t output_buffer_size = input_buffer_size / sample_rate_reduction_factor;
    float *input_buffer = (float *)malloc(input_buffer_size * sizeof(float));
    short *output_buffer = (short *)malloc(output_buffer_size * sizeof(short));

    if (!input_buffer || !output_buffer) {
        fprintf(stderr, "Memory allocation failed.\n");
        sf_close(sndfile_in);
        sf_close(sndfile_out);
        free(input_buffer);
        free(output_buffer);
        return 1;
    }

    // Create a FIR filter input struct
    FilterInput filterInput;
    

    // Process audio using our chunks 
    sndfile_out = process_audio_chunks(sndfile_in, sndfile_out, input_buffer, output_buffer, chunk_frames, sample_rate_reduction_factor, bit_depth, sfinfo_in.channels, filterInput, coefficients);
    
    // Cleanup
    sf_close(sndfile_in);
    sf_close(sndfile_out);
    free(input_buffer);
    free(output_buffer);

    printf("Bitcrushing and sample rate reduction completed.\n");
    return 0;
}

//potential features: filtering
//ui for user to select bit depth and sample rate reduction factor