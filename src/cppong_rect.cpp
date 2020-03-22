template <typename T>
struct Rect {
    T x, y, w, h;
};

template <typename T>
Rect<T> constexpr rect(Vec2<T> pos, T w, T h) {
    return { pos.x, pos.y, w, h };
}

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
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;

    return lhs;
}

SDL_Rect rect_to_sdl(Rect<float> rect) {
    return { 
        (int) floorf(rect.x),
        (int) floorf(rect.y),
        (int) floorf(rect.w),
        (int) floorf(rect.h),
    };
}

using Rectf = Rect<float>;

