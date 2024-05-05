#include "../includes/Command.hpp"

// //"<client> <nick> :Erroneus nickname"
// //432: !hola Nickname is unavailable: Illegal characters
// // :dan-!d@localhost NICK Mamoped

// @time=2023-01-05T19:21:26.465Z testing JOIN #kk * Marcos Arza garcia
// @time=2023-01-05T19:21:32.204Z testing NICK loler
// 601: testing ~marza-ga B125170E.C0ED857D.425D160E.IP 1672946492 logged offline
// 600: loler ~marza-ga B125170E.C0ED857D.425D160E.IP 1672946492 logged online
// WATCH -testing
// WATCH +loler
// ISON loler
// 602: testing * * 0 stopped watching
// 604: loler ~marza-ga B125170E.C0ED857D.425D160E.IP 1672946492 is online
// 303: loler 


int Command::Nick(Message *received_message)
{
	std::string response;
	std::string nick;
	std::vector<Client *> all_clients_in_same_channels;
	std::vector<Channel *> channels;
	Channel *tmp_channel;

	all_clients_in_same_channels.clear();
	nick = CapController::extractNickName(received_message->getMessageStr());
	if(!UtilsController::isValidNickName(nick))
	{
		return (this->ErrorNickErroneous(this->_client, nick));
	}
	else if(UtilsController::isEmptyNickName(nick))
	{
		return (this->ErrorNoNicknameGiven(this->_client, nick));
	}
	else if(!UtilsController::isNickNameNotInUse(nick, this->_server))
	{
		return (this->ErrorNicknameInUse(this->_client, nick));
	}
	else
	{

		channels = this->_client->getCopyOfVectorOfChannels();
		//std::cout << "USERS IN CHANNELS WHIT THE USER" << std::endl;
		for(std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		{   

			tmp_channel = (*it);
			std::vector<Client *> tmp_vector_of_clients = tmp_channel->getCopyOfVectorOfClients();

			for(std::vector<Client *>::iterator it = tmp_vector_of_clients.begin(); it != tmp_vector_of_clients.end(); ++it)
			{

				if(std::find(all_clients_in_same_channels.begin(), all_clients_in_same_channels.end(), (*it)) == all_clients_in_same_channels.end())
				{
					all_clients_in_same_channels.push_back((*it));
					std::cout << (*it)->getNick() << std::endl;
				}
			} 
			tmp_vector_of_clients = tmp_channel->getCopyOfVectorOfOperators();

			for(std::vector<Client *>::iterator it = tmp_vector_of_clients.begin(); it != tmp_vector_of_clients.end(); ++it)
			{

				if(std::find(all_clients_in_same_channels.begin(), all_clients_in_same_channels.end(), (*it)) == all_clients_in_same_channels.end())
				{
					all_clients_in_same_channels.push_back((*it));
					std::cout << (*it)->getNick() << std::endl;
				}
			}  
		}

		//Compose the response (it have to start whit the old nick)
		response =  this->_client->getResponsePrefix();
		//Change the nick	
		this->_client->setNick(nick);
		//Keep composing rest of the response
		response += " NICK :";
		response += nick;
		response += "\r\n";

		// //Send the response to the user
		// this->SendResponse(this->_client, response);
		
		//Send the response to users that are in same channel than the user
		for(std::vector<Client *>::iterator it = all_clients_in_same_channels.begin(); it != all_clients_in_same_channels.end(); ++it)
		{   
			this->SendResponse((*it), response);
		}
		return(1);
	}
	return(0);
}