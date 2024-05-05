#include "../includes/Command.hpp"

// QUIT me voy a tocarme los huevs
// ERROR Closing link: (marza-ga@195.55.43.195) [Quit: A]
// ERROR Closing link: (marza-ga@195.55.43.195) [Quit: me voy a tocarme los huevs]

//:dan-!d@localhost QUIT :Quit: Bye for now!
int Command::Quit(Message *received_message)
{
	std::vector<Channel *> channels;
	Channel *channel;
	std::string response;
	std::string reason;

	if(received_message->getNumArgs() != 0)
		reason = received_message->getArgsInString();

	channels = this->_client->getCopyOfVectorOfChannels();
	for(std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{   

		channel = (*it);
		this->_client->removeChannel(channel);
		channel->removeClient(this->_client);

		if(channel->getNumUsers() <= 0)
			this->_server->removeChannel(channel);
		else
		{
			response =  this->_client->getResponsePrefix();
			response += " QUIT :";
			if(!reason.empty())
				response += reason;
			response += "\r\n";  
			channel->sendMsgToAllChannel(response);
		}
	}

	response =  this->_client->getResponsePrefix();
	response += " QUIT :";
	if(!reason.empty())
		response += reason;
	response += "\r\n";
	this->SendResponse(this->_client, response);
	this->_client->setEndConection();
	return(1);
}
