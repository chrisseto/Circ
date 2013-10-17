#include "../headers/IRC_Connection.h"

//How to know if currently in channel?

void start_IRC_loop(IRC *irc, char *channel) //channel will be expanded or overloaded later
{
	connect_to_server(irc);
	join_channel(irc,channel);
	char *buff = malloc(MAXMESSAGESIZE);
	IRC_Message temp;
	while(next_line(irc,buff) && irc->connected)
	{
		temp = chunk_message(buff);
		switch(temp.type)
		{
			case PRIVMSG:
			{
				if(strcmp(temp.target,irc->nick)==0)
					irc->Message_Recieved(&temp);
				else
					irc->Bot_Messaged(&temp);
				break;
			}
			case PING:
				pong(irc,temp.message);
				break;
			default:
				break;
		}
	}
	free(buff);
}

static int connect_to_server(IRC *irc)
{
	//Creating socket and other networking black magic
	struct sockaddr_in server;
	struct hostent *host;
	host = gethostbyname(irc->server);
	if(host == NULL)
	{
		printf("ERROR: No such host\n");
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
	char *buff = malloc(strlen(irc->pass)+strlen(irc->nick)+strlen(irc->user)+21);
	sprintf(buff,"PASS %s\r\nNICK %s\r\nUSER %s\r\n",irc->pass,irc->nick,irc->user);
	send_raw(irc,buff);
	free(buff);
	printf("Connected to %s:%d\n",irc->server,irc->port);
	irc->connected = 1;
	return 1;
}
int join_channel(IRC *irc, char *channel)
{
	printf("Joining %s\n",channel);
	char *buff = malloc(strlen(channel)+7);
	sprintf(buff,"JOIN %s\r\n",channel);
	send_raw(irc,buff);
	free(buff);
	//Add check to make sure of channel joining
	add_element(&irc->channels,channel);
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
	return 1;
}
static void pong(IRC *irc, char *arg)
{
	char *buff = malloc(strlen(arg) + 9);
	sprintf(buff,"PONG %s\r\n",arg); 
	printf("%s",buff);
	send_raw(irc,buff);
	free(buff);
}
static int next_line(IRC *irc, char *msg)
{
	//read in line
	//chunk into irc_message
	//free things?
	char next;
	int i = 0;
	while(read(irc->socket,&next,1)>0 && i < MAXMESSAGESIZE)
	{
		if(next == '\n')
		{
			msg[i] = '\0';
			printf("Read: %s\n",msg);
			return 1;
		}
		else if(next != '\r')
		{
			msg[i] = next;
			i++;
		}
	}
	//This function needs to be more refined
	return 0;
}
static IRC_Message chunk_message(char* msg)
{
	//:<sender> <command> <params> :<message>
	//<command> :<message>
	//IN PROGRESS
	IRC_Message chunked;
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
		chunked.sender = strtok(chunked.sender,"!");	
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
static Message_Type get_type(char *parse)
{
	if(strcmp(parse,"PING") == 0 )
		return PING;
	if(strcmp(parse,"PRIVMSG") == 0 )
		return PRIVMSG;
	if(strcmp(parse,"MODE") == 0 )
		return MODE;
	if(strcmp(parse,"KICK") == 0 )
		return KICK;
	if(strcmp(parse,"MOTD") == 0 )
		return MOTD;
	if(strcmp(parse,"PART") == 0 )
		return PART;
	if(strcmp(parse,"NOTICE") == 0 )
		return NOTICE;
	if(strcmp(parse,"NAMES") == 0 )
		return NAMES;
	return -1;
	//if(strcmp(parse,"") == 0 )
		//return ;
}
void IRC_quit(IRC *irc, char *reason)
{
	char *buff = malloc(strlen(reason)+6);
	sprintf(buff,"QUIT :%s\n",reason);
	send_raw(irc,buff);
	free(buff);
	close(irc->socket);
	free_irc(irc);
}
