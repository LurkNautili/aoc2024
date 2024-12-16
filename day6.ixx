export module day6;
import dayBase;
import <format>;
import <mdspan>;
import <print>;
import <algorithm>;
import <set>;

enum class Dir {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

Dir rotateClockwise(Dir dir) {
    return static_cast<Dir>((static_cast<int>(dir) + 1) % 4);
}

struct Pt {
    size_t x;
    size_t y;

    auto operator<=>(const Pt& o) const = default;
};

class World;
class Walker {
public:
    Walker(size_t x, size_t y, Dir dir, World* World);
    bool step();
    bool wallAhead(Dir dir);
    std::set<Pt> path;
    bool looped{ false };
private:
    std::pair<int, int> unpackDir(Dir dir);
    char dirToChar(Dir dir);
    size_t hash(size_t x, size_t y, Dir dir);

    size_t x_;
    size_t y_;
    Dir dir_;
    World* world_;
    std::set<size_t> tail_;
};

class World {
public:
    World(std::string str, size_t width, size_t height);
    char getChar(size_t x, size_t y);
    void setChar(size_t x, size_t y, char c);
    std::pair<size_t, size_t> getSolution();
    size_t width;
    size_t height;
private:
    std::pair<size_t, size_t> getStartPos();
    void walkGuardPath();
    size_t getGuardPathLength();

    std::string data_;
    std::mdspan<char, std::dextents<size_t, 2>> M_;
    Walker walker_{ 0, 0, Dir::UP, this };
};



Walker::Walker(size_t x, size_t y, Dir dir, World* world) 
    : x_(x), y_(y), dir_(dir), world_(world) {}

bool Walker::step() {
    if (wallAhead(dir_)) {
        // done
        return true;
    }

    auto [dx, dy] = unpackDir(dir_);
    size_t nextX = x_ + dx;
    size_t nextY = y_ + dy;
    if (world_->getChar(nextX, nextY) == '#') {
        // obstructed, turn right
        dir_ = rotateClockwise(dir_);
        world_->setChar(x_, y_, dirToChar(dir_));
    }
    else {
        // unobstructed, update position, record visit
        world_->setChar(nextX, nextY, dirToChar(dir_));
        path.insert(Pt{ nextX, nextY });
        x_ = nextX;
        y_ = nextY;
    }
    auto h = hash(x_, y_, dir_);
    if (tail_.contains(h)) {
        // looping detected
        looped = true;
        return true;
    }
    tail_.insert(h);
    return false;
}

bool Walker::wallAhead(Dir dir) {
    if (dir == Dir::UP) return y_ == 0;
    if (dir == Dir::DOWN) return y_ == world_->height - 1;
    if (dir == Dir::LEFT) return x_ == 0;
    if (dir == Dir::RIGHT) return x_ == world_->width - 1;
    throw std::runtime_error{ "Illegal Dir enum member encountered" };
}

std::pair<int, int> Walker::unpackDir(Dir dir) {
    if (dir == Dir::UP) return { 0, -1 };
    if (dir == Dir::DOWN) return { 0, 1 };
    if (dir == Dir::LEFT) return { -1, 0 };
    if (dir == Dir::RIGHT) return { 1, 0 };
    throw std::runtime_error{ "Illegal Dir enum member encountered" };
}

char Walker::dirToChar(Dir dir) {
    if (dir == Dir::UP) return '^';
    if (dir == Dir::RIGHT) return '>';
    if (dir == Dir::DOWN) return 'v';
    if (dir == Dir::LEFT) return '<';
    throw std::runtime_error{ "Illegal Dir enum member encountered" };
}

size_t Walker::hash(size_t x, size_t y, Dir dir) {
    // ...00XXXXXXXXYYYYYYYYDD
    //      ^8      ^8      ^2
    return (x << 10) | (y << 2) | static_cast<size_t>(dir);
}

World::World(std::string str, size_t width, size_t height) 
        : data_(str), width(width), height(height), M_(std::mdspan(data_.data(), height, width))
{
    auto [x, y] = getStartPos();
    walker_ = Walker{ x, y, Dir::UP, this };
}

char World::getChar(size_t x, size_t y) {
    return M_[std::array{ y, x }];
}

void World::setChar(size_t x, size_t y, char c) {
    M_[std::array{ y, x }] = c;
}

size_t World::getGuardPathLength() {
    std::string str{ M_.data_handle() };
    return rng::count_if(str, [](auto n) { return rng::any_of("^>v<"sv, [&n](auto m) { return n == m; }); });
}

std::pair<size_t, size_t> World::getStartPos() {
    size_t pos = data_.find('^');
    size_t W = M_.extents().extent(0);
    size_t x = pos % W; // calling cols X, rows Y
    size_t y = pos / W; // i.e. X horizontal, Y vertical
    return { x, y };
}

void World::walkGuardPath() {
    while (!walker_.step()) {}
}

std::pair<size_t, size_t> World::getSolution() {
    std::string str{ M_.data_handle() };
    walkGuardPath();
    auto len = getGuardPathLength();
    size_t count{};
    for (const auto& p : walker_.path) {
        World sub{ str, M_.extents().extent(0) , M_.extents().extent(1) };
        sub.setChar(p.x, p.y, '#');
        sub.walkGuardPath();
        if (sub.walker_.looped) {
            count++;
            std::println("Found loop obstacle at ({}, {})", p.x, p.y);
        }
    }
    return std::make_pair(len, count);
}



export std::string day6() {
    auto input = readFile("day6_input.txt");

    auto lines = input | vws::split("\n"sv);
    auto width = lines.front().size(); // assumption: input is a rectangular block of text
    auto withoutNewlines = vws::join(lines) | rng::to<std::string>();
    auto height = withoutNewlines.size() / width;

    World world{ withoutNewlines, width, height };
    auto [posCount, loopCount] = world.getSolution();

    return std::format("\nPart 1) guard visits {} distinct positions\
                        \nPart 2) found {} looping obstacle positions\n", posCount, loopCount);
}
