#ifndef IRC_CONNECTION_H
#define IRC_CONNECTION_H
#include "IRC_Types.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#define MAXMESSAGESIZE 551

//Everything will return 1 on success and 0 on failure

Message_Type get_type(char *parse);
IRC_Message chunk_message(char* msg);
int next_line(IRC *irc, char *msg);
void pong(IRC *irc, char *arg);
int say_to_channel_(IRC *irc, char *channel, char *message);
int send_raw(IRC *irc, char *message);
int join_channel(IRC *irc, char *channel);
int connect_to_server(IRC *irc);
void start_IRC_loop(IRC *irc, char *channel);
void IRC_quit(IRC *irc, char *reason);
#endif 
