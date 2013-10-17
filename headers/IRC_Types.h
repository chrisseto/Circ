#ifndef IRC_Types_H
#define IRC_Types_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAXUSERSIZE 20

typedef enum
{
	PRIVMSG = 0,
	PING = 1,
	MODE = 2,
	MOTD = 3,
	KICK = 4,
	NOTICE = 5,
	NAMES = 6,
	PART = 7
} Message_Type;

typedef struct List
{
	char **array;
	int size;
	int used;
} List;

typedef struct IRC_Message
{
	Message_Type type;
	char *target;
	char *sender;
	char *message;
} IRC_Message;

typedef struct IRC
{
	char *server;
	int port;
	int socket;
	char *pass;
	char *nick;
	char *user;
	int connected;
	List channels;
	void (*Message_Recieved)(IRC_Message*);
	void (*Bot_Messaged)(IRC_Message*);
} IRC;

void create_list(List *n, int initsize);
void add_element(List *list, char *handle);
void remove_element(List *list, char *handle);
void remove_elementn(List *list, int index);
int list_contains(List *list, char *search);
void free_list(List *list);
void IRC_init(IRC *irc, char *Server, int Port, char* Nick, char* Pass);
void free_irc(IRC *irc);
void print_message(IRC_Message *msg);
#endif
