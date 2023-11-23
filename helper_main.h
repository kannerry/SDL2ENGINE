#pragma once

#ifndef MAIN_HELPER_H

#define MAIN_HELPER_H
#include <iostream>

template<typename T>
struct Vector2T {
    T x, y;
    Vector2T() : x(T()), y(T()) {}
    Vector2T(const T& _x, const T& _y) : x(_x), y(_y) {}

    Vector2T<T> operator+(const Vector2T<T>& other) const {
        return Vector2T<T>(x + other.x, y + other.y);
    }

    Vector2T<T> operator-(const Vector2T<T>& other) const {
        return Vector2T<T>(x - other.x, y - other.y);
    }

};

#endif // !MAIN_HELPER_H
