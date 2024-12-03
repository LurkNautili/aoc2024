export module day3;
import <cctype>;
import dayBase;

enum class Stage { PRE, A, B };

export std::string day3() {
	auto input = readFile("day3_input.txt");
	int64_t pt1Sum{ 0 };
	int64_t pt2Sum{ 0 };

	std::string mulPattern{ "mul(" };
	std::string doPattern{ "do()" };
	std::string dontPattern{ "don't()" };

	size_t prePos{ 0 };
	std::string argA;
	std::string argB;
	Stage stage{ Stage::PRE };
	bool multEnabled{ true };

	for (const auto& c : input) {
		if (stage == Stage::PRE) {
			if (prePos < mulPattern.size() && c == mulPattern[prePos]) {
				// continue matching more of preamble
				prePos++;
				if (prePos == mulPattern.size()) {
					stage = Stage::A;
					prePos = 0;
				}
			}
			else if (prePos < doPattern.size() && c == doPattern[prePos]) {
				prePos++;
				if (prePos == doPattern.size()) {
					// enable mult and reset
					prePos = 0;
					multEnabled = true;
				}
			}
			else if (prePos < dontPattern.size() && c == dontPattern[prePos]) {
				prePos++;
				if (prePos == dontPattern.size()) {
					// disable mult and reset
					prePos = 0;
					multEnabled = false;
				}
			}
			else {
				// fail
				prePos = 0;
			}
		}
		else if (stage == Stage::A) {
			if (std::isdigit(c)) {
				// extend A
				argA += c;
			}
			else if (c == ',' && !argA.empty()) {
				// move onto B
				stage = Stage::B;
			}
			else {
				// fail
				argA = "";
				stage = Stage::PRE;
			}
		}
		else if (stage == Stage::B) {
			if (std::isdigit(c)) {
				// extend B
				argB += c;
			}
			else if (c == ')' && !argB.empty()) {
				// done, record mult and reset
				int64_t a;
				int64_t b;
				std::from_chars(argA.data(), argA.data() + argA.size(), a);
				std::from_chars(argB.data(), argB.data() + argB.size(), b);
				pt1Sum += a * b;
				if (multEnabled) {
					pt2Sum += a * b;
				}
				argA = "";
				argB = "";
				stage = Stage::PRE;
			}
			else {
				// fail
				argA = "";
				argB = "";
				stage = Stage::PRE;
			}
		}
	}

	return std::format("Part 1) ... {}\nPart 2) ... {}\n",
		pt1Sum, pt2Sum);
}