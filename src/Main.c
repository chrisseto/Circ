#include "../headers/IRC_Connection.h"

void main(int argc, char *args[])
{
	IRC bot;
	IRC_init(&bot,"chat.freenode.net",8000,"SetBot","none");
	IRC_quit(&bot,"Leaving");
}
