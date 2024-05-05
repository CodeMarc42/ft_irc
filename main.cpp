#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include "includes/Server.hpp"
#include <sys/poll.h>
#include <iostream>
#include <signal.h>

#include "includes/ft_irc.h"

Server *ircserver;

void my_handler(int s)
{   
    (void)s;    
    std::cout << "\nClosing IRC Server" << std::endl;
    ircserver->Exit();
}

int check_port_arg_is_valid(char *port)
{
    int i;

    i = 0;
    while(port[i])
    {
        if(port[i] < '0' || port[i] > '9')
        {
            std::cerr << "Error: Invalid characters in port argument" << std::endl;
            return (0);
        }
        i++;
    }
    if(i == 0)
    {
        std::cerr << "Error: port argument is empty" << std::endl;
        return (0);
    }
    return (1);
}

int main(int argc, char **argv)
{
       

    std::string port;

    if(argc != 3)
    {
        std::cerr << "Error: Invalid num of params, 2 pams requireds (password, port)" << std::endl;
        return(-1);
    }
        
    if(!check_port_arg_is_valid(argv[1]))
        return(-1);

    port = argv[1];

    signal(SIGINT, &my_handler);
    Server server_object(std::stoi(port), argv[2]);
    ircserver = &server_object;
    ircserver->Init();
    return(0);
}