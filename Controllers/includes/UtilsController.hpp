#ifndef NICK_CONTROLLER_HPP
# define NICK_CONTROLLER_HPP

#include "../../includes/Server.hpp"

class UtilsController
{

	private:

	public:
		UtilsController();
		~UtilsController();
		static bool isValidNickName(std::string nick);
		static bool isNickNameNotInUse(std::string nick, Server *server);
		static bool isEmptyNickName(std::string nick);
		static std::string HashIp(std::string ip, int log_rounds);
		static std::vector<std::string> SplitPartArgument(std::string arg);
		static std::vector<std::string> SplitArgument(std::string arg);
};

# endif