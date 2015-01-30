#pragma once

struct Range {
    int from;
    int to;
};

typedef struct Range Range;

Range createRange(int from, int to);
int inRange(Range *range, int val);
int inNumRange(int from, int to, int target_val);
