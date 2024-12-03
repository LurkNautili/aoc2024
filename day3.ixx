export module day3;
import <cctype>;
import <cassert>;
import dayBase;

int64_t parseMult(std::string_view argStr) {
	auto cPos = argStr.find(',');
	auto secondCommaPos = argStr.find(',', cPos + 1);
	if (secondCommaPos != std::string::npos) {
		return 0; // edge case, e.g. mul(5,6,7) should be rejected
	}
	auto a = argStr.substr(0, cPos);
	auto b = argStr.substr(cPos + 1);
	int64_t n;
	int64_t m;
	auto [ptrA, ecA] = std::from_chars(a.data(), a.data() + a.size(), n);
	auto [ptrB, ecB] = std::from_chars(b.data(), b.data() + b.size(), m);
	if ( (ecA != std::errc()) || (ecB != std::errc()) ) {
		return 0; // pretty sure this can't happen but for completeness' sake
	}
	return n * m;
}

void setCond(std::string_view input, size_t head, bool& cond) {
	static std::string doPattern{ "do()" };
	static std::string dontPattern{ "don't()" };
	auto doPos = input.rfind(doPattern, head); // reading backwards from head
	auto dontPos = input.rfind(dontPattern, head); // find closest do() and don't() positions
	auto found = [](size_t p) { return p != std::string::npos; };
	
	if (found(doPos) && found(dontPos)) {
		cond = doPos > dontPos ? true : false; // set based on which is closer to head, i.e. higher in position
	}
	else if (found(doPos)) {
		cond = true;
	}
	else if (found(dontPos)) {
		cond = false;
	}
}

export std::string day3() {
	auto raw = readFile("day3_input.txt");
	auto input = std::string_view{ raw };

	std::string mulPattern{ "mul(" };

	int64_t part1{};
	int64_t part2{};
	bool cond{ true };

	size_t head = input.find(mulPattern);
	setCond(input, head, cond);

	while (head != std::string::npos) {
		head += mulPattern.size(); // in mult(ACB,DEF), read head now points at A
		auto tail = input.find(')', head);
		if (tail == std::string::npos) {
			break;
		}
		auto substr = input.substr(head, tail - head); // should be e.g. "123,456" from "mul(123,456)"
		auto mistake = substr.find_first_not_of("0123456789,");
		if (mistake == std::string::npos) { // only numbers and commas detected
			auto mult = parseMult(substr);
			part1 += mult;
			if (cond) {
				part2 += mult;
			}
			head = tail + 1; // move read head past the closing parenthesis
		}
		else {
			head = head + mistake; // move read head to point where pattern was broken
		}

		// move read head to next instance of "mul("
		head = input.find(mulPattern, head);
		// set multiplication condition based on closest do/dont match
		setCond(input, head, cond);
	}

	return std::format("Part 1) sum of all multiplications {}\nPart 2) sum of conditional multiplications {}\n",
		part1, part2);
}