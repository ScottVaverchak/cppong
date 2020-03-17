#ifndef VEC_CPP_
#define VEC_CPP_

template<typename T>
struct Vec2 {
    T x;
    T y;  
};

template<typename T>
Vec2<T> constexpr operator + (Vec2<T> lhs, Vec2<T> rhs) {
    Vec2<T> result = { lhs.x + rhs.x, lhs.y + rhs.y };
    return result;
}

template<typename T>
Vec2<T> constexpr operator - (Vec2<T> lhs, Vec2<T> rhs) {
    Vec2<T> result = { lhs.x - rhs.x, lhs.y - rhs.y };
    return result;
}

template<typename T>
Vec2<T> constexpr &operator += (Vec2<T> &lhs, Vec2<T> rhs) {
    lhs = lhs + rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr &operator -= (Vec2<T> &lhs, Vec2<T> rhs) {
    lhs = lhs - rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr operator * (Vec2<T> lhs, Vec2<T> rhs) {
    Vec2<T> result = { lhs.x * rhs.x, lhs.y * rhs.y };
    return result;
}

template<typename T>
Vec2<T> constexpr operator / (Vec2<T> lhs, Vec2<T> rhs) {
    assert(rhs.x != 0);
    assert(rhs.y != 0);

    Vec2<T> result = { lhs.x / rhs.x, lhs.y / rhs.y };
    return result;
}

template<typename T>
Vec2<T> constexpr &operator *= (Vec2<T> &lhs, Vec2<T> rhs) {
    lhs = lhs * rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr &operator /= (Vec2<T> &lhs, Vec2<T> rhs) {
    lhs = lhs / rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr operator + (Vec2<T> lhs, T rhs) {
    Vec2<T> result = { lhs.x + rhs, lhs.y + rhs };
    return result;
}

template<typename T>
Vec2<T> constexpr operator - (Vec2<T> lhs, T rhs) {
    Vec2<T> result = { lhs.x - rhs, lhs.y - rhs };
    return result;
}

template<typename T>
Vec2<T> constexpr operator * (Vec2<T> lhs, T rhs) {
    Vec2<T> result = { lhs.x * rhs, lhs.y * rhs };
    return result;
}

template<typename T>
Vec2<T> constexpr operator / (Vec2<T> lhs, T rhs) {
    assert(rhs != 0);
    
    Vec2<T> result = { lhs.x / rhs, lhs.y / rhs };
    return result;
}

template<typename T>
Vec2<T> constexpr &operator *= (Vec2<T> &lhs, T rhs) {
    lhs = lhs * rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr &operator /= (Vec2<T> &lhs, T rhs) {
    lhs = lhs / rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr &operator += (Vec2<T> &lhs, T rhs) {
    lhs = lhs + rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr &operator -= (Vec2<T> &lhs, T rhs) {
    lhs = lhs - rhs;
    return lhs;
}

template<typename T>
Vec2<T> constexpr operator -(Vec2<T> vec) {
    Vec2<T> result = { -vec.x, -vec.y };
    return result;
}

using Vec2i = Vec2<int>;

template<typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

template<typename T>
T min(T a, T b) {
    return a > b ? b : a;
}


#endif // VEC_CPP_
