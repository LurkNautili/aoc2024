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

    std::unordered_map<int, std::vector<int>> leadersOf;

    for (auto r : ruleStr | vws::split("\n"sv)) {
        auto parts = r | vws::split("|"sv);
        // parts: left|right
        auto left = (parts | vws::take(1)).front();
        auto right = (parts | vws::drop(1)).front();
        // left -> right, right must follow left, left must precede right
        int leader;
        int follower;
        std::from_chars(left.data(), left.data() + left.size(), leader);
        std::from_chars(right.data(), right.data() + right.size(), follower);

        // collect list of elements that must precede a given number
        leadersOf[follower].push_back(leader);
        // now leadersOf[follower] is a vector of numbers that cannot come after follower
    }

    size_t sumPt1{};
    size_t sumPt2{};

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
            // grab all the numbers that the rules say must precede num
            // forbid them from the rest of the update since they cannot follow num without violating rules
            forbidden.insert_range(leadersOf[num]);
        }

        if (valid) {
            // update is valid, add the middle element to our sum
            sumPt1 += update.at(update.size() / 2);
        }
        else {
            // update violates a rule, sort update based on rules and add the middle element to the other sum
            rng::sort(update, [&leadersOf](const auto& left, const auto& right) {
                auto aPredecessors = leadersOf[left];
                // B = [b1, b2, b3...] -> left; left < right := !B.contains(right)
                return !rng::contains(aPredecessors, right);
                });
            sumPt2 += update.at(update.size() / 2);
        }
    }

    return std::format("\nPart 1) sum of valid update middle elements: {}\
                        \nPart 2) sum of fixed middle elements: {}\n",
                        sumPt1, sumPt2
    );
}
