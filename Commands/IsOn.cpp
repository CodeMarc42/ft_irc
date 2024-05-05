#include "../includes/Command.hpp"

//TODO no se que pasa bien cuando pongo mi mismo usuario como pprimero que no aparece
int Command::IsOn(Message *received_message)
{
    std::string params;
    std::string nick;
    std::string response;
    int returned_nicks;
    size_t start; 

    start = 0;
    returned_nicks = 0;

	//Check Errors
    params = received_message->getArgsInString();
    if(params.empty())
    {
    	return(Command::ErrorNeedMoreParams(this->_client, "ISON"));
    }

 	//Generate response
    response =  this->_client->getResponsePrefix();
    response +=  " 303 :";
 	while(params[start] != '\0' && params[start] != '\r')
 	{
 		while(params[start] == ' ')
 			start++;
 		while(params[start] != ' ' && params[start] != '\0' && params[start] != '\r' && params[start] != ',')
	    {
	      	nick.push_back(params[start]);
	      	start++;
	    }
	    if(!nick.empty() && this->_server->getClientByNick(nick))
	    {
	    	if(returned_nicks > 0)
	    		response += " ";
	    	response += nick;
	    	returned_nicks++;
	    }
	    nick.clear();
 	}
 	response += "\r\n";
 	return(this->SendResponse(this->_client, response));
}