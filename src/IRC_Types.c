#include "../headers/IRC_Types.h"

//everything here needs better error handling

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
	for(int i = 0; i < list->used; i++)
	{
		if(strcmp(handle,list->array[i])==0)
		{
			remove_elementn(list,i);
			break;		
		}
	}
}

void remove_elementn(List *list, int index)
{
	char **temp = list->array;
	list->array = malloc((list->size-1) * sizeof(char *));
	memcpy(temp,list->array,index-1);
	memcpy(temp+(sizeof(char *)*index),list->array+(sizeof(char *)*index),list->size - index);//this needs work
	free(temp);	
	list->size--;
	list->used--;
}

int list_contains(List *list, char *search)
{
	for(int i = 0; i < list->used; i++)
	{
		if(strcmp(search,list->array[i])==0)
			return 1;
	}
	return 0;
}

void free_list(List *list)
{
	free(list->array);
	list->size=0;
	list->used=0;
}

void IRC_init(IRC *irc, char *Server, int Port, char* Nick, char* Pass)
{
	irc->server = Server;
	irc->port = Port;
	irc->nick = Nick;
	irc->pass = Pass;
	irc->user = malloc((strlen(Nick)+1)*4);
	sprintf(irc->user,"%s %s %s %s",Nick,Nick,Nick,Nick);
	irc->connected = 0;
	irc->socket = 0;
	create_list(&irc->channels,3);
	irc->Bot_Messaged = NULL;
	irc->Message_Recieved = NULL;
}

void free_irc(IRC *irc)
{
	irc = NULL;
}

void print_message(IRC_Message *msg)
{
	printf("%s: %s\n",msg->sender,msg->message);
}




