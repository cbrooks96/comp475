#include "GBitmap.h"
#include "GMatrix.h"
#include "GShader.h"

class BitmapShader : public GShader {
public:
    BitmapShader(const GBitmap& newBitmap, const GMatrix& localInv, GShader::TileMode newTile)
        : fSourceBitmap(newBitmap)
        , fLocalMatrix(localInv)
        , fTile(newTile) {}

    bool setContext(const GMatrix& ctm) override {
        if (!ctm.invert(&fInverse)) {
            return false;
        }

        fInverse.postConcat(fLocalMatrix);

        return true;
    }

    bool isOpaque() override {
        return false;
    }

    void shadeRow(int x, int y, int count, GPixel row[]) override {
        GPoint pt = fInverse.mapXY(x + 0.5f, y + 0.5f);

        for (int i = 0; i < count; ++i) {
            int start_x = GRoundToInt(pt.fX);
            int start_y = GRoundToInt(pt.fY);

            if (fTile == TileMode::kRepeat) {
                start_x %= fSourceBitmap.width();
                if (start_x < 0) {
                    start_x += fSourceBitmap.width();
                }

                start_y %= fSourceBitmap.height();
                if (start_y < 0) {
                    start_y += fSourceBitmap.height();
                }
            } else if (fTile == TileMode::kMirror) {
                float new_x = pt.fX / fSourceBitmap.width();
                float new_y = pt.fY / fSourceBitmap.height();

                new_x *= .5;
                new_x = new_x - floor(new_x);
                if (new_x > .5) {
                    new_x = 1 - new_x;
                }
                new_x *= 2;

                new_y *= .5;
                new_y = new_y - floor(new_y);
                if (new_y > .5) {
                    new_y = 1 - new_y;
                }
                new_y *= 2;

                start_x = GRoundToInt(new_x * fSourceBitmap.width());
                start_y = GRoundToInt(new_y * fSourceBitmap.height());
            }

            start_x = std::max(0, std::min(fSourceBitmap.width() - 1, start_x));
            start_y = std::max(0, std::min(fSourceBitmap.height() - 1, start_y));

            row[i] = *fSourceBitmap.getAddr(start_x, start_y);

            pt.fX += fInverse[GMatrix::SX];
            pt.fY += fInverse[GMatrix::KY];
        }
    }

private:
    GBitmap fSourceBitmap;
    GMatrix fInverse;
    GMatrix fLocalMatrix;
    TileMode fTile;
};

std::unique_ptr<GShader> GCreateBitmapShader(const GBitmap& newBitmap, const GMatrix& localInv, GShader::TileMode newTile) {
    if (!newBitmap.pixels()) {
        return nullptr;
    }
    return std::unique_ptr<GShader>(new BitmapShader(newBitmap, localInv, newTile));
}