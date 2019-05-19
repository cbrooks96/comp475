#include <stdlib.h>
#include "GFilter.h"
#include "GLayer.h"
#include "GMatrix.h"
#include "GPaint.h"
#include "GPixel.h"
#include "GRect.h"
#include "Blend.h"

GLayer::GLayer(const GBitmap* bitmap, GMatrix mat, GIRect bound) {
    this->fBitmap = *bitmap;
    this->fCTM = mat;
    this->fBounds = bound;
    this->fIsLayer = false;
}

GLayer::GLayer(GBitmap& bitmap, GMatrix mat, GIRect bound, GPaint paint) {
    this->fBitmap = bitmap;
    this->fCTM = mat;
    this->fBounds = bound;
    this->fPaint = paint;
    this->fIsLayer = true;
}

void GLayer::draw(GBitmap* base) {
    GIRect bound = this->fBounds;
    int x_off = bound.left();
    int y_off = bound.top();

    ModeProc blend = blendProc(this->fPaint.getBlendMode());
    GFilter* filter = this->fPaint.getFilter();

    for (int y = 0; y < this->fBitmap.height(); ++y) {
        int w = this->fBitmap.width();
        GPixel* line = (GPixel*) malloc(w * sizeof(GPixel));

        if (filter == nullptr) {
            line = this->fBitmap.getAddr(0, y);
        } else {
            filter->filter(line, this->fBitmap.getAddr(0, y), w);
        }

        for (int x = 0; x < w; ++x) {
            GPixel src = line[x];
            GPixel* dest = base->getAddr(x + x_off, y + y_off);

            *dest = blend(src, *dest);
        }
    }
}
