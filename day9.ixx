export module day9;
import dayBase;
import <algorithm>;
import <deque>;
import <iostream>;

struct Block {
    size_t addr;
    size_t size;
    size_t id;
};

export std::string day9() {
    auto input = readFile("day9_input.txt");

    constexpr size_t FREE{ (size_t)(-1) };

    std::deque<size_t> unsorted;
    std::vector<size_t> sorted;
    std::vector<size_t> tail;
    std::vector<Block> pt2todo;
    std::vector<Block> pt2done;
    std::vector<Block> pt2free;
    size_t addr{};
    size_t id{};
    bool file{ true };
    bool first{ true };
    for (auto c : input) {
        auto sz = static_cast<int>(c - '0');
        if (file) {
            unsorted.insert(unsorted.end(), sz, id);
            if (sz > 0)
                pt2todo.emplace_back(addr, sz, id);
            id++;
        }
        else {
            unsorted.insert(unsorted.end(), sz, FREE);
            if (sz > 0)
                pt2free.emplace_back(addr, sz, FREE);
        }
        addr += sz;
        file = !file;
    }

    // pt1
    while (!unsorted.empty()) {
        if (unsorted.front() != FREE) {
            // move the front of unsorted to the back of sorted
            // e.g. sorted: 009981, unsorted: 11...2...[]888...
            // becomes sorted: 0099811, unsorted: 1...2...[]888...
            sorted.push_back(unsorted.front());
            unsorted.pop_front();
        }
        else {
            // move the back of unsorted
            if (unsorted.back() == FREE) {
                // move the back of unsorted to the back of tail
                tail.push_back(unsorted.back());
                unsorted.pop_back();
            }
            else {
                // swap the back of unsorted with the front of unsorted
                std::swap(*(unsorted.begin()), *(unsorted.end() - 1));
            }
        }
    }

    size_t sum{};
    for (size_t i = 0; i < sorted.size(); ++i) {
        sum += (i * sorted[i]);
    }

    // pt2
    while (!pt2todo.empty()) {
        // grab the file block with the highest address and clear it from the queue
        auto b = pt2todo.back();
        pt2todo.pop_back();
        // find first free block capable of holding b
        // shrink the free block accordingly, possibly removing it
        // the find_if gets the first free because the vector is sorted due to how it was created
        // it remains sorted when a block shrinks within its own bounds
        if (auto ff = rng::find_if(pt2free, [&b](const auto& f) { return f.size >= b.size; }); 
            ff != pt2free.end() && ff->addr < b.addr)
        {
            b.addr = ff->addr;
            ff->size -= b.size;
            ff->addr += b.size;
            if (ff->size == 0) {
                pt2free.erase(ff);
            }
        }
        // add the processed blocks into done
        // (even if they find no free space to move to; they all only get once chance)
        pt2done.push_back(b);
    }

    size_t sum2{};
    for (const auto& e : pt2done) {
        for (size_t i = 0ULL; i < e.size; ++i) {
            sum2 += (e.addr + i) * e.id;
        }
    }

    return std::format("Part 1) file checksum: {}\nPart 2) file checksum: {}\n", sum, sum2);
}
