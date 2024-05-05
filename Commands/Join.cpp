#include "../includes/Command.hpp"

int Command::Join(Message *received_message)
{
	Channel *channel;
	std::string response;
	std::string topic;
	std::string channel_name;
	std::vector<Client *> clients;
	std::vector<Client *> operators;
	std::string args;
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	std::vector<std::string>::iterator keys_iterator;

	//If not enoff params for this command
	if(received_message->getNumArgs() == 0)
	{
		return(Command::ErrorNeedMoreParams(this->_client, "JOIN"));
	}

	args = received_message->getFirstParam();
	channels = UtilsController::SplitArgument(args);

	if(received_message->getNumArgs() > 1)
		keys = UtilsController::SplitArgument(received_message->getSecondParam());
	keys_iterator = keys.begin();

	for(std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		channel_name = *it;
		if(channel_name[0] != '#')
		{
			Command::ErrorChannelMustStartWhitHashMark(this->_client, channel_name);
			continue;
		}
		channel_name.erase(0,1);
		channel = this->_server->getChannel(channel_name);
		if(channel)
		{
			//Check if it is invite only
			//Check if i m in ban list
			//Check channel is full
			if(channel->isModeActived('i') && !channel->isClientInInvitedList(this->_client))
			{
				Command::ErrorInviteOnlyChannel(this->_client, channel_name);
				continue;
			}
			if(channel->isBanned(this->_client))
			{
				Command::ErrorBannedFromChannel(this->_client, channel_name);
				continue;
			}
			if(channel->isFull())
			{
				Command::ErrorChannelIsFull(this->_client, channel_name);
				continue;
			}
			//Check channel key
			if(channel->isModeActived('k'))
			{
				if(keys_iterator == keys.end() || !channel->checkPasswordMatch((*keys_iterator)))
				{
					if(keys_iterator != keys.end())
						keys_iterator++;
					Command::ErrorBadChannelKey(this->_client, channel_name);
					continue;
				}
				keys_iterator++;
			}
			channel->addClient(this->_client);
		}
		else
		{
			//if channel not existed , we create chanel and user is promoted to operator
			this->_server->addChannel(channel_name);
			channel = this->_server->getChannel(channel_name);
			channel->addOperator(this->_client);
		}
		
		this->_client->addChannel(channel);

		response =  this->_client->getResponsePrefix();
		response += " JOIN :#";
		response += channel->getName();
		response += "\r\n";
		channel->sendMsgToAllChannel(response);

		//Send Channel Topic information
		topic = channel->getTopic();
		response = this->_client->getResponsePrefix();
		response += " 332 #";
		response += channel->getName();
		response += " :";
		response += topic;
		response += "\r\n";
		this->SendResponse(this->_client, response);


		//SHOW JOINED CLIENT WHO IS IN THE CHANNEL
		//SHOW OPERATORS CONECTEDS
		operators = channel->getOperators();
		for(std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); ++it)
		{
			response = ":127.0.0.1 353 ";
			response += (*it)->getNick();
			response += " = #";
			response += channel->getName();
			response += " :@";
			response += (*it)->getNick();
			response += "\r\n";
			this->SendResponse(this->_client, response);

		}

		//SHOW CLIENGTS CONECTEDS
		clients = channel->getClients();
		for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			response = ":127.0.0.1 353 ";
			response += (*it)->getNick();
			response += " = #";
			response += channel->getName();
			response += " :";
			response += (*it)->getNick();
			response += "\r\n";
			this->SendResponse(this->_client, response);
		}

		//END NAMES LIST
		response = ":127.0.0.1 366 ";
		response += this->_client->getNick();
		response += " #";
		response += channel->getName();
		response += " :End of /NAMES list.";
		response += "\r\n";
		this->SendResponse(this->_client, response);

	}

	return(1);
}