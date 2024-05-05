#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "ft_irc.h"
# include "Channel.hpp"
# include "Message.hpp"

class Message;
class Channel;

class Client
{

	private:
		std::vector<Channel *> _channels;
		std::vector<Channel *> _invited;
		std::string _name;
		std::string _user;
		std::string _nick;
		std::string _host;
		std::string _ip;
		std::string _hashed_host;
		std::string _modes;
		int _conected;
		int _cap_steep;
		int _end_conection;
		int _socket_fd;
		Message *_message;
	public:
		char buffer[512];
		Client(int socket_fd);
		~Client();
		std::string 			getName() const;
		std::string 			getUser() const;
		std::string 			getNick() const;
		std::string 			getHost() const;
		std::string 			getIp() const;
		std::string 			getResponsePrefix();
		std::string 			getModesString() const;
		Message 				*getMessage();
		int						getCapSteep() const;
		int 					getSocketFd() const;
		int 					setName(std::string name);
		int 					setUser(std::string user);
		int 					setNick(std::string nick);
		void 					setIp(std::string ip);
		void 					setSocketFd(int fd);
		void 					setCapSteep(int steep);
		void 					setRegisteredOn();
		void 					setEndConection();
		void 					addChannel(Channel *channel);
		void 					removeChannel(Channel *channel);
		void 					addChannelToInvitedList(Channel *channel);
		void 					removeChannelFromInvitedList(Channel *channel);
		bool					isChannelInInvitedList(Channel *channel);
		bool					isRegistered();
		bool					wantToDisconect();
		std::string 			list_channels_in_str();
		std::vector<Channel *> 	getCopyOfVectorOfChannels();
		//bool isClientLoggedToChannel(std::string channel_name);
};

# endif
