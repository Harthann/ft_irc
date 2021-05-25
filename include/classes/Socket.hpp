#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
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


		
		/****************************************************************/
		/*					Socket action/role control					*/
		/****************************************************************/
		bool			Bind();
		bool			Connect();
		bool			Listen();
		Socket			*Accept();
		

		/****************************************************************/
		/*							Setters								*/
		/****************************************************************/
		void			setPassword(std::string &);
		void			setPinged(time_t = 0);
		
		/****************************************************************/
		/*					Getters and Info							*/
		/****************************************************************/
		const std::string 	&getPassword() const;
		time_t				getTime() const;
		std::string			strTime() const;
		const time_t		&getPingedTime() const;
		Addr				getInfo() const;
		const int &			getSocket() const;
		std::string			getHostName() const;
		std::string			IP() const;
		bool				bufferEmpty() const;
		
		/****************************************************************/
		/*					Read/write buffers control					*/
		/****************************************************************/
		void			Send(std::string , int );
		void			Confirm(std::string );
		std::string		Receive();
		void			bufferize(Commands &, int = 0);
		void			bufferize(std::string, int = 0);
		void			flushWrite();
		std::string		flush();

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
		time_t					ping_ts;
		std::string				password;
		std::string				recv_buffer;
		std::vector<Commands>	cmd_buffer;
		bool					writable;
		int						usage;
};

#endif
