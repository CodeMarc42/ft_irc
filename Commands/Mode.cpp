#include "../includes/Command.hpp"

//RPL_BANLIST (367)  "<client> <channel> <mask> [<who> <set-ts>]"
//strcpy(this->_client->buffer, ":127.0.0.1 368 #hola :End of Channel Ban List\r\n");

//mode #channel +b *!*@hostmask
int Command::Mode(Message *received_message)
{
	int num_args;
	Channel *target_channel;
	Client *target_client;
	std::string target_modes;
	std::string target;
	std::string response;
	std::string modestring;
	std::string response_modestring;
	std::string response_used_args;
	char signal;
	char response_signal;
	int argument_num;
	int success_command_type;
	int temp;


	signal = '+';
	response_signal = '0';
	success_command_type = 0;

	//Get the num of arguments
	num_args = received_message->getNumArgs();

	//If not enoff params for this command
	if(num_args == 0)
	{
		return(Command::ErrorNeedMoreParams(this->_client, "MODE"));
	}

	target = received_message->getFirstParam();


	//MODE WHEN TARGET IS A CHANNEL
	if(target[0] == '#')
	{
		//Initial check about valid target
    	target.erase(0,1);
		target_channel = this->_server->getChannel(target);
		if(!target_channel)
			return(Command::ErrorNoSuchChannel(this->_client, target));

		//Checks on modestring arg
		if(num_args < 2)
		{
			target_modes = target_channel->getModesString();
			response = ":127.0.0.1 324 #";
			response += target_channel->getName();
			response += " :";
			if(!target_modes.empty())
			{
				response += "+" + target_modes;
			}
			response += "\r\n";
			return(send(this->_client->getSocketFd(), response.c_str(), response.length(), 0));
		}


		//ALL CHECKS PASSEDS FOR TARGER CHANNEL LETS PROCESS IT
		modestring = received_message->getSecondParam();
		argument_num = 3; //extra arguments start in arg position 3

		for(int i = 0; modestring[i]; i++)
		{
			if(modestring[i] == '+')
				signal = '+';
			else if(modestring[i] == '-')
				signal = '-';
			else
			{
				success_command_type = 0;

				if(signal == '+')
				{
					if(modestring[i] == 'b')
					{	
						if(argument_num > num_args)
						{
							ModeController::Mode_B_ShowBanlist(target_channel, this->_client);
						}
						else if(ModeController::Mode_B_AddBan(target_channel, received_message->getParamNumber(argument_num), this->_client))
							success_command_type = 1;
					}
					if(modestring[i] == 'o')
					{	
						if(ModeController::Mode_o_Add(target_channel, this->_server->getClientByNick(received_message->getParamNumber(argument_num)), this->_client))
							success_command_type = 1;
					}
					else if(modestring[i] == 'v')
					{	
						if(ModeController::Mode_v_Add(target_channel, this->_server->getClientByNick(received_message->getParamNumber(argument_num)), this->_client))
							success_command_type = 1;
					}
					else if(modestring[i] == 'k')
					{	
						if(ModeController::Mode_k_Add(target_channel, received_message->getParamNumber(argument_num), this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'm') 
					{
						if(ModeController::Mode_m_Add(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'n') 
					{
						if(ModeController::Mode_n_Add(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 't') 
					{
						if(ModeController::Mode_t_Add(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'i') 
					{
						if(ModeController::Mode_i_Add(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'l')
					{
						temp = ModeController::Mode_l_Add(target_channel, received_message->getParamNumber(argument_num), this->_client);
						if(temp == 1)
							success_command_type = 1;
						else if(temp == -1)
						{
							ModeController::Mode_l_Remove(target_channel, this->_client);
							if(response_signal != '-')
								response_modestring += "-l";
							else
								response_modestring += "l";
							response_signal = '-';
							argument_num++;
							continue;
						}	
					}
				}
				else
				{
					if(modestring[i] == 'b')
					{
						//std::cout << "Lamman a mode -b" << std::endl;
						if(ModeController::Mode_B_RemoveBan(target_channel, received_message->getParamNumber(argument_num), this->_client))
							success_command_type = 1;
					}
					else if(modestring[i] == 'o')
					{
						if(ModeController::Mode_o_Remove(target_channel, this->_server->getClientByNick(received_message->getParamNumber(argument_num)), this->_client))
							success_command_type = 1;
						
					}
					else if(modestring[i] == 'v')
					{
						if(ModeController::Mode_v_Remove(target_channel, this->_server->getClientByNick(received_message->getParamNumber(argument_num)), this->_client))
							success_command_type = 1;
						
					}
					else if(modestring[i] == 'm')
					{
						if(ModeController::Mode_m_Remove(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'k')
					{
						if(ModeController::Mode_k_Remove(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'n')
					{
						if(ModeController::Mode_n_Remove(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 't')
					{
						if(ModeController::Mode_t_Remove(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'i')
					{
						if(ModeController::Mode_i_Remove(target_channel, this->_client))
							success_command_type = 2;
					}
					else if(modestring[i] == 'l')
					{
						if(ModeController::Mode_l_Remove(target_channel, this->_client))
							success_command_type = 2;
					}
				}

				if(success_command_type == 1)
				{
					response_modestring = ModeController::ConstructResponseModeString(response_modestring, modestring[i], signal, response_signal);
					response_used_args = ModeController::ConstructResponseUsedArgs(response_used_args, received_message->getParamNumber(argument_num));
					response_signal = signal;
				}
				else if(success_command_type == 2)
				{
					response_modestring = ModeController::ConstructResponseModeString(response_modestring, modestring[i], signal, response_signal);
					response_signal = signal;
				}


				if(modestring[i] == 'b' || modestring[i] == 'o' || modestring[i] == 'v' || (modestring[i] == 'l' && signal == '+') || (modestring[i] == 'k' && signal == '+'))
				{
					argument_num++;
				}
			}
		}

		if(!response_modestring.empty())
		{
			response = ":";
			response += this->_client->getNick();
			response += "!";
			response += this->_client->getUser();
			response += "@127.0.0.1 MODE #";
			response += target_channel->getName();
			response += " ";
			response += response_modestring;
			response += " ";
			response += response_used_args;
			response += "\r\n";
			target_channel->sendMsgToAllChannel(response);
		}

		return(1);
	}


	//MODE WHEN TARGET IS A USER

	//Initial check about valid target
	target_client = this->_server->getClientByNick(target);
	if(!target_client)
		return(Command::ErrorNoSuchNick(this->_client, target));
	if(target_client != this->_client)
		return(Command::ErrorUserDontMatch(this->_client));

	//Checks on modestring arg
	if(num_args < 2)
	{
		//If <modestring> is not given, the RPL_UMODEIS (221) numeric is sent back containing the current modes of the target user.
		//:irc.example.org 221 foo :+i
		target_modes = this->_client->getModesString();
		response = ":127.0.0.1 221 ";
		response += this->_client->getNick();
		response += " :";
		if(!target_modes.empty())
		{
			response += "+" + target_modes;
		}
		response += "\r\n";
		return(send(this->_client->getSocketFd(), response.c_str(), response.length(), 0));
	}

	return(1);
}