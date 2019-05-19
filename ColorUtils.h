#ifndef ColorUtils_DEFINED
#define ColorUtils_DEFINED
#include "GColor.h"

static inline int floatToPixelValue(float value) {
    GASSERT(0 <= value && value <= 1);

    return GRoundToInt(value * 255);
}

static inline GPixel colorToPixel(const GColor& color) {
    int a = floatToPixelValue(color.fA);
    int r   = floatToPixelValue(color.fR * color.fA);
    int g = floatToPixelValue(color.fG * color.fA);
    int b  = floatToPixelValue(color.fB * color.fA);

    return GPixel_PackARGB(a, r, g, b);
}

#endif
