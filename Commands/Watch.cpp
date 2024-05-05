#include "../includes/Command.hpp"

// For development of this comand , we dont have database and we dont store users, 
// then we can't show offline users, that's why we will send a errorNosuchnick
// when the user is not online. Also we don't have implemented the AWAY comand
// Thats why we will only respond if user is online, or error. 

int Command::Watch(Message *received_message)
{
	std::string target_nick;
	std::string response;
	Client *target_client;

	//If not enoff params for this command
	if(received_message->getNumArgs() == 0)
	{
		return(Command::ErrorNoNicknameGiven(this->_client, target_nick));
	}

  	//Try to take Target nick
	target_nick = received_message->getFirstParam();
  	//Check there is first param (target user)
  	if(target_nick.empty())
  	{
  	  	return(Command::ErrorNoNicknameGiven(this->_client, target_nick));
  	}

  	//Try to take the target client from given nick
	target_client = this->_server->getClientByNick(target_nick);
	//If client not found
	if(!target_client)
	{
		return(Command::ErrorNoSuchNick(this->_client, target_nick));
	}

	response =  this->_client->getResponsePrefix();
	response += " 604 :";
	response += target_nick;
	response = " ~";
	response += target_client->getUser();
	response += "B125170E.C0ED857D.425D160E.IP 1670690712 is online";
	response += "\r\n";
	return(this->SendResponse(this->_client, response));
}
