#include "../headers/IRC_Connection.h"

void main(int argc, char *args[])
{
	IRC bot;
	IRC_init(&bot,"chat.freenode.net",8000,"SetBot","none");
	start_IRC_loop(&bot, "#watwat"); 
	IRC_quit(&bot,"Leaving");
}
