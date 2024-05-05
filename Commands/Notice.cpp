#include "../includes/Command.hpp"


//TODO ERR_TOOMANYTARGETS (407)

int Command::Notice(Message *received_message)
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
		return (0);
	}

	temp = 0;
	first_param = received_message->getFirstParam();
	msg = received_message->getAllFromParamNumber(2);
	if(msg[0] == ':')
		msg.erase(0,1);

	//CASE 0 DCC
	std::string second_param = received_message->getParamNumber(2);
	if(std::strcmp(second_param.c_str(), ":\1DCC") == 0)
	{
		std::cout << "Detectado DCC en NOTICE" << std::endl;
		return(Command::Dcc(received_message));
	}
	
	//CASE 1: Send to a user
	if(first_param[0] != '#')
	{
		destiny_client = this->_server->getClientByNick(first_param);
		//If the user not exist
		if(!destiny_client)
			return(0);
		//If There is nothing to send
		if(msg.empty())
			return(0);

		response = ":";
		response += this->_client->getNick();
		response += " NOTICE ";
		response += first_param;
		response += " :";
		response += msg;
		response += "\r\n";

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
		return(0);

	//Si el canal esta moderado, y no tenemos voz
	if(destiny_channel->isModeActived('m') && !destiny_channel->isClientWhitVoiceMode(this->_client))
		return(0);

	//Si el canal no permite mensajes externos y no estamos dentro
	if(destiny_channel->isModeActived('n') && !destiny_channel->isClientInChannel(this->_client))
		return(0);

	//Si no se ha agregado un mensaje que mandar
	if(msg.empty())
		return(0);

	response = this->_client->getResponsePrefix();
	response += " ";
	response += response += this->_client->getNick();
	response += " NOTICE ";
	response += "#";
	response += destiny_channel->getName();
	response += " :";
	response += msg;
	destiny_channel->sendNoticeToChannel(this->_client, response);
	return(1);
}