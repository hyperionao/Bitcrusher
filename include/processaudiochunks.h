#ifndef PROCESSAUDIOCHUNKS_H
#define PROCESSAUDIOCHUNKS_H

#include <sndfile.h>
#include <vector>
#include "firfilter.h"

SNDFILE* process_audio_chunks(SNDFILE* sndfile_in, SNDFILE* sndfile_out, float* input_buffer, short* output_buffer,
                              size_t chunk_frames, int sample_rate_reduction_factor, int bit_depth, int input_channels,
                              FilterInput& filterInput, std::vector<float>& coefficients);

#endif // PROCESSAUDIOCHUNKS_H
