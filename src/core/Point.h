#pragma once

class Point
{
public:
    Point() : time(0.0), value(0.0) {}
    Point(double t, double v) : time(t), value(v) {}
    double time;
    double value;
};

