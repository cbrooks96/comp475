#ifndef GBlitter_DEFINED
#define GBlitter_DEFINED

#include "GBitmap.h"
#include "GPaint.h"

class GBlitter {
public:

    GBlitter(const GBitmap& bitmap, const GPaint& paint)
        : fBitmap(bitmap)
        , fPaint(paint) {}

    void blitRow(int y, int left_x, int right_x);

private:
    const GBitmap fBitmap;
    const GPaint fPaint;
};

#endif
