export module dayBase;
export import <string>;
export import <ranges>;
export import <string_view>;
export import <vector>;
export import <format>;
export import <charconv>;
export import inputReader;

import <numeric>;

export namespace rng = std::ranges;
export namespace vws = std::ranges::views;

export using namespace std::literals;

export int64_t toInteger(auto str) {
    int64_t i;
    std::from_chars(str.data(), str.data() + str.size(), i);
    return i;
}


export int64_t iPow(std::unsigned_integral auto base, std::unsigned_integral auto exp) {
    if (exp == 0u) return 1;
    if (exp == 1u) return base;
    int64_t result{ base * base };
    auto max = std::numeric_limits<int64_t>::max();
    if (base > max / base) {
        throw std::overflow_error{ "iPow overlow" };
    }
    for (size_t i = 2; i < exp; ++i) {
        result *= base;
    }
    return result;
}