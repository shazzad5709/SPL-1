#pragma once

#include "Point.h"
#include "tinyxml2.h"
#include <iostream>

struct Rectangle
{
    Point point;
    int width, height;

    #define ERR 5

    bool contains(const Point& p) const {
        return
        point.x <= p.x + ERR && p.x - ERR <= point.x + width &&
        point.y <= p.y + ERR && p.y - ERR <= point.y + height;
    }

    bool contains(const Rectangle& rect) const {
        return contains(rect.point)
        && contains(Point{rect.point.x+rect.width, rect.point.y+rect.height});
    }

    Rectangle()
    :point()
    ,width(0)
    ,height(0)
    {}

    Rectangle(Point point_, int w_, int h_)
    :point(point_)
    ,width(w_)
    ,height(h_)
    {}
};

const tinyxml2::XMLElement* operator>>(const tinyxml2::XMLElement* element, Rectangle& rect) {

    rect.point.x = element->FirstChildElement("x")->IntText();
    rect.point.y = element->FirstChildElement("y")->IntText();
    rect.width = element->FirstChildElement("w")->IntText();
    rect.height = element->FirstChildElement("h")->IntText();

    return element;
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rect)
{
    return os << "{ "
    << "x: " << rect.point.x << ", "
    << "y: " << rect.point.y << ", "
    << "w: " << rect.width << ", "
    << "h: " << rect.height << "}";
}