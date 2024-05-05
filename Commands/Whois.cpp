#include "../includes/Command.hpp"

/*
	This command could have more returned info, but like is not a mandatory command we returned RPL_311 and RPL_319
	We implemented it, to return something when the IRC Client launch this command.
*/

int Command::WhoIs(Message *received_message)
{
	std::string target_nick;
	std::string response;
	Client *target_client;

	//If not enoff params for this command
	if(received_message->getNumArgs() == 0)
	{
		return(Command::ErrorNoNicknameGiven(this->_client, "WHOIS"));
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

	//SI TODO CORRECTO
	//SEND 311 RPL!!!
	response =  this->_client->getResponsePrefix();
	response += " 311 ";
	response += target_client->getNick();
	response += " ~";
	response += target_client->getUser();
	response += " B125170E.C0ED857D.425D160E.IP * ";
	response += target_client->getName();
	response += "\r\n";
	this->SendResponse(this->_client, response);

	//SEND 319 RPL!!!
	response =  this->_client->getResponsePrefix();
	response += " 319 ";
	response += target_client->getNick();
	response += " ";
	response += target_client->list_channels_in_str();
	response += "\r\n";
	this->SendResponse(this->_client, response);

	//END WHOIS MESAJE
	response =  this->_client->getResponsePrefix();
	response += " 318 ";
	response += target_nick;
	response += " :End of /WHOIS list.\r\n";
	return(this->SendResponse(this->_client, response));
}
