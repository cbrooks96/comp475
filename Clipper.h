#ifndef Clipper_DEFINED
#define Clipper_DEFINED

#include <deque>
#include "GPoint.h"
#include "GRect.h"

struct Edge {

    int y_top;
    int y_bot;

    float x_curr;
    float slope;

    short wind;

    bool init(GPoint p0, GPoint p1, int wind);

    bool operator<(const Edge& other);
};

Edge* clipLine(GPoint p0, GPoint p1, GRect bounds, Edge* edge);

#endif
