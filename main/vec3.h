/**
 * ===============================================================================================
 * @addtogroup vec3
 * @{
 * ------------------------------------------------------------------------------------------------
 * @file    vec3.h
 * @author  Pascal-Emmanuel Lachance
 * @p       <a href="https://www.github.com/Raesangur">Raesangur</a>
 * @p       <a href="https://www.raesangur.com/">https://www.raesangur.com/</a>
 *
 * @brief   A 3D vector utility class, used to do operations on x, y, z coordinates
 *
 * ------------------------------------------------------------------------------------------------
 * @copyright Copyright (c) 2024 Pascal-Emmanuel Lachance | Raesangur
 *
 * @par License: <a href="https://opensource.org/license/mit/"> MIT </a>
 *               This project is released under the MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ------------------------------------------------------------------------------------------------
 * File History:
 * @version 0.1
 * 2024-03-07 - Raesangur
 *      - Initial implementation
 *
 * @version 0.2
 * 2024-03-08 - Raesangur
 *      - Fixing compilation issues
 *      - Adding edge cases to angle function
 *      - Added distance and lerp functions
 *      - Moved comparison lambda in its own function
 *
 * @version 0.3
 * 2024-03-08 - Raesangur
 *      - Added `operator+` and `operator-` for scalar values
 *      - Added `operator*` and `operator/` for vectors
 *      - Fixed possible edge case for unsigned `<T>` types
 * ===============================================================================================
 */
#ifndef VEC3_H
#define VEC3_H


/** ===============================================================================================
 *  INCLUDES
 */
#include <cmath>
#include <compare>
#include <iostream>
#include <limits>
#include <numbers>


/** -----------------------------------------------------------------------------------------------
 * @class vec3
 *
 * @brief Vector of 3 coordinates, and operators to work on them.
 */
template<typename T = float>
class vec3
{
public:
    T x{};
    T y{};
    T z{};

    constexpr vec3()             = default;
    constexpr vec3(const vec3&)  = default;
    constexpr vec3(const vec3&&) = default;
    constexpr vec3(T x, T y, T z);

    constexpr T       dot(const vec3<T>& b) const;
    constexpr vec3<T> cross(const vec3<T>& b) const;

    constexpr T        magnitude() const;
    constexpr vec3<T>& normalize();
    constexpr vec3<T>  lerp(const vec3<T>& b, T t) const;

    constexpr T angle(const vec3<T>& b) const;
    constexpr T distance(const vec3<T>& b) const;

    void print(std::ostream& os = std::cout) const;

    constexpr inline bool operator==(const vec3<T>& b) const;
    constexpr inline bool operator!=(const vec3<T>& b) const;

    constexpr inline vec3<T>  operator-() const;
    constexpr inline vec3<T>  operator+(T scalar) const;
    constexpr inline vec3<T>  operator-(T scalar) const;
    constexpr inline vec3<T>  operator*(T scalar) const;
    constexpr inline vec3<T>  operator/(T scalar) const;
    constexpr inline vec3<T>  operator+(const vec3<T>& b) const;
    constexpr inline vec3<T>  operator-(const vec3<T>& b) const;
    constexpr inline vec3<T>  operator*(const vec3<T>& b) const;
    constexpr inline vec3<T>  operator/(const vec3<T>& b) const;
    constexpr inline vec3<T>& operator+=(const vec3<T>& b);
    constexpr inline vec3<T>& operator-=(const vec3<T>& b);
    constexpr inline vec3<T>& operator*=(T scalar);
    constexpr inline vec3<T>& operator/=(T scalar);

private:
    constexpr inline bool comp(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        auto abs = [](T x) {
            return x >= 0 ? x : -x;
        };

        return abs(a - b) <= epsilon;
    }
};


/** ===============================================================================================
 *  METHOD DEFINITIONS
 */

/**
 * @brief Initialize a vector with x, y, z values specified.
 */
template<typename T>
constexpr vec3<T>::vec3(T x, T y, T z) : x{x}, y{y}, z{z}
{
}

/**
 * @brief Calculate the dot product of two vectors.
 */
template<typename T>
constexpr T vec3<T>::dot(const vec3<T>& b) const
{
    return x * b.x + y * b.y + z * b.z;
}

/**
 * @brief Calculate the cross product of two vectors.
 *
 * @return Resulting vector from the cross product.
 */
template<typename T>
constexpr vec3<T> vec3<T>::cross(const vec3<T>& b) const
{
    return vec3{y * b.z - b.y * z, x * b.z - b.x * z, x * b.y - b.x * y};
}


/**
 * @brief Calculate the magnitude (length - |a|) of a vector.
 */
template<typename T>
constexpr T vec3<T>::magnitude() const
{
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

/**
 * @brief Normalize the vector so its magnitude becomes 1.
 *        Its angle remains the same and the ratio between each axis is also maintained.
 */
template<typename T>
constexpr vec3<T>& vec3<T>::normalize()
{
    T mag = magnitude();
    x /= mag;
    y /= mag;
    z /= mag;

    return *this;
}

/**
 * @brief Calculate the linear interpolation between two vectors.
 *        A `t` step of `0` means 100% of the resulting vector is from the original vector.
 *        A `t` step of `1` means 100% of the resulting vector is from the `b` vector.
 *        A `t` inbetween means the resulting vector is somewhere in between.
 */
template<typename T>
constexpr vec3<T> vec3<T>::lerp(const vec3<T>& b, T t) const
{
    return *this + (b - *this) * t;
}


/**
 * @brief Calculate the angle between two vectors.
 * @details This uses the fomula cos(theta) = (a.b) / (|a| * |b|).
 *
 * @return Angle in radian between the vectors.
 */
template<typename T>
constexpr T vec3<T>::angle(const vec3<T>& b) const
{
    T dotProduct = std::max(std::min(dot(b), static_cast<T>(1.0)), static_cast<T>(-1.0));
    if(comp(dotProduct, static_cast<T>(1.0)))
    {
        return static_cast<T>(0.0);        // Parallel vectors;
    }
    if(comp(dotProduct, static_cast<T>(0)))
    {
        return static_cast<T>(std::numbers::pi);        // Opposite vectors;
    }
    return std::acos(dotProduct / (magnitude() * b.magnitude()));
}

/**
 * @brief Calculate the distance between two vectors.
 *
 * @details This function creates a temporary vector between the ends of both vectors, and
 *          calculates the magnitude of this temporary vector.
 */
template<typename T>
constexpr T vec3<T>::distance(const vec3<T>& b) const
{
    T dx = x - b.x;
    T dy = y - b.y;
    T dz = z - b.z;

    vec3<T> dV{dx, dy, dz};

    return dV.magnitude();
}


/**
 * @brief Print the vector to the specified output stream.
 *        This method does not flush the output stream.
 *
 * @details The formatting is as follows:
 *      X: [x value]\n
 *      Y: [y value]\n
 *      Z: [z value]\n
 */
template<typename T>
void vec3<T>::print(std::ostream& os) const
{
    os << "X: " << x << "\nY: " << y << "\nZ: " << z << '\n';
}


/* ---------------------------------------- Operators ------------------------------------------ */

/**
 * @brief Check if two vectors are equal.
 *
 * @warning The epsilon value provided by `std::numeric_limits` is used.
 *          This value is the minimum value representable by the hardware in the specified type,
 *          and may not adequately represent the desired resolution.
 *          It is possible for this function to give false negatives.
 */
template<typename T>
constexpr inline bool vec3<T>::operator==(const vec3<T>& b) const
{
    return comp(x, b.x) && comp(y, b.y) && comp(z, b.z);
}

/**
 * @brief Check if two vectors are not equal.
 *
 * @warning The same warnings apply to this method than to the `operator==` method.
 */
template<typename T>
constexpr inline bool vec3<T>::operator!=(const vec3<T>& b) const
{
    return !(*this == b);
}


/**
 * @brief Inverse the vector around 0.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator-() const
{
    return vec3{-x, -y, -z};
}

/**
 * @brief Add a scalar value to every value in the vector.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator+(T scalar) const
{
    return vec3{x + scalar, y + scalar, z + scalar};
}

/**
 * @brief Substract a scalar value from every value in the vector.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator-(T scalar) const
{
    return vec3{x - scalar, y - scalar, z - scalar};
}

/**
 * @brief Scale a vector with a specified scalar.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator*(T const scalar) const
{
    return vec3{x * scalar, y * scalar, z * scalar};
}

/**
 * @brief Scale a vector with a specified scalar.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator/(T const scalar) const
{
    return vec3{x / scalar, y / scalar, z / scalar};
}


/**
 * @brief Add a vector to the current vector.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator+(const vec3<T>& b) const
{
    return vec3{x + b.x, y + b.y, z + b.z};
}

/**
 * @brief Substract a vector to the current vector.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator-(const vec3<T>& b) const
{
    return vec3{x - b.x, y - b.y, z - b.z};
}

/**
 * @brief Multiply a vector with the current vector.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator*(const vec3<T>& b) const
{
    return vec3{x * b.x, y * b.y, z * b.z};
}

/**
 * @brief Divine the current vector with a vector.
 */
template<typename T>
constexpr inline vec3<T> vec3<T>::operator/(const vec3<T>& b) const
{
    return vec3{x / b.x, y / b.y, z / b.z};
}


/**
 * @brief Add two vectors together.
 */
template<typename T>
constexpr inline vec3<T>& vec3<T>::operator+=(vec3<T> const& b)
{
    x += b.x;
    y += b.y;
    z += b.z;
    return *this;
}

/**
 * @brief Substract two vectors.
 */
template<typename T>
constexpr inline vec3<T>& vec3<T>::operator-=(vec3<T> const& b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;
    return *this;
}

/**
 * @brief Scale a vector with a specified scalar.
 */
template<typename T>
constexpr inline vec3<T>& vec3<T>::operator*=(T scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

/**
 * @brief Scale a vector with a specified scalar.
 */
template<typename T>
constexpr inline vec3<T>& vec3<T>::operator/=(T scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}


#endif        // VEC3_H

/**
 * ------------------------------------------------------------------------------------------------
 * @}
 */
