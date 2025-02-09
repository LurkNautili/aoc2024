import day9;
import <print>;
import <chrono>;

int main()
{
	auto start = std::chrono::system_clock::now();
	auto ans = day9();
	auto end = std::chrono::system_clock::now();
	std::print("{}", ans);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::println("Computing answer took {} milliseconds", ms);
}
