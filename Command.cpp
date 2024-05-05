#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstring>
#include <netinet/in.h>
#include <errno.h>
#include "includes/Command.hpp"
#include "includes/Message.hpp"
#include "includes/Server.hpp"
#include <sys/poll.h>
#include <iostream>
#include "includes/ft_irc.h"
#include <iterator>

Command::Command(Client *cliente, Server *server)
{
	//std::cout << "Command constructor" << std::endl;
	this->_client = cliente;
	this->_server = server;
}

Command::~Command()
{
	//std::cout << "Command destructor" << std::endl;
}

int Command::Exec()
{
	return(this->SearchAndExec());
}

int Command::SearchAndExec()
{
	Message *received_message;
	std::string command_name;

	received_message = this->_client->getMessage();
	command_name = received_message->getCommandName();

	std::cout << "COMANDO RECIBIDO: |" << command_name << "| " << "(buffer es = " << this->_client->buffer << ")" << std::endl;

	if(!this->_client->isRegistered())
	{	
		if(std::strncmp(received_message->getMessageStr(), "CAP LS", 6) == 0)
		{
			return(this->NewClientRequest(received_message));
		}
		else if(std::strncmp(received_message->getMessageStr(), "CAP REQ ",8) == 0)
		{
			return(this->NewClientRequest2());
		}
		else if(std::strncmp(received_message->getMessageStr(), "CAP END", 7) == 0)
		{
			return(this->NewClientRequest3());
		}
		else if(std::strcmp(command_name.c_str(), "PING") == 0)
		{
			return(Command::Ping(received_message));
		}
	}
	

	if(this->_client->isRegistered())
	{
		//std::cout << "ENTERING TO REGISTERED COMMANDS" << std::endl;

		if(std::strcmp(command_name.c_str(), "PING") == 0)
		{
			return(Command::Ping(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "QUIT") == 0)
		{
			return(Command::Quit(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "NICK") == 0)
		{
			return(Command::Nick(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "JOIN") == 0)
		{
			return(Command::Join(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "PRIVMSG") == 0)
		{
			return(Command::PrivtMsg(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "ISON") == 0)
		{
			return(Command::IsOn(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "PART") == 0)
		{
			return(Command::Part(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "MODE") == 0)
		{
			return(Command::Mode(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "TOPIC") == 0)
		{
			return(Command::Topic(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "INVITE") == 0)
		{
			return(Command::Invite(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "WHO") == 0)
		{
			return(Command::Who(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "WHOIS") == 0)
		{
			return(Command::WhoIs(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "KICK") == 0)
		{
			return(Command::Kick(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "WATCH") == 0)
		{
			return(Command::Watch(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "NOTICE") == 0)
		{
			return(Command::Notice(received_message));
		}
		else if(std::strcmp(command_name.c_str(), "CLOSE") == 0)
		{
			std::cout << "COMANDO CLOSE" << std::endl;
			this->_server->Exit();
		}
		else if(std::strncmp(received_message->getMessageStr(), "NICKSERV IDENTIFY ", 18) == 0)
		{
			return(Command::Identify());
		}
		else
		{
			return(Command::ErrorUnknownCommand(this->_client, command_name.c_str()));
		}
	}	
	return(0);
}

int Command::ReplyNotice(std::string notice_msg)
{
	std::string response;

	response = ":";
	response += "127.0.0.1";
	response += " NOTICE";
	response += " * *** ";
	response += notice_msg;
	response += "\r\n";
	return (this->SendResponse(this->_client, response));
}


int Command::NewClientRequest(Message *received_message)
{
	int temp;
	std::string msg;
	std::string name;
	std::string nick;
	Client *check_nick;
	std::string user;
	std::string password;
	std::string response;

	//EXTRACT CLIENT NAME
	msg = received_message->getMessageStr();

	// //Cheking server password!!!
	// password = CapController::extractPass(msg);
	// if(!this->_server->isServerPassword(password))
	// {
	// 	Command::ErrorPasswordMissMatch(this->_client);
	// 	this->_client->_end_conection = 1;
	// 	return 0;
	// }

	name = CapController::extractUserName(msg);
	this->_client->setName(name);
	nick = CapController::extractNickName(msg);
	check_nick = this->_server->getClientByNick(nick);
	if(check_nick)
	{
		//ERROR Closing link: (marza-ga@195.55.43.195)
		Command::ErrorNicknameInUse(this->_client, nick);
		response =  ":127.0.0.1 ERROR Closing link: (";
		response += nick;
		response += "@localhost)";
		response += "\r\n";
		this->SendResponse(this->_client, response);

		response =  ":127.0.0.1 QUIT";
		response += "\r\n";
		this->SendResponse(this->_client, response);
		return(1);
	}
	this->_client->setNick(nick);
	user = CapController::extractUser(msg);
	this->_client->setUser(user);


	temp = this->ReplyNotice("Looking up your hostname...");
	temp = this->ReplyNotice("Checking ident...");
	temp = this->ReplyNotice("Couldn't resolve your hostname; using your IP address instead");
	temp = this->ReplyNotice("No ident response; username prefixed with ~");
	
	response = ":127.0.0.1 CAP * LS :unrealircd.org/link-security=2 unrealircd.org/plaintext-policy=user=allow,oper=deny,server=deny unrealircd.org/history-storage=memory extended-join chghost cap-notify userhost-in-names multi-prefix away-notify sasl=EXTERNAL,PLAIN tls account-notify message-tags batch server-time account-tag echo-message labeled-response draft/chathistory\r\n";
	this->SendResponse(this->_client, response);
	this->_client->setCapSteep(1);
	return(temp);
}

int Command::NewClientRequest2()
{
	if(this->_client->getCapSteep() != 1)
	{
		return(0);
	}
	
	std::string response = ":127.0.0.1 CAP * ACK extended-join chghost cap-notify userhost-in-names multi-prefix away-notify tls account-notify server-time account-tag echo-message \r\n";
	this->SendResponse(this->_client, response);
	this->_client->setCapSteep(2);
	return(1);
}

int Command::NewClientRequest3()
{
	std::string response;

	if(this->_client->getCapSteep() != 2)
	{
		return(0);
	}

	response = ":127.0.0.1 001 ";
	response += this->_client->getNick();
	response += " :Welcome to IRC.Marc&Jaime.ProTeam, ";
	response += this->_client->getNick();
	response += "!";
	response += "\r\n";
	this->SendResponse(this->_client, response);

	response = ":127.0.0.1 002 ";
	response += this->_client->getNick();
	response += " :Your host is 127.0.0.1, running version UnrealIRCd-5.2.2-git\r\n";
	this->SendResponse(this->_client, response);

	response = ":127.0.0.1 003 ";
	response += this->_client->getNick();
	response += " :This server was created Wed Mar 23 2022 at 13:36:19 UTC\r\n";
	this->SendResponse(this->_client, response);

	response = ":127.0.0.1 004 ";
	response += this->_client->getNick();
	response += " :127.0.0.1 UnrealIRCd-5.2.2-git iowrsxzdHtIDZRqpWGTSB lvhopsmntikraqbeIHzMQNRTOVKDdGLPZSCcf\r\n";
	this->SendResponse(this->_client, response);

	response = ":127.0.0.1 005 ";
	response += this->_client->getNick();
	response += ":EXTBAN=~,GptmTSOcarnqjf INVEX KICKLEN=307 KNOCK MAP MAXCHANNELS=50 MAXLIST=b:100,e:100,I:100 MAXNICKLEN=30 MINNICKLEN=0 MODES=12 NAMESX NETWORK=SwiftIRC are supported by this server\r\n";
	this->SendResponse(this->_client, response);
	this->_client->setRegisteredOn();
	return(1);
}

int Command::Identify()
{
	std::string received_password;
	std::string response;

	received_password = this->_client->buffer;
	received_password = received_password.substr(18, received_password.length() - 20);
	std::cout << "|" << received_password << "|" << std::endl;
	this->_client->buffer[0] = '\0';
	if(this->_server->isServerPassword(received_password))
	{
		response = ":NickServ!NickServ@services. NOTICE ";
		response += this->_client->getNick();
		response += " :Password accepted - you are now recognized.\r\n";
		this->SendResponse(this->_client, response);
		return(1);
	}
	else
	{
		response = ":NickServ!NickServ@services. NOTICE ";
		response += this->_client->getNick();
		response += " :Password Incorrect\r\n";
		this->SendResponse(this->_client, response);
		return(0);
	}


}


int Command::SendResponse(Client *client, std::string response)
{
  int temp;

  temp = send(client->getSocketFd(), response.c_str(), response.length(), 0);
  if (temp < 0)
  {
   		perror("send() failed");
  }
  return(temp);
}
