#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

int main() {
    SF_INFO sfinfo;
    SNDFILE *sndfile = sf_open("test.wav", SFM_READ, &sfinfo);
    if (sndfile == nullptr) {
        std::cerr << "Error opening file: " << sf_strerror(nullptr) << std::endl;
        return 1;
    }

    float *buffer = (float *)malloc(sfinfo.frames * sfinfo.channels * sizeof(float));
    if (!buffer) {
        printf("Malloc failed.\n");
        sf_close(sndfile);
        return 1;
    }

    sf_count_t items_read = sf_read_float(sndfile, buffer, sfinfo.frames * sfinfo.channels);
    printf("Read %lld items\n", items_read);

    std::cout << "Sample rate: " << sfinfo.samplerate << "\nChannels: " << sfinfo.channels
              << "\nFormat: " << sfinfo.format << std::endl;

    SF_INFO sfinfo_out = sfinfo;
    sfinfo_out.channels = 2; 
    sfinfo_out.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE *sndfile_out = sf_open("output.wav", SFM_WRITE, &sfinfo_out);
    if (sndfile_out == nullptr) {
        std::cerr << "Error opening file: " << sf_strerror(nullptr) << std::endl;
        free(buffer);
        return 1;
    }

    short *buffer_out = (short *)malloc(sfinfo.frames * sfinfo_out.channels * sizeof(short));
    if (!buffer_out) {
        printf("Malloc failed.\n");
        sf_close(sndfile);
        sf_close(sndfile_out);
        free(buffer);
        return 1;
    }

    for (sf_count_t i = 0; i < sfinfo.frames; ++i) {
        for (int ch = 0; ch < sfinfo_out.channels; ++ch) {
            float sample = buffer[i * sfinfo.channels + ch];
            if (sample > 1.0f) sample = 1.0f;  
            if (sample < -1.0f) sample = -1.0f; 
            buffer_out[i * sfinfo_out.channels + ch] = (short)(sample * 32767.0f);
        }
    }

    sf_count_t items_written = sf_write_short(sndfile_out, buffer_out, sfinfo.frames * sfinfo_out.channels);
    printf("Written %lld items\n", items_written);

    sf_close(sndfile);
    sf_close(sndfile_out);
    free(buffer);
    free(buffer_out);
    return 0;
}
