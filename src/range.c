#include "range.h"

Range createRange(int from, int to) {
    Range r;

    r.from = from;
    r.to = to;

    return r;
}

int inRange(Range *range, int val) {
    return inNumRange(range->from, range->to, val);
}

int inNumRange(int from, int to, int target_val) {
    if (target_val >= from && target_val < to)
	return 1;

    return 0;
}

int rangeSize(Range *r) {
    return r->to - r->from;
}

