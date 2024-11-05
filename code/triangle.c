#include "display.h"
#include "triangle.h"

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t colour) {
    float inverted_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inverted_slope_2 = (float)(x2 - x0) / (y2 - y0);

    float x_start = x0;
    float x_end = x0;

    for (int y = y0; y <= y2; y++) {
        draw_line(x_start, x_end, y, y, colour);
        x_start += inverted_slope_1;
        x_end += inverted_slope_2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t colour) {
    float inverted_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inverted_slope_2 = (float)(x2 - x1) / (y2 - y1);

    float x_start = x2;
    float x_end = x2;

    for (int y = y2; y >= y1; y--) {
        draw_line(x_start, x_end, y, y, colour);
        x_start -= inverted_slope_1;
        x_end -= inverted_slope_2;
    }
}

void draw_filled_triangle(triangle_t triangle, uint32_t colour) {
    int x0 = triangle.points[0].x;
    int y0 = triangle.points[0].y;
    int x1 = triangle.points[1].x;
    int y1 = triangle.points[1].y;
    int x2 = triangle.points[2].x;
    int y2 = triangle.points[2].y;

    // Sort Vertices to the following order (y0 < y1 < y2)

    if (y0 > y1) {
        swap(&y0, &y1);
        swap(&x0, &x1);
    }
    if (y1 > y2) {
        swap(&y1, &y2);
        swap(&x1, &x2);
    }
    if (y0 > y1) {
        swap(&y0, &y1);
        swap(&x0, &x1);
    }

    // Avoid dividing by zero when calculating the inverted slopes.
    if (y1 == y2) {
        fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, colour);
    } else if (y0 == y1) {
        fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, colour);
    } else {
        int My = y1;
        int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;

        fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, colour);
        fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, colour);
    }

}