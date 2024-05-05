#include "../includes/Command.hpp"


//TODO ERR_TOOMANYTARGETS (407)

int Command::PrivtMsg(Message *received_message)
{
	std::string first_param;
	std::string msg;
	std::string response;
	std::string channel_name;
	Client *destiny_client;
	Channel *destiny_channel;
	int temp;
	int num_args;

	num_args = received_message->getNumArgs();
	if(num_args == 0)
	{
		return(this->ErrorNoRecipientGiven(this->_client, "PRIVMSG"));
	}

	temp = 0;
	first_param = received_message->getFirstParam();
	msg = received_message->getAllFromParamNumber(2);
	if(msg[0] == ':')
		msg.erase(0,1);
	
	//CASE 1: Send to a user
	if(first_param[0] != '#')
	{
		destiny_client = this->_server->getClientByNick(first_param);
		//If the user not exist
		if(!destiny_client)
			return(this->ErrorNoSuchNick(this->_client, first_param));
		//If There is nothing to send
		if(msg.empty())
			return(this->ErrorNoTextToSend(this->_client));

		response = ":";
		response += this->_client->getNick();
		response += " PRIVMSG ";
		response += first_param;
		response += " :";
		response += msg;
		response += "\r\n";

		//Confirmation to the sender
		this->SendResponse(this->_client, response);
		//Send to the receiver
		this->SendResponse(destiny_client, response);
		return(1);
	}

	//CASE 2: Send to a channel
	channel_name = received_message->getFirstParam();
	if(channel_name[0] == '#')
		channel_name.erase(0,1);

	//Si el canal no existe
	destiny_channel = this->_server->getChannel(channel_name);
	if(!destiny_channel)
		return(this->ErrorNoSuchChannel(this->_client, channel_name));

	//Si el canal esta moderado, y no tenemos voz
	if(destiny_channel->isModeActived('m') && !destiny_channel->isClientWhitVoiceMode(this->_client))
		return(this->ErrorCanNotSendToChannel(this->_client, channel_name));

	//Si el canal no permite mensajes externos y no estamos dentro
	if(destiny_channel->isModeActived('n') && !destiny_channel->isClientInChannel(this->_client))
		return(this->ErrorCanNotSendToChannel(this->_client, channel_name));

	//Si no se ha agregado un mensaje que mandar
	if(msg.empty())
		return(this->ErrorNoTextToSend(this->_client));

	response = ":";
	response += this->_client->getNick();
	response += " PRIVMSG #";
	response += destiny_channel->getName();
	response += " :";
	response += msg;
	response += "\r\n";
	return(destiny_channel->sendMsgToAllChannel(response));
}