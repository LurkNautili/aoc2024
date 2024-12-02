export module day2;

import <string>;
import <ranges>;
import <string_view>;
import <vector>;
import <format>;
import <charconv>;
//import <cctype>;
import <cassert>;
import inputReader;

namespace rng = std::ranges;
namespace vws = std::ranges::views;

using namespace std::literals;

enum class reportDir {
	UNSET,
	INCREASING,
	DECREASING,
	UNCHANGING
};

reportDir diffToDir(int64_t diff) {
	reportDir dir;
	if (diff > 0) {
		dir = reportDir::INCREASING;
	}
	else if (diff < 0) {
		dir = reportDir::DECREASING;
	}
	else {
		dir = reportDir::UNCHANGING;
	}
	return dir;
}

bool match(reportDir dir, int64_t diff) {
	return diffToDir(diff) == dir;
}

bool isSafe(const std::vector<int64_t>& report) {
	if (report.empty()) {
		return false;
	}
	bool answer{ true };
	int64_t prev = report[0];
	reportDir dir{ reportDir::UNSET };
	for (size_t i = 1; i < report.size(); ++i) {
		int64_t curr = report[i];
		auto diff = curr - prev;
		prev = curr;
		if (std::abs(diff) > 3) {
			answer = false;
			break;
		}
		if (dir == reportDir::UNSET) {
			dir = diffToDir(diff);
			if (dir == reportDir::UNCHANGING) {
				answer = false;
				break;
			}
		}
		else {
			if (!match(dir, diff)) {
				answer = false;
				break;
			}
		}
	}
	return answer;
}

struct ReportCard {
	std::vector<int64_t> report;
	bool safe{ false };
};

export std::string day2() {
	auto input = readFile("day2_input.txt");

	auto lines = input | vws::split("\n"sv);

	// Pt1
	std::vector<ReportCard> reports;
	size_t safeCount{ 0 };
	for (auto l : lines) {
		if (l.empty()) continue;
		std::vector<int64_t> report;
		for (auto numStrSubrange : l | vws::split(" "sv)) {
			auto numStr = numStrSubrange | rng::to<std::string>();
			report.emplace_back();
			std::from_chars(numStr.data(), numStr.data() + numStr.size(), report.back());
		}
		bool safe = isSafe(report);
		safeCount += safe ? 1 : 0;
		reports.push_back(ReportCard{ report, safe });
	}

	// Pt2
	// first, brute force
	size_t safeCount2{ 0 };
	for (const auto& c : reports) {
		auto safe = c.safe;
		if (safe) {
			safeCount2++;
			continue;
		}
		for (size_t i = 0; i < c.report.size(); ++i) {
			// mask out ith element, test with rest
			auto masked = c.report;
			masked.erase(masked.begin() + i);
			safe = isSafe(masked);
			if (safe) {
				safeCount2++;
				break;
			}
		}
	}

	return std::format("Part 1) number of safe reports is {}\nPart 2) number of safe reports is {}\n",
		safeCount, safeCount2);
};