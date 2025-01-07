#include "processaudiochunks.h"
#include "quantizer.h"
#include "FIRFilter.h"
#include <cstdlib>    
#include <cstring>    
#include <stdexcept>  

SNDFILE* process_audio_chunks(SNDFILE* sndfile_in, SNDFILE* sndfile_out, float* input_buffer, short* output_buffer,
                              size_t chunk_frames, int sample_rate_reduction_factor, int bit_depth, int input_channels,
                              FilterInput& filterInput, std::vector<float>& coefficients) {
    
    filterInput.c = coefficients.data();
    filterInput.filterLength = coefficients.size();

    // Allocate buffers
    float* filtered_buffer = new float[chunk_frames * input_channels];
    float* history_buffer = new float[(filterInput.filterLength - 1) * input_channels]();

    if (!filtered_buffer || !history_buffer) {
        throw std::runtime_error("Memory allocation failed for audio buffers.");
    }

    sf_count_t frames_read;

    while ((frames_read = sf_readf_float(sndfile_in, input_buffer, chunk_frames)) > 0) {
        sf_count_t output_frames = 0;

        // Apply FIR filter per channel
        for (int ch = 0; ch < input_channels; ++ch) {
            float* channel_input = new float[frames_read + filterInput.filterLength - 1];
            if (!channel_input) {
                delete[] filtered_buffer;
                delete[] history_buffer;
                throw std::runtime_error("Memory allocation failed for channel input buffer.");
            }

            memcpy(channel_input, history_buffer + ch * (filterInput.filterLength - 1), 
                   (filterInput.filterLength - 1) * sizeof(float));
            
            for (sf_count_t i = 0; i < frames_read; ++i) {
                channel_input[i + (filterInput.filterLength - 1)] = input_buffer[i * input_channels + ch];
            }

            // Apply FIR filter
            filterInput.x = channel_input;
            filterInput.inputLength = frames_read + filterInput.filterLength - 1;
            filterInput.y = filtered_buffer + ch * chunk_frames;
            filterInput.outputLength = frames_read;

            firFilter(filterInput);

            // Update history buffer
            memcpy(history_buffer + ch * (filterInput.filterLength - 1), 
                   channel_input + frames_read, (filterInput.filterLength - 1) * sizeof(float));

            delete[] channel_input;
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

        sf_writef_short(sndfile_out, output_buffer, output_frames);
    }

    delete[] filtered_buffer;
    delete[] history_buffer;
    return sndfile_out;
}
