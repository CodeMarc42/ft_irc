#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include "includes/Channel.hpp"
#include <sys/poll.h>
#include <iostream>
#include "includes/ft_irc.h"

Channel::Channel()
{
	
}

Channel::Channel(std::string name)
{
	int i;

	i = 0;
	//std::cout << "Channel " << name << " constructor" << std::endl;
	while(name[i] != '\n' && name[i] != '\r' && name[i] != '\0')
	{
		this->_name.push_back(name[i]);
		i++;
	}
	this->_modes = "nt";
	this->_max_users = 0;
	this->_num_users = 0;
}

Channel::~Channel()
{
	//std::cout << "Channel " << this->_name << " destructor" << std::endl;
}

std::string Channel::getName() const
{
	return this->_name;
}

std::vector<Client *> Channel::getClients() const
{
	return this->_clients;
}

std::vector<Client *> Channel::getOperators() const
{
	return this->_operators;
}

std::string Channel::getBanList() const
{
	return this->_ban_list;
}

std::string Channel::getModesString() const
{
	return this->_modes;
}

int Channel::getNumUsers() const
{
	return this->_num_users;
}

int Channel::getMaxUsers() const
{
	return this->_max_users;
}

std::string Channel::getTopic() const
{
	return this->_topic;
}

void Channel::setPassword(std::string password)
{
	this->_password = password;
}

void Channel::setNumUsers(int num)
{
	this->_num_users = num;
}

void Channel::setMaxUsers(int num)
{
	this->_max_users = num;
}
void Channel::setTopic(std::string topic)
{
	this->_topic = topic;
}


void	Channel::addClient(Client *client)
{
	if(!this->isClientInChannel(client))
	{
		this->_clients.push_back(client);
		this->_num_users++;
		//std::cout << "Client has been added to channel" << std::endl;
	}
}

void	Channel::addOperator(Client *client)
{
	if(this->isClientInChannel(client))
	{
		this->removeClient(client);
	}
	this->_operators.push_back(client);
	this->_num_users++;
		//std::cout << "Client has been added to channel, and upped to operator" << std::endl;
	// }
}

void	Channel::addClientToInviteds(Client *client)
{
	if(!this->isClientInInvitedList(client))
	{
		this->_inviteds.push_back(client);
	}
}

void	Channel::removeClient(Client *client)
{
	Client *tmp_client;

	tmp_client = NULL;

	//remove client from channel voice string if him is inside
	this->removeClientFromVoice(client);

	//remove client from clients if he is in the clients vector
	for(std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client == client)
	    {
	    	this->_clients.erase(it);
	    	this->_num_users--;
	    	return ;
	    }
	}
	//remove client from operators if he is in the operators vector
	for(std::vector<Client *>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client == client)
	    {
	    	this->_operators.erase(it);
	    	this->_num_users--;
	    	return ;
	    }
	}
	//std::cout << "Client not removed from channel, cause client not found in it" << std::endl; 
}

void	Channel::removeClientFromInviteds(Client *client)
{
	if(this->isClientInInvitedList(client))
	{
		for(std::vector<Client *>::iterator it = this->_inviteds.begin(); it != this->_inviteds.end(); ++it)
		{   
		    if((*it) == client)
		    {
		    	this->_inviteds.erase(it);
		    	return ;
		    }
		}
	}
}

void	Channel::downGradeOperator(Client *client)
{
	std::cout << "In downGradeOperator" << std::endl;
	if(this->isClientChannelOperator(client))
	{
		for(std::vector<Client *>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
		{   
		    //tmp_client = *it;
		    if((*it) == client)
		    {
		    	this->_operators.erase(it);
		    	this->_clients.push_back(client);
		    	std::cout << "Operator has been downgrade to normal channel user" << std::endl;
		    	return ;
		    }
		}
		std::cout << "Operator has NOT been downgrade, cause user was not a operator" << std::endl;
		return ;
	}
}

void	Channel::upGradeToOperator(Client *client)
{
	if(!this->isClientChannelOperator(client))
	{
		this->removeClient(client);
		this->addOperator(client);
		//std::cout << "Client has been promoted to channel operators" << std::endl;
		return ;
	}
	//std::cout << "Client NOT promoted to channel operators, cause client not found" << std::endl;
}

bool	Channel::isClientInChannel(Client *client)
{
	Client *tmp_client;

	for(std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client->getNick() == client->getNick())
	      return(1);
	}
	for(std::vector<Client *>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client->getNick() == client->getNick())
	      return(1);
	}
	return(0);
}

bool	Channel::isClientChannelOperator(Client *client)
{
	Client *tmp_client;

	for(std::vector<Client *>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client->getNick() == client->getNick())
	      return(1);
	}
	return(0);
}

bool	Channel::isClientInInvitedList(Client *client)
{
	Client *tmp_client;

	//std::cout << "CHEKING IF CLIENT IS IN IVITE LIST" << std::endl;
	for(std::vector<Client *>::iterator it = this->_inviteds.begin(); it != this->_inviteds.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client == client)
	      return(1);
	}
	return(0);
}

bool	Channel::checkPasswordMatch(std::string password)
{
	if(password == this->_password)
		return(true);
	return(false);
}

void 	Channel::addBan(std::string ban_mask)
{

	//std::cout << "SOY CHANNEL ME PIDEN BANEAR A!!!!! -> " << ban_mask << std::endl;
	if(this->_ban_list.find(ban_mask) == std::string::npos)
	{
		this->_ban_list += ban_mask;
		this->_ban_list += " ";
		//std::cout << "NUEVA BAN LIST: " << this->_ban_list << std::endl;
	}
}

void 	Channel::removeBan(std::string ban_mask)
{
	//std::cout << "SOY CHANNEL ME PIDEN DESBANEAR A!!!!! -> " << ban_mask << std::endl;
	
	size_t pos;

	pos = this->_ban_list.find(ban_mask);

	if(pos != std::string::npos)
	{
		this->_ban_list.erase(pos, ban_mask.length());
		//std::cout << "NUEVA BAN LIST: " << this->_ban_list << std::endl;
	}
}

bool	Channel::isBanned(Client *target_client)
{
	//std::cout << "MIRANDO SI USUARIO ESTA BANEADO!!!!!" << std::endl;
	//std::cout << "BAN LIST: " << this->_ban_list << std::endl;
	// if(this->_ban_list.find(target_client->getNick() + "!" + target_client->getUser() + "@" + target_client->getHost()) != std::string::npos)
	// {
	// 	return(1);
	// }
	// if(this->_ban_list.find("*!" + target_client->getUser() + "@" + target_client->getHost()) != std::string::npos)
	// {
	// 	return(1);
	// }
	// if(this->_ban_list.find(target_client->getNick() + "!*@" + target_client->getHost()) != std::string::npos)
	// {
	// 	return(1);
	// }
	// if(this->_ban_list.find(target_client->getNick() + "!*@*") != std::string::npos)
	// {
	// 	return(1);
	// }
	// if(this->_ban_list.find("*!*@" + target_client->getHost()) != std::string::npos)
	// {
	// 	return(1);
	// }
	std::string client_mask;
	std::string	banned_mask;
	int ban_list_index = 0;

	client_mask = target_client->getNick() + "!" + target_client->getUser() + "@" + target_client->getHost();

	while(this->_ban_list[ban_list_index])
	{
		//Take a mask from the ban_list
		while(this->_ban_list[ban_list_index] != ' ' && this->_ban_list[ban_list_index] != '\0')
		{
			banned_mask.push_back(this->_ban_list[ban_list_index]);
			ban_list_index++;
		}

		//If we can t take a new mask
		if(banned_mask.empty())
			return(false);

		if(this->checkMaskPattern(client_mask.c_str(), banned_mask.c_str()))
			return true;

		banned_mask.clear();
		//keep advancing if no end
		if(this->_ban_list[ban_list_index] != '\0')
			ban_list_index++;
	}
	return(false);
}

int Channel::checkMaskPattern(const char* client_mask, const char* banned_mask)
{

    int wildcard = 0;

    const char* last_banned_mask_start = 0;
    const char* last_client_mask_start = 0;
    do
    {
        if (*banned_mask == *client_mask)
        {
            if(wildcard == 1)
                last_client_mask_start = client_mask + 1;

            client_mask++;
            banned_mask++;
            wildcard = 0;
        }
        else if (*banned_mask == '?')
        {
            if(*(client_mask) == '\0') // the line is ended but char was expected
                return 0;
            if(wildcard == 1)
                last_client_mask_start = client_mask + 1;
            client_mask++;
            banned_mask++;
            wildcard = 0;
        }
        else if (*banned_mask == '*')
        {
            if (*(banned_mask + 1) == '\0')
                return 1;

            last_banned_mask_start = banned_mask;
            //last_client_mask_start = line + 1;
            wildcard = 1;

            banned_mask++;
        }
        else if (wildcard)
        {
            if (*client_mask == *banned_mask)
            {
                wildcard = 0;
                client_mask++;
                banned_mask++;
                last_client_mask_start = client_mask + 1 ;
            }
            else
            {
                client_mask++;
            }
        }
        else
        {
            if ((*banned_mask) == '\0' && (*client_mask) == '\0')  // end of mask
                return 1; // if the line also ends here then the pattern match
            else
            {
                if (last_banned_mask_start != 0) // try to restart the mask on the rest
                {
                    banned_mask = last_banned_mask_start;
                    client_mask = last_client_mask_start;
                    last_client_mask_start = 0;
                }
                else
                    return 0;
            }
        }

    } while (*client_mask);

    if (*banned_mask == '\0')
        return 1;
	return(0);
}

bool	Channel::isModeActived(char mode) const
{
	int i;

	i = 0;
	while(this->_modes[i])
	{
		if(this->_modes[i] == mode)
			return true;
		i++;
	}
	return false;
}

bool	Channel::isFull() const
{
	std::cout << "Numero de usuarios" << this->_num_users <<  " max users " << this->_max_users << std::endl;
	if(this->isModeActived('l') && (this->_num_users >= this->_max_users))
		return (true);
	return (false);
}

void	Channel::setMode(char mode)
{
	if(!Channel::isModeActived(mode))
		this->_modes.push_back(mode);
}

void	Channel::removeMode(char mode)
{
	std::string new_mode;

	for (size_t i = 0; i < this->_modes.size(); i++) 
	{
       if (this->_modes[i] != mode)
           new_mode.push_back(this->_modes[i]);
	}
	this->_modes = new_mode;
}

bool	Channel::isClientWhitVoiceMode(Client *client)
{
	Client *tmp_client;

	for(std::vector<Client *>::iterator it = this->_voice_users.begin(); it != this->_voice_users.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client->getNick() == client->getNick())
	      return(1);
	}
	for(std::vector<Client *>::iterator it = this->_operators.begin(); it != this->_operators.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client->getNick() == client->getNick())
	      return(1);
	}
	return(0);
}

void	Channel::addClientToVoice(Client *client)
{
	Client *tmp_client;

	for(std::vector<Client *>::iterator it = this->_voice_users.begin(); it != this->_voice_users.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client == client)
	    {
	      	return ;
	    }
	}
	this->_voice_users.push_back(client);
}

int	Channel::removeClientFromVoice(Client *client)
{
	Client *tmp_client;

	for(std::vector<Client *>::iterator it = this->_voice_users.begin(); it != this->_voice_users.end(); ++it)
	{   
	    tmp_client = *it;
	    if(tmp_client == client)
	    {
	      	this->_voice_users.erase(it);
	      	return(1);
	    }
	}
	return(0);
}

int	Channel::sendMsgToAllChannel(std::string response)
{
	int temp;
	std::vector<Client *> clients;

	clients = this->getOperators();
	//std::cout << "COJO OPERADORES" << std::endl;
	for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{

		//std::cout << "OPERADOR" << std::endl;
		//MSG SEND TO THE OPERATOR
		temp = send((*it)->getSocketFd(), response.c_str(), response.length(), 0);
		if (temp < 0)
		{
			//std::cout << "Error sending" << std::endl;
			 perror("send() failed");
			 //end_con = true;
			 //break;
		}
	}
	
	clients = this->getClients();
	//std::cout << "COJO CLIENTES" << std::endl;
	for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{

		//std::cout << "USUARIO" << std::endl;
		//MSG SEND TO THE OPERATOR
		temp = send((*it)->getSocketFd(), response.c_str(), response.length(), 0);
		if (temp < 0)
		{
			//std::cout << "Error sending" << std::endl;
			 perror("send() failed");
			 //end_con = true;
			 //break;
		}
	}
	//std::cout << "TERMINO sendMsgToAllChannel" << std::endl;
	return(temp);
}

int	Channel::sendNoticeToChannel(Client *sender, std::string response)
{
	int temp;
	std::vector<Client *> clients;

	clients = this->getOperators();
	//std::cout << "COJO OPERADORES" << std::endl;
	for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{

		//std::cout << "OPERADOR" << std::endl;
		//MSG SEND TO THE OPERATOR
		if(sender != (*it))
		{
			temp = send((*it)->getSocketFd(), response.c_str(), response.length(), 0);
			if (temp < 0)
			{
				//std::cout << "Error sending" << std::endl;
				 perror("send() failed");
				 //end_con = true;
				 //break;
			}
		}
	}
	
	clients = this->getClients();
	//std::cout << "COJO CLIENTES" << std::endl;
	for(std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if(sender != (*it))
		{
			//MSG SEND TO THE OPERATOR
			temp = send((*it)->getSocketFd(), response.c_str(), response.length(), 0);
			if (temp < 0)
			{
				//std::cout << "Error sending" << std::endl;
				 perror("send() failed");
				 //end_con = true;
				 //break;
			}
		}
	}
	//std::cout << "TERMINO sendMsgToAllChannel" << std::endl;
	return(temp);
}


std::vector<Client *> Channel::getCopyOfVectorOfClients()
{
	return(this->_clients);
}

std::vector<Client *> Channel::getCopyOfVectorOfOperators()
{
	return(this->_operators);
}



// int sendResponseToAllChannel()
// {

// }
