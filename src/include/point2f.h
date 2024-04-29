#ifndef H_POINT2F
#define H_POINT2F

typedef struct Point2f {
    double x;
    double y;
} Point2f;

void p2f_init(Point2f *ptr, double x, double y);

Point2f p2f_mul(Point2f *ptr, double mul);

void p2f_add_inplace(Point2f *original, Point2f *add);
void p2f_mul_inplace(Point2f *original, double mul);

#endif
