export module day3;
import dayBase;

export std::string day3() {
	auto input = readFile("day3_input.txt");

	auto lines = input | vws::split("\n"sv);

	return std::format("Part 1) ... {}\nPart 2) ... {}\n",
		0, 0);
}