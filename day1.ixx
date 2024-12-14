export module day1;

import <string>;
import <ranges>;
import <string_view>;
import <vector>;
import <charconv>;
import <algorithm>;
import <tuple>;
import <format>;
import <unordered_map>;
import inputReader;

namespace rng = std::ranges;
namespace vws = std::ranges::views;

using namespace std::literals;

export std::string day1() {
	//auto input = readFile("day1_test_input.txt");
	auto input = readFile("day1_input.txt");

	//std::vector<std::string> lines = input | vws::split("\n"sv) | rng::to<std::vector<std::string>>();
	auto lineViews = input | vws::split("\n"sv) | rng::to<std::vector>();

	std::vector<size_t> leftCol;
	std::vector<size_t> rightCol;

	for (auto l : lineViews) {
		if (l.empty()) continue;
		auto left = (l | vws::split("   "sv) | vws::take(1)).front();
		auto right = (l | vws::split("   "sv) | vws::drop(1)).front();
		leftCol.emplace_back();
		rightCol.emplace_back();
		std::from_chars(left.data(), left.data() + left.size(), leftCol.back());
		std::from_chars(right.data(), right.data() + right.size(), rightCol.back());
	}

	rng::sort(leftCol);
	rng::sort(rightCol);

	// Pt 1
	size_t sum{ 0ULL };
	for (auto pair : vws::zip(leftCol, rightCol)) {
		size_t l = std::get<0>(pair);
		size_t r = std::get<1>(pair);
		sum += std::max(l, r) - std::min(l, r);
	}

	// Pt 2
	std::unordered_map<size_t, size_t> rightCounts;
	for (const auto& r : rightCol) {
		rightCounts[r]++;
	}
	size_t score{ 0ULL };
	for (const auto& l : leftCol) {
		score += l * rightCounts[l];
	}

	return std::format("Part 1) sum of distances is {}\n Part 2) similarity score is {}\n", sum, score);
}
