#include "../includes/Command.hpp"

// ERR_NEEDMOREPARAMS (461) 
//   "<client> <command> :Not enough parameters"

int Command::SendError(Client *client, std::string error)
{
  int temp;

  temp = send(client->getSocketFd(), error.c_str(), error.length(), 0);
  if (temp < 0)
  {
   perror("send() failed");
  }
  return(temp);
}

int Command::ErrorNoSuchNick(Client *client, std::string nick)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 401 ";
  error += nick;
  error += " :No such nick/channel\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorNoSuchChannel(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 403 #";
  error += channel_name;
  error += " :No such channel\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorCanNotSendToChannel(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 404 #";
  error += channel_name;
  error += " :Cannot send to channel\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorNoRecipientGiven(Client *client, std::string command)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 411";
  error += " :No recipient given ";
  error += command;
  error += "\r\n";

  return(Command::SendError(client, error));
}


int Command::ErrorNoTextToSend(Client *client)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 412 ";
  error += " :No text to send\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorUnknownCommand(Client *client, std::string command)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 421 ";
  error += command;
  error += " :Unknown command\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorNoNicknameGiven(Client *client, std::string nick)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 431 ";
  error += nick;
  error += " :No Nickname given\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorNickErroneous(Client *client, std::string nick)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 432 ";
  error += nick;
  error += " :Erroneous nickname\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorNicknameInUse(Client *client, std::string nick)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 433 ";
  error += nick;
  error += " :Nickname is already in use\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorUserNotInChannel(Client *client,  std::string nick, std::string channel)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 441 ";
  error += client->getNick();
  error += " ";
  error += nick;
  error += " #";
  error += channel;
  error += " :They aren't on that channel\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorNotOnChannel(Client *client, std::string channel)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 442 #";
  error += channel;
  error += " :You're not on that channel\r\n";

  return(Command::SendError(client, error));
}


int Command::ErrorUserOnChannel(Client *client, std::string channel, std::string target_user)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 443 ";
  error += target_user;
  error += " #";
  error += channel;
  error += " :is already on channel\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorChannelMustStartWhitHashMark(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 448 ";
  error += ": ";
  error += channel_name;
  error += " Cannot join channel: Channel name must start with a hash mark (#)\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorNeedMoreParams(Client *client, std::string command)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 461 ";
  error += command;
  error += " :Not enough parameters\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorPasswordMissMatch(Client *client)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 464 ";
  //error += command;
  error += " :Password incorrect\r\n";

  Command::SendError(client, error);

  error = "ERROR Closing link: (marza-ga@195.55.43.195)\r\n";
  // error = client->getResponsePrefix();
  // error += " ERROR ";
  // //error += command;
  // error += " :Password incorrect\r\n";
  return(Command::SendError(client, error));
}

int Command::ErrorChannelIsFull(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 471 #";
  error += channel_name;
  error += " :Cannot join channel (+l)\r\n";

  return(Command::SendError(client, error));
}


int Command::ErrorInviteOnlyChannel(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 473 ";
  error += client->getNick();
  error += " #";
  error += channel_name;
  error += " :Cannot join channel (+i)\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorBannedFromChannel(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 474 ";
  error += client->getNick();
  error += " #";
  error += channel_name;
  error += " :Cannot join channel (+b)\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorBadChannelKey(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 475 ";
  error += client->getNick();
  error += " #";
  error += channel_name;
  error += " :Cannot join channel (+k)\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorYouAreNotChannelOperator(Client *client, std::string channel_name)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 482 #";
  error += channel_name;
  error += " :You're not channel operator\r\n";

  return(Command::SendError(client, error));
}

int Command::ErrorUserDontMatch(Client *client)
{
  std::string error;

  error = client->getResponsePrefix();
  error += " 502 ";
  error += ":Cant change mode for other users\r\n";

  return(Command::SendError(client, error));
}



