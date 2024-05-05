#include "../includes/ft_irc.h"
#include "includes/ModeController.hpp"
#include "../includes/Message.hpp"

int ModeController::Mode_B_ShowBanlist(Channel *channel, Client *client)
{
	size_t i;
	std::string response;
	std::string ban_list;
	std::string banned_mask;

	ban_list = channel->getBanList();
	i = 0;

	if(!ban_list.empty())
	{
		while(i < ban_list.length())
		{
			while(ban_list[i] != ' ')
			{
				banned_mask.push_back(ban_list[i]);
				i++;
			}
			if(banned_mask.length() > 0)
			{
				response = client->getResponsePrefix();
				response += " 367 #";
				response += channel->getName();
				response += " :";
				response += banned_mask;
				response += "\r\n";
				Command::SendResponse(client, response);
				banned_mask = "";
			}
			i++;
		}
	}
	response = client->getResponsePrefix();
	response += " 368 #";
	response += channel->getName();
	response += " :End of Channel Ban List\r\n";
	Command::SendResponse(client, response);
	return(1);
}

int ModeController::Mode_B_AddBan(Channel *channel, std::string ban_mask, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	channel->addBan(ban_mask);
	return(1);
}

int ModeController::Mode_B_RemoveBan(Channel *channel, std::string ban_mask, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	channel->removeBan(ban_mask);
	return(1);
}

int ModeController::Mode_m_Add(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(channel->isModeActived('m'))
		return(0);
	channel->setMode('m');
	return(1);
}

int ModeController::Mode_m_Remove(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isModeActived('m'))
		return(0);
	channel->removeMode('m');
	return(1);
}

int ModeController::Mode_i_Add(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(channel->isModeActived('i'))
		return(0);
	channel->setMode('i');
	return(1);
}

int ModeController::Mode_i_Remove(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isModeActived('i'))
		return(0);
	channel->removeMode('i');
	return(1);
}

int ModeController::Mode_n_Add(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(channel->isModeActived('n'))
		return(0);
	channel->setMode('n');
	return(1);
}

int ModeController::Mode_n_Remove(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isModeActived('n'))
		return(0);
	channel->removeMode('n');
	return(1);
}

int ModeController::Mode_t_Add(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(channel->isModeActived('t'))
		return(0);
	channel->setMode('t');
	return(1);
}

int ModeController::Mode_t_Remove(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isModeActived('t'))
		return(0);
	channel->removeMode('t');
	return(1);
}

int ModeController::Mode_l_Add(Channel *channel, std::string arg, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	int max_users;

	if(arg.empty())
		return(0);

	max_users = std::atoi(arg.c_str());
	
	//negative values are converted to 1
	if(max_users < 0)
		max_users = 1;

	//if value is 0, we return -1 indicate special case, cause we will not have to add, we will need to unset l option
	if(max_users == 0)
		return -1;

	if(channel->isModeActived('l') && max_users == channel->getMaxUsers())
		return(0);

	channel->setMode('l');
	channel->setMaxUsers(max_users);
	return(1);
}

int ModeController::Mode_l_Remove(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isModeActived('l'))
		return(0);
	channel->removeMode('l');
	return(1);
}

int ModeController::Mode_k_Add(Channel *channel, std::string password, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}

	if(password.empty())
		return(0);

	channel->setMode('k');
	channel->setPassword(password);
	return(1);
}

int ModeController::Mode_k_Remove(Channel *channel, Client *moderator)
{
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isModeActived('k'))
		return(0);
	channel->removeMode('k');
	return(1);
}


std::string ModeController::ConstructResponseModeString(std::string response_modestring, char mode, char signal, char signal_before)
{
	if(signal != signal_before)
		 response_modestring.push_back(signal);
	response_modestring.push_back(mode);
	return(response_modestring);
}

std::string ModeController::ConstructResponseUsedArgs(std::string response_used_args, std::string new_arg)
{
	if(!new_arg.empty())
	{
		if(response_used_args.empty())
			return(new_arg);
		else
		{
			response_used_args += " ";
			response_used_args += new_arg;
		}
	}
	return(response_used_args);
}

int ModeController::Mode_o_Add(Channel *channel, Client *target_user, Client *moderator)
{
	//Check is a valid client
	if(!target_user	)
		return(0);
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isClientInChannel(target_user))
	{
		Command::ErrorUserNotInChannel(moderator, target_user->getNick(), channel->getName());
		return(0);
	}
	if(channel->isClientChannelOperator(target_user))
		return(0);
	channel->addOperator(target_user);
	return(1);
}

int ModeController::Mode_o_Remove(Channel *channel, Client *target_user, Client *moderator)
{
	//Check is a valid client
	//std::cout << "Entro a Mode o remove" << std::endl;
	if(!target_user	)
		return(0);
	//std::cout << "Aqui 1" << std::endl;
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
		//std::cout << "Aqui 2" << std::endl;
	if(!channel->isClientInChannel(target_user))
	{
		Command::ErrorUserNotInChannel(moderator, target_user->getNick(), channel->getName());
		return(0);
	}
		//std::cout << "Aqui 3" << std::endl;
	if(!channel->isClientChannelOperator(target_user))
		return(0);
		//std::cout << "Aqui 4" << std::endl;
	channel->downGradeOperator(target_user);
		//std::cout << "Aqui 5" << std::endl;
	return(1);
}

int ModeController::Mode_v_Add(Channel *channel, Client *target_user, Client *moderator)
{
	//Check is a valid client
	if(!target_user)
		return(0);
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isClientInChannel(target_user))
	{
		Command::ErrorUserNotInChannel(moderator, target_user->getNick(), channel->getName());
		return(0);
	}
	if(channel->isClientWhitVoiceMode(target_user))
		return(0);
	channel->addClientToVoice(target_user);
	return(1);
}

int ModeController::Mode_v_Remove(Channel *channel, Client *target_user, Client *moderator)
{
	//Check is a valid client
	if(!target_user	)
		return(0);
	if(!channel->isClientChannelOperator(moderator))
	{
		Command::ErrorYouAreNotChannelOperator(moderator, channel->getName());
		return(0);
	}
	if(!channel->isClientInChannel(target_user))
	{
		Command::ErrorUserNotInChannel(moderator, target_user->getNick(), channel->getName());
		return(0);
	}
	if(!channel->isClientWhitVoiceMode(target_user))
		return(0);
	if(channel->removeClientFromVoice(target_user))
	{
		return(1);
	}
	return(0);
}


