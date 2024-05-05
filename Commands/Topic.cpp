#include "../includes/Command.hpp"


int Command::Topic(Message *received_message)
{
	std::string channel_name;
	std::string topic;
	std::string response;
	Channel *channel;
	int num_args;


	//If not enoff params for this command
	num_args = received_message->getNumArgs();
	if(num_args == 0)
	{
		return(Command::ErrorNeedMoreParams(this->_client, "TOPIC"));
	}

	
	//Trying to get the channel
	channel_name = received_message->getFirstParam();
	if(channel_name[0] == '#')
		channel_name.erase(0,1);
	channel = this->_server->getChannel(channel_name);
	if(!channel)
		return(Command::ErrorNoSuchChannel(this->_client, channel_name));

	//if channel exist, check if user is in the channel
	if(!channel->isClientInChannel(this->_client))
		return(Command::ErrorNotOnChannel(this->_client, channel_name));

	if(num_args == 1)
	{
		std::cout << "ENTRO EN CASO SOLO UN ARGUMENTO" << std::endl;
		topic = channel->getTopic();
		if(topic.empty())
		{
			response = this->_client->getResponsePrefix();
			response += " 331 #";
			response += channel->getName();
			response += " :No topic is set.";
			response += "\r\n";
			this->SendResponse(this->_client, response);
		}
		else
		{
			response = this->_client->getResponsePrefix();
			response += " 332 #";
			response += channel->getName();
			response += " :";
			response += topic;
			response += "\r\n";
			this->SendResponse(this->_client, response);
		}
		return(1);
	}

	//if topic string is given, check if user can change the topic
	if(channel->isModeActived('t') && !channel->isClientChannelOperator(this->_client))
		return(Command::ErrorYouAreNotChannelOperator(this->_client, channel_name));

	//If User have permisions
	topic = received_message->getAllFromParamNumber(2);
	if(topic.empty() || (topic.length() == 1 && topic[0] == ':'))
	{
		channel->setTopic("");
	}
	else
	{
		if(topic[0] == ':')
			topic.erase(0,1);
		channel->setTopic(topic);
	}

	response = this->_client->getResponsePrefix();
	response += " TOPIC #";
	response += channel->getName();
	response += " :";
	response += channel->getTopic();
	response += "\r\n";
	channel->sendMsgToAllChannel(response);	
	return(1);
}