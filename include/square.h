#include "shape.h"

template <Scalar T>
class Square : public Shape<T>
{
public:
    Square(): Shape<T>() {};
    Square(const Point<T> &center, T radius) : Shape<T>(center, radius) {
        this->make_figure();
    }

    Square(const Square& other) : Shape<T>(other.center, other.radius) {
        for (const auto& point : other.coords) {
            this->coords.push_back(std::make_unique<Point<T>>(*point));
        }
    }

    virtual double Squere() const override {
        return 2*this->radius * this->radius;
    }

protected:
    void make_figure() override {
        this->coords.clear();
        for (int i = 0; i < 4; i++) {
            T angle = 2 * M_PI * i / 4;
            T x = this->center.get_x() + this->radius * std::cos(angle);
            T y = this->center.get_y() + this->radius * std::sin(angle);
            this->coords.push_back(std::make_unique<Point<T>> (x, y));
        }
    }

    const char* get_name() const override {
        return "Square";
    }
};
