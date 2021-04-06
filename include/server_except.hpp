#ifndef SERVER_EXCEPT_HPP
# define SERVER_EXCEPT_HPP

#include <exception>
#include "utils.hpp"
#include <sstream>
#include "Addr.hpp"

namespace se {

	struct	ServerException {

		ServerException(std::string err = "ServerException") : error(err) {};
		ServerException(Addr x, std::string err = "ServerException") : info(x), error(err)
		{
			error.append("\n");
			error.append(this->who());
		};

		const char *what() const throw() { return (error.c_str()); };
		const std::string who() const {
			std::string str;
			std::stringstream str_stream;
			
			str = info.getIP();
			str += ':';
			str += utils::itos(info.getPort());
			// str = str_stream.str();
			return (str);
		};

		Addr	info;
		std::string	error;
	};

	struct InvalidAccept : public se::ServerException {
		InvalidAccept() : ServerException("Couldn't accept connection") {};
		InvalidAccept(Addr info) : ServerException(info, "Couldn't accept connection") {};
	};

	struct InvalidBind : public se::ServerException {
		InvalidBind() : ServerException("Couldn't bind socket to port") {};
		InvalidBind(Addr info) : ServerException(info, "Couldn't bind socket to port") {};
	};

	struct ReadFailed : public se::ServerException {
		ReadFailed() : ServerException("Error reading socket of :") {};
		ReadFailed(Addr info) : ServerException(info, "Error reading socket of :") {};
	};
	
	struct SocketFailed : public se::ServerException {
		SocketFailed() : ServerException("Socket creation failed") {};
	};

	struct ConnectionFailed : public se::ServerException {
		ConnectionFailed() : ServerException("Failed to connect to distant host") {};
		ConnectionFailed(Addr info) : ServerException(info, "Failed to connect to distant host") {};
	};
	
	struct LostConnection : public se::ServerException {
		LostConnection() : ServerException("Lost connection to distant host") {};
		LostConnection(Addr info) : ServerException(info, "Lost connection to distant host") {};
	};

	struct SelectFailed : public se::ServerException {
		SelectFailed() : ServerException("Couldn't select request") {};
	};

}

#endif