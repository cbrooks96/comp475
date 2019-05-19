#include <stack>
#include "GBitmap.h"
#include "GBlitter.h"
#include "GCanvas.h"
#include "GColor.h"
#include "GFilter.h"
#include "GLayer.h"
#include "GMatrix.h"
#include "GPath.h"
#include "GPixel.h"
#include "GPoint.h"
#include "GRect.h"
#include "GScanConverter.h"
#include "GShader.h"
#include "Blend.h"
#include "Clipper.h"
#include "ColorUtils.h"
#include "MathUtils.h"

class MyCanvas : public GCanvas {
public:
    MyCanvas(const GBitmap& device) {
        GMatrix identity;
        identity.setIdentity();

        GIRect bounds = GIRect::MakeWH(device.width(), device.height());

        layers.push(GLayer(&device, identity, bounds));
    }

    void concat(const GMatrix& matrix) override {
        layers.top().getCTM().preConcat(matrix);
    }

    void restore() override {
        GLayer top = layers.top();
        layers.pop();
        GLayer base = layers.top();

        if (top.isLayer()) {
            top.draw(&base.getBitmap());
        }
    }

    void save() override {
        GLayer current = layers.top();
        layers.push(GLayer(&current.getBitmap(), current.getCTM(), current.getBounds()));
    }

    void drawPath(const GPath& path, const GPaint& paint) override {
        GLayer layer = layers.top();

        if (paint.getShader() != nullptr
            && !paint.getShader()->setContext(layer.getCTM())) {
            return;
        }

        GPoint pts[6 * path.countPoints()];
        int e_count = 0;
        GPath::Edger e = GPath::Edger(path);

        GPath::Verb v;
        do {
            GPoint nextPts[4];
            v = e.next(nextPts);

            if (v == GPath::Verb::kLine) {
                pts[2 * e_count] = nextPts[0];
                pts[2 * e_count + 1] = nextPts[1];
                e_count++;
            } else if (v == GPath::Verb::kCubic) {
                pts[2 * e_count] = nextPts[0];
                pts[2 * e_count + 1] = nextPts[1];
                e_count++;
                pts[2 * e_count] = nextPts[1];
                pts[2 * e_count + 1] = nextPts[2];
                e_count++;
                pts[2 * e_count] = nextPts[2];
                pts[2 * e_count + 1] = nextPts[3];
                e_count++;
            } else if (v == GPath::Verb::kQuad) {
                pts[2 * e_count] = nextPts[0];
                pts[2 * e_count + 1] = nextPts[1];
                e_count++;
                pts[2 * e_count] = nextPts[1];
                pts[2 * e_count + 1] = nextPts[2];
                e_count++;
            }
        } while (v != GPath::Verb::kDone);

        layer.getCTM().mapPoints(pts, pts, 2 * e_count);

        GRect bounds = GRect::MakeWH(
                layer.getBitmap().width(),
                layer.getBitmap().height());
        Edge storage[3 * e_count];
        Edge* edge = storage;

        for (int i = 0; i < e_count; ++i) {
            GPoint p0 = pts[2 * i];
            GPoint p1 = pts[2 * i + 1];

            edge = clipLine(p0, p1, bounds, edge);
        }

        e_count = edge - storage;
        if (e_count == 0) {
            return;
        }

        GBlitter blitter = GBlitter(layer.getBitmap(), paint);

        GScanConverter::scanComplex(storage, e_count, blitter);
    }

    void drawConvexPolygon(const GPoint srcPoints[], int count, const GPaint& paint) override {
        GLayer layer = layers.top();

        if (paint.getShader() != nullptr
                && !paint.getShader()->setContext(layer.getCTM())) {
            return;
        }

        GPoint points[count];
        layer.getCTM().mapPoints(points, srcPoints, count);

        GRect bounds = GRect::MakeWH(
            layer.getBitmap().width(),
            layer.getBitmap().height());
        Edge storage[count * 3];
        Edge* edge = storage;

        for (int i = 0; i < count; ++i) {
            GPoint p0 = points[i];
            GPoint p1 = points[(i + 1) % count];

            edge = clipLine(p0, p1, bounds, edge);
        }

        int edgeCount = edge - storage;
        if (edgeCount == 0) {
            return;
        }

        GBlitter blitter = GBlitter(layer.getBitmap(), paint);

        GScanConverter::scan(storage, edgeCount, blitter);
    }

    void drawPaint(const GPaint& paint) override {
        GBitmap bitmap = layers.top().getBitmap();
        GRect bounds = GRect::MakeWH(bitmap.width(), bitmap.height());
        drawRect(bounds, paint);
    }

    void drawRect(const GRect& rect, const GPaint& paint) override {
        GPoint pts[4] = {
            GPoint::Make(rect.left(), rect.top()),
            GPoint::Make(rect.right(), rect.top()),
            GPoint::Make(rect.right(), rect.bottom()),
            GPoint::Make(rect.left(), rect.bottom())
        };

        drawConvexPolygon(pts, 4, paint);
    }

    void onSaveLayer(const GRect* boundsPtr, const GPaint& paint) override {
        GIRect bound;
        GIRect last_bound = layers.top().getBounds();
        if (boundsPtr == nullptr) {
            bound = GIRect::MakeWH(last_bound.width(), last_bound.height());
        } else {
            GPoint pts[4] = {
                GPoint::Make(boundsPtr->left(), boundsPtr->top()),
                GPoint::Make(boundsPtr->right(), boundsPtr->top()),
                GPoint::Make(boundsPtr->right(), boundsPtr->bottom()),
                GPoint::Make(boundsPtr->left(), boundsPtr->bottom())
            };
            layers.top().getCTM().mapPoints(pts, pts, 4);

            float x[4], y[4];
            for (int i = 0; i < 4; ++i) {
                x[i] = pts[i].fX;
                y[i] = pts[i].fY;
            }

            float l = mult_min(x, 4);
            float t = mult_min(y, 4);
            float r = mult_max(x, 4);
            float b = mult_max(y, 4);

            GRect fl_bnds = GRect::MakeLTRB(l, t, r, b);
            bound = fl_bnds.round();

            if (!bound.intersect(last_bound)) {
                bound = GIRect::MakeWH(0, 0);
            }
        }

        GBitmap bitmap;
        size_t rowBytes = bound.width() * sizeof(GPixel);
        bitmap.reset(
            bound.width(),
            bound.height(),
            rowBytes,
            (GPixel*)calloc(bound.height(), rowBytes),
            GBitmap::kNo_IsOpaque);

        GMatrix last_ctm = layers.top().getCTM();
        GMatrix curr_ctm = GMatrix(
            last_ctm[0], last_ctm[1], last_ctm[2],
            last_ctm[3], last_ctm[4], last_ctm[5]);
        curr_ctm.postTranslate(
            last_bound.left() - bound.left(),
            last_bound.top() - bound.top());

        GLayer newLayer = GLayer(bitmap, curr_ctm, bound, paint);
        layers.push(newLayer);
    }

    final_createRadialGradient(GPoint& center, float radius, const GColor colors[],int count) override {

    }

private:
    std::stack<GLayer> layers;
};

//std::unique_ptr<GShader> final_createRadialGradient(GPoint center, float radius, const GColor colors[], int count);

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap& device) {
    if (!device.pixels()) {
        return nullptr;
    }
    return std::unique_ptr<GCanvas>(new MyCanvas(device));
}