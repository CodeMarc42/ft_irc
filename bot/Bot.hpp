
#ifndef BOT_HPP
# define BOT_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cerrno>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <iostream>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include <algorithm>

class Bot
{
	private:
		// bool *sig;
		// std::string addr;
		int port;
		int sock;
		int on;
		struct sockaddr_in addr;
		//struct sockaddr_in serv_addr;

		std::string pass;
		std::string channel;
		bool connected;

		std::string receive;
		std::vector<std::string> messages;

		void send_msg(std::string msg, bool toLog = false);
		void recv_msg();
		void send_pass();
		void send_nick();
		void isScreaming(std::string message);
		void input_options(int stage);

		std::vector<std::string> split(std::string str, std::string delim);
	public:
		Bot(std::string pass, int port, std::string channel);
		~Bot();
		int	getSock();
		std::string	getChannel();

		void run();
};


#endif
