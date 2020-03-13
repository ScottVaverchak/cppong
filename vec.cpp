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
Vec2<T> constexpr operator * (Vec2<T> lhs, T rhs) {
    Vec2<T> result = { lhs.x * rhs, lhs.y * rhs };
    return result;
}

template<typename T>
Vec2<T> operator / (Vec2<T> lhs, T rhs) {
    assert(rhs != 0);
    
    Vec2<T> result = { lhs.x / rhs, lhs.y / rhs };
    return result;
}

template<typename T>
Vec2<T> &operator += (Vec2<T> &lhs, Vec2<T> rhs) {
    lhs = lhs + rhs;
    return lhs;
}

template<typename T>
Vec2<T> &operator -= (Vec2<T> &lhs, Vec2<T> rhs) {
    lhs = lhs - rhs;
    return lhs;
}

template<typename T>
Vec2<T> &operator *= (Vec2<T> &lhs, T rhs) {
    lhs = lhs * rhs;
    return lhs;
}

template<typename T>
Vec2<T> &operator /= (Vec2<T> &lhs, T rhs) {
    lhs = lhs / rhs;
    return lhs;
}

/*
template<typename T>
vec2<T> operator - (vec2<T> lhs, vec2<T> rhs) {
    vec2<T> result = { lhs.x - rhs.x, lhs.y - rhs.y };
    return result;
}

template<typename T>
vec2<T> operator * (vec2<T> lhs, T rhs) {
    vec2<T> result = { lhs.x * rhs, lhs.y * rhs };
    return result;
}

template<typename T>
vec2<T> operator / (vec2<T> lhs, T rhs) {
    assert(rhs != 0);
    
    vec2<T> result = { lhs.x / rhs, lhs.y / rhs };
    return result;
}
*/
using Vec2i = Vec2<int>;
