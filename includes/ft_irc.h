#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <iostream>
#include <vector>

extern std::string identify_password;

void newClientLogin(int server);