#ifndef H_POINT2F
#define H_POINT2F

#include <raylib.h>

typedef struct Point2f {
    double x;
    double y;
} Point2f;

Point2f p2f_add(Point2f a, Point2f b);
Point2f p2f_fadd(Point2f a, double add);
Point2f p2f_sub(Point2f a, Point2f b);
Point2f p2f_fmul(Point2f a, double mul);
Point2f p2f_fdiv(Point2f a, double div);

double p2f_dist(Point2f a);
Point2f p2f_get_unit_vector(Point2f a);

#endif
