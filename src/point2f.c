#include <point2f.h>

void p2f_init(Point2f *ptr, float x, float y) {
    ptr->x = x;
    ptr->y = y;
}

Point2f p2f_mul(Point2f *a, float mul) {
    Point2f res;
    res.x = a->x * mul;
    res.y = a->y * mul;
    return res;
}

void p2f_add_inplace(Point2f *original, Point2f *add) {
    original->x += add->x;
    original->y += add->y;
}

void p2f_mul_inplace(Point2f *original, float mul) {
    original->x *= mul;
    original->y *= mul;
}
