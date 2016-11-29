#include "source/includes.hpp"
#include "source/parameters.hpp"
#include "source/connection_manager.hpp"

ConnectionManager connectionManager;

int main(int argc, char **argv)
{
    if (argc == 3)
    {
		printf("NeXtRAD VLC Telnet Controller\n");
		printf("-----------------------------\n");
		system("x-terminal-emulator -e \"vlc -I telnet\n\"");
		sleep(1);
		connectionManager.connectToSocket();   
		connectionManager.configureVideoStream();	
		connectionManager.startRecording(atoi(argv[1]) + 2, atoi(argv[2]) + 4); //buffer stores 2 seconds, thus start two seconds later and record for 2 seconds longer.
		//connectionManager.startRecording(time(NULL) + atoi(argv[1]), time(NULL) + atoi(argv[1]) + atoi(argv[2]) + 2);
		connectionManager.stopRecording(); 
		return 0;
	}
	else
	{
		printf("Not enough launch parameters!\nMake sure start and end time have been supplied.");
		exit(0);
	}
}
