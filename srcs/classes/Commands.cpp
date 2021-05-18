#include "Commands.hpp"

Commands::Commands(std::string &datas, int type)
: valid(true), type(type)
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
	if (datas[0] == ':')
	{
		tmp2 = datas.find(' ', tmp);
		if (tmp2 == std::string::npos)
			tmp2 = datas.length();
		this->prefix = datas.substr(tmp, tmp2);
		datas.erase(tmp, tmp2 + 1);
	}
	// do {
	// 	tmp2 = datas.find(' ', tmp);
	// 	if (tmp2 == std::string::npos)
	// 		tmp2 = datas.length();
	// 	length = tmp2 - tmp;
	// 	this->cmd.push_back(datas.substr(tmp, length));
	// 	tmp = tmp2 + 1;
	// } while (tmp2 != datas.length());
	this->cmd = utils::split(datas, ':');
	std::cout << "Prefix = " << this->prefix << std::endl;
	for (size_t i = 0; i < cmd.size(); i++)
		std::cout << "cmd[" << i << "] = " << cmd[i] << std::endl;
}

Commands::Commands(const Commands &x)
{
	*this = x;
}

Commands	&Commands::operator=(const Commands &x)
{
	this->cmd = x.cmd;
	this->valid = x.valid;
	this->type = x.type;
	return *this;
}

Commands::~Commands()
{

}

std::string		Commands::as_string()
{
	std::string ret;

	ret.append(this->prefix + " ");
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
	return (this->prefix);
}

void			Commands::setFrom(std::string str) {
	this->prefix = str;
}

std::string		Commands::name()
{
	return (this->cmd[0]);
}

size_t			Commands::length() const
{
	return (this->cmd.size());
}

std::string		&Commands::getCmdParam(size_t i)
{
	if (i > this->cmd.size())
		throw Commands::out_of_range();
	return (this->cmd[i]);
}

std::string	&Commands::operator[](size_t i)
{
	if (i > this->cmd.size())
		throw Commands::out_of_range();
	return (this->cmd[i]);
}

void	Commands::setType(int &type)
{
	this->type = type;
}

const int	&Commands::getType() const
{
	return this->type;
}

size_t	Commands::length()
{
	return this->cmd.size();
}
