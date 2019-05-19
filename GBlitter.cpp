#include "GBlitter.h"
#include "GFilter.h"
#include "GPaint.h"
#include "GPixel.h"
#include "GShader.h"
#include "Blend.h"
#include "ColorUtils.h"

void GBlitter::blitRow(int y, int left_x, int right_x) {
    GASSERT(left_x <= right_x);
    left_x = std::max(0, left_x);
    right_x = std::min(this->fBitmap.width(), right_x);

    ModeProc blend = blendProc(this->fPaint.getBlendMode());

    GShader* shade = this->fPaint.getShader();
    if (shade == nullptr) {
        GColor color = this->fPaint.getColor().pinToUnit();
        GPixel source[1] = {colorToPixel(color)};

        if (this->fPaint.getFilter() != nullptr) {
            this->fPaint.getFilter()->filter(source, source, 1);
        }

        for (int x = left_x; x < right_x; ++x) {
            GPixel* addr = this->fBitmap.getAddr(x, y);
            *addr = blend(source[0], *addr);
        }
    } else {
        int count = right_x - left_x;
        GPixel* shading = (GPixel*) malloc(count * sizeof(GPixel));
        shade->shadeRow(left_x, y, count, shading);

        if (this->fPaint.getFilter() != nullptr) {
            this->fPaint.getFilter()->filter(shading, shading, count);
        }

        for (int x = left_x; x < right_x; ++x) {
            GPixel* addr = this->fBitmap.getAddr(x, y);
            *addr = blend(shading[x - left_x], *addr);
        }

        free(shading);
    }
}
