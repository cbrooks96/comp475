#ifndef GLayer_DEFINED
#define GLayer_DEFINED

#include "GBitmap.h"
#include "GMatrix.h"
#include "GPaint.h"
#include "GRect.h"

class GLayer {
public:

    GLayer(const GBitmap* bitmap, GMatrix mat, GIRect bound);

    GLayer(GBitmap& bitmap, GMatrix mat, GIRect bound, GPaint paint);

    void draw(GBitmap* base);

    bool isLayer() { return fIsLayer; }
    GBitmap& getBitmap() { return fBitmap; }
    GIRect getBounds() { return fBounds; }
    GMatrix& getCTM() { return fCTM; }

private:
    bool fIsLayer = false;
    GBitmap fBitmap;
    GIRect fBounds;
    GMatrix fCTM;
    GPaint fPaint;
};

#endif
