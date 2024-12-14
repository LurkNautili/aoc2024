export module day6;
import dayBase;
import <format>;
import <mdspan>;
import <set>;
import <numeric>;
import <print>;

enum class Dir {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class World {
public:
    World(std::string& str, size_t width, size_t height) 
        : M_(std::mdspan(str.data(), height, width)), inputPtr_(&str)
    {
        std::tie(x_, y_) = getStartPos();
        M_[std::array{ y_, x_ }] = 'X';
        visited_.insert(flattenIndex(x_, y_));
    }

private:
    std::pair<size_t, size_t> getStartPos() {
        size_t pos = inputPtr_->find('^');
        size_t W = M_.extents().extent(0);
        size_t x = pos % W; // calling cols X, rows Y
        size_t y = pos / W; // i.e. X horizontal, Y vertical
        return { x, y };
    }

    size_t walkAndReturnCount() {
        while (!step()) {}
        return visited_.size();
    }

    bool step() {
        auto [dx, dy] = unpackDir(currDir_);
        if (escapesBounds(0, x_, dx) || escapesBounds(1, y_, dy)) {
            // done
            return true;
        }

        size_t nextX = x_ + dx;
        size_t nextY = y_ + dy;
        auto test = M_[std::array{ nextY, nextX }];
        if (M_[std::array{ nextY, nextX }] == '#') {
            // obstructed, turn right
            currDir_ = rotate(currDir_);
        }
        else {
            // unobstructed, update position, record visit
            x_ = nextX;
            y_ = nextY;
            M_[std::array{ nextY, nextX }] = 'X';
            visited_.insert(flattenIndex(x_, y_));
        }
        //std::print("Visiting ({}, {})\n", x_, y_);
        return false;
    }

    Dir rotate(Dir dir) {
        return static_cast<Dir>((static_cast<int>(dir) + 1) % 4);
    }

    bool escapesBounds(size_t dim, size_t pos, int diff) {
        auto max = M_.extents().extent(dim) - 1;
        if (diff == 1 && pos == max) return true;
        else if (diff == -1 && pos == 0) return true;
        return false;
    }

    std::pair<int, int> unpackDir(Dir dir) {
        if (dir == Dir::UP) return { 0, -1 };
        if (dir == Dir::DOWN) return { 0, 1 };
        if (dir == Dir::LEFT) return { -1, 0 };
        if (dir == Dir::RIGHT) return { 1, 0 };
        else throw std::runtime_error{"Illegal Dir enum member encountered"};
    }

    size_t flattenIndex(size_t x, size_t y) {
        return M_.mapping()(x, y);
    }

    std::string toString() {
        std::string str;
        size_t W = M_.extents().extent(0);
        size_t H = M_.extents().extent(1);
        bool first{ true };
        for (size_t i = 0; i < H; ++i) {
            if (first) {
                first = false;
            }
            else {
                str += '\n';
            }
            for (size_t j = 0; j < W; ++j) {
                str += M_[std::array{ i, j }];
            }
        }
        return str;
    }

    size_t doubleCheck() {
        size_t count{};
        auto s = toString();
        for (auto c : s) {
            if (c == 'X') {
                count++;
            }
        }
        return count;
    }

    std::mdspan<char, std::dextents<size_t, 2>> M_;
    const std::string* inputPtr_;
    std::set<size_t> visited_;
    size_t x_;
    size_t y_;
    Dir currDir_{ Dir::UP };

    friend std::string day6();
};

export std::string day6() {
    auto input = readFile("day6_input.txt");

    auto lines = input | vws::split("\n"sv);
    auto width = lines.front().size(); // assumption: input is a rectangular block of text
    auto withoutNewlines = vws::join(lines) | rng::to<std::string>();
    auto height = withoutNewlines.size() / width;

    World world{ withoutNewlines, width, height };

    auto posCount = world.walkAndReturnCount();
    auto foo = world.toString();

    auto dbgCount = world.doubleCheck();

    return std::format("\nPart 1) guard visits {} distinct positions\
                        \nPart 2) ???: {}\n", posCount, posCount);
}