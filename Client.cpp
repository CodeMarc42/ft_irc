#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include "includes/Client.hpp"
#include "includes/Message.hpp"
#include <sys/poll.h>
#include <iostream>
#include "includes/ft_irc.h"

Client::Client(int socket_fd): _socket_fd(socket_fd)
{
	this->_message = new Message("");
	this->_host = "localhost";
	this->_end_conection = 0;
	this->_modes = "ix";
	this->_conected = 0;
	//std::cout << "Client constructor" << std::endl; 

}

Client::~Client()
{
    delete this->_message;
    //std::cout << "Client Destructor for  " << this->_nick << std::endl; 

    for(std::vector<Channel *>::iterator it = this->_invited.begin(); it != this->_invited.end(); ++it)
    {   
        (*it)->removeClientFromInviteds(this);
    }
}

std::string Client::getName() const
{
	return(this->_name);
}

std::string Client::getNick() const
{
	return(this->_nick);	
}

std::string Client::getUser() const
{
	return(this->_user);
}

std::string Client::getHost() const
{
	return(this->_host);
}

std::string Client::getIp() const
{
	return(this->_ip);
}

std::string Client::getModesString() const
{
	return(this->_modes);
}

int Client::getCapSteep() const
{
	return(this->_cap_steep);
}

int Client::getSocketFd() const
{
	return(this->_socket_fd);
}

Message *Client::getMessage()
{
	return(this->_message);
}

std::string Client::list_channels_in_str()
{
	std::string channel_list_str;

	for(std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{
		channel_list_str += "#";
		channel_list_str += (*it)->getName();
	}
	return(channel_list_str);
}

int Client::setName(std::string name)
{
	this->_name = name;
	std::cout << "Set Name: |" << this->_name << "|" << std::endl;
	return(1);
}

int Client::setNick(std::string nick)
{
	this->_nick = nick;
	std::cout << "Set NickName: |" << this->_nick << "|" << std::endl;
	return(1);
}

int Client::setUser(std::string user)
{
	this->_user = user;
	std::cout << "Set User: |" << this->_user << "|" << std::endl;
	return(1);
}

void Client::setRegisteredOn()
{
	this->_conected = 1;
}

void Client::setCapSteep(int steep)
{
	this->_cap_steep = steep;
}

void Client::setIp(std::string ip)
{
	this->_ip = ip;
}

void Client::setSocketFd(int fd)
{
	this->_socket_fd = fd;
}

void Client::addChannel(Channel *channel)
{
	this->_channels.push_back(channel);
}

void Client::addChannelToInvitedList(Channel *channel)
{
	if(!this->isChannelInInvitedList(channel))
	{
		this->_invited.push_back(channel);
	}
}

void Client::removeChannelFromInvitedList(Channel *channel)
{
	if(this->isChannelInInvitedList(channel))
	{
		for(std::vector<Channel *>::iterator it = this->_invited.begin(); it != this->_invited.end(); ++it)
		{   
		    if((*it) == channel)
		    {
		    	this->_invited.erase(it);
		    	return ;
		    }
		}
	}
}

bool	Client::isChannelInInvitedList(Channel *channel)
{
	for(std::vector<Channel *>::iterator it = this->_invited.begin(); it != this->_invited.end(); ++it)
	{   
	    if((*it) == channel)
	      return(1);
	}
	return(0);
}

bool	Client::isRegistered()
{
	if(this->_conected)
		return(true);
	return(false);
}


void Client::removeChannel(Channel *channel)
{
	Channel *tmp_channel;

	tmp_channel = NULL;

	// std::cout << "LLAMAN A BORRAR EL CANAL" << std::endl;
	for(std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
	{   
	    tmp_channel = *it;
	    if(tmp_channel == channel)
	    {
	    	this->_channels.erase(it);
	    	return ;
	    }
	}
}

std::string Client::getResponsePrefix()
{
	std::string response;

	response = ":" + _nick + "!" + _user + "@" + _host;
	return(response);
}

std::vector<Channel *> Client::getCopyOfVectorOfChannels()
{
	return(this->_channels);
}

void Client::setEndConection()
{
	this->_end_conection = 1;
}

bool	Client::wantToDisconect()
{
	if(this->_end_conection)
		return(true);
	return(false);
}


// bool isClientLoggedToChannel(std::string channel_name)
// {
// 	vector<Channel *>::iterator it;

// 	for (it = this->_channels.begin(); it != this->_channels.end(); it++)
// 	{
// 	    if ((*it)->getName() == channel_name)
// 	    {
// 	        return(true);
// 	    }
// 	}
// 	return(false);
// }

