#include <quantizer.h>

short quantize_to_bit(float sample, int bit_depth) {
    int max_value = (1 << (bit_depth - 1)) - 1; // max positive value for the bit depth, 2^(bit_depth-1) - 1
    int quantized = (int)(sample * max_value);  // map to range [-max_value, max_value]
    if (quantized > max_value) quantized = max_value;
    if (quantized < -max_value - 1) quantized = -max_value - 1;
    return (short)(quantized * (32767 / max_value)); // scale back to 16-bit range
}