#include "../includes/Command.hpp"

/*
	This Who command only accept channels, cause the command has to not be implemented, but we implemented cause the
	used IRC Client will make WHO to channels. In case that the WHO not target a channel name, we will return
	ErrorChannelMustStartWhitHashMark
*/

int Command::Who(Message *received_message)
{
	std::string channel_name;
	std::vector<Client *> clients;
	std::string response;
	Channel *channel;

	//If no args sent
	if(received_message->getNumArgs() == 0)
	{
		return(Command::ErrorNeedMoreParams(this->_client, "WHO"));
	}

	//If arg channel name dont start whit #
	channel_name = received_message->getFirstParam();
	if(channel_name[0] != '#')
	{
		return(Command::ErrorChannelMustStartWhitHashMark(this->_client, channel_name));
	}

	//Extract the # from the channel_name
	channel_name.erase(0,1);
	channel = this->_server->getChannel(channel_name);

	//if channel not exist
	if(!channel)
	{
		//END WHO MESAJE
		response = ":127.0.0.1 315 #";
		response += channel->getName();
		response += " :End of /WHO list.\r\n";
		this->SendResponse(this->_client, response);
		return(1);
	}

	//BUCLE AROUND ALL USES IN CHANNEL (OPERATORS AND NORMAL USERS)
	for(int i = 0; i < 2; i++)
	{
		if(i == 0)
			clients = channel->getOperators();
		else
			clients = channel->getClients();

		for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			response = ":127.0.0.1 352 #";
			response += channel->getName();
			response += " ~";
			response += (*it)->getUser();
			response += " B125170E.C0ED857D.425D16";
			response += "0E.IP * ";
			response += (*it)->getNick();
			response += " H 0 ";
			response += (*it)->getName();
			response += "\r\n";
		    this->SendResponse(this->_client, response);
		}
	}

	//END WHO MESAJE
	response = ":127.0.0.1 315 #";
	response += channel->getName();
	response += " :End of /WHO list.\r\n";
	this->SendResponse(this->_client, response);
	return(1);
}