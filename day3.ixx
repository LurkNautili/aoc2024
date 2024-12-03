export module day3;
import <cctype>;
import <iostream>;
import dayBase;

enum class Stage { PRE, A, B };

export std::string day3() {
	auto input = readFile("day3_input.txt");

	//auto lines = input | vws::split("\n"sv);
	std::string preamble{ "mul(" };
	std::string doPattern{ "do()" };
	std::string dontPattern{ "don't()" };
	size_t prePos{ 0 };
	std::string argA;
	std::string argB;
	std::string dbgHistory;
	Stage stage{ Stage::PRE };
	int64_t sum{ 0 };
	bool multEnabled{ true };
	for (const auto& c : input) {
		auto dbgCnd = dbgHistory == "mul(561";
		if (stage == Stage::PRE) {
			if (prePos < preamble.size() && c == preamble[prePos]) {
				// continue matching more of preamble
				dbgHistory += c;
				prePos++;
				if (prePos == preamble.size()) {
					stage = Stage::A;
					prePos = 0;
				}
			}
			else if (prePos < doPattern.size() && c == doPattern[prePos]) {
				dbgHistory += c;
				prePos++;
				if (prePos == doPattern.size()) {
					// enable mult and reset
					dbgHistory = "";
					prePos = 0;
					std::cout << "Toggled mult ON\n";
					multEnabled = true;
				}
			}
			else if (prePos < dontPattern.size() && c == dontPattern[prePos]) {
				dbgHistory += c;
				prePos++;
				if (prePos == dontPattern.size()) {
					// disable mult and reset
					dbgHistory = "";
					prePos = 0;
					std::cout << "Toggled mult OFF\n";
					multEnabled = false;
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
				if (multEnabled) {
					int64_t a;
					int64_t b;
					std::from_chars(argA.data(), argA.data() + argA.size(), a);
					std::from_chars(argB.data(), argB.data() + argB.size(), b);
					sum += a * b;
					std::cout << dbgHistory << "\n";
				}
				dbgHistory = "";
				argA = "";
				argB = "";
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