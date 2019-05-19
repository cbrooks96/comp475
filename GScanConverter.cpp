#include <algorithm>
#include <iostream>
#include <vector>
#include "GBlitter.h"
#include "GMath.h"
#include "GScanConverter.h"
#include "GTypes.h"
#include "Clipper.h"

void GScanConverter::scan(Edge* edges, int count, GBlitter& blitter) {
    GASSERT(count >= 2);

    std::sort(edges, edges + count);

    int lastY = edges[count - 1].y_bot;

    Edge left = edges[0];
    Edge right = edges[1];

    int next = 2;

    float curr_y = left.y_top;

    float leftX = left.x_curr;
    float rightX = right.x_curr;

    while (curr_y < lastY) {
        blitter.blitRow(curr_y, GRoundToInt(leftX), GRoundToInt(rightX));
        curr_y++;

        if (curr_y > left.y_bot) {
            left = edges[next];
            next++;

            leftX = left.x_curr;
        } else {
            leftX += left.slope;
        }

        if (curr_y > right.y_bot) {
            right = edges[next];
            next++;

            rightX = right.x_curr;
        } else {
            rightX += right.slope;
        }
    }
}

int getEdgeIndex(Edge* edge, std::vector<Edge*> edges) {
    int loc = std::find(edges.begin(), edges.end(), edge) - edges.begin();

    if (loc >= edges.size()) {
        GASSERT(false);
    }
    return loc;
}


bool isEdgeLess(Edge* one, Edge* two) {
    return one->x_curr <= two->x_curr;
}

void resortBackwards(Edge* edge, std::vector<Edge*>& edges) {
    int pos = getEdgeIndex(edge, edges);

    std::sort(edges.begin(), edges.begin() + pos + 1, isEdgeLess);
}

void GScanConverter::scanComplex(Edge* edges, int count, GBlitter& blitter) {
    GASSERT(count >= 2);

    std::sort(edges, edges + count);

    std::vector<Edge*> edgeVec;
    for (int i = 0; i < count; ++i) {
        edgeVec.push_back(&edges[i]);
    }

    int y_min = edgeVec.front()->y_top;
    int y_max = edgeVec.back()->y_bot;

    for (int y = y_min; y < y_max;) {
        int wind = 0;
        Edge* e_front = edgeVec.front();
        Edge* e_next;

        int x0, x1;

        while (e_front != nullptr && e_front->y_top <= y) {
            if (wind == 0) {
                x0 = GRoundToInt(e_front->x_curr);
            }

            wind += e_front->wind;

            if (wind == 0) {
                x1 = GRoundToInt(e_front->x_curr);
                blitter.blitRow(y, x0, x1);
            }

            int next_i = getEdgeIndex(e_front, edgeVec) + 1;
            if (next_i >= edgeVec.size()) {
                e_next = nullptr;
            } else {
                e_next = edgeVec[next_i];
            }

            if (e_front->y_bot == y + 1) {
                edgeVec.erase(edgeVec.begin() + getEdgeIndex(e_front, edgeVec));
            } else {
                e_front->x_curr += e_front->slope;
                resortBackwards(e_front, edgeVec);
            }

            e_front = e_next;
        }
        y++;

        while (e_front != nullptr && e_front->y_top == y) {
            int next_i = getEdgeIndex(e_front, edgeVec) + 1;
            if (next_i >= edgeVec.size()) {
                e_next = nullptr;
            } else {
                e_next = edgeVec[next_i];
            }
            resortBackwards(e_front, edgeVec);
            e_front = e_next;
        }
    }
}
