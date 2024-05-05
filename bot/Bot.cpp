#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/poll.h>
#include <iostream>
#include <string>
#include "Bot.hpp"
#include <netdb.h>


#define TIMEOUT 180000
#define BUFFERSIZE 1024

Bot *bot;

int Guard(int n, char * err);


int check_port_arg_is_valid(char *port)
{
    int i;

    i = 0;
    if (std::string(port).find_first_not_of("0123456789", 0) != std::string::npos)
    {
        std::cerr << "Error: Invalid characters in port argument" << std::endl;
        return (0);
    }
    if(!port)
    {
        std::cerr << "Error: port argument is empty" << std::endl;
        return (0);
    }
    return (1);
}

int check_channel_arg_is_valid(char *channel)
{
    int i;

    i = 0;
    if (std::string(channel).find_first_not_of("qwertyuiopasdfghjklzxcvbnmñ", 0) != std::string::npos)
    {
        std::cerr << "Error: Invalid characters in channel argument" << std::endl;
        return (0);
    }
    if(!channel)
    {
        std::cerr << "Error: port argument is empty" << std::endl;
        return (0);
    }
    return (1);
}

void send_cmd(std::string cmd,int fpsocket)
{
    send(fpsocket, (cmd + "\r\n").c_str(), strlen((cmd + "\r\n").c_str()), 0);
}

std::string Bot::getChannel()
{
    return this->channel;
}

void handler(int s)
{
    std::cout << "\nClosing BOT" << std::endl;
    //send_cmd("PART #" + bot->getChannel(), bot->getSock());
    send_cmd("QUIT", bot->getSock());
    close(bot->getSock());
    exit(s);
}

int main(int argc, char **argv)
{
    int port;

    if(argc != 4)
    {
        std::cerr << "Error: Invalid num of params, 3 params requireds (port, password, channel)" << std::endl;
        return(-1);
    }

    if(!check_channel_arg_is_valid(argv[3]))
        return(-1);

    if(!check_port_arg_is_valid(argv[1]))
        return(-1);

    port = std::stoi(argv[1]);

    
    Bot bot_object(argv[2], port, argv[3]);
    bot = &bot_object;
    signal(SIGINT, &handler);
    bot->run();
}

int Bot::getSock()
{
    return this->sock;
}

void    Bot::isScreaming(std::string message)
{
    size_t privmsg_number = 0;
    while (message.find("PRIVMSG", privmsg_number) != std::string::npos)
    {
        size_t temp = message.find(" PRIVMSG");
        privmsg_number = temp + 3;
        if (message.find('#') == std::string::npos)
            continue;
        std::string name = message.substr(message.rfind(':', temp) + 1, temp - message.rfind(':', temp));
        
        std::string server = message.substr(message.find('#', temp), message.find(' ', message.find('#', temp)) - message.find('#', temp));

        temp = message.find(':', temp);
        //std::cerr << " temp = " << temp << " name = " << name << " server = " << server << "sa\n";
        if (message.find_first_of("QWERTYUIOPASDFGHJKLZXCVBNMÑ", temp + 1) != std::string::npos && message.find_first_of("qwertyuiopasdfghjklzxcvbnmñ", temp + 1) == std::string::npos)
        {
            //std::cerr << "messsage sent is <<<<<<<<<<< " << "KICK " + server + " " + name << " >>>>>>>>>> \n";
            send_cmd("KICK " + server + " " + name, sock);
        }
    }

}

void Bot::run()
{
    int         temp;
    char        buffer[BUFFERSIZE];
    std::string buff;

    //Guard(bind(this->sock, (struct sockaddr *) &addr, sizeof(addr)), const_cast<char *>("could not bind"));
    //Guard(listen(this->sock, 100), const_cast<char *>("could not listen"));
    if (connect(sock, (const sockaddr*)&addr, sizeof(addr)) != 0)
    {
        std::cerr << errno;
    }
    struct pollfd   fd[1];
    int stage = 0;
    fd[0].fd = sock;
    fd[0].events = POLLOUT;
    int             nfd = 1;
    do {
        temp = poll(fd, nfd, TIMEOUT);
        if (temp < 0) {
            perror("poll() failed");
            break;
        }
        else if (temp == 0) {
            std::cout << "Poll Timed out\n";
        }
        else if (temp > 0) {
            fd[0].revents = POLLOUT;
        }
        bzero(buffer, BUFFERSIZE);
        while (stage <= 7)
        {
            if (fd[0].revents & POLLIN)
            {
                temp = recv(fd[0].fd, buffer, BUFFERSIZE, 0);
                if (temp < 0)
                {
                    if (errno != EWOULDBLOCK)
                        perror("recv() failed");
                }
                else if (temp == 0)
                    continue;
                else if (temp > 0)
                {
                    buffer[BUFFERSIZE - 1] = '\0';
                    //std::cout << buffer;
                    if (temp < BUFFERSIZE)
                    {
                        if ((stage == 1 && std::string(buffer).find("CAP * LS") != std::string::npos)
                        || (stage == 2 && std::string(buffer).find("CAP * ACK") != std::string::npos)
                        || (stage == 3 && std::string(buffer).find("are supported by this server") != std::string::npos)
                        || (stage == 4 && std::string(buffer).find("End of /NAMES list.") != std::string::npos)
                        || (stage == 5 && std::string(buffer).find("End of /WHO list.") != std::string::npos)
                        || (stage == 6 && std::string(buffer).find("End of Channel Ban List") != std::string::npos)
                        || (stage == 7 && std::string(buffer).find("you are now recognized.") != std::string::npos))
                        {
                            //std::cout << std::endl;
                            fd[0].revents = POLLOUT;
                            //std::cerr << "stage === " << stage << std::endl;
                        }
                    }
                    bzero(buffer, BUFFERSIZE);
                }
            }
            if (stage == 7)
            {
                stage++;
                fd[0].revents = POLLIN;
                break;
            }
            if (fd[0].revents & POLLOUT)
            {
                input_options(stage);
                fd[0].revents = POLLIN;
                stage++;
            }
        }
        if (stage > 7)
        {
            fd[0].revents = POLLIN;
            if (fd[0].revents & POLLIN)
            {
                errno = 0;
                temp = recv(fd[0].fd, buffer, BUFFERSIZE, 0);
                if (temp < 0)
                {
                    if (errno != EWOULDBLOCK)
                        perror("recv() failed");
                }
                else if (temp == 0)
                {
                    std::cout << "Connection closed\n";
                    close(sock);
                    exit(1);
                    fd[0].revents = POLLOUT;
                }
                else if (temp > 0)
                {
                    buff += buffer;
                    //std::cout << buffer;
                    //std::cerr << "buff is (((((((( " << buff << " )))))))))))) " << std::endl;
                    if (temp < BUFFERSIZE && buff.find("bot PRIVMSG") == std::string::npos && buff.find('#') != std::string::npos)
                    {
                        if (buff.find("PRIVMSG #") != std::string::npos)
                        {
                            fd[0].revents = POLLOUT;
                            isScreaming(buff);
                            buff.clear();
                            bzero(buffer, BUFFERSIZE);
                            fd[0].revents = POLLIN;
                        }
                    }
                    bzero(buffer, BUFFERSIZE);
                }
            }
            if (fd[0].revents & POLLOUT)
                fd[0].revents = POLLIN;
        }
        else
            stage++;
    } while (42);
}

int Guard(int n, char * err)
{
    if (n == -1) { perror(err); exit(1); } return n;
}


Bot::Bot(std::string pass, int port, std::string channel): port(port), pass(pass), channel(channel)
{
    this->on = 1;
    this->sock = Guard(socket(AF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/), const_cast<char *>("could not create TCP listening socket"));
    if (setsockopt(this->sock, SOL_SOCKET,  SO_REUSEADDR, &on, sizeof(int)) < 0) {
        perror("setsockopt() failed");
        close(this->sock);
        exit(-1);
    }
    //Guard(fcntl(this->sock, F_SETFL, O_NONBLOCK), const_cast<char *>("could not set TCP listening socket to be non-blocking"));
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
}

Bot::~Bot()
{
    send_cmd("QUIT", sock);
    close(sock);
}

void    Bot::input_options(int stage)
{
    switch (stage)
    {
        case 0:
        {
            // std::string msg = "CAP LS 302\nPASS " + pass + "\nNICK bot\nUSER bot 0 * ircbots";
            // send_cmd(msg.c_str(), sock);
            send_cmd("CAP LS 302", sock);
            send_cmd("PASS " + pass, sock);
            send_cmd("NICK " + std::string("bot"), sock);
            send_cmd("USER " + std::string("bot") + " . . :" + std::string("bot"), sock);
            // std::cerr << "---CAP LS 302" << std::endl;
            // std::cerr << "---PASS " + pass << std::endl;
            // std::cerr << "---NICK " + std::string("bot") << std::endl;
            // std::cerr << "---USER " + std::string("bot") + " . . :" + std::string("bot") << std::endl;
            break;
        }
        case 1:
        {
            send_cmd("CAP REQ extended-join chghost cap-notify userhost-in-names multi-prefix away-notify sasl tls account-notify server-time account-tag echo-message", sock);
            //std::cerr << "---CAP REQ extended-join chghost cap-notify userhost-in-names multi-prefix away-notify sasl tls account-notify server-time account-tag echo-message" << std::endl;
            break;
        }
        case 2:
        {
            send_cmd("CAP END", sock);
            //std::cerr << "---CAP END" << std::endl;
            break;
        }
        case 3:
        {
            send_cmd("JOIN #" + channel, sock);
            //std::cerr << "---JOIN #" + channel << std::endl;
            break;
        }
        case 4:
        {
            send_cmd("WHO #" + channel, sock);
            //std::cerr << "---WHO #" + channel << std::endl;
            break;
        }
        case 5:
        {
            send_cmd("MODE #" + channel + " b", sock);
            //std::cerr << "MODE #" + channel + " b" << std::endl;
            break;
        }
        case 6:
        {
            send_cmd("NICKSERV IDENTIFY " + pass, sock);
            //std::cerr << "---NICKSERV IDENTIFY " + pass << std::endl;
            break;
        }
        
        default:
            break;
    }
}