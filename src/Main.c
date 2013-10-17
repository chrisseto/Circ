#define LOGGING 1
#define DEBUG 1
#include "../headers/IRC_Connection.h"

void main(int argc, char *args[])
{
	IRC bot;
	IRC_init(&bot,"chat.freenode.net",8000,"SetBot","none");
	start_IRC_loop(&bot, "#wutwut"); 
	IRC_quit(&bot,"Leaving");
}
