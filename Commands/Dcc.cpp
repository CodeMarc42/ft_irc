#include "../includes/Command.hpp"

// CTCP REQUEST loler DCC ACCEPT 32 0 0 2
// @time=2023-01-15T21:23:05.162Z marza-ga PRIVMSG loler DCC ACCEPT 32 0 0 2


//CTCP REQUEST loler DCC SEND 32 16843009 0 12 2 T
// @time=2023-01-15T21:22:57.562Z marza-ga PRIVMSG loler DCC SEND 32 16843009 0 12 2 T
// @time=2023-01-15T21:23:04.981Z loler PRIVMSG marza-ga DCC RESUME 32 0 0 2

int Command::Dcc(Message *received_message)
{
	std::string ctcp_option;
	int num_args;
	Client *destiny_client;

	num_args = received_message->getNumArgs();
	//If not enoff params for this command
	if( num_args == 0)
	{
		return(Command::ErrorNeedMoreParams(this->_client, "CTCP"));
	}

	if(received_message->getCommandName() == "PRIVMSG")
	{
		
		if(num_args < 3)
			return(Command::ErrorNeedMoreParams(this->_client, "CTCP"));
		
		destiny_client  = this->_server->getClientByNick(received_message->getFirstParam());
		if(!destiny_client)
			return(Command::ErrorNoSuchNick(this->_client, received_message->getFirstParam()));

		if(received_message->getParamNumber(3) == "SEND")
		{

			return(DccController::DccRequest(this->_client, destiny_client, received_message));
		}
		if(received_message->getParamNumber(3) == "RESUME")
		{

			return(DccController::DccResume(this->_client, destiny_client, received_message));
		}
		if(received_message->getParamNumber(3) == "ACCEPT")
		{

			return(DccController::DccAccept(this->_client, destiny_client, received_message));
		}

	}

	if(received_message->getCommandName() == "NOTICE")
	{
		
		if(num_args < 3)
			return(Command::ErrorNeedMoreParams(this->_client, "CTCP"));
		
		destiny_client  = this->_server->getClientByNick(received_message->getFirstParam());
		if(!destiny_client)
			return(Command::ErrorNoSuchNick(this->_client, received_message->getFirstParam()));

		if(received_message->getParamNumber(3) == "REJECT")
		{
			return(DccController::DccReject(this->_client, destiny_client, received_message));
		}
	}

	return(1);
}
