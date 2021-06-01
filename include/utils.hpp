#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include "User.hpp"

namespace utils {
	std::vector<std::string>	split_params(std::string &datas);
	std::string					reunite_params(std::vector<std::string> &params);
	int							stoi(std::string str);
	std::string					itos(int i);
	std::vector<std::string>	split(std::string str, char delimiter);
	std::string					trim(std::string &str, char c);
	std::string					&delete_char(std::string &str, char c);
	std::string					mode_msg(int n, int i, User *user, std::string channel_name);
	void						setting_a_bit(short &mode, int n);
	bool						checking_a_bit(short &mode, int n);
	void						clearing_a_bit(short &mode, int n);
}

#endif
