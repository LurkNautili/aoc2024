export module day5;

import <format>;
import <algorithm>;
import <unordered_map>;
import <print>;
import dayBase;

struct Rule {
    int lo;
    int hi;

    auto operator<=>(const Rule& other) const = default;
};

export std::string day5() {
    auto input = readFile("day5_input.txt");

    auto halves = input | vws::split("\n\n"sv);
    auto ruleStr = (halves | vws::take(1)).front() | rng::to<std::string>();
    auto updateStr = (halves | vws::drop(1)).front() | rng::to<std::string>();

    //std::unordered_map<int, std::vector<int>> followerMap;
    std::unordered_map<int, std::vector<int>> ruleMap;

    for (auto r : ruleStr | vws::split("\n"sv)) {
        auto parts = r | vws::split("|"sv);
        auto left = (parts | vws::take(1)).front();
        auto right = (parts | vws::drop(1)).front();
        int precedes;
        int follows;
        std::from_chars(left.data(), left.data() + left.size(), precedes);
        std::from_chars(right.data(), right.data() + right.size(), follows);

        //auto& followers = followerMap[precedes];
        //followers.push_back(follows);
        ruleMap[follows].push_back(precedes);
        //if (precedes > follows) {
        //    auto& exceptions = orderExceptions[precedes];
        //    exceptions.push_back(follows);
        //}
    }

    //for (auto& f : followerMap) {
    //    rng::sort(f.second);
    //}

    auto dbgPrint = [](auto vec, auto valid) {
        if (valid) {
            std::print("Passes: ", nullptr);
        }
        else {
            std::print("Fails: ", nullptr);
        }
        //std::print("{}", vec);
        for (const auto& el : vec) {
            std::print("{}, ", el);
        }
        std::print("\n", nullptr);
    };

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
            continue; // no middle element
        }
        // for every element in our update list, check if we're allowed to precede the rest of the list
        for (auto pivot = update.begin(); pivot != update.end(); ++pivot) {
            //auto& exceptions = orderExceptions[*pivot];
            auto& pivotPredecessors = ruleMap[*pivot];
            for (auto it = pivot; it != update.end(); ++it) {
                // for all elements in the tail following our pivot, 
                // make sure it's not in the pivot's precedent list
                // if it is, fail the whole update
                if (rng::contains(pivotPredecessors, *it)) {
                    dbgPrint(update, false);
                    goto fail; // remember when apple did an oopsie? fun times
                }
            }
        }
        // update is valid, add the middle element to our sum
        dbgPrint(update, true);
        sum += update.at(update.size() / 2);
        continue;
    fail:
        rng::sort(update, [&ruleMap](const auto& left, const auto& right) {
            auto aPredecessors = ruleMap[left];
            // B = [b1, b2, b3...] -> left; left < right := !B.contains(right)
            return !rng::contains(aPredecessors, right);
        });
        std::print("After sorting: \n", nullptr);
        dbgPrint(update, true);
        sum2 += update.at(update.size() / 2);
        continue;
    }

    return std::format("\nPart 1) sum of valid update middle elements: {}\
                        \nPart 2) sum of fixed middle elements: {}\n",
                        sum, sum2
    );
}
