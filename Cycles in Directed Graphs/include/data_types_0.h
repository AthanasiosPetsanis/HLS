#include "ac_int.h"

static const short max_size = 1023; // Scallability

typedef ac_int<max_size, false> matrix;
typedef ac_int< ac::log2_ceil<max_size+1>::val, false> node_bits;
