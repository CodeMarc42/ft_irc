#include "../includes/Command.hpp"

int Command::Ping(Message *msg)
{
	std::string response;
	std::string token;

	token = msg->getArgsInString();

	if(token.empty())
	{
		return (this->ErrorNeedMoreParams(this->_client, "PING"));
	}
	response = ":" + this->_server->getHostName() + " PONG " + this->_server->getHostName() + " " + token + "\r\n";
	return(this->SendResponse(this->_client, response));
}