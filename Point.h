#pragma once

#include <iostream>

struct Point {
    int x, y;
    
    Point(): x(0), y(0) {}

    Point(int x_, int y_): x(x_), y(y_) {}
};

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    return os<<"("<<point.x<<", "<<point.y<<")";
}