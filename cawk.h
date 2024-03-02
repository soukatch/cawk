#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using i128 = __int128_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using u128 = __uint128_t;

using f32 = float;
using f64 = double;

using std::string;

template <typename T__> using slice = std::vector<T__>;

template <typename T__>
[[nodiscard]] inline bool match__(T__ &&x__, T__ &&y__) noexcept {
  return std::regex_match(std::regex{to_string(y__)}, to_string(x__));
}

template <typename T__>
[[nodiscard]] inline bool match__(T__ &&x__, std::string_view y__) noexcept {
  return std::regex_match(std::regex{y__.data()}, to_string(x__));
}

inline static constexpr struct {
  template <typename T1__, typename T2__>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    return static_cast<T1__>(x__);
  }

  template <typename T1__, typename T2__>
    requires(std::is_same_v<T1__, i8> || std::is_same_v<T1__, i16> ||
             std::is_same_v<T1__, i32>) &&
            std::is_same_v<std::remove_cvref_t<T2__>, string>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{strtol(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }
    return y__;
  }

  template <typename T1__, typename T2__>
    requires(std::is_same_v<T1__, i8> || std::is_same_v<T1__, i16> ||
             std::is_same_v<T1__, i32>) &&
            std::is_same_v<std::remove_cvref_t<T2__>, std::span<char>>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    try {
      const auto y__{std::exchange(*std::end(x__), '\0')};
      const auto z__{std::strtol(x__.data(), nullptr)};
      *std::end(x__) = y__;
      return z__;
    } catch (...) {
      exit(EXIT_FAILURE);
    }
  }

  template <typename T1__, typename T2__>
    requires std::is_same_v<T1__, i64> &&
             std::is_same_v<std::remove_cvref_t<T2__>, string>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{strtoll(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }
    return y__;
  }

  template <typename T1__, typename T2__>
    requires(std::is_same_v<T1__, u8> || std::is_same_v<T1__, u16> ||
             std::is_same_v<T1__, u32>) &&
            std::is_same_v<std::remove_cvref_t<T2__>, string>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{strtoul(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }
    return y__;
  }

  template <typename T1__, typename T2__>
    requires std::is_same_v<T1__, u64> &&
             std::is_same_v<std::remove_cvref_t<T2__>, string>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{strtoull(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }
    return y__;
  }

  template <typename T1__, typename T2__>
    requires std::is_same_v<T1__, f32> &&
             std::is_same_v<std::remove_cvref_t<T2__>, string>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{strtof(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }

    return y__;
  }

  template <typename T1__, typename T2__>
    requires std::is_same_v<T1__, f32> &&
             std::is_same_v<std::remove_cvref_t<T2__>, std::span<char>>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{std::exchange(*std::end(x__), '\0')};
    const auto z__{strtof(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }

    *std::end(x__) = y__;
    return z__;
  }

  template <typename T1__, typename T2__>
    requires std::is_same_v<T1__, f64> &&
             std::is_same_v<std::remove_cvref_t<T2__>, string>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{strtof(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }

    return y__;
  }

  template <typename T1__, typename T2__>
    requires std::is_same_v<T1__, f64> &&
             std::is_same_v<std::remove_cvref_t<T2__>, std::span<char>>
  [[nodiscard]] constexpr T1__ operator()(T2__ &&x__) const noexcept {
    const auto y__{std::exchange(*std::end(x__), '\0')};
    const auto z__{strtod(x__.data(), nullptr)};
    if (errno == EINVAL || errno == ERANGE) [[unlikely]] {
      perror("");
      exit(errno);
    }

    *std::end(x__) = y__;
    return z__;
  }
} cast__{};

inline constexpr struct {
  template <typename T__> constexpr void operator()(T__ &&x__) const noexcept {
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, i8>)
      printf("%hh\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, i16>)
      printf("%h\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, i32>)
      printf("%d\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, i64>)
      printf("%ll\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, u8>)
      printf("%uhh\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, u16>)
      printf("%uh\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, u32>)
      printf("%u\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, u64>)
      printf("%ull\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, f32>)
      printf("%f\n", x__);
    if constexpr (std::is_same_v<std::remove_cvref_t<T__>, f64>)
      printf("%e\n", x__);
  }
} print__{};

template <typename T__>
std::ostream &operator<<(std::ostream &os__, const std::span<T__> &s__) {
  for (const auto &x__ : s__)
    os__ << x__;

  return os__;
}

using std::cos;
using std::exp;
using std::log;
using std::rand;
using std::sin;
using std::sqrt;
using std::srand;

std::vector<std::span<char>> fields__{};
std::string record__{};

uint64_t NR{}, NF{};
bool BEGIN{true}, END{}, mid__{false};

inline struct {
  bool operator()(std::istream &is__) const noexcept {
    fields__.clear();
    NF = 0;
    fields__.emplace_back();

    fields__.clear();

    if (!std::getline(is__, record__))
      return false;

    fields__.emplace_back(std::begin(record__), std::end(record__));

    string::size_type first__{}, last__{};
    bool curr__{};

    for (string::size_type i__{}; auto &&x__ : record__) {
      if (!std::isspace(x__) && !curr__) {
        first__ = i__;
        curr__ = true;
      } else if (std::isspace(x__) && curr__) {
        last__ = i__;
        fields__.emplace_back(record__.data() + first__, last__ - first__);
      }
      ++i__;
    }

    if (curr__)
      fields__.emplace_back(record__.data() + first__,
                            std::size(record__) - first__);

    ++NR;
    return true;
  }
} read_line__{};

inline std::function<void(void)> run_begin__{};
inline std::function<void(void)> run_mid__{};
inline std::function<void(void)> run_end__{};

inline struct {
  void operator()(std::istream &is__) const noexcept {
    std::invoke(run_begin__);
    for (; read_line__(is__);)
      run_mid__();
    run_end__();
  }
} run__{};

inline void init__() noexcept;

int main(int argc, char **argv) {
  init__();
  if (argc == 2) {
    std::ifstream in__{argv[1]};
    run__(in__);
  } else {
    run_end__();
    run_begin__();
  }
}