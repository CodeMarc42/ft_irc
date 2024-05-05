#ifndef CAP_CONTROLLER_HPP
# define CAP_CONTROLLER_HPP

#include "../../includes/Server.hpp"

class CapController
{

	private:

	public:
		CapController();
		~CapController();
		static std::string extractPass(std::string msg);
		static std::string extractNickName(std::string msg);
		static std::string extractUserName(std::string msg);
		static std::string extractUser(std::string msg);
};

# endif