#ifndef DCC_CONTROLLER_HPP
# define DCC_CONTROLLER_HPP

#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"

extern Server ircserver;

class DccController
{

	private:

	public:
		DccController();
		virtual ~DccController();
		static int DccRequest(Client *client, Client *destiny_client,  Message *received_message);
		static int DccReject(Client *client, Client *destiny_client, Message *received_message);
		static int DccResume(Client *client, Client *destiny_client, Message *received_message);
		static int DccAccept(Client *client, Client *destiny_client, Message *received_message);
};

# endif