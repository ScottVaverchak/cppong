
template<typename T>
struct Vec2 {
    T x;
    T y;  
};

template <typename T>
Vec2<T> constexpr vec2(T x, T y) {
    return { x, y };
}

template<typename T>
Vec2<T> constexpr operator + (Vec2<T> lhs, Vec2<T> rhs) {
    return { lhs.x + rhs.x, lhs.y + rhs.y };
}

template<typename T>
Vec2<T> constexpr operator - (Vec2<T> lhs, Vec2<T> rhs) {
    return { lhs.x - rhs.x, lhs.y - rhs.y };
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
    return { lhs.x * rhs.x, lhs.y * rhs.y };
}

template<typename T>
Vec2<T> constexpr operator / (Vec2<T> lhs, Vec2<T> rhs) {
    assert(rhs.x != 0);
    assert(rhs.y != 0);

    return  { lhs.x / rhs.x, lhs.y / rhs.y };
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
    return { lhs.x + rhs, lhs.y + rhs };
}

template<typename T>
Vec2<T> constexpr operator - (Vec2<T> lhs, T rhs) {
    return { lhs.x - rhs, lhs.y - rhs };
}

template<typename T>
Vec2<T> constexpr operator * (Vec2<T> lhs, T rhs) {
    return { lhs.x * rhs, lhs.y * rhs };
}

template<typename T>
Vec2<T> constexpr operator / (Vec2<T> lhs, T rhs) {
    assert(rhs != 0);
    
    return { lhs.x / rhs, lhs.y / rhs };
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
    return { -vec.x, -vec.y };
}

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;

template<typename T>
T constexpr max(T a, T b) {
    return a > b ? a : b;
}

template<typename T>
T constexpr min(T a, T b) {
    return a > b ? b : a;
}

const float PI { 3.141592f };
