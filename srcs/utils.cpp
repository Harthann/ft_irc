#include <iostream>
#include "ft_irc.hpp"

namespace utils {

int			stoi(std::string str)
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

}