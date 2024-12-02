import day3;
import <iostream>;
import <print>;
import <chrono>;

int main()
{
	auto start = std::chrono::system_clock::now();
	auto ans = day3();
	auto end = std::chrono::system_clock::now();
	std::print("{}", ans);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::print("Computing answer took {} milliseconds\n", ms);
}
