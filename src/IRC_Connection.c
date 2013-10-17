#include "../headers/IRC_Connection.h"



void start_IRC_loop(IRC *irc, char *channel) //channel will be expanded or overloaded later
{
	connect_to_server(irc);
	join_channel(channel);
	char *buff = malloc(MAXMESSAGESIZE);
	IRC_Message temp;
	while(next_line(irc,&buff) && irc->connected)
	{
		temp = chunk_message(buff);
		switch(temp.type)
		{
			case PRIVMSG:
				irc->Message_Recieved(&temp);
				irc->Bot_Message(&temp);
			break;
			case PING:
				pong(temp.message);
			break;
		}
	}
	free(buff);
	//parse input call callbacks etc
	//endwhile
}

int connect_to_server(IRC *irc)
{
	//Creating socket and other networking black magic
	struct sockaddr_in server;
	struct hostent *host;
	host = gethostbyname(irc->server);
	if(host == NULL)
	{
		printf("EROR: No such host\n");
		return 0;
	}
	memcpy(&server.sin_addr,host->h_addr_list[0],host->h_length);
	server.sin_family = AF_INET;
	server.sin_port = htons( irc->port );
	irc->socket = socket(AF_INET , SOCK_STREAM , 0);
	if(irc->socket < 0){
		printf("ERROR: Could not open socket %d\n",errno);
		return 0;
	}
	if(connect(irc->socket , (struct sockaddr *)&server , sizeof(server))< 0)
	{
		printf("ERROR: could not connect to server %d\n",errno);
		return 0;
	}
	//<\black magic>

	//Sending information to join sever
	buff = malloc(strlen(irc->pass)+strlen(irc->nick)+strlen(irc->user)+21);
	sprintf(buff,"PASS %s\r\nNICK %s\r\nUSER %s\r\n",irc->pass,irc->nick,irc->user);
	send_raw(buff);
	free(buff);
	if(DEBUG)
		printf("Connected to %s:%d\n",irc->server,irc->port);
	irc->connected = 1;
	return 1;
}
int join_channel(IRC *irc, char *channel)
{
	if(DEBUG)
		printf("Joining %s\n",channel);
	char *buff = malloc(strlen(channel)+7);
	sprintf(buff,"JOIN %s\r\n",channel);
	send_raw(buff);
	free(buff);
	//Add check to make sure of channel joining
	add_element(irc->channels,channel);
	return 1;	
}
int send_raw(IRC *irc, char *message)
{
	if(write(irc->socket, message, strlen(message)) < 0)
	{
		printf("ERROR: Failed to write to socket %d\n",errno);
		return 0;
	}
	return 1;
}
int say_to_channel_(IRC *irc, char *channel, char *message)
{
	char *buff = malloc(strlen(channel)+strlen(message)+10);
	sprintf(buff,"PRVMSG %s :%s\r\n",channel,message);
	send_raw(irc,buff);
	free(buff);
}
void pong(IRC *irc, char *arg)
{
	char *buff = malloc(strlen(arg) + 9);
	sprintf(buff,"PONG %s\r\n",arg);
	if(DEBUG) 
		printf("%s",buff);
	send_raw(irc,buff);
	free(buff);
}
void next_line(IRC *irc, char *msg)
{
	//read in line
	//chunk into irc_message
	//free things?
	char next;
	int i = 0;
	while(read(sock,&next,1)>0 && i < MAXMESSAGESIZE)//change me to a #DEFINE or some shit
	{
		if(next == '\n')
		{
			msg[i] = '\0';
			return 1;
		}
		else if(next != '\r')
		{
			msg[i] = next;
			i++;
		}
	}
	//This function needs to be more refined
	if(DEBUG)
		printf("Read 0 bytes from socket,Something went wrong\n");
	return 0;
}
IRC_Message chunk_message(char* msg)
{
	//:<sender> <command> <params> :<message>
	//<command> :<message>
	//IN PROGRESS
	IRC_M chunked;
	char *buff;
	buff = strtok(msg," ");
	if(msg[0] == ':')
	{
		chunked.sender = buff + 1;
		buff = strtok(NULL," ");
		chunked.type = get_type(buff);
		buff = strtok(NULL," ");
		chunked.target = buff;
		buff = strtok(NULL,"\n");
		if(buff != NULL)
			chunked.message = buff;
		else
			chunked.message = NULL;
	}
	else
	{
		chunked.type = get_type(buff);
		buff = strtok(NULL," ");
		chunked.message = buff + 1;
		chunked.sender = NULL;
		chunked.target = NULL;
	}
	return chunked;
}
Message_Type get_type(char *parse)
{
	if(strcmp(parse,"PING")
		return PING;
	if(strcmp(parse,"PRIVMSG")
		return PRIVMSG;
	if(strcmp(parse,"MODE")
		return MODE;
	if(strcmp(parse,"KICK")
		return KICK;
	if(strcmp(parse,"MOTD")
		return MOTD;
	if(strcmp(parse,"PART")
		return PART;
	if(strcmp(parse,"NOTICE")
		return NOTICE;
	if(strcmp(parse,"NAMES")
		return NAMES;
	return -1;
	//if(strcmp(parse,"")
		//return ;
}
