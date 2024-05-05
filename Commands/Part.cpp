#include "../includes/Command.hpp"

//:marza-ga!marza-ga@127.0.0.1.IP JOIN :#hola
//:marza-ga321312!marza-ga321@ihw-te6.8kr.55.195.IP PART #HOLA
int Command::Part(Message *received_message)
{
	std::string channel_name;
	std::string response;
	std::string reason;
	std::vector<std::string> channels;
	Channel *channel;

	if(received_message->getNumArgs() == 0)
	{
		return (this->ErrorNeedMoreParams(this->_client, "PART"));
	}

	channels = UtilsController::SplitPartArgument(received_message->getMessageStr());

	for(std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it)
	{   
		channel_name = *it;
	    if(channel_name[0] == '#')
	    	channel_name.erase(0,1);
	    
	    //Check channel exist and took it
	    channel = this->_server->getChannel(channel_name);
		if(!channel)
	    {
	    	this->ErrorNoSuchChannel(this->_client, channel_name);
	    	continue;
	    }
	    //Check user in channel
    	if(!channel->isClientInChannel(this->_client))
        {
        	this->ErrorNotOnChannel(this->_client, channel_name);
        	continue;
        }

        reason = received_message->getSecondParam();
	    
	    response =  this->_client->getResponsePrefix();
	    response += " PART #";
	    response += channel_name;
	    if(!reason.empty())
	    	response += " :" + reason;	 
	    response += "\r\n";	    
	    channel->sendMsgToAllChannel(response);

		this->_client->removeChannel(channel);
		channel->removeClient(this->_client);

		if(channel->getNumUsers() <= 0)
			this->_server->removeChannel(channel);
	}
	return(1);
}