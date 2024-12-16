export module day8;
import dayBase;
import <set>;
import <unordered_map>;
import <numeric>;

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

    std::set<Point> antinodes1;
    std::set<Point> antinodes2;
    for (const auto& [c, v] : antennas) {
        for (auto [a, b] : vws::cartesian_product(v, v)) {
            if (a <= b) continue;
            int dx = a.x - b.x;
            int dy = a.y - b.y;
            // Pt1
            auto ap1 = Point{ a.x + dx, a.y + dy };
            auto ap2 = Point{ b.x - dx, b.y - dy };
            if (inBounds(ap1)) {
                antinodes1.insert(ap1);
            }
            if (inBounds(ap2)) {
                antinodes1.insert(ap2);
            }

            // Pt2
            int gcd = std::gcd(dx, dy);
            int mx = dx / gcd;
            int my = dy / gcd;
            int k{ 0 };
            while (true) {
                auto m1 = Point{ a.x + (k * mx), a.y + (k * my) };
                if (inBounds(m1)) {
                    antinodes2.insert(m1);
                    k++;
                }
                else {
                    k = 0;
                    break;
                }
            }
            while (true) {
                auto m2 = Point{ b.x - (k * mx), b.y - (k * my) };
                if (inBounds(m2)) {
                    antinodes2.insert(m2);
                    k++;
                }
                else {
                    break;
                }
            }

        }
    }

    return std::format("Part 1) antinode count: {}\nPart 2) antinode count: {}\n", 
        antinodes1.size(), antinodes2.size());
}
