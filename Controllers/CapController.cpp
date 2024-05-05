#include "../includes/ft_irc.h"
#include "includes/CapController.hpp"
#include "../includes/Message.hpp"


std::string CapController::extractPass(std::string msg)
{
    std::string password;
    size_t start; 

    password = "";
    if(msg.find("PASS ") == std::string::npos)
    {
        return password;
    }
    start = 0;
    start = msg.find("PASS ");
    start = start + 5;
    while(msg[start] != '\n' && msg[start] != '\0' && msg[start] != '\r')
    {
      password.push_back(msg[start]);
      start++;
    }
    std::cout << "Extract Password: |" << password << "|" << std::endl;
    return(password);
}

std::string CapController::extractNickName(std::string msg)
{
    std::string nick;
    size_t start; 

    //EXTRACT CLIENT NICK
    start = 0;
    nick = "";
    if(msg.find("NICK ") == std::string::npos)
    {
        return nick;
    }
    start = msg.find("NICK ");
    start = start + 5;
    while(msg[start] != '\n' && msg[start] != '\0' && msg[start] != ' ' && msg[start] != '\r' )
    {
      nick.push_back(msg[start]);
      start++;
    }
    std::cout << "Extract NickName: |" << nick << "|" << std::endl;
    return(nick);
}

std::string CapController::extractUserName(std::string msg)
{
    std::string username;
    size_t start; 

    start = 0;
    if(msg.find("* :") == std::string::npos)
    {
        return username;
    }
    start = msg.find("* :");
    start = start + 3;
    while(msg[start] != '\n' && msg[start] != '\0' && msg[start] != '\r')
    {
      username.push_back(msg[start]);
      start++;
    }
    std::cout << "Extract UserName: |" << username << "|" << std::endl;
    return(username);
}

std::string CapController::extractUser(std::string msg)
{
    std::string user;
    size_t start; 

    start = 0;
    if(msg.find("* :") == std::string::npos)
    {
        return user;
    }
    start = msg.find("USER ");
    start = start + 5;
    while(msg[start] != '\n' && msg[start] != '\0' && msg[start] != ' ' && msg[start] != '\r')
    {
      user.push_back(msg[start]);
      start++;
    }
    std::cout << "Extract User: |" << user << "|" << std::endl;
    return(user);
}
