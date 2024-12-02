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

bool sameSign(int64_t a, int64_t b) {
	// intentionally ignoring == 0 here since differences of 0 are invalid anyway
	return a > 0 && b > 0 || a < 0 && b < 0;
}

bool isSafe(const std::vector<int64_t>& report, bool useDampener = false) {
	std::optional<int64_t> prevVal;
	std::optional<int64_t> initDiff;
	std::optional<size_t> dampenedIndex;
	auto testValidity = [&initDiff](int64_t prev, int64_t curr) {
		auto diff = curr - prev;
		auto diffMag = std::abs(diff);
		if (diffMag < 1 || diffMag > 3) {
			return false;
		}
		if (initDiff.has_value()) {
			if (!sameSign(diff, initDiff.value())) {
				return false;
			}
		}
		return true;
	};
	auto testWithOmission = [&report, &testValidity, &initDiff](size_t ind) {
		std::optional<size_t> dampIndex;
		// special cases:
		if (ind == 1 && report.size() >= 4) {
			// the first pair is a freebie, any time we fail the first diff we can just omit report[0]
			auto nextDiff = report[ind + 1] - report[ind];
			auto nextNextDiff = report[ind + 2] - report[ind + 1];
			if (sameSign(nextDiff, nextNextDiff)) {
				dampIndex = 0;
				// remember to reset initDiff since we've masked out the first element and thus initDiff is now invalid
				initDiff.reset();
			}
		}
		else if (ind == 2 && report.size() >= 4) {
			auto diff = report[ind] - report[ind - 1];
			auto nextDiff = report[ind + 1] - report[ind];
			if (!sameSign(diff, initDiff.value()) && sameSign(diff, nextDiff)) {
				// annoying edge case where prevDiff != diff and removing the first element at ind - 2 fixes it
				dampIndex = 0;
				initDiff.reset();
			}
		}
		else if (ind == report.size() - 1) {
			// similarly to the first case, if we've gotten to the end without spending the dampener,
			// and we fail on the last diff, we can just discard the final element in the report
			dampIndex = ind;
		}
		
		// normal cases, if we haven't found a valid omission yet: 
		if (!dampIndex.has_value()) {
			// test removing curr
			if ((ind + 1) < report.size()
				&& testValidity(report[ind - 1], report[ind + 1]))
			{
				dampIndex = ind;
			}

			// test removing prev (if the above case didn't work)
			if (!dampIndex.has_value() && ind >= 2 && ind + 1 < report.size()
				&& testValidity(report[ind - 2], report[ind])) 
			{
				auto diff1 = report[ind] - report[ind - 2];
				auto diff2 = report[ind + 1] - report[ind];
				if (sameSign(diff1, diff2)) {
					dampIndex = ind - 1;
					if (ind - 1 <= 1) {
						// we've messed with one of the two elements constituting initDiff, so we reset it
						initDiff.reset();
					}
				}
			}
		}
		return dampIndex;
	};
	auto dampened = [&useDampener, &dampenedIndex, &testWithOmission](size_t testInd) {
		if (!useDampener) {
			return false;
		}
		if (dampenedIndex.has_value()) {
			return false;
		}

		auto res = testWithOmission(testInd);
		if (res.has_value()) {
			dampenedIndex = res.value();
			return true;
		}
	};
	for (size_t i = 0; i < report.size(); ++i) {
		auto val = report[i];
		if (!prevVal.has_value()) {
			// i = 0, init prevVal
			prevVal = val;
			continue;
		}
		else if (!initDiff.has_value()) {
			// i = 1, init initDiff
			auto diff = val - prevVal.value();
			initDiff = diff;
		}

		auto prev = report[i - 1];
		auto curr = report[i];
		if (dampenedIndex.has_value() && dampenedIndex.value() == i - 1) {
			prev = report[i - 2];
		}

		if (!testValidity(prev, curr) && !dampened(i)) {
			goto fail; // remember that one? fun times
		}
	}
	return true;
fail:
	return false;
}

export std::string day2() {
	auto input = readFile("day2_input.txt");

	auto lines = input | vws::split("\n"sv);

	size_t safeCount{ 0 };
	size_t safeCount2{ 0 };
	for (auto line : lines) {
		if (line.empty()) continue;
		std::vector<int64_t> report;
		for (auto piece : line | vws::split(" "sv)) {
			auto numStr = piece | rng::to<std::string>();
			report.emplace_back();
			std::from_chars(numStr.data(), numStr.data() + numStr.size(), report.back());
		}
		auto safe1 = isSafe(report);
		safeCount += safe1;
		if (safe1) {
			safeCount2 += safe1;
		}
		else {
			auto safe2 = isSafe(report, true);
			safeCount2 += safe2;
		}
	}

	return std::format("Part 1) number of safe reports is {}\nPart 2) number of safe reports is {}\n",
		safeCount, safeCount2);
};