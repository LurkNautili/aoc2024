export module day7;
import dayBase;
import <algorithm>;
import <span>;

int64_t concat(int64_t a, int64_t b) {
    return toInteger(std::to_string(a) + std::to_string(b));
}

bool valid(int64_t target, int64_t sum, std::span<int64_t> vals, bool conc) {
    if (vals.size() == 1) {
        return target == sum + vals[0] ||
            target == sum * vals[0] ||
            conc && target == concat(sum, vals[0]);
    }
    else {
        return valid(target, sum + vals[0], vals.subspan(1), conc) ||
            valid(target, sum * vals[0], vals.subspan(1), conc) ||
            conc && valid(target, concat(sum, vals[0]), vals.subspan(1), conc);
    }
}

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

        std::span span{ nums };
        if (valid(target, 0, nums, false)) {
            total += target;
        }
        if (valid(target, 0, nums, true)) {
            totalWithConcat += target;
        }
    }

    return std::format("Part 1) total calibration result: {}\
                      \nPart 2) total with concatenation: {}\n", total, totalWithConcat
    );
}