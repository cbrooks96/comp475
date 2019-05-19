#ifndef MathUtils_DEFINED
#define MathUtils_DEFINED

#include <math.h>

static inline float clamp(float val, float min, float max) {
    return std::max(min, std::min(max, val));
}

static inline float mult_max(float* nums, int count) {
    GASSERT(count >= 1);

    float max = nums[0];
    for (int i = 1; i < count; ++i) {
        max = std::max(max, nums[i]);
    }

    return max;
}

static inline float mult_min(float* nums, int count) {
    GASSERT(count >= 1);

    float min = nums[0];
    for (int i = 1; i < count; ++i) {
        min = std::min(min, nums[i]);
    }

    return min;
}

#endif
