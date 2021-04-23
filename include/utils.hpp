#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

namespace utils {
	std::vector<std::string>	split_params(std::string &datas);
	std::string					reunite_params(std::vector<std::string> &params);
	int							stoi(std::string &str);
	std::string					itos(int i);
}

#endif