#include "rect.h"

Rect createRect(int x, int y, int w, int h) {
    Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    return r;
}

void moveRect(Rect *r, int x_offset, int y_offset) {
    r->x += x_offset;
    r->y += y_offset;
}

int isInside(Rect *inner, Rect *outer) {
    if (inner->x < outer->x || inner->y < outer->y) return 0;
    if ((inner->x + inner->w) > (outer->x + outer->w)
	|| (inner->y + inner->h) > (outer->y + outer->h)) return 0;

    return 1;
}

