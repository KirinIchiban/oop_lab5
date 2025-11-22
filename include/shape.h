#pragma once
#include <iostream>
#include <string.h>
#include <stdexcept>
#include <vector>
#include <memory>
#include <utility>
#include "point.h"


using namespace std;

template <Scalar T>
class Shape
{
    template <Scalar U>
    friend ostream& operator<<(ostream& os, const Shape<T>& f);
    template <Scalar U>
    friend istream& operator>>(istream& is, const Shape<T>& f);

public:
    Shape() : center(Point<T>{0.0, 0.0}), radius(T{1.0}) {}
    Shape(const Point<T> &center, T radius) : center(center), radius(radius) {}

    Point<T> Center() const {return center;}
    T get_radius() const {return radius;}
    virtual void print_figure(ostream& os) const;
    virtual void get_coords(istream& is);
    virtual const char* get_name() const = 0;

    operator double() const;
    virtual bool operator==(const Shape<T>& other) const;

    Shape(Shape&& other) noexcept
        : center(std::move(other.center)), 
          radius(std::move(other.radius)),
          coords(std::move(other.coords)) 
    {
        other.radius = T{};
    }

    Shape(const Shape& other) : Shape<T>(other.center, other.radius) {
        for (const auto& point : other.coords) {
            this->coords.push_back(std::make_unique<Point<T>>(*point));
        }
    }

    Shape& operator=(const Shape& other) {
        if (this != &other) {
            this->center = other.center;
            this->radius = other.radius;
            this->coords.clear();
            for (const auto& point : other.coords) {
                this->coords.push_back(std::make_unique<Point<T>>(*point));
            }
        }
        return *this;
    }

    Shape& operator=(Shape&& other) noexcept {
        if (this != &other) {
            center = std::move(other.center);
            radius = std::move(other.radius);
            coords = std::move(other.coords);
            other.radius = T{};
        }
        return *this;
    }

    virtual double Squere() const {
        double sq = 0.0;
        for (size_t i = 0; i < coords.size(); i++) {
            size_t j = (i + 1) % coords.size();
            sq += coords[i]->get_x() * coords[j]->get_y() - coords[j]->get_x() * coords[i]->get_y();
        }
        return std::abs(sq) / 2.0;
    }

    virtual ~Shape() = default;

protected:
    Point<T> center;
    T radius;
    std::vector<std::unique_ptr<Point<T>>> coords;
    virtual void make_figure() = 0;
};

template<Scalar T>
inline ostream& operator<<(ostream& os, const Shape<T>& area) {
    area.print_figure(os);
    return os;
}

template<Scalar T>
inline istream& operator>>(istream& is, Shape<T>& area) {
    area.get_coords(is);
    return is;
}

template<Scalar T>
Shape<T>::operator double() const {
    return Squere();
}

template<Scalar T>
bool Shape<T>::operator==(const Shape<T> &other) const {
    return (radius == other.radius) && (coords.size() == other.coords.size()) && (string(this->get_name()) == string(other.get_name()));
}


template<Scalar T>
void Shape<T>::print_figure(std::ostream &os) const {
    os << get_name() << "[Center: " << center << ", Radius: " << radius << "] Coords: ";
    for (const auto &point : coords) {
        os << *point << " ";
    }
    os << ";";
}

template<Scalar T>
void Shape<T>::get_coords(std::istream &is) {
    is >> center.m_x >> center.m_y >> radius;
    make_figure();
}
