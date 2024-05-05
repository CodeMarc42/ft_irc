#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include "includes/Message.hpp"
#include <sys/poll.h>
#include <iostream>
#include "includes/ft_irc.h"

Message::Message(std::string content): _content(content)
{
    if(this->TotalyConstructed())
        this->_ready = 1;
    else
        this->_ready = 0;

    //std::cout << "Message Constructor" << std::endl;
}

Message::~Message()
{
    //std::cout << "Message Destructor" << std::endl;
}

void Message::Compose(std::string content)
{
    this->_content = content;
}

void Message::Add(std::string content)
{
    if(this->_content.empty())
    {
        this->_content = content;
    }
    else
    {
        this->_content = this->_content + content;
    }
    if(this->TotalyConstructed())
        this->_ready = 1;
}

int Message::TotalyConstructed()
{
    int lenght;
    std::string::iterator it;

    lenght = this->_content.length();
    it = this->_content.end();
    if(lenght >= 2)
    {
        it--;
        if(*it == '\n')
        {
            it--;
            if(*it == '\r')
                return(1);
        }
    }
    return(0);
}

int Message::SendTo(int dst)
{
    if(send(dst, this->_content.c_str(), this->_content.length(),0) == -1)
        return(0);
    else
        return(1);
}

const char *Message::getMessageStr()
{
    return(this->_content.c_str());
}

int Message::isReady()
{
    return(this->_ready);
}

void Message::reset()
{
    this->_ready = 0;
    this->_content = "";

}

std::string Message::getArgsInString()
{
    std::string args;
    size_t index;


    //if no space found then not args, returning empty string
    index = this->_content.find(" ");
    if(index == std::string::npos)
    {
        return args;
    }
    //Tryng to take args and return
    index++;
    while(this->_content[index] != '\0' && this->_content[index] != '\r' )
    {
        args.push_back(this->_content[index]);
        index++;
    }
    return (args);
}

std::string Message::getCommandName()
{
    std::string command_name;
    size_t start; 

    start = 0;
    while(this->_content[start] != '\n' && this->_content[start] != '\0' && this->_content[start] != ' ' && this->_content[start] != '\r' )
    {
      command_name.push_back(std::toupper(this->_content[start]));
      start++;
    }
    //command_name = command_name.to_upper();
    return (command_name);
}

std::string Message::getFirstParam()
{
    std::string first_param;
    size_t index;


    //if no space found then not args, returning empty string
    index = this->_content.find(" ");
    if(index == std::string::npos)
    {
        return first_param;
    }
    while(this->_content[index] == ' ')
        index++;
    while(this->_content[index] != '\0' && this->_content[index] != '\r' && this->_content[index] != ' ')
    {
        first_param.push_back(this->_content[index]);
        index++;
    }
    // std::cout << "PVTMSG Extract Destiny Nick: |" << destiny_nick << "|" << std::endl;
    return(first_param);
}

std::string Message::getSecondParam()
{
    std::string second_param;
    size_t index;


    //if no space found then not args, returning empty string
    index = this->_content.find(" ");
    if(index == std::string::npos)
    {
        return second_param;
    }
    while(this->_content[index] == ' ')
        index++;
    while(this->_content[index] != '\0' && this->_content[index] != '\r' && this->_content[index] != ' ')
        index++;
    while(this->_content[index] == ' ')
        index++;
    while(this->_content[index] != '\0' && this->_content[index] != '\r' && this->_content[index] != ' ')
    {
        second_param.push_back(this->_content[index]);
        index++;
    }
    return(second_param);
}

std::string Message::getParamNumber(int num)
{
    std::string param;
    int i;
    int index;

    if(num > this->getNumArgs())
        return(param);

    i = 0;
    index = 0;
    while(i < num)
    {
        while(this->_content[index] != ' ')
            index++;
        while(this->_content[index] == ' ')
            index++;
        i++;
    }
    while(this->_content[index] != '\0' && this->_content[index] != '\r' && this->_content[index] != ' ')
    {
        param.push_back(this->_content[index]);
        index++;
    }
    return(param);
}

// TOPIC #hola sadjdkja dass a
std::string Message::getAllFromParamNumber(int num)
{
    std::string param;
    int i;
    int index;

    if(num > this->getNumArgs())
        return(param);

    i = 0;
    index = 0;
    while(i < num)
    {
        while(this->_content[index] != ' ')
            index++;
        while(this->_content[index] == ' ')
            index++;
        i++;
    }
    while(this->_content[index] != '\0' && this->_content[index] != '\r')
    {
        param.push_back(this->_content[index]);
        index++;
    }
    return(param);
}

int Message::getNumArgs()
{
    int i = 0;
    int num_args = 0;

    while(this->_content[i])
    {
        if(i == 0 && this->_content[0] != ' ')
            num_args++;
        if(i > 0 && this->_content[i] != ' ' && this->_content[i - 1] == ' ')
            num_args++;
        i++;
    }
    if(num_args > 0)
        num_args--;
    return (num_args);
}


