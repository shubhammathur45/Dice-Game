
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>



int main(int argc, char *argv[]){

        srand(time(NULL));
        int port;
	int clientSocket, n;
	struct sockaddr_in clientAddr;
	char buffer[1024];

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	
	clientAddr.sin_family = AF_INET;
        sscanf(argv[2], "%d", &port);
	clientAddr.sin_port = htons((uint16_t)port);

// "137.207.82.51"
	if(inet_pton(AF_INET, argv[1], &clientAddr.sin_addr)<0){
		fprintf(stderr, " inet_pton() has failed\n");
		exit(2);
	}

	if((connect(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr))) < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}

	printf("[+]Connected to Server.\n");
	printf("Waiting for referee's message... \n");



	while(1){		/* reading server's messages */
		if(n=read(clientSocket, buffer, 1024)){
                buffer[n]='\0';
 		fprintf(stderr,"Referee : %s\n", buffer);
	          if(!(strcasecmp(buffer,"You can now play"))){
		//Generating random number as points for the player
		  int dice;
  		  printf ("Rolling dice\n");
      		  dice = rand() % 6 + 1;
      		  printf ("Got %d points\n", dice);
                  printf("\n"); 
		  sprintf(buffer, "%d", dice);
                //Sending score to server 
		  write(clientSocket, buffer, sizeof(buffer));
	          }
                //Receiving winning message from server
                else if(!(strcasecmp(buffer, "Game over: you won the game"))){
		printf("I won the game\n");
		close(clientSocket);
		exit(0);
		}
                //Receiving losing message from server  
                else if(!(strcasecmp(buffer, "Game over: you lost the game"))){
		printf("I lost the game\n");
		close(clientSocket);
		exit(0);
		}
	        }
       }

return 0;
}
