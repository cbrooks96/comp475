#include "Clipper.h"

bool Edge::init(GPoint p0, GPoint p1, int wind) {
    this->wind = wind;

    if (p0.y() > p1.y()) {
        std::swap(p0, p1);
        this->wind = -this->wind;
    }

    this->y_top = GRoundToInt(p0.y());
    this->y_bot = GRoundToInt(p1.y());

    if (y_top == y_bot) {
        return false;
    }

    this->slope = (p1.x() - p0.x()) / (y_bot - y_top);

    float dx = this->slope * (this->y_top - p0.y() + 0.5f);
    this->x_curr = p0.x() + dx;

    return true;
}

bool Edge::operator<(const Edge& other) {
    if (this->y_top < other.y_top) {
        return true;
    }

    if (other.y_top < this->y_top) {
        return false;
    }

    if (this->x_curr < other.x_curr) {
        return true;
    }

    if (other.x_curr < this->x_curr) {
        return false;
    }

    return this->slope <= other.slope;
}

Edge* clipLine(GPoint p0, GPoint p1, GRect bounds, Edge* edge) {
    int wind = 1;

    if (p0.y() > p1.y()) {
        std::swap(p0, p1);
        wind = -wind;
    }

    if (p1.y() <= bounds.top() || p0.y() >= bounds.bottom()) {
        return edge;
    }

    if (p0.y() < bounds.top()) {
        float newX = p0.x() + (p1.x() - p0.x()) * (bounds.top() - p0.y()) / (p1.y() - p0.y());
        p0.set(newX, bounds.top());
    }

    if (p1.y() > bounds.bottom()) {
        float newX = p1.x() - (p1.x() - p0.x()) * (p1.y() - bounds.bottom()) / (p1.y() - p0.y());
        p1.set(newX, bounds.bottom());
    }

    if (p0.x() > p1.x()) {
        std::swap(p0, p1);
        wind = -wind;
    }

    if (p1.x() <= bounds.left()) {
        p0.fX = p1.fX = bounds.left();
        return edge + edge->init(p0, p1, wind);
    }

    if (p0.x() >= bounds.right()) {
        p0.fX = p1.fX = bounds.right();
        return edge + edge->init(p0, p1, wind);
    }

    if (p0.x() < bounds.left()) {
        float newY = p0.y() + (bounds.left() - p0.x()) * (p1.y() - p0.y()) / (p1.x() - p0.x());
        edge += edge->init(
            GPoint::Make(bounds.left(), p0.y()),
            GPoint::Make(bounds.left(), newY),
            wind);

        p0.set(bounds.left(), newY);
    }

    if (p1.x() > bounds.right()) {
        float newY = p1.y() - (p1.x() - bounds.right()) * (p1.y() - p0.y()) / (p1.x() - p0.x());
        edge += edge->init(
            GPoint::Make(bounds.right(), newY),
            GPoint::Make(bounds.right(), p1.y()),
            wind);

        p1.set(bounds.right(), newY);
    }

    return edge + edge->init(p0, p1, wind);
}
