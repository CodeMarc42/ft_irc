#ifndef MESSAGE_HPP
# define MESSAGE_HPP
# include "ft_irc.h"

class Message
{

	private:
		std::string _content;
		int _ready;

	public:
		Message(std::string content);
		~Message();
		void Compose(std::string content);
		int SendTo(int dst);
		void Add(std::string content);
		int TotalyConstructed();
		int isReady();
		int getNumArgs();
		const char *getMessageStr();
		std::string getArgsInString();
		std::string getCommandName();
		std::string getFirstParam();
		std::string getSecondParam();
		std::string getParamNumber(int num);
		std::string getAllFromParamNumber(int num);
		void reset();

};

# endif
