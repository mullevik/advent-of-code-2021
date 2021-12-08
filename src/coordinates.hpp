#include <iostream>
#include <cassert>

class Coordinates {
    int x_part;
    int y_part;
public:
    Coordinates(int x, int y): x_part(x), y_part(y) {}
    Coordinates(const Coordinates& other): x_part(other.x()), y_part(other.y()) {}
    
    const int x() const {return this->x_part;}
    const int y() const {return this->y_part;}
    
    friend bool operator==(const Coordinates& lhs, const Coordinates& rhs) {
        return lhs.x() == rhs.x() and lhs.y() == rhs.y();
    }
    friend bool operator!=(const Coordinates& lhs, const Coordinates& rhs) {
        return ! (lhs == rhs);
    }
    friend Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs) {
        auto new_coordinates = Coordinates(lhs.x() + rhs.x(), lhs.y() + rhs.y());
        return new_coordinates;
    }
    friend std::ostream& operator<< (std::ostream& stream, const Coordinates& coords) {
        stream << "(" << coords.x() << ", " << coords.y() << ")";
        return stream;
    }
};

class CoordinatesLine {
    Coordinates s;
    Coordinates t;
public:
    CoordinatesLine(const Coordinates& source, const Coordinates& target): s(source), t(target) {}
    CoordinatesLine(const CoordinatesLine& other): s(other.source()), t(other.target()) {}

    const Coordinates source() const {return this->s;}
    const Coordinates target() const {return this->t;}
    const Coordinates direction() const {
        int x_distance = -(this->s.x() - this->t.x());
        if (x_distance > 0) x_distance = 1;
        if (x_distance < 0) x_distance = -1;

        int y_distance = -(this->s.y() - this->t.y());
        if (y_distance > 0) y_distance = 1;
        if (y_distance < 0) y_distance = -1;

        return Coordinates(x_distance, y_distance);
    }
};

void test_Coordinates() {
    auto a = Coordinates(1, 1);
    auto b = Coordinates(-2, 3);

    std::cout << a << std::endl;

    auto c = a;
    auto d = a + b;

    assert(c.x() == 1 and c.y() == 1);
    assert(d.x() == -1 and d.y() == 4);

    a = b;
    assert(c.x() == 1 and c.y() == 1);
    assert(a.x() == -2 and a.y() == 3);
    std::cout << "test_Coordinates passed" << std::endl;
}