#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include "includes/Server.hpp"
#include "includes/Message.hpp"
#include "includes/Client.hpp"
#include "includes/Command.hpp"
#include <sys/poll.h>
#include <iostream>

Server::Server(int port, std::string password): nfds(1)
{
    this->_port = port;
    this->_password = password;
    this->_host_ip = "127.0.0.1";
    this->_host_name = "IRC.Marc&Jaime.ProTeam";
    this->_exit = 0;
	std::cout << "Server ircserver constructed" << std::endl;
}

Server::~Server()
{
    std::cout << "Server Destructor" << std::endl;
}

std::string Server::getHostIp() const
{
    return(this->_host_ip);
}

std::string Server::getHostName() const
{
    return(this->_host_name);
}

int Server::Guard(int n, const char * err)
{
	if (n < 0){ 
        std::cerr << err << std::endl; return(0);
        return (1);
    } 
    return (0);
}

int Server::Init()
{
		int	on = 1;

		this->listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (this->Guard(this->listen_socket_fd , "could not create TCP listening socket"))
            return(0);
		if (this->Guard(setsockopt(this->listen_socket_fd, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(int)), "setsockopt() failed")) {
			close(this->listen_socket_fd);
			return(0);
		}
		if (this->Guard(fcntl(this->listen_socket_fd, F_SETFL, O_NONBLOCK), "could not set TCP listening socket to be non-blocking"))
            return(0);
		//struct sockaddr_in addr;
		std::memset(&this->_addr, 0, sizeof(this->_addr));
		this->_addr.sin_family = AF_INET;
		this->_addr.sin_port = htons(this->_port);
		this->_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if (this->Guard(bind(this->listen_socket_fd, (struct sockaddr *) &this->_addr, sizeof(this->_addr)), "could not bind"))
            return(0);

		if (this->Guard(listen(this->listen_socket_fd, 100), "could not listen"))
            return(0);
		std::memset(this->fds, 0 , sizeof(this->fds));
		this->fds[0].fd = this->listen_socket_fd;
		this->fds[0].events = POLLIN;

        //On time all the init of sockets is done, let s enter to the main loop
        this->main_loop();
        this->cleanLeaksBeforeExit();
		return(1);
}

void Server::main_loop()
{
	int		temp = 0;
	bool	end_server = false;
	bool	compress_array = false;

	do {
		temp = poll(this->fds, this->nfds, TIMEOUT);
		if (temp < 0)
        {
			//std::cerr << "poll() failed" << std::endl;
			break;
		}
		else if (temp == 0) {
			//std::cerr << "Poll Timed out\n" << std::endl;
			break;
		}
		handleClients(&end_server, &compress_array);
		if (compress_array)
			compressArray(&compress_array);
        if(this->_exit == 1)
            end_server = true;

	} while (end_server == false);

	for (int i = 0; i < this->nfds; i++)
		if(this->fds[i].fd >= 0)
			close(this->fds[i].fd);
}

void	Server::handleClients(bool *end_server, bool *compress_array)
{
	int		current_size;
	int		new_fd;

    Message *msg;
    Client  *client;
	current_size = this->nfds;
    new_fd = 0;
	for (int i = 0; i < current_size; i++) {
		if(this->fds[i].revents == 0)
			continue;
		//if(this->fds[i].revents != POLLIN && this->fds[i].revents != 17) {
        if(!(this->fds[i].revents & POLLIN)) {
			std::cout << this->fds[i].revents;
			std::cerr << "Error! in revents: " << this->fds[i].revents << std::endl;
			*end_server = true;
			break;
		}
		if (this->fds[i].fd == this->listen_socket_fd) {
			do {

				struct sockaddr_in clientaddr;
				socklen_t addrlen = sizeof(clientaddr);
				//new_fd = accept(this->listen_socket_fd, NULL, NULL);
				new_fd = accept(this->listen_socket_fd, (struct sockaddr *)&clientaddr, (socklen_t*)&addrlen);
				if (new_fd < 0) {
					if (errno != EWOULDBLOCK) {
						std::cout << "ALGO NO VA BIEN!!!!" << std::endl;
						std::perror("accept() failed");
						*end_server = true;
					}
					break;
				}

				getsockname(new_fd, (struct sockaddr *)&clientaddr, &addrlen);
				char *ip = inet_ntoa(clientaddr.sin_addr);
    			//int port = ntohs(clientaddr.sin_port);
				//std::cerr << "adress ======== " << ip << " and port ======  " << port << std::endl;



				this->fds[this->nfds].fd = new_fd;
				this->fds[this->nfds].events = POLLIN;
				this->nfds++;

                //Creo un nuevo cliente y lo añado a mi vector clientes
				//std::cout << "NEW SOCKET CONECTION IN " << new_fd << std::endl;
                client = new Client(new_fd);
                client->setIp(ip);
            
                this->_map_clients_by_fd.insert(std::pair<int, Client *>(new_fd, client));
                this->_clients.push_back(client);
               //break;

			} while (new_fd != -1);
		}
		else
        {
				int     read_bytes;
				bool    end_con;
                
                //Get the client of this fd              
                std::map<int, Client *>::iterator it_map;
                it_map = this->_map_clients_by_fd.find(this->fds[i].fd);
                client = (*it_map).second;

                //std::cout << "CLIENT FD ES " << client->getSocketFd() << " NEW FD ES " << new_fd << "this->fds[i].fd es" << this->fds[i].fd  << "Revent state " << this->fds[i].revents <<std::endl;    

				if(this->fds[i].revents == 0)
					continue;
                read_bytes = 0;
				end_con = false;

                //If there is something to read
				if(this->fds[i].revents & POLLIN){
					//try to read
                    read_bytes = recv(this->fds[i].fd, client->buffer, BUFFERSIZE, 0);
                    client->buffer[read_bytes] = '\0';
					//check read errors in recv
                    if (read_bytes <= 0) {
                        if (read_bytes == 0) {
                            std::cerr << "Error in recv: Connection closed for client " << client->getNick() << std::endl;
                            end_con = true;
                        }
						else if (read_bytes < 0 && errno != EWOULDBLOCK)
                        {
                            std::cerr << "Error recv() failed for " << client->getNick() << std::endl;
							end_con = true;
						}
					}
					
                    //if there is something received in the buffer we try to parse until get a complete mesaje
					if(client->buffer[0] != '\0')
					{
						msg = client->getMessage();
						msg->Add(client->buffer);
                        if(msg->isReady())
                           this->fds[i].revents = POLLOUT;
					}

                    //if there is a totaly parsed mesaje received for client we execute it
					if(this->fds[i].revents & POLLOUT && msg->isReady()){
                            Command cmd(client, this);
                            cmd.Exec();
							msg->reset();
					}
				}
                this->fds[i].revents = POLLIN;
                //If client conection have to be finished we close it correctly

				if (client->wantToDisconect() || end_con)
                {
					close(this->fds[i].fd);
					this->_map_clients_by_fd.erase(it_map);


					*compress_array = true;
                    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
                    {
                        if(client == (*it))
                        {
                            std::cout << "BORRANDO CLIENTE" << std::endl;
                            std::cout << "CLIENT FD ES " << client->getSocketFd() << "NEW FD ES " << new_fd << "this->fds[i].fd es" << this->fds[i].fd  <<std::endl;    
                            delete client;
                            this->_clients.erase(it);
                            break;
                        }
                    }
                    this->fds[i].fd = -1;
				}

		}
	}
}

void	Server::compressArray(bool *compress_array)
{
	*compress_array = false;
	for (int i = 0; i < this->nfds; i++) {
		if (this->fds[i].fd == -1) {
			for(int j = i; j < this->nfds; j++)
				this->fds[j].fd = this->fds[j+1].fd;
			i--;
			this->nfds--;
		}
	}
}

void Server::cleanLeaksBeforeExit()
{
    Client *client;
    Channel *channel;
    std::vector<Client *>::iterator it;
    std::vector<Channel *>::iterator it2;

    for (it2 = this->_channels.begin(); it2 != this->_channels.end(); it2++)
    {
        channel = (*it2);
        delete channel;
    }
    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        client = (*it);
        delete client;
    }
}

std::vector<Channel *> Server::getChannels()
{
	return this->_channels;
}

bool Server::isNickNameInUse(std::string nick)
{
    Client *client;

    for(std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {   
        client = *it;
        if(client->getNick() == nick)
          return(1);
    }
    return(0);
}

void	Server::addChannel(std::string name)
{
	this->_channels.push_back(new Channel(name));
}

void    Server::removeChannel(Channel *channel)
{
    for(std::vector<Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); ++it)
    {   
        if((*it) == channel)
        {
            this->_channels.erase(it);
            delete channel;
            return ;
        }
    }
}


bool    Server::channelExist(std::string channel_name)
{
    std::vector<Channel *>::iterator it;

    for (it = this->_channels.begin(); it != this->_channels.end(); it++)
    {
        if ((*it)->getName() == channel_name)
        {
            return(1);
        }
    }
    return(0);
}

Channel   *Server::getChannel(std::string channel_name)
{
    std::vector<Channel *>::iterator it;

    for (it = this->_channels.begin(); it != this->_channels.end(); it++)
    {
        if ((*it)->getName() == channel_name)
        {
            return((*it));
        }
    }
    return(NULL);
}

Client   *Server::getClientByNick(std::string nick)
{
    for (std::vector<Client *>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if(nick == (*it)->getNick())
        {
            return (*it);
        }
    }
    return(NULL);
}

bool     Server::isServerPassword(std::string password)
{
    if(password.empty())
        return (false);
    if(password == this->_password)
        return (true);
    return (false);
}

void Server::Exit()
{
	this->_exit = 1;
}

void	Server::check_revents(int temp, int i, Client * client)
{
	switch (this->fds[i].revents)
	{
		case POLLHUP:
			std::cout << "Revent is POLLHUP" << std::endl;
			std::cout << "en temp tengo = " << temp << std::endl;
			std::cout << "buffer es = " << client->buffer << std::endl;
			break;
		case POLLIN:
			std::cout << "Revent is POLLIN" << std::endl;
			std::cout << "en temp tengo = " << temp << std::endl;
			std::cout << "buffer es = " << client->buffer << std::endl;
			break;
		case POLLPRI:
			std::cout << "Revent is POLLPRI" << std::endl;
			std::cout << "en temp tengo = " << temp << std::endl;
			std::cout << "buffer es = " << client->buffer << std::endl;
			break;
		case POLLOUT:
			std::cout << "Revent is POLLOUT" << std::endl;
			std::cout << "en temp tengo = " << temp << std::endl;
			std::cout << "buffer es = " << client->buffer << std::endl;
			break;
		case POLLERR:
			std::cout << "Revent is POLLERR" << std::endl;
			std::cout << "en temp tengo = " << temp << std::endl;
			std::cout << "buffer es = " << client->buffer << std::endl;
			break;
		case POLLNVAL:
			std::cout << "Revent is POLLNVAL" << std::endl;
			std::cout << "en temp tengo = " << temp << std::endl;
			std::cout << "buffer es = " << client->buffer << std::endl;
			break;
		default:
			//17 cuando no hay conexion
			std::cout << "Revent is otra cosa ->" << this->fds[i].revents << std::endl;
			std::cout << "en temp tengo = " << temp << std::endl;
			std::cout << "buffer es = " << client->buffer << std::endl;
			break;
	}
}
