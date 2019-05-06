#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#define PORT "10069"

char Rbuff[255];
char Sbuff[255];
int sockfd = 0;

void *get_in_addr(struct sockaddr *sa);
void sigint_handler(int sig);
void * response_thrd(void *fd);
void * command_thrd(void *fd);
int is_empty(const char *s);

int main(int argc, char *argv[])
{
  // handle all given info
  struct addrinfo hints, *result, *x;  //hold address info
  int status, n;  // variables to check if there was no error
  char ipstr[INET6_ADDRSTRLEN];  //will hold ip address
  char * user_given_port;  //user give port num
  char * host_name;  //user given host name
  char * portnum = PORT;  // actual port number
  
  // check to see if client gives both host name and portnumber
  if (argc != 3)
  {
    fprintf(stderr,"usage: provdie ip hostname and port number\n");
    return 1;
  }
  
  // checks to see if the port number given matches the one of the server
  user_given_port = argv[2];
  if (strcmp(portnum,user_given_port) != 0){
      fprintf(stderr,"Invalid port number given\n");
    exit(1);
  }
  host_name = argv[1];  
    
  //initializes struct to 0 make sure everything is cleared
  memset(&hints,0,sizeof(hints));
  hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 if you want to use a specific version
  hints.ai_socktype = SOCK_STREAM; // accepts a tcp port
  
  //set address infor to given name and port of specified type by hints and puts it all into the result
  if((status = getaddrinfo(host_name,PORT,&hints,&result)) != 0)
  {
    //if something goes wrong exits main method ending client program
    fprintf(stderr,"getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }
  //connects to first availiable socket given if not closes socket
  for(x = result; x != NULL ; x = x->ai_next){
    if((sockfd = socket(x->ai_family, x->ai_socktype, x->ai_protocol)) == -1){
      perror("client: socket");
      continue;
    }
    int attempts = 0;
    while(attempts < 100){
    if(connect(sockfd, x->ai_addr, x->ai_addrlen) == -1){
      printf("Connecting please wait\n");
      attempts++;
      sleep(3);
     // close(sockfd);
      }
      else
      {
        break;
      } 
    }
    break;
  }// end of for loop
 
  
    if(x == NULL){
      //close(sockfd);
      fprintf(stderr,"Failed to connect\n");
      return 2;
    }
  //if socket was not found then returns with error
    /*int y = 0;
    while(x == NULL){
    printf("Please wait tyring to connect\n");
    sleep(3);
    y++;
    if(y == 10){
      fprintf(stderr,"Failed to connect\n");
      return 2;
    }
  }*/
  // convert ip address given from binary to text and then stores it in x
  inet_ntop(x->ai_family, get_in_addr((struct sockaddr *)x->ai_addr),ipstr, sizeof(ipstr));
  
  printf("client: connecting to %s\n", ipstr);
  
  // successfully connects to server address info was passed and longer needed to struct is free
  freeaddrinfo(result);
  
  // creates threads for handling responses from server and sending commands to server
  pthread_t response;
  pthread_t command;
  
  // checks for sigint
  signal(SIGINT, sigint_handler);
  
  // creates thread to send msgs to server
  if(pthread_create(&command,0,command_thrd,&sockfd) != 0)
  {
    error("Error on command input thread\n");
  }
  //creates thread to get responses from server
  if(pthread_create(&response,0,response_thrd,&sockfd) != 0)
  {
    error("Error on response output thread\n");
  }
  
  // waites until done sending msgs before extiting main method and closing socket
  pthread_join(command,NULL);
  printf("Ending connection Goodbye.\n");
  close(sockfd);
  
  return 0;
}// end of main method
//==================================================================================================
// get sockaddr of IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa)
{
  if(sa->sa_family == AF_INET){
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}// end of get addr IPv4 or IPv6 method

// in case ctrl+c was pressed it closes the socket gracefully
void sigint_handler(int sig)
{
  memset(Sbuff, '\0',strlen(Sbuff));
  strcpy(Sbuff,"Done");
  if((send(sockfd,Sbuff,strlen(Sbuff),0))== -1)
  {
    printf("Error could not close client account session.\n");
    close(sockfd);
    exit(1);
  }
  exit(0);
}
//==================================================================================================
// will allow for communication with thread 
void * command_thrd(void *fd)
{
  // takes the file descpriptor which allows to communicate through the socket
  int cmndfd = *(int *)fd;
  
  printf("Welcome to the online Banking System\n");
  // loop that continously sends msgs to to server until given certain command
  while(1){
    printf("\nYou may now enter a command\n");
    //puts("Create an account by entering 'create'");
    //puts("Log into an existing account with 'serve'");
    //puts("Exit Banking System with 'quit'\n");
    
    //printf("\nYour Command: ");    
    //will take the command given by user
    char cmnd[200];
    memset(cmnd, '\0', 200);
    memset(Sbuff,'\0',strlen(Sbuff));
    
    // will hold message based on if it as erro, operation to be done or argument
    char error[100];
    char operations[100];
    char arguments[100];
    
    //gets said command from user via stdin
    fgets(cmnd,200,stdin);
    
    //takes the newline character of the string input and changes it into a null character
    if((strlen(cmnd)>0) && (cmnd[strlen(cmnd)-1]=='\n'))
      cmnd[strlen(cmnd)-1] == '\0';
      
    // NOT SURE YET
    int params = sscanf(cmnd, "%s %s %s", operations, arguments, error);
    
    if(params == 2)
    {
      strcat(operations," ");
      strcat(operations, arguments);
    }
    else if(params > 2)
    {
      printf("Command not recognized.\n");
      continue;
    }
    strcpy(Sbuff,operations);
    //strcpy(Sbuff,cmnd);
    if((send(cmndfd,Sbuff,strlen(Sbuff),0))== -1)
    {
      printf("Message was not sent.\n");
      close(cmndfd);
      exit(1);
    }
    sleep(2);
  }// end of command while loop
  return NULL;
}// end of command method
//==================================================================================================
// method thread to receive responsed from server
void * response_thrd(void *fd)
{
  //opens file descriptor socket'same socket for command' that allows responses to reach client
  int respondfd = *(int *)fd;
  while(1){
    //makes sure the receive buffer is cleared before message is received from server
    memset(Rbuff,'\0',strlen(Rbuff));
    int res = recv(respondfd,Rbuff, sizeof(Rbuff),0);
    // if result comes back 1 then error occured if 0 then closing connection
    if(res <= 0)
    {
      printf("Connection Closed due to Error.\n");
      exit(1);
    }
    if(strcmp(Rbuff,"end")==0)
    {
      printf("Connection Closed By Server\n");
      exit(0);
    }
    if(is_empty(Rbuff) == 0){
			printf("%s\n", Rbuff);
		}
    //printf("herror");
  }// end of respond while loop
  return NULL;
}
//==================================================================================================
int is_empty(const char *s) 
{
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}
 /* printf("IP addresses for %s:\n\n",argv[1]);
  
  for(p = result; p != NULL; p = p->ai_next)
  {
    void * addr;
    char * ipver;
    
    //get pointer from the address
    if(p->ai_family == AF_INET)
    {
      struct sockaddr_in * ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPv4";
    }
    else{
      struct sockaddr_in6 * ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPv6";
    }
    
    inet_ntop(p->ai_family,addr,ipstr, sizeof(ipstr));
    printf("  %s:  %s\n", ipver,ipstr);
    
  }// end of for loop*/