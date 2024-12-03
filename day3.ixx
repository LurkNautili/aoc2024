export module day3;
import <cctype>;
import <iostream>;
import dayBase;

enum class Stage { PRE, A, B };

export std::string day3() {
	auto input = readFile("day3_input.txt");

	//auto lines = input | vws::split("\n"sv);
	std::string preamble{ "mul(" };
	size_t prePos{ 0 };
	std::string argA;
	std::string argB;
	std::string dbgHistory;
	Stage stage{ Stage::PRE };
	int64_t sum{ 0 };
	for (const auto& c : input) {
		auto dbgCnd = dbgHistory == "mul(561";
		if (stage == Stage::PRE) {
			if (c == preamble[prePos]) {
				// continue matching more of preamble
				dbgHistory += c;
				prePos++;
				if (prePos == 4) {
					stage = Stage::A;
					prePos = 0;
				}
			}
			else {
				// fail
				dbgHistory = "";
				prePos = 0;
			}
		}
		else if (stage == Stage::A) {
			if (std::isdigit(c)) {
				// extend A
				dbgHistory += c;
				argA += c;
			}
			else if (c == ',' && !argA.empty()) {
				// move onto B
				dbgHistory += c;
				stage = Stage::B;
			}
			else {
				// fail
				dbgHistory = "";
				argA = "";
				stage = Stage::PRE;
			}
		}
		else if (stage == Stage::B) {
			if (std::isdigit(c)) {
				// extend B
				dbgHistory += c;
				argB += c;
			}
			else if (c == ')' && !argB.empty()) {
				// done, record mult and reset
				dbgHistory += c;
				int64_t a;
				int64_t b;
				std::from_chars(argA.data(), argA.data() + argA.size(), a);
				std::from_chars(argB.data(), argB.data() + argB.size(), b);
				std::cout << dbgHistory << "\n";
				dbgHistory = "";
				argA = "";
				argB = "";
				sum += a * b;
				stage = Stage::PRE;
			}
			else {
				// fail
				dbgHistory = "";
				argA = "";
				argB = "";
				stage = Stage::PRE;
			}
		}
	}

	return std::format("Part 1) ... {}\nPart 2) ... {}\n",
		sum, 0);
}