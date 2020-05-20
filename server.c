
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>

void servicePlayers(int, int);

int main(int argc, char *argv[]){

	int sd, port;
	struct sockaddr_in serverAddr;
	int noOfClients =0;

	socklen_t addr_size;

	if(argc != 2){
		printf("Call model: %s <Port #>\n", argv[0]);
		exit(0);
	}

	// Creating server socket..
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	//Configuring settings of the server address struct...
	serverAddr.sin_family = AF_INET;
	sscanf(argv[1], "%d", &port);
	serverAddr.sin_port = htons((uint16_t)port);
	serverAddr.sin_addr.s_addr =  htonl(INADDR_ANY);

	//Binding the address struct to the socket...
	if((bind(sd, (struct sockaddr*)&serverAddr, sizeof(serverAddr))) < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", ((uint16_t)port));


	//Socket istening for the clients, with max 10 connection requests queded... 
	if(listen(sd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in listening.\n");
	}

	while(1){		

		int client1;
		int client2;

		sleep(1);


		client1 = accept(sd, (struct sockaddr*) NULL, NULL);
		if(client1 < 0){
			exit(1);
		}
		printf("\n**********Initializing the Game**********\n");

		printf("Player 1 has joined the lobby\n");
		printf("\n");
		printf("Waiting for another player to start the game...\n");
		printf("\n");

		client2 = accept(sd, (struct sockaddr*) NULL , NULL);
		if(client2 < 0){
			exit(1);
		}
		printf("Player 2 has joined the lobby\n");
		printf("\n");
      
      		//Once two player joins the lobby it forks...
		int pid;
		pid = fork();	
		if(pid == 0){
			// Child Process will play role of Referee
			servicePlayers(client1, client2);	
		}

		printf("**********Game Started!**********\n");
		printf("\n");
	}
	return 0;
}

void servicePlayers(int client1, int client2){

	//referee
	char buffer[1024];
	int n;
	int points1 =0;
	int points2=0;

//	sleep(2);
	write(client1, "You are TOTO\n", 1024);
	write(client2, "You are TITI\n", 1024);
	sleep(1);  
	while(1){        
   
   	//handling Client1
		write(client1,"You can now play", 1024);

		//reading client1 messages...

		printf("\n");
		if(n=read(client1, &buffer, 1024)){
			buffer[n] = '\0';
			fprintf(stderr, "TOTO Scored : %s\n", buffer);
        		points1 += atoi(buffer);
        		printf("Current Total Score: %d\n", points1);
		}
		sleep(2);

	//handling Client2	
   		write(client2,"You can now play", 1024);
    
    	
		// reading client2 messages
		if(n=read(client2, &buffer, 1024)){
			buffer[n] ='\0';
			fprintf(stderr, "TITI Scored : %s\n", buffer);
			points2 += atoi(buffer);
			printf("Current Total Score: %d\n", points2);
	
		}
  
    	if(points1 >= 100 ){ // if TOTO: client 1 wins the game    
    	       write(client1, "Game over: you won the game", 1024);
    	       write(client2, "Game over: you lost the game", 1024);
    	       printf("\n\n");
		printf("**********Game Over**********\n");
		printf("!!!!!## WINNER : TOTO ##!!!!!\n");
		printf("\n\n");
    	       close(client1);
    	       close(client2);
		exit(0);

	}else if(points2 >= 100 ){ // If TITI: client 2 wins the game
	       write(client2, "Game over: you won the game", 1024);
    	       write(client1, "Game over: you lost the game", 1024);
    	       printf("\n\n");
    		printf("**********Game Over**********\n");
		printf("!!!!!## WINNER : TITI ##!!!!!\n");
		printf("\n\n");
	       close(client1);
    	       close(client2);
	       exit(0);
		}
	}    	 
}


