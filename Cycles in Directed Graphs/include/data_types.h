#include "ac_int.h"

static const short size = 4; // Scallability

typedef ac_int<size, false> matrix;
typedef ac_int< ac::log2_ceil<size+1>::val, false> node_bits;
