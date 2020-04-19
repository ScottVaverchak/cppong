template <typename T>
struct Rect {
    T x, y, w, h;
};

template <typename T>
Rect<T> constexpr rect(Vec2<T> pos, T w, T h) {
    return { pos.x, pos.y, w, h };
}

template <typename T>
Rect<T> constexpr rect(T x, T y, T w, T h) {
    return { x, y, w, h };
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

SDL_Rect constexpr rect_to_sdl(Rect<int> rect) {
    return { 
        rect.x,
        rect.y,
        rect.w,
        rect.h,
    };
}

template <typename T>
Rect<T> constexpr sdl_to_rect(SDL_Rect rect) {
    return {
        (T) rect.x, 
        (T) rect.y,
        (T) rect.w,
        (T) rect.h
    };
}

template <typename U, typename T>
Rect<U> constexpr rect_cast(Rect<T> rect) {
    return {
        (U) rect.x,
        (U) rect.y,
        (U) rect.w,
        (U) rect.h
    };
}

template <typename T>
bool rect_contains_point(Rect<T> rect, Vec2<T> vec) {
    return vec.x >= rect.x && vec.x <= rect.x + rect.w &&
            vec.y >= rect.y && vec.y <= rect.y + rect.h;
}

using Rectf = Rect<float>;
using Recti = Rect<int>;
