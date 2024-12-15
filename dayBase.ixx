export module dayBase;
export import <string>;
export import <ranges>;
export import <string_view>;
export import <vector>;
export import <format>;
export import <charconv>;
export import inputReader;

export namespace rng = std::ranges;
export namespace vws = std::ranges::views;

export using namespace std::literals;

export int64_t toInteger(auto str) {
    int64_t i;
    std::from_chars(str.data(), str.data() + str.size(), i);
    return i;
}