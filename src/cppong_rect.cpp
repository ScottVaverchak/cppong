template <typename T>
struct Rect {
    T x, y, w, h;
};

template <typename T>
Rect<T> constexpr operator + (Rect<T> lhs, Vec2<T> rhs) { 
    return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.w };
}

template <typename T>
Rect<T> constexpr operator - (Rect<T> lhs, Vec2<T> rhs) { 
    return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.w, lhs.w };
}

template <typename T>
Rect<T> constexpr &operator += (Rect<T> &lhs, Vec2<T> rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;

    return lhs;
}

template <typename T>
Rect<T> constexpr &operator -= (Rect<T> &lhs, Vec2<T> rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;

    return lhs;
}

