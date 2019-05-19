#include "GMatrix.h"
#include "GPath.h"
#include "GPoint.h"
#include "GRect.h"
#include "MathUtils.h"

GPoint terp(const GPoint &v0, const GPoint &v1, const GPoint &t) {
    return GPoint::Make(v0.fX + (v1.fX - v0.fX) * t.fX, v0.fY + (v1.fY - v0.fY) * t.fY);
}

void GPath::ChopQuadAt(const GPoint src[3], GPoint dst[5], float t) {

    GPoint tt = GPoint::Make(t,t);
    GPoint ab = terp(src[0], src[1], tt);
    GPoint bc = terp(src[1], src[2], tt);

    dst[0] = src[0];
    dst[1] = ab;
    dst[2] = terp(ab, bc, tt);
    dst[3] = bc;
    dst[4] = src[2];
}

void GPath::ChopCubicAt(const GPoint src[4], GPoint dst[7], float t) {

    GPoint tt = GPoint::Make(t,t);
    GPoint ab = terp(src[0], src[1], tt);
    GPoint bc = terp(src[1], src[2], tt);
    GPoint cd = terp(src[2], src[3], tt);
    GPoint abc = terp(ab, bc, tt);
    GPoint bcd = terp(bc, cd, tt);
    GPoint abcd = terp(abc, bcd, tt);

    dst[0] = src[0];
    dst[1] = ab;
    dst[2] = abc;
    dst[3] = abcd;
    dst[4] = bcd;
    dst[5] = cd;
    dst[6] = src[3];
}

GPath& GPath::addCircle(GPoint center, float radius, Direction dir) {
    GRect r = GRect::MakeLTRB(center.x() - radius, center.y() - radius, center.x() + radius, center.y() + radius);
    return this->addRect(r, dir);
}

GPath& GPath::addPolygon(const GPoint pts[], int count) {
    if (count <= 1) { return *this; }

    this->moveTo(pts[0]);
    for (int i = 1; i < count; ++i) {
        this->lineTo(pts[i]);
    }
    return *this;
}

GPath& GPath::addRect(const GRect& rect, Direction dir) {
    this->moveTo(GPoint::Make(rect.left(), rect.top()));

    if (dir == Direction::kCW_Direction) {
        this->lineTo(GPoint::Make(rect.right(), rect.top()));
        this->lineTo(GPoint::Make(rect.right(), rect.bottom()));
        this->lineTo(GPoint::Make(rect.left(), rect.bottom()));
        //this->lineTo(GPoint::Make(rect.left(), rect.top()));
    } else {
        this->lineTo(GPoint::Make(rect.left(), rect.bottom()));
        this->lineTo(GPoint::Make(rect.right(), rect.bottom()));
        this->lineTo(GPoint::Make(rect.right(), rect.top()));
        //this->lineTo(GPoint::Make(rect.left(), rect.top()));
    }
    return *this;
}

GRect GPath::bounds() const {
    int count = this->fPts.size();

    if (count == 0) {
        return GRect::MakeWH(0, 0);
    }

    float x[count];
    float y[count];

    for (int i = 0; i < count; ++i) {
        x[i] = fPts[i].fX;
        y[i] = fPts[i].fY;
    }

    return GRect::MakeLTRB(
        mult_min(x, count),
        mult_min(y, count),
        mult_max(x, count),
        mult_max(y, count));
}

void GPath::transform(const GMatrix& matrix) {
    matrix.mapPoints(this->fPts.data(), this->fPts.data(), this->fPts.size());
}