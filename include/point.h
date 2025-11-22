#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <cmath>
#include <concepts>
#include <memory>

template <typename T>
concept Scalar = std::is_scalar_v<T>;

template <Scalar T>
class Point
{
public:
    T m_x, m_y;

    Point() = default;
    Point(T x, T y) : m_x(x), m_y(y) {}
    Point(const Point<T>& other) = default;
    Point(Point<T>&& other) noexcept = default;

    Point<T> &operator=(const Point<T> &other) {
        if (this != &other) {
            m_x = other.m_x;
            m_y = other.m_y;
        }
        return *this;
    }

    Point<T> &operator=(Point<T> &&other) noexcept {
        if (this != &other) {
            m_x = std::move(other.m_x);
            m_y = std::move(other.m_y);
        }
        return *this;
    }

    T get_x() const { return m_x; }
    T get_y() const { return m_y; }
  
    template <Scalar U>
    friend std::istream& operator>>(std::istream& is, const Point<T>& p);

    template <Scalar U>
    friend std::ostream& operator<<(std::ostream& os, const Point<T>& p);
    
    bool operator==(const Point<T>& other) const {
        return m_x == other.m_x && m_y == other.m_y;;
    }

    ~Point() = default;

};

template <Scalar T>
inline std::istream& operator>>(std::istream& is, const Point<T>& p) {
    is >> p.m_x >> p.m_y;
    return is;
}

template <Scalar T>
inline std::ostream& operator<<(std::ostream& os, const Point<T>& p) {
    os << "(" << p.m_x << ", " << p.m_y<< ")";
    return os;
}

#endif