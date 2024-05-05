#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "ft_irc.h"
# include "Client.hpp"

class Client;

class Channel
{

	private:
		std::string				_name;
		std::vector<Client *>	_clients;
		std::vector<Client *>	_operators;
		std::vector<Client *>	_voice_users;
		std::vector<Client *>	_inviteds;
		std::string				_ban_list;
		std::string				_modes;
		std::string				_password;
		std::string				_topic;
		int 					_max_users;
		int  					_num_users;

	public:
		Channel();
		Channel(std::string);
		~Channel();

		std::string 			getName() const;
		std::vector<Client *> 	getClients() const;
		std::vector<Client *> 	getOperators() const;
		std::string			  	getBanList() const;
		std::string			  	getModesString() const;
		std::string			  	getTopic() const;
		int		  				getMaxUsers() const;
		int 				  	getNumUsers() const;
		void 				  	setMode(char mode);
		void 				  	setTopic(std::string topic);
		void 				  	setNumUsers(int num);
		void 				  	setMaxUsers(int num);
		void 				  	setPassword(std::string password);
		void 				  	removeMode(char mode);
		bool				  	isModeActived(char mode) const;
		void					addClient(Client *);
		bool					isClientInChannel(Client *);
		bool					isClientInInvitedList(Client *);
		bool					isClientChannelOperator(Client *);
		bool					checkPasswordMatch(std::string password);
		void					removeClient(Client *);
		void					addOperator(Client *);
		void					downGradeOperator(Client *);
		void					addClientToInviteds(Client *);
		void					removeClientFromInviteds(Client *);
		void					upGradeToOperator(Client *client);
		void 					addBan(std::string ban_mask);
		void 					removeBan(std::string ban_mask);
		bool					isBanned(Client *);
		bool					isClientWhitVoiceMode(Client *);
		bool					isFull() const;
		int 					sendMsgToAllChannel(std::string msg);
		int	 					sendNoticeToChannel(Client *sender, std::string response);
		void					addClientToVoice(Client *client);
		int						removeClientFromVoice(Client *client);
		int 					checkMaskPattern(const char* client_mask, const char* banned_mask);
		std::vector<Client *> 	getCopyOfVectorOfClients();
		std::vector<Client *> 	getCopyOfVectorOfOperators();
};

# endif
