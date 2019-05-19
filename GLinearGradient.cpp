#include <math.h>
#include "GColor.h"
#include "GMatrix.h"
#include "GPixel.h"
#include "GPoint.h"
#include "GShader.h"
#include "ColorUtils.h"
#include "MathUtils.h"

class GLinearGradient : public GShader {
public:
    GLinearGradient(GPoint a, GPoint b, const GColor colors[], int count, TileMode tile) {
        fColors = (GColor*) malloc(count * sizeof(GColor));
        memcpy(fColors, colors, count * sizeof(GColor));
        fColorCount = count;
        fTile = tile;

        if (a.fX > b.fX) {
            std::swap(a, b);
        }

        float dx = b.fX - a.fX;
        float dy = b.fY - a.fY;
        fUnitMatrix.set6(dx, -dy, a.fX, dy, dx, a.fY);
    }

    ~GLinearGradient() {
        free(fColors);
    }

    bool isOpaque() override {
        return false;
    }

    bool setContext(const GMatrix& ctm) override {
        fLocalMatrix.setConcat(ctm, fUnitMatrix);

        return fLocalMatrix.invert(&fLocalMatrix);
    }

    void shadeRow(int x, int y, int count, GPixel row[]) override {
        for (int i = 0; i < count; ++i) {
            GPoint pt = fLocalMatrix.mapXY(x + i, y);

            float t = pt.fX;

            if (fTile == TileMode::kRepeat) {
                t = t - floor(t);
            } else if (fTile == TileMode::kMirror) {
                t *= 0.5;
                t = t - floor(t);
                if (t > .5) {
                    t = 1 - t;
                }
                t *= 2;
            }

            t = clamp(t, 0.0f, 1.0f);

            if (t == 0) {
                row[i] = colorToPixel(fColors[0].pinToUnit());
            } else if (t == 1) {
                row[i] = colorToPixel(fColors[fColorCount - 1].pinToUnit());
            } else {
                int index = floor(t * (fColorCount - 1));
                float span = 1.0f / (fColorCount - 1);
                float start = index * span;

                GColor col0 = fColors[index].pinToUnit();
                GColor col1 = fColors[index + 1].pinToUnit();

                t = clamp((t - start) / span, 0.0f, 1.0f);

                GColor color = GColor::MakeARGB(
                    col0.fA * (1 - t) + col1.fA * t,
                    col0.fR * (1 - t) + col1.fR * t,
                    col0.fG * (1 - t) + col1.fG * t,
                    col0.fB * (1 - t) + col1.fB * t);

                row[i] = colorToPixel(color);
            }
        }
    }

private:
    GColor* fColors;
    GMatrix fInverse;
    GMatrix fLocalMatrix;
    GMatrix fUnitMatrix;
    TileMode fTile;
    int fColorCount;
};

std::unique_ptr<GShader> GCreateLinearGradient(
        GPoint a,
        GPoint b,
        const GColor colors[],
        int count,
        GShader::TileMode tile) {
    if (count < 1) {
        return nullptr;
    }

    return std::unique_ptr<GShader>(new GLinearGradient(a, b, colors, count, tile));
}
