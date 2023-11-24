#pragma once

#ifndef MAIN_HELPER_H

#define MAIN_HELPER_H
#include <iostream>
#include <random>

template<typename T>
struct Vector2T { // dynamically typed vector2, can be float, can be int, etc...
    T x, y;
    Vector2T() : x(T()), y(T()) {}
    Vector2T(const T& _x, const T& _y) : x(_x), y(_y) {}

    Vector2T<T> operator+(const Vector2T<T>& other) const {
        return Vector2T<T>(x + other.x, y + other.y);
    }

    Vector2T<T> operator-(const Vector2T<T>& other) const {
        return Vector2T<T>(x - other.x, y - other.y);
    }

    bool operator==(const Vector2T<T>& other) const {
        return x == other.x && y == other.y;
    }

};

template<typename T>
struct Vector3T {
    T x, y, z;

    Vector3T() : x(T()), y(T()), z(T()) {}
    Vector3T(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}

    Vector3T<T> operator+(const Vector3T<T>& other) const {
        return Vector3T<T>(x + other.x, y + other.y, z + other.z);
    }

    Vector3T<T> operator-(const Vector3T<T>& other) const {
        return Vector3T<T>(x - other.x, y - other.y, z - other.z);
    }

    bool operator==(const Vector3T<T>& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};


#endif // !MAIN_HELPER_H
