export module day5;

import <format>;
import <algorithm>;
import <unordered_map>;
import <set>;
import dayBase;

export std::string day5() {
    auto input = readFile("day5_input.txt");

    auto halves = input | vws::split("\n\n"sv);
    auto ruleStr = (halves | vws::take(1)).front() | rng::to<std::string>();
    auto updateStr = (halves | vws::drop(1)).front() | rng::to<std::string>();

    std::unordered_map<int, std::vector<int>> ruleMap;

    for (auto r : ruleStr | vws::split("\n"sv)) {
        auto parts = r | vws::split("|"sv);
        auto left = (parts | vws::take(1)).front();
        auto right = (parts | vws::drop(1)).front();
        int precedes;
        int follows;
        std::from_chars(left.data(), left.data() + left.size(), precedes);
        std::from_chars(right.data(), right.data() + right.size(), follows);

        ruleMap[follows].push_back(precedes);
    }

    size_t sum{};
    size_t sum2{};

    for (auto u : updateStr | vws::split("\n"sv)) {
        std::vector<int> update;
        for (auto v : u | vws::split(","sv)) {
            int num;
            std::from_chars(v.data(), v.data() + v.size(), num);
            update.push_back(num);
        }
        if (update.size() % 2 == 0) {
            continue; // no middle element, can't sum anyway I guess
        }

        bool valid{ true };
        std::set<int> forbidden;
        for (auto num : update) {
            if (forbidden.contains(num)) {
                valid = false;
                break;
            }
            auto f = ruleMap[num];
            forbidden.insert_range(f);
        }

        if (valid) {
            // update is valid, add the middle element to our sum
            sum += update.at(update.size() / 2);
        }
        else {
            // update violates a rule, sort update based on rules and add the middle element to the other sum
            rng::sort(update, [&ruleMap](const auto& left, const auto& right) {
                auto aPredecessors = ruleMap[left];
                // B = [b1, b2, b3...] -> left; left < right := !B.contains(right)
                return !rng::contains(aPredecessors, right);
                });
            sum2 += update.at(update.size() / 2);
        }
    }

    return std::format("\nPart 1) sum of valid update middle elements: {}\
                        \nPart 2) sum of fixed middle elements: {}\n",
                        sum, sum2
    );
}
