#pragma once

#include <cmath>
#include <format>
#include <string_view>

namespace rt {

    struct Vec3 {

        union {
            float elements[3];
            struct {
                float x;
                float y;
                float z;
            };
        };

        Vec3() {}
        Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

        float &operator[](int idx) { return elements[idx]; }
        float operator[](int idx) const { return elements[idx]; }
        Vec3 operator+() const { return *this; }
        Vec3 operator-() const { return Vec3(-x, -y, -z); }

        Vec3 &operator+=(Vec3 const &v);
        Vec3 &operator-=(Vec3 const &v);
        Vec3 &operator*=(Vec3 const &v);
        Vec3 &operator/=(Vec3 const &v);
        Vec3 &operator*=(float val);
        Vec3 &operator/=(float val);

        float length() const { return sqrt(x * x + y * y + z * z); }
        float squared_length() const { return (x * x + y * y + z * z); }

        void normalize();
    };

    inline Vec3 &Vec3::operator+=(Vec3 const &v) {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    inline Vec3 &Vec3::operator-=(Vec3 const &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    inline Vec3 &Vec3::operator*=(Vec3 const &v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;

        return *this;
    }

    inline Vec3 &Vec3::operator/=(Vec3 const &v) {
        x /= v.x;
        y /= v.y;
        z /= v.z;

        return *this;
    }

    inline Vec3 &Vec3::operator*=(float val) {
        x *= val;
        y *= val;
        z *= val;

        return *this;
    }

    inline Vec3 &Vec3::operator/=(float val) {
        x /= val;
        y /= val;
        z /= val;

        return *this;
    }

    inline void Vec3::normalize() {
        const float k = 1 / length();
        x *= k;
        y *= k;
        z *= k;
    }

    inline Vec3 operator+(const Vec3 &lval, const Vec3 &rval) {
        return Vec3{
            lval.x + rval.x,
                lval.y + rval.y,
                lval.z + rval.z,
        };
    }

    inline Vec3 operator-(const Vec3 &lval, const Vec3 &rval) {
        return Vec3{
            lval.x - rval.x,
                lval.y - rval.y,
                lval.z - rval.z,
        };
    }

    inline Vec3 operator*(const Vec3 &lval, const Vec3 &rval) {
        return Vec3{
            lval.x * rval.x,
                lval.y * rval.y,
                lval.z * rval.z,
        };
    }

    inline Vec3 operator/(const Vec3 &lval, const Vec3 &rval) {
        return Vec3{
            lval.x / rval.x,
                lval.y / rval.y,
                lval.z / rval.z,
        };
    }

    inline Vec3 operator*(const Vec3 &vec, float val) {
        return Vec3(vec.x * val, vec.y * val, vec.z * val);
    }

    inline float dot_product(Vec3 const &lval, Vec3 const &rval) {
        return lval.x * rval.x + lval.y * rval.y + lval.z * rval.z;
    }

    inline Vec3 cross_product(Vec3 const &lval, Vec3 const &rval) {
        return Vec3{lval.y * rval.z - lval.z * rval.y,
            lval.z * rval.x - lval.x * rval.z,
            lval.x * rval.y - lval.y * rval.x};
    }

    inline Vec3 normalize_vec(Vec3 const &vec) {
        const float div = 1.f / vec.length();
        return Vec3(vec.x * div, vec.y * div, vec.z * div);
    }
}

template <>
struct std::formatter<rt::Vec3> : public std::formatter<std::string_view> {
  auto format(const rt::Vec3 &vec, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "{{{}, {}, {}}}", vec.x, vec.y, vec.z);
  }
};
