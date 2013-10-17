#ifndef IRC_CONNECTION_H
#define IRC_CONNECTION_H
#include "IRC_Types.h"

#define MAXMESSAGESIZE 551

//Everything will return 1 on success and 0 on failure

static Message_Type get_type(char *parse);
static IRC_Message chunk_message(char* msg);
static void next_line(IRC *irc, char *msg);
static void pong(IRC *irc, char *arg);
int say_to_channel_(IRC *irc, char *channel, char *message);
int send_raw(IRC *irc, char *message);
int join_channel(IRC *irc, char *channel);
static int connect_to_server(IRC *irc);
void start_IRC_loop(IRC *irc, char *channel);
#endif 
