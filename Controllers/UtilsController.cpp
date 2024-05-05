#include "../includes/ft_irc.h"
#include "../includes/Message.hpp"
#include "includes/UtilsController.hpp"
#include <iostream>

bool UtilsController::isValidNickName(std::string nick)
{
    int i;

    i = 0;
    if(nick[0] == '$' || nick[0] == ':' || nick[0] == '#' || nick[0] == '\0')
       return(0);
    while(nick[i] != '\0')
    {
      if(nick[i] == ' ' || nick[i] == ',' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@' || nick[i] == '"' || nick[i] == '\'')
        return(0);
      i++;
    }
    return(1);
}

bool UtilsController::isNickNameNotInUse(std::string nick, Server *server)
{
    if(server->isNickNameInUse(nick))
        return(0);
    return(1);
}

bool UtilsController::isEmptyNickName(std::string nick)
{
    if(nick.empty())
        return(1);
    return(0);
}

std::string UtilsController::HashIp(std::string ip, int log_rounds)
{
    (void) ip;
    (void) log_rounds;
    std::string haship;
    // unsigned char hash[BCRYPT_HASHSIZE];
    // char hexstring[BCRYPT_HASHSIZE];
    // bcrypt_hashpw(ip.c_str(), hash, log_rounds);
    // for (int i = 0; i < BCRYPT_HASHSIZE; i++) {
    //     sprintf(&hexstring[i], "%02x", hash[i]);
    // }
    //return hexstring;
    return(haship);
}

std::vector<std::string> UtilsController::SplitArgument(std::string arg)
{
    std::vector<std::string> tokenized;
    size_t pos = 0;
    std::string token;
    while ((pos = arg.find(",")) != std::string::npos) {
        token = arg.substr(0, pos);
        tokenized.push_back(token);
        arg.erase(0, pos + 1);
    }
    tokenized.push_back(arg);
    return tokenized;
}


std::vector<std::string> UtilsController::SplitPartArgument(std::string arg)
{
    std::vector<std::string> tokenized;
    std::string channel_name;
    int index;

    index = 0;
    channel_name = "";
    while(arg[index])
    {
        if(arg[index] == '#')
        {
            index++;
            while(arg[index] != '\0' && arg[index] != ' ' && arg[index] != '\n' && arg[index] != '\r')
            {
                channel_name.push_back(arg[index]);
                index++;
            }
            if(!channel_name.empty())
                tokenized.push_back(channel_name);
            channel_name = "";
        }
        if(arg[index] != '\0')
            index++;
    }
    return tokenized;
}