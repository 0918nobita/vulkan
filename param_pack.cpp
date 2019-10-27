template <typename... Types> struct Hoge {
  static constexpr unsigned int size = sizeof...(Types);
};

int main() { static_assert(Hoge<void, void>::size == 2); }
