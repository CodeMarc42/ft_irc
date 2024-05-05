#include "../includes/ft_irc.h"
#include "includes/DccController.hpp"
#include "../includes/Message.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"

// CTCP REQUEST loler DCC SEND 32 16843009 0 12 4 T
// loler CTCP REQUEST DCC SEND 32 0 6668 12 4

// CTCP REQUEST loler DCC SEND 32 16843009 0 12 9 T
// loler CTCP REQUEST DCC SEND 32 0 6668 12 9

int DccController::DccRequest(Client *client, Client *destiny_client, Message *received_message)
{
    std::string response;
    std::string first_param;
    std::string dcc_msg;
    int num_args;

    num_args = received_message->getNumArgs();
    std::cout << "Num Args es: " <<  num_args << std::endl;

    if(num_args < 8)
        return(Command::ErrorNeedMoreParams(client, "CTCP"));

    if(num_args == 9)
    {
        // //EL DE AHIR!!!!!!!
        if(std::strcmp(received_message->getParamNumber(3).c_str(), "SEND") == 0)
        {
            dcc_msg = received_message->getAllFromParamNumber(1);

            response = client->getResponsePrefix();
            response += " PRIVMSG ";
            response += dcc_msg;
            response += "\r\n";
            Command::SendResponse(destiny_client, response);
        }
        std::cout << "REsponse to Request:: " << response << std::endl;     
    }

    if(num_args == 8)
    {
        // //EL DE AHIR!!!!!!!
        if(std::strcmp(received_message->getParamNumber(3).c_str(), "SEND") == 0)
        {
            dcc_msg = received_message->getAllFromParamNumber(1);

            response = client->getResponsePrefix();
            response += " PRIVMSG ";
            response += dcc_msg;
            response += "\r\n";
            Command::SendResponse(destiny_client, response);
        }
        std::cout << "REsponse to Request:: " << response << std::endl;     
    }
    return(1);
}

//CTCP REPLY loler DCC REJECT SEND 32131 0 1024 0 T
//marza-ga NOTICE loler DCC REJECT SEND 223132132 0 1024 0 T

int DccController::DccReject(Client *client, Client *destiny_client, Message *received_message)
{
    std::string response;
    std::string msg;
    int num_args;

    num_args = received_message->getNumArgs();
    std::cout << "Num Args es: " <<  num_args << std::endl;
    if(num_args < 9)
    {
        return(Command::ErrorNeedMoreParams(client, "CTCP"));
    }


    msg = received_message->getAllFromParamNumber(2);

    response = client->getResponsePrefix();
    response += " NOTICE ";
    response += destiny_client->getNick();
    response += " ";
    response += msg;
    response += "\r\n";
    Command::SendResponse(destiny_client, response);
    std::cout << "REsponse to Reject:: " << response << std::endl;
    return(1);
}


//loler PRIVMSG marza-ga DCC RESUME 32 0 0 2
int DccController::DccResume(Client *client, Client *destiny_client, Message *received_message)
{
    std::string response;
    std::string first_param;
    std::string dcc_msg;
    int num_args;

    num_args = received_message->getNumArgs();

    if(num_args < 8)
        return(Command::ErrorNeedMoreParams(client, "CTCP"));

    if(std::strcmp(received_message->getParamNumber(3).c_str(), "RESUME") == 0)
    {
        dcc_msg = received_message->getAllFromParamNumber(1);

        response = client->getResponsePrefix();
        response += " PRIVMSG ";
        response += dcc_msg;
        response += "\r\n";
        Command::SendResponse(destiny_client, response);
        std::cout << "REsponse to Resume:: " << response << std::endl;
    }
    return(1);
}

int DccController::DccAccept(Client *client, Client *destiny_client, Message *received_message)
{
    std::string response;
    std::string first_param;
    std::string dcc_msg;
    int num_args;

    num_args = received_message->getNumArgs();

    if(num_args < 8)
        return(Command::ErrorNeedMoreParams(client, "CTCP"));

    if(std::strcmp(received_message->getParamNumber(3).c_str(), "SEND") == 0)
    {
        dcc_msg = received_message->getAllFromParamNumber(1);

        response = client->getResponsePrefix();
        response += " PRIVMSG ";
        response += dcc_msg;
        response += "\r\n";
        Command::SendResponse(destiny_client, response);
        std::cout << "REsponse to Accept:: " << response << std::endl;
    }
    return(1);
}