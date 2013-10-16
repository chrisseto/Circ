#include "../headers/IRC_Types.h"

void create_list(List *n, int initsize)
{
	n->size = initsize;
	n->used = 0;
	n->array = (char **)malloc(initsize * sizeof(char *));
}

void add_element(List *list, char *handle)
{
	if(list->size == list->used)
	{
		list->size++;
		list->array = (char **)realloc(list->array,list->size*sizeof(char *));
	}
	list->array[list->used++] = handle;
}

void remove_element(List *list, char *handle)
{
	
}

int list_contains(List *list, char *search)
{
	
}

void free_list(List *list)
{

}

void IRC_init(IRC *irc, char *Server, int Port, char* Nick, char* Pass)
{
	irc->server = Server;
	irc->port = Port;
	irc->nick = Nick;
	irc->pass = Pass;
	char *buff = malloc((strlen(Nick)+1)*4);
	sprintf(buff,"%s %s %s %s",Nick,Nick,Nick,Nick);
	irc->user = buff;
	free(buff);
	irc->connected = 0;
	irc->socket = 0;
}

void free_irc(IRC *irc)
{

}
