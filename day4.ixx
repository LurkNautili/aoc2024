export module day4;
import <mdspan>;
import <format>;
import dayBase;

class Dir {
public:
    Dir(int x, int y) : x_(normalize(x)), y_(normalize(y)) {}

    int x() { return x_; }
    int y() { return y_; }

private:
    int normalize(int arg) {
        if (arg < 0) return -1;
        if (arg > 0) return 1;
        else return 0;
    }

    int x_;
    int y_;
};

export std::string day4() {
    auto input = readFile("day4_input.txt");

    auto pieces = input | vws::split("\n"sv);
    auto width = pieces.front().size();
    auto txt = vws::join(pieces) | rng::to<std::string>();
    auto height = txt.size() / width;

    auto data = std::mdspan(txt.data(), width, height);

    constexpr std::string_view pattern{ "XMAS" };
    constexpr auto XMAS = static_cast<int64_t>(pattern.size());

    // Pt1
    size_t count{};
    auto testAndCount = [&data, &count, &pattern] (auto row, auto col, auto dir) {
        for (auto i = 0; i < pattern.size(); ++i) {
            auto match = pattern[i] == data[std::array{ row + (i * dir.y()), col + (i * dir.x()) }];
            if (!match) {
                return;
            }
        }
        count++;
    };

    // Pt2
    size_t count2{};
    auto testAndCount2 = [&data, &count2](auto row, auto col) {
        constexpr auto mas{ "MAS"sv };
        constexpr auto sam{ "SAM"sv };
        std::string d1;
        std::string d2;
        for (int i = 0; i < 3; ++i) {
            d1.push_back(data[std::array{ row + i, col + i }]);
            d2.push_back(data[std::array{ row + i, col + 2 - i }]);
        }
        if ((d1 == mas || d1 == sam) && (d2 == mas || d2 == sam)) {
            count2++;
        }
    };

    const auto H = static_cast<int64_t>(width);
    const auto W = static_cast<int64_t>(height);
    if (H < 0 || W < 0) {
        return "Input too large";
    }
    for (auto i = 0; i < height; ++i) {
        for (auto j = 0; j < width; ++j) {
            // Pt1
            auto row = static_cast<int64_t>(i);
            auto col = static_cast<int64_t>(j);
            bool spaceAbove{ 0 <= (row - XMAS + 1) };
            bool spaceBelow{ (row + XMAS - 1) < H };
            bool spaceAhead{ (col + XMAS - 1) < W };
            bool spaceBehind{ 0 <= (col - XMAS + 1) };
            if (spaceAbove) testAndCount(row, col, Dir{0, -1});
            if (spaceBelow) testAndCount(row, col, Dir{ 0, 1 });
            if (spaceAhead) testAndCount(row, col, Dir{ 1, 0 });
            if (spaceBehind) testAndCount(row, col, Dir{ -1, 0 });
            if (spaceAbove && spaceBehind) testAndCount(row, col, Dir{ -1, -1 });
            if (spaceAbove && spaceAhead) testAndCount(row, col, Dir{ 1, -1 });
            if (spaceBelow && spaceBehind) testAndCount(row, col, Dir{ -1, 1 });
            if (spaceBelow && spaceAhead) testAndCount(row, col, Dir{ 1, 1 });

            // Pt2
            // check each 3x3 sub-square, with (i,j) pointing to the top left of the sub-square
            // that is, we need to have at least another two cols ahead and two rows below
            if (col < W - 2 && row < H - 2) {
                testAndCount2(row, col);
            }
        }
    }

    return std::format("Part 1) XMAS count: {}\nPart 2) X-MAS count: {}\n", count, count2);
}
