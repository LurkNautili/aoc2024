export module day7;
import dayBase;
import <algorithm>;

export std::string day7() {
    auto input = readFile("day7_input.txt");

    int64_t total{};

    for (auto l : input | vws::split("\n"sv)) {
        auto parts = l | vws::split(":"sv);
        auto left = parts | vws::take(1);
        auto notEmpty = [](auto s) { return s.size() > 0; };
        auto right = (parts | vws::drop(1)).front() | vws::split(" "sv) | vws::filter(notEmpty);

        int64_t target = toInteger(left.front());
        std::vector<int64_t> nums;
        rng::for_each(right, [&nums](auto v) { nums.push_back(toInteger(v)); });

        if (nums.empty()) continue;
        auto operatorCount = nums.size() - 1;
        auto options = 1ull << operatorCount;
        for (size_t pattern = 0; pattern < options; ++pattern) {
            std::vector<bool> operators;
            for (size_t i = 0; i < operatorCount; ++i) {
                operators.push_back((pattern >> i) & 1ull);
            }
            size_t k{};
            auto applyOperator = [&k, &operators](auto a, auto b) {
                return operators[k++] ? a * b : a + b;
            };
            uint64_t sum = rng::fold_left_first(nums, applyOperator).value();
            if (sum == target) {
                total += sum;
                break;
            }
        }
    }

    return std::format("Part 1) total calibration result: {}\n", total);
}