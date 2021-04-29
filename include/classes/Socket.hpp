#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "server_except.hpp"
#include <iterator>
#include "ft_irc.hpp"
#include "Addr.hpp"
#include "Commands.hpp"

class Commands;

class Socket
{
	public:
		Socket(int port = 0, std::string = "", std::string IP = "127.0.0.1");
		Socket(host_info &);
		Socket(Socket const &);
		Socket &operator=(Socket const&);
		~Socket();

		const int &		getSocket() const;
		Addr			getInfo() const;
		
		//	Socket control operators

		bool			Bind();
		bool			Connect();
		bool			Listen();
		std::string		IP() const;
		std::string		getHostName() const;
		Socket			*Accept();
		
		void			Send(std::string message);
		std::string		Receive();

		void			setPassword(std::string &);
		std::string 	&getPassword();
		
		//	Control socket timers
		time_t			getTime() const;
		std::string		strTime() const;
		
		//		Flush respectively read buffer and write buffer
		void			bufferize(Commands &);
		void			bufferize(std::string);
		std::string		flush();
		void			flushWrite();

		bool	operator==(Socket const& x) {
			return (x.socketfd == this->socketfd);
		};

		bool	operator!=(Socket const& x) {
			return (x.socketfd != this->socketfd);
		};

	protected:
		Socket(int fd, Addr addr, int addr_l);

		Addr					addr;
		int						socketfd;
		time_t					timestamp;
		std::string				password;
		std::vector<Commands>	cmd_buffer;
		bool					writable;
};

#endif
