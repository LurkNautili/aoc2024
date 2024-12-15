export module day7;
import dayBase;
import <algorithm>;

int64_t concat(int64_t a, int64_t b) {
    return toInteger(std::to_string(a) + std::to_string(b));
}

enum class op {
    PLUS,
    TIMES,
    CONCAT
};

export std::string day7() {
    auto input = readFile("day7_input.txt");

    int64_t total{};
    int64_t totalWithConcat{};

    for (auto l : input | vws::split("\n"sv)) {
        auto parts = l | vws::split(":"sv);
        auto left = parts | vws::take(1);
        auto notEmpty = [](auto s) { return s.size() > 0; };
        auto right = (parts | vws::drop(1)).front() | vws::split(" "sv) | vws::filter(notEmpty);

        int64_t target = toInteger(left.front());
        std::vector<int64_t> nums;
        rng::for_each(right, [&nums](auto v) { nums.push_back(toInteger(v)); });

        bool found{ false };

        if (nums.empty()) continue;

        // pt1
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
                found = true;
                break;
            }
        }

        if (found) {
            total += target;
            totalWithConcat += target;
            found = false;
        }
        else {
            // pt2
            auto variations = iPow(3u, operatorCount);
            for (size_t pattern = 0; pattern < variations; ++pattern) {
                std::vector<op> operators;
                for (size_t i = 0; i < operatorCount; ++i) {
                    auto asd = pattern / iPow(3u, i);
                    operators.push_back(static_cast<op>(asd % 3));
                }
                size_t k{};
                auto applyOperator = [&k, &operators](auto a, auto b) {
                    uint64_t ans{};
                    if (operators[k] == op::PLUS) ans = a + b;
                    if (operators[k] == op::TIMES) ans = a * b;
                    if (operators[k] == op::CONCAT) ans = concat(a, b);
                    k++;
                    return ans;
                };
                uint64_t sum = rng::fold_left_first(nums, applyOperator).value();
                if (sum == target) {
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            // pt2
            totalWithConcat += target;
        }
    }

    return std::format("Part 1) total calibration result: {}\
                      \nPart 2) total with concatenation: {}\n", total, totalWithConcat
    );
}