SRCS =  main.cpp \
		Server.cpp \
		Message.cpp \
		Client.cpp \
		Channel.cpp \
		Command.cpp \
		Commands/Ping.cpp \
		Commands/Errors.cpp \
		Commands/Nick.cpp \
		Commands/Quit.cpp \
		Commands/Join.cpp \
		Commands/Privmsg.cpp \
		Commands/IsOn.cpp \
		Commands/Part.cpp \
		Commands/Whois.cpp \
		Commands/Watch.cpp \
		Commands/Who.cpp \
		Commands/Kick.cpp \
		Commands/Mode.cpp \
		Commands/Topic.cpp \
		Commands/Invite.cpp \
		Commands/Notice.cpp \
		Commands/Dcc.cpp \
		Controllers/CapController.cpp \
		Controllers/UtilsController.cpp \
		Controllers/ModeController.cpp \
		Controllers/DccController.cpp \
		
SRCSBOT = bot/Bot.cpp


OBJS = ${SRCS:.cpp=.o}
DEPS = ${SRCS:.cpp=.d}
OBJSBOT = ${SRCSBOT:.cpp=.o}
DEPSBOT = ${SRCSBOT:.cpp=.d}
CPP = c++
RM = rm -f
CPPFLAGS =  -Wall -Werror -Wextra -std=c++98 -Wshadow -Wno-shadow -MMD
HEADER = includes/ft_irc.h
HEADERBOT = bot/Bot.hpp
NAME = ft_irc

%.o:%.cpp
	${CPP} ${CPPFLAGS} -I ${HEADER} -c $< -o ${<:.cpp=.o}

all:		${NAME}

-include $(DEPS)
${NAME}:	${OBJS}		
			${CPP} ${CPPFLAGS} ${OBJS} -o ${NAME}

-include ${DEPSBOT}
bonus:	${OBJSBOT}
			${CPP} ${CPPFLAGS} ${OBJSBOT} -o irc_bot
			@touch $@ 

test:		${NAME}
			./ft_irc

clean: 		
			${RM} ${OBJS} $(DEPS)

fclean: 	clean
			${RM} ${NAME}
			@${RM} irc_bot
			@${RM} bot/Bot.o bot/Bot.d bonus

re:			fclean
			${MAKE}

.PHONY: all clean fclean re test