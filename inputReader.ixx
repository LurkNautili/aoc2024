export module inputReader;
import <string>;
import <fstream>;
import <sstream>;

export std::string readFile(std::string fileName) {
	std::ifstream inputFile;
	inputFile.open(fileName);
	std::stringstream buf;
	buf << inputFile.rdbuf();
	return buf.str();
};