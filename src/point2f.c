#include <math.h>
#include <raylib.h>

#include <point2f.h>

Point2f p2f_add(Point2f a, Point2f b) {
    return (Point2f){.x = a.x + b.x, .y = a.y + b.y};
}

Point2f p2f_fadd(Point2f a, double add) {
    return (Point2f){.x = a.x + add, .y = a.y + add};
}

Point2f p2f_sub(Point2f a, Point2f b) {
    return (Point2f){.x = a.x - b.x, .y = a.y - b.y};
}

Point2f p2f_fmul(Point2f a, double mul) {
    return (Point2f){.x = a.x * mul, .y = a.y * mul};
}

Point2f p2f_fdiv(Point2f a, double div) {
    return (Point2f){.x = a.x / div, .y = a.y / div};
}

double p2f_dist(Point2f a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

Point2f p2f_get_unit_vector(Point2f a) {
    double magnitude = p2f_dist(a);
    return p2f_fdiv(a, magnitude);
}
