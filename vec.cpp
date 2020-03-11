template<typename T>
struct vec2 {
    T x;
    T y;  
};

template<typename T>
vec2<T> operator + (vec2<T> lhs, vec2<T> rhs) {
  vec2<T> result = { lhs.x + rhs.x, lhs.y + rhs.y };
  return result;
}

template<typename T>
vec2<T> operator - (vec2<T> lhs, vec2<T> rhs) {
  vec2<T> result = { lhs.x - rhs.x, lhs.y - rhs.y };
  return result;
}

using vec2i = vec2<int>;
