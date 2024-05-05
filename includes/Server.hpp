#ifndef SERVER_HPP
# define SERVER_HPP
# include "ft_irc.h"
# include <map>

#define	TIMEOUT 180000
#define	BUFFERSIZE 512

class Message;
class Client;
class Channel;
class Command;

class Server
{

	private:
		int						_port;
		int						nfds;
		int						listen_socket_fd;
		int 					_exit;
		std::string				_host_name;
		std::string				_host_ip;
		std::string 			_password;
		struct pollfd			fds[1024];
		struct sockaddr_in 		_addr;
		std::vector<Client *>	_clients;
		std::vector<Channel *>	_channels;
		std::map<int, Client*>	_map_clients_by_fd;

	private:
		void	handleClients(bool *, bool *);
		void	compressArray(bool *);

	public:
		Server(int port, std::string);
		virtual	~Server();

		int						Init();
		void						Exit();
		int						Guard(int n, const char * err);
		void					main_loop();
		std::vector<Channel *> 	getChannels();
		std::string 			getHostIp() const;
		std::string 			getHostName() const;
		void	 				addChannel(std::string);
		void     				removeChannel(Channel *channel);
		bool 	 				isNickNameInUse(std::string nick);
		void	 				check_revents(int, int, Client *);
		bool     				channelExist(std::string channel_name);
		bool     				isServerPassword(std::string password);
		Channel  				*getChannel(std::string channel_name);
		Client   				*getClientByNick(std::string nick);
		void 	 				cleanLeaksBeforeExit();
};

# endif
