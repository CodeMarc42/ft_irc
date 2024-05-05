#ifndef COMMAND_HPP
# define COMMAND_HPP
# include "ft_irc.h"
# include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "../Controllers/includes/UtilsController.hpp"
#include "../Controllers/includes/CapController.hpp"
#include "../Controllers/includes/ModeController.hpp"
#include "../Controllers/includes/DccController.hpp"

class Server;
class Message;
class Client;
class Channel;

class Command
{

	private:
		std::string _input;
		int _fd;
		Client *_client;
		Server *_server;

	public:
		Command(Client *cliente, Server *server);
		~Command();
		int Exec();
		int SearchAndExec();
		
		//COMANDS
		int NewClientRequest(Message *msg);
		int NewClientRequest2();
		int NewClientRequest3();
		int Ping(Message *received_message);
		int Quit(Message *received_message);
		int Identify();
		int	Join(Message *received_message);
		int	Who(Message *received_message);
		int Mode(Message *received_message);
		int ReplyNotice(std::string notice_msg);
		int Part(Message *received_message);
		int Nick(Message *received_message);
		int PrivtMsg(Message *received_message);
		int IsOn(Message *received_message);
		int Watch(Message *received_message);
		int Kick(Message *received_message);
		int WhoIs(Message *received_message);
		int Topic(Message *received_message);
		int Invite(Message *received_message);
		int Dcc(Message *received_message);
		int Notice(Message *received_message);

		static int SendResponse(Client *client, std::string response);
		static int SendError(Client *client, std::string error);
		
		//LAUNCH ERRORS TO CLIENT
		static int ErrorNeedMoreParams(Client *client, std::string command);
		static int ErrorUnknownCommand(Client *client, std::string command);
		static int ErrorNoNicknameGiven(Client *client, std::string nick);
		static int ErrorNickErroneous(Client *client, std::string nick);
		static int ErrorNicknameInUse(Client *client, std::string nick);
		static int ErrorChannelIsFull(Client *client, std::string nick);
		static int ErrorInviteOnlyChannel(Client *client, std::string nick);
		static int ErrorBannedFromChannel(Client *client, std::string nick);
		static int ErrorNoSuchNick(Client *client, std::string nick);
		static int ErrorNoSuchChannel(Client *client, std::string channel_name);
		static int ErrorCanNotSendToChannel(Client *client, std::string channel_name);
		static int ErrorNoTextToSend(Client *client);
		static int ErrorPasswordMissMatch(Client *client);
		static int ErrorNotOnChannel(Client *client, std::string channel);
		static int ErrorChannelMustStartWhitHashMark(Client *client, std::string channel_name);
		static int ErrorUserDontMatch(Client *client);
		static int ErrorYouAreNotChannelOperator(Client *client, std::string channel_name);
		static int ErrorUserOnChannel(Client *client, std::string channel_name, std::string target_user);
		static int ErrorUserNotInChannel(Client *client,  std::string nick, std::string channel);
		static int ErrorBadChannelKey(Client *client, std::string channel_name);
		static int ErrorNoRecipientGiven(Client *client, std::string command);
};

# endif
