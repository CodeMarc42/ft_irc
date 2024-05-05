#ifndef MODE_CONTROLLER_HPP
# define MODE_CONTROLLER_HPP

#include "../../includes/Server.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

class ModeController
{

	private:

	public:
		ModeController();
		~ModeController();
		static int Mode_B_ShowBanlist(Channel *channel, Client *client);
		static int Mode_B_AddBan(Channel *channel, std::string ban_mask, Client *moderator);
		static int Mode_B_RemoveBan(Channel *channel, std::string ban_mask, Client *moderator);
		static int Mode_o_Add(Channel *channel, Client *target_user, Client *moderator);
		static int Mode_o_Remove(Channel *channel, Client *target_user, Client *moderator);
		static int Mode_v_Add(Channel *channel, Client *target_user, Client *moderator);
		static int Mode_v_Remove(Channel *channel, Client *target_user, Client *moderator);
		static int Mode_m_Add(Channel *channel, Client *moderator);
		static int Mode_m_Remove(Channel *channel, Client *moderator);
		static int Mode_i_Add(Channel *channel, Client *moderator);
		static int Mode_i_Remove(Channel *channel, Client *moderator);
		static int Mode_n_Add(Channel *channel, Client *moderator);
		static int Mode_n_Remove(Channel *channel, Client *moderator);
		static int Mode_l_Add(Channel *channel, std::string arg, Client *moderator);
		static int Mode_l_Remove(Channel *channel, Client *moderator);
		static int Mode_k_Add(Channel *channel, std::string password, Client *moderator);
		static int Mode_k_Remove(Channel *channel, Client *moderator);
		static int Mode_t_Add(Channel *channel, Client *moderator);
		static int Mode_t_Remove(Channel *channel, Client *moderator);
		static std::string ConstructResponseUsedArgs(std::string response_used_args, std::string new_arg);
		static std::string ConstructResponseModeString(std::string response_modestring, char mode, char signal, char signal_before);
};

# endif