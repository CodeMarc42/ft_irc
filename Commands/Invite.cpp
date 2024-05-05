#include "../includes/Command.hpp"

int Command::Invite(Message *received_message)
{
	std::string channel_name;
	Client *target_client;
	Channel *channel;
	std::string response;
	int num_args;
	//int sender_is_moderator;


	//If not enoff params for this command
	num_args = received_message->getNumArgs();
	if(num_args < 2)
	{
		return(Command::ErrorNeedMoreParams(this->_client, "INVITE"));
	}

	//TRying to get the target user
	target_client = this->_server->getClientByNick(received_message->getFirstParam());
	if(!target_client)
		return(Command::ErrorNoSuchNick(this->_client, received_message->getFirstParam()));

	//Trying to get the channel
	channel_name = received_message->getSecondParam();
	if(channel_name[0] == '#')
		channel_name.erase(0,1);
	channel = this->_server->getChannel(channel_name);
	if(!channel)
		return(Command::ErrorNoSuchChannel(this->_client, channel_name));

	//if channel exist, check if the sender user is in the channel
	if(!channel->isClientInChannel(this->_client))
		return(Command::ErrorNotOnChannel(this->_client, channel_name));

	//If mode i is activated only op can do the action
	if(channel->isModeActived('i'))
	{
		if(!channel->isClientChannelOperator(this->_client))
			return(Command::ErrorYouAreNotChannelOperator(this->_client, channel_name));
	}

	//If client is currently in channel , send error client is in channel
	if(channel->isClientInChannel(target_client))
		return(Command::ErrorUserOnChannel(this->_client, channel_name, target_client->getNick()));

	if(channel->isClientChannelOperator(this->_client))
	{
		channel->addClientToInviteds(target_client);
		target_client->addChannelToInvitedList(channel);
	}

	//Send response to Sender
	response = this->_client->getResponsePrefix();
	response += " 341 :";
	response += target_client->getNick();
	response += " #";
	response += channel->getName();
	response += "\r\n";
	this->SendResponse(this->_client, response);

	//Send respond to target user
	response = this->_client->getResponsePrefix();
	response += " INVITE ";
	response += target_client->getNick();
	response += " #";
	response += channel->getName();
	response += "\r\n";
	this->SendResponse(target_client, response);
	return(1);
}