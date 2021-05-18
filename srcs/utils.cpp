#include <iostream>
#include "ft_irc.hpp"

namespace utils {

	int			stoi(std::string &str)
	{
		int i = 0;

		for (size_t j = 0; str[j]; ++j)
		{
			if (str[j] < '0' || str[j] > '9')
				throw WrongPortFormat();
			i = i * 10 + str[j] - '0';
		}
		return (i);
	}

	std::string	itos(int i)
	{
		std::string ret;
		long		tmp;
		int			sign = 1;

		if (i == 0)
			return ("0");
		if (i < 0)
			sign = -1;
		tmp = i * sign;
		while (tmp != 0)
		{
			ret.insert(ret.begin(), tmp % 10 + '0');
			tmp /= 10;
		}
		if (sign == -1)
			ret.insert(ret.begin(), '-');
		return (ret);
	}

	std::vector<std::string>		split(std::string str, char delimiter) {
		size_t						tmp = 0;
		size_t						tmp2 = 0;
		int							length;
		std::vector<std::string>	ret;

		do {
			tmp2 = str.find(delimiter, tmp + (str[tmp] == delimiter));
			if (tmp2 == std::string::npos)
				tmp2 = str.length();
			length = tmp2 - tmp;
			ret.push_back(str.substr(tmp, length));
			tmp = tmp2;
		} while (tmp2 != str.length());
		return ret;
	}

	std::string						trim(std::string &str, char c) {
		while (*str.begin() == c)
			str.erase(str.begin());
		while ((*str.end() - 1) == c)
			str.erase(str.end() - 1);
		return (str);
	}

}
