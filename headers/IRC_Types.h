#ifndef IRC_Types_H
#define IRC_Types_H

#define MAXUSERSIZE 20

typedef struct List
{
	char **array;
	int size;
	int used;
} List;

typedef struct IRC_Message
{
	IRC_Channel *orgin;
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
#endif
