export module day8;
import dayBase;
import <set>;
import <unordered_map>;
import <print>;

export std::string day8() {
    auto input = readFile("day8_input.txt");

    auto lines = input | vws::split("\n"sv);
    auto width = lines.front().size();
    auto trim = vws::join(lines) | rng::to<std::string>();
    auto height = trim.size() / width;

    std::unordered_map<char, std::vector<Point>> antennas;
    for (auto [i, c] : vws::zip(vws::iota(0), trim)) {
        if (c != '.') {
            auto x = static_cast<int>(i % width);
            auto y = static_cast<int>(i / width);
            antennas[c].emplace_back(x, y);
        }
    }

    auto inBounds = [&width, &height](Point p) {
        return (0 <= p.x && p.x < width) && (0 <= p.y && p.y < height);
    };

    std::set<Point> antipodes;
    for (const auto& [k, v] : antennas) {
        for (auto [a, b] : vws::cartesian_product(v, v)) {
            if (a <= b) continue;
            int dx = a.x - b.x;
            int dy = a.y - b.y;
            auto ap1 = Point{ a.x + dx, a.y + dy };
            auto ap2 = Point{ b.x - dx, b.y - dy };
            if (inBounds(ap1)) antipodes.insert(ap1);
            if (inBounds(ap2)) antipodes.insert(ap2);
        }
    }

    std::string reconstruction;
    for (auto& a : antipodes) {
        trim[a.y * width + a.x] = '#';
    }
    for (auto s : (trim | vws::chunk(width) | rng::to<std::vector<std::string>>())) {
        auto foo = std::string{ s };
        reconstruction += (s + "\n");
    }

    return std::format("Part 1) antipode count: {}\n", antipodes.size());
}
