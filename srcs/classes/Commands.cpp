#include "Commands.hpp"

Commands::Commands(std::string &datas)
: valid(true)
{
	size_t	tmp = 0;
	size_t	tmp2 = 0;
	int		length;

	if (datas.find("\n", 0) != std::string::npos)
	{
		datas.erase(datas.find("\n", 1));
		if (datas.find("\r", 0) != std::string::npos)
			datas.erase(datas.find("\r", 1));
	}
	else
		this->valid = false;
	do {
		tmp2 = datas.find(' ', tmp);
		if (tmp2 == std::string::npos)
			tmp2 = datas.length();
		length = tmp2 - tmp;
		this->cmd.push_back(datas.substr(tmp, length));
		tmp = tmp2 + 1;
	} while (tmp2 != datas.length());
}

Commands::~Commands()
{

}

Commands::Commands(Commands const &)
{

}

Commands	&Commands::operator=(Commands const &)
{
	return (*this);
}

std::string		Commands::as_string()
{
	std::string ret;
	for (std::vector<std::string>::iterator it = this->cmd.begin(); it != this->cmd.end(); ++it) {
		ret += *it;
		if (it + 1 != this->cmd.end())
			ret += " ";
	}
	ret.append("\r\n");
	return ret;
}

bool	Commands::isValid()
{
	return this->valid;
}

void	Commands::add(std::string x)
{
	this->cmd.push_back(x);
}

std::string		Commands::from()
{
	if (this->cmd[0].find(':') == 0)
		return (this->cmd[0]);
	return ("");
}

std::string		Commands::name()
{
	if (this->cmd[0].find(':') == 0)
		return (this->cmd[1]);
	return (this->cmd[0]);
}

size_t			Commands::length() const
{
	if (this->cmd[0].find(':') == 0)
		return (this->cmd.size() - 1);
	return (this->cmd.size());
}

std::string		&Commands::getCmdParam(size_t i)
{
	if (i > this->cmd.size())
		throw Commands::out_of_range();
	if (this->cmd[0].find(':') == 0)
		return (this->cmd[i + 1]);
	return (this->cmd[i]);
}

std::string	&Commands::operator[](size_t i)
{
	if (i > this->cmd.size())
		throw Commands::out_of_range();
	if (this->cmd[0].find(':') == 0)
		return (this->cmd[i + 1]);
	return (this->cmd[i]);
}