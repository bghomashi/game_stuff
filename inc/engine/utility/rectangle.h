#pragma once

template <typename T>
struct TRectangle {
    T x,y, width, height;
};

typedef TRectangle<float> Rectangle;
typedef TRectangle<int> iRectangle;

template <typename T>
bool intersectRect(const TRectangle<T>& r1, const TRectangle<T>& r2) {
    return !(r2.x >= (r1.x+r1.width) || 
            (r2.x+r2.width) <= r1.x || 
            (r2.y+r2.height) <= r1.y ||
            r2.y >= (r1.y+r1.height));
}
