export module day2;

import <string>;
import <ranges>;
import <string_view>;
import <vector>;
import <format>;
import <charconv>;
import <cassert>;
import <optional>;
import inputReader;

namespace rng = std::ranges;
namespace vws = std::ranges::views;

using namespace std::literals;

enum class reportDelta {
	UNSET,
	POSITIVE,
	NEGATIVE,
	INVALID
};

reportDelta diffToDelta(int64_t diff) {
	reportDelta dir{ reportDelta::INVALID };
	if (diff == 0 || std::abs(diff) > 3) {
		dir = reportDelta::INVALID;
	}
	else if (diff > 0) {
		dir = reportDelta::POSITIVE;
	}
	else if (diff < 0) {
		dir = reportDelta::NEGATIVE;
	}
	return dir;
}

bool isSafe(const std::vector<int64_t>& report, std::optional<size_t> ignoreIndex) {
	if (report.empty()) {
		return false;
	}
	bool safe{ true };
	std::optional<int64_t> prev{};
	reportDelta prevDelta{ reportDelta::UNSET };
	for (size_t i = 0; i < report.size(); ++i) {
		// optionally ignore exactly one entry in the report
		if (ignoreIndex.has_value() && ignoreIndex.value() == i) {
			continue;
		}

		int64_t curr = report[i];
		if (!prev.has_value()) {
			// skip first element, because we need two to start evaluating differences
			prev = curr;
			continue;
		}
		
		// calculate difference, categorize as reportDelta based on requirements given in the assignment
		auto diff = curr - prev.value();
		prev = curr;
		auto delta = diffToDelta(diff);
		// break out early since any failed interval invalidates the whole report
		if (delta == reportDelta::INVALID) {
			safe = false;
			break;
		}

		// keep track of delta in report, mark report as unsafe if delta changes
		if (prevDelta == reportDelta::UNSET) {
			prevDelta = delta;
		}
		else if (delta != prevDelta) {
			safe = false;
			break;
		}
	}
	return safe;
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
	for (auto line : lines) {
		if (line.empty()) continue;
		reports.emplace_back();
		auto& card = reports.back();
		auto& report = card.report;
		for (auto piece : line | vws::split(" "sv)) {
			auto numStr = piece | rng::to<std::string>();
			report.emplace_back();
			std::from_chars(numStr.data(), numStr.data() + numStr.size(), report.back());
		}
		card.safe = isSafe(report, {});
		safeCount += card.safe ? 1 : 0;
	}

	// Pt2
	// brute force permutations of reports with an element missing
	// (still trying to think of a cleverer way to do this, not sure if one exists)
	size_t safeCount2{ 0 };
	for (const auto& c : reports) {
		auto safe = c.safe;
		if (safe) { // per the assignment, unsafe can become safe with the dampener but not vice versa
			safeCount2++;
			continue;
		}
		// try masking out each individual element in the report one at a time and testing with the remainder
		for (size_t i = 0; i < c.report.size(); ++i) {
			safe = isSafe(c.report, i);
			if (safe) {
				safeCount2++;
				break;
				// early exit of course, since we only need to succeed with one for a damper to take effect
			}
		}
	}

	return std::format("Part 1) number of safe reports is {}\nPart 2) number of safe reports is {}\n",
		safeCount, safeCount2);
};