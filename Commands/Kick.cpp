#include "../includes/Command.hpp"

//https://dd.ircdocs.horse/refs/commands/kick
int Command::Kick(Message *received_message)
{
	std::string kicked_user_nick;
	std::string channel_name;
	std::string response;
	std::string reason;
	Channel *channel;
	Client *kicked_user;
	std::vector<Client *> clients;

	//If not enoff params for this command
	if(received_message->getNumArgs() < 2)
	{
		return(Command::ErrorNeedMoreParams(this->_client, "KICK"));
	}

	//Take target channel
	channel_name = received_message->getFirstParam();
	if(channel_name[0] == '#')
		channel_name.erase(0,1);
	channel = this->_server->getChannel(channel_name);

	//Take target user
	kicked_user_nick = received_message->getSecondParam();
	kicked_user = this->_server->getClientByNick(kicked_user_nick);

	//Take Reason of kick , if no reason sent then the reason is th UserNick
	if(received_message->getNumArgs() > 2)
		reason = received_message->getParamNumber(3);
	if(reason.empty())
		reason = this->_client->getNick();

	//If channel or user don t exist
	if(!channel || !kicked_user)
	{
		return (Command::ErrorNoSuchNick(this->_client, kicked_user_nick));
	}

	//If comand sender is not a operator
	if(!channel->isClientChannelOperator(this->_client))
	{
		return (Command::ErrorYouAreNotChannelOperator(this->_client, channel_name));
	}

	//If the target user is not in channel
	if(!channel->isClientInChannel(kicked_user))
	{
		return (Command::ErrorUserNotInChannel(this->_client,  kicked_user_nick, channel_name));
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
			response = ":";
			response += this->_client->getNick();
			response += "!";
			response += this->_client->getUser();
			response += "@127.0.0.1 KICK #";
			response += channel->getName();
			response += " ";
			response += kicked_user_nick;
			response += " :";
			response += reason;
			response += "\r\n";
		    this->SendResponse((*it), response);
		}
	}

	kicked_user->removeChannel(channel);
	channel->removeClient(kicked_user);

	return(1);
}