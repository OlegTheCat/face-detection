#pragma once

struct Rect {
    int x;
    int y;
    int w;
    int h;
};

typedef struct Rect Rect;

Rect createRect(int x, int y, int w, int h);
void moveRect(Rect *r, int x_offset, int y_offset);
int isInside(Rect *inner, Rect *outer);
