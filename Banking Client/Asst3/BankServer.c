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
#include <errno.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
//#include "AccountLog.h"
#include "AccountLog.c"
#include "BankServer.h"
#define GROWTH 10

int numAccounts;
int BankSize = GROWTH;

pthread_mutex_t accounts[50];
pthread_mutex_t account_lock = PTHREAD_MUTEX_INITIALIZER;
sem_t Diagnostic;

AccountLog ** Logger;
fdnode * fdhead = NULL;
volatile sig_atomic_t print_flag = false;

//==================================================================================================
// will create intial socket then create new thread to check for connecting clients
int main(int argc, char *argv[])
{
  //struct sigaction sa;
  //struct itimerval timer;
  sem_init(&Diagnostic, 0, 1);
  Logger = (AccountLog **)malloc(50*sizeof(AccountLog *));
  pthread_t accept_clients;  // thread to listen and accept clients
  pthread_t account_info;  // thread to print the account information
  
  //memset(&sa,0,sizeof(sa));
  //sa.sa_handler = &timer_handler;
  //sigaction(SIGVTALARM,&sa,NULL);
  
  signal(SIGINT, sigint_handler);  //will wait for when SIGINT and shut down all clients
  signal(SIGALRM, sem_handler);
  
  // create thread to go find clients and accept them
  if(pthread_create(&accept_clients, 0, listen_connect, 0) !=0)
    error("Error on thread to begin server\n");
  
  pthread_t caller;  
  pthread_create(&caller,0,Print_All_Accounts,0);
  pthread_join(accept_clients,NULL);  // wait until all clients are done before sever closes
  sem_destroy(&Diagnostic);
  
  return 0;
}// end of main method

//==================================================================================================
//thread method to open a socket and listen for sockets trying to connect
void * listen_connect(void * args)
{
  char addr[INET6_ADDRSTRLEN];  //hold ip address
  char buffer[600];  //send msgs with buffer
  int yes = 1;
  int status; // to check if there isn't errors with status
  int listenfd = 0; // sever socket'listen'
  int connectfd = 0; //accept client socket 'connect'
  socklen_t sin_size;  // size of socket
  struct addrinfo hints, *servinfo, *temp;   // sturct to hold address info
  struct sockaddr_storage client_addr;  // struct to hold client address info
  
  // initialize listeners
  listenfd = socket(AF_INET,SOCK_STREAM,0);
  
  // set buffer to null and struct to hold address to 0
  memset(buffer,'\0',sizeof(buffer));
  memset(&hints, 0, sizeof(hints));  // make sure struct is empty
  hints.ai_family = AF_UNSPEC;      // dont matter if its IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sock
  //hints.ai_flags = AI_PASSIVE;  // fills in IP with local host not needed
 
   //set server address to be given info then move it to servinfo for final result 
  if((status = getaddrinfo("kill.cs.rutgers.edu","10069",&hints,&servinfo))!= 0){
    fprintf(stderr,"getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
  //check for results and bind to connection 
  for(temp = servinfo; temp != NULL; temp = temp->ai_next)
  {
    // listen for socket if found retrieves all info
    if((listenfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1){
      perror("Error on listenfd\n");
      continue;
    }
    // set up socket operations to be done and to reuse socket if closed
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
      perror("setsockopt\n");
      continue;
    }
    // binds the sockets together by getting the address and the address length
    if ((status = bind(listenfd,servinfo->ai_addr,servinfo->ai_addrlen)) == -1){
      perror("Error when trying to bind\n");
      continue;
    }
    break; 
  }// end of for loop
  // done with getting info from server struct addr
  freeaddrinfo(servinfo);
  
  // if bind was unsuccessful to socket then temp will be null
  if(temp == NULL){
    error("server: failed to bind\n");
  }
  // if able to open allow up to 5 clients to connect to the socket at once
  if(listen(listenfd, 5) == -1){
    error("listen\n");
  }
  
  printf("server: waiting for connections...\n");
  
  while(1){
    // continously listen for connection
    if(listen(listenfd,5)== -1){
      printf("Could not listen\n");
    }
    // if connection found accept and begin client sever communication
    connectfd = accept(listenfd,(struct sockaddr *)&client_addr, &sin_size);
    printf("Connected to a client\n");
    strcpy(buffer,"SEVER: connection was successful! Enter anything to begin.\n");
    // write first msg to client
    write(connectfd,buffer,strlen(buffer));
    
    // create thread for specific client
    pthread_t account_use;
    
    if(pthread_create(&account_use,0, client_thread, &connectfd) != 0){
      printf("Could not launch service thread.\n");
      sleep(1);
    }
  }
  return NULL;  
  //wellif(listen(listenfd,5) == -1
  // this commented out section is for clearing the socket but when do i use it?  
  /*int yes=1;
  //char yes='1'; // Solaris people use this

  // lose the pesky "Address already in use" error message
  if (setsockopt(listener,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1)
      error("setsockopt"); */


  //return 0;
}// end of listen connect method
//==================================================================================================
// 
void * client_thread(void *args)
{
  // client server file descriptor
  int fd = *(int *)args;
  add(fd);
  // to get the name of the client
  char UserName[255] = {'\0'};
  //int indicator = 0;
  
  while(1){
    int num;
    //printf("Clear buff\n");
    char buf[255] = {'\0'}; // set buff to null
    
    //receive msg from client and put it in buff
    if((num = recv(fd,buf,255,0))==-1)
    {
      printf("Client exited unexpectedly during session.\n");
      break;
    }
    else if(num == 0)
    {
      printf("SEVER: Client ended connection before entering a command.\n");
      break;
    }
    
    buf[num]='\0';  //will be greater than msg sent and so makes that last part null character
   // printf("SEVER: Data collected from client: %s\n",buf);
   
  // printf("Cleared op and arg\n"); 
   char operations[100];
   char arguments[100] = {'\0'};
   sscanf(buf,"%s %s",operations, arguments);
   
   memset(buf,'\0',255);
   client_communication(operations,arguments,UserName,buf,fd);
   /*if(indicator == 0){
     indicator = client_communication(operations,arguments,UserName,buf,fd);
    }
    else if(indicator == 1){
      pthread_mutex_lock(&account_lock);
      indicator = Serve_Acc(operations,arguments,UserName,buf);
      pthread_mutex_unlock(&account_lock);
    }*/
    if((send(fd,buf,100,0))== -1){
      printf("Unable to send message\n");
      close(fd);
      break;
    }
  }// end of service while loop
  delete(fd);
  close(fd);
  return NULL;
}// end of sevices method
//==================================================================================================
void client_communication(char *op, char *arg, char *name, char buf[],int fd)
{
  //printf("helper\n");
 // printf("SEVER: Communication with client has begun: (%s) [%s]\n",op,arg);
  memset(buf, '\0', strlen(buf));
  //printf("HELP");
  
  if(strcmp(op,"create") == 0){
    if(is_empty(arg) != 0){
      sprintf(buf,"Please enter a name to create an account.");
      return;
    }
    if(name[0] != '\0'){
      sprintf(buf,"Account is in session can't open.");
      return;
    }
    if((If_Acc_Exist(Logger,numAccounts, arg)) == 1){
      sprintf(buf,"Account already exits.");
      return;
    }
    pthread_mutex_lock(&account_lock);
    AccountLog * newAcc = Add_To_Log(Logger,numAccounts,arg);     
      
    /*int i = 0;
    for(i = 0; i < numAccounts; i++)
    { 
      printf("account: %s\n", Logger[i]->Account_Name);
    }*/
      
    numAccounts++;
    sprintf(buf,"Opened Account [%s] successfully",newAcc->Account_Name);
    
    pthread_mutex_unlock(&account_lock);
      //printf("Client Exited pthread\n");   
  }// end of create condition
  else if(strcmp(op,"serve")==0){
    if(name[0] != '\0'){
      sprintf(buf,"Already in a session finish current session to start a new one.");
      return;
     }
     if(is_empty(arg) != 0){
       sprintf(buf,"Please enter an account name.");
       return;
     } 
     int locator = Find_Account(Logger,numAccounts,arg);
     if(locator == -1){
       sprintf(buf,"Account not found please try entering the name again.");
       return;
     }
     if(pthread_mutex_trylock(&accounts[locator]) != 0){
       sprintf(buf,"Account is already in use please try again at a later time.");
       return;
     }
     Logger[locator]->Session_Flag = 0;
     strcpy(name,arg);
     sprintf(buf,"SEVER: Account: [%s] OPENED",name);
  }// end of serve condition
  else if(strcmp(op,"deposit") == 0){
    if(name[0] == '\0'){
      sprintf(buf,"You are not in session enter a session to deposit amount.");
      return;
    }
    if(is_empty(arg) != 0){
      sprintf(buf,"Amount entered invalid please try again");
      return;
    }
    if(Deposit(Logger,name,numAccounts,atof(arg)) == -1){
      sprintf(buf,"Could not enter amount.");
      return;
    }
    sprintf(buf,"Amount deposited successful.");
  }// end  deposit condition
  else if(strcmp(op,"withdraw") == 0){
   if(name[0] == '\0'){
      sprintf(buf,"You are not in session enter a session to withdraw amount.");
      return;
    }
    if(is_empty(arg) != 0){
      sprintf(buf,"Amount entered invalid please try again");
      return;
    }
    if(Withdraw(Logger,name,numAccounts,atof(arg)) == -1){
      sprintf(buf,"Could not withdraw that amount please enter lower amount.");
      return;
    }
    sprintf(buf,"Amount withdrawn successful.");
  }// end of withdraw condition
  else if(strcmp(op,"query") == 0){
    if(name[0] == '\0'){
      sprintf(buf,"Can only access account info once in serve session");
      return;
    }
    int locate = Find_Account(Logger,numAccounts,name);
    sprintf(buf,"Name: %s\tBalance: $%.2f",Logger[locate]->Account_Name,Logger[locate]->Account_Balance);
  }// end of query condition
  else if(strcmp(op,"end") == 0){
    if(name[0] == '\0'){
      sprintf(buf,"You are not in the account serve");
      return;
    }
    int locator = Find_Account(Logger,numAccounts,name);
    pthread_mutex_unlock(&accounts[locator]); 
    Logger[locator] ->Session_Flag = 1;
    name[0] = '\0';
    sprintf(buf,"Left serve session");
  }// end of end condition
  else if(strcmp(op,"quit")==0){
    if(name[0] != '\0'){
      int locator = Find_Account(Logger,numAccounts,name);
      pthread_mutex_unlock(&accounts[locator]);
      Logger[locator]->Session_Flag = 1;
    }
    sprintf(buf,"end");
  }// end of quit condition
  else{
    sprintf(buf,"Invalid input ");
    return;
  }
}// end of communication method
//==================================================================================================
/*int Serve_Acc(char *op, char *arg, char *name, char buff[]){
  printf("SEVER: (Serving) with client has begun: (%s) [%s]\n",op,arg);
  if(strcmp(op,"query") == 0){
  
    return 1;
  }
  else if(strcmp(op,"withdraw")== 0){
  
    return 1;
  }
  else if(strcmp(op,"deposit")== 0){
    return 1;
  }
  else if(strcmp(op,"end") == 0)
    return 0;// to go back to communication method
    
  return 1;
}
//==================================================================================================
void * print_all_accounts(void * arg)
{
  //Print("========== DISPLAYING ALL ACCOUNT INFORMATION =========\n\n");
  while(1){
    // use semaphore here and signal for when trying to create account during diagnostic
    Print_Log(Logger,numAccounts);
    
    //setitimer(15)?
  }
}*/
//==================================================================================================
// get sockaddr of IPv4 or IPv6
void *get_in_addr(struct sockaddr *sa)
{
  if(sa->sa_family == AF_INET){
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}// end of get addr IPv4 or IPv6 method
//==================================================================================================
void sigint_handler(int sig)
{
  printf("SERVER: shutting down, will now be closing all clients connected.\n");
  
  fdnode * closer = fdhead;
  
  while(closer != NULL){
  
    int fd = closer->fd;
    
    if((send(fd,"end",3,0))== -1){
      printf("Error: Could not shut down a client.\n");
      close(fd);
      break;
    }  
    closer = closer->next;
  }// end of while loop
  
  exit(0); 
}
//==================================================================================================
void * Print_All_Accounts(void * args)
{
  alarm(15);
  while(1)
  {
    sem_wait(&Diagnostic);
    if(print_flag)
    {
      Print_Log();
      print_flag = false;
      alarm(15);
    }
    sem_post(&Diagnostic);
  }
}// end of print all logs method
//==================================================================================================
void Print_Log()
{
  //AccountLog ** temp = log;
  int i;
  printf("\n[Current Accounts]\n");
  for(i = 0; i <50; i++)
  {
    if(Logger[i]==NULL){
      break;
    }
    else{
      if(Logger[i]->Session_Flag == 1)
        printf("%s\t$%.2f\t\n",Logger[i]->Account_Name,Logger [i]->Account_Balance);
      else
        printf("%s\t$%.2f\tIN SESSION\n",Logger[i]->Account_Name,Logger [i]->Account_Balance);   
    }
    printf("\n");
   // temp++;
  }
}
//==================================================================================================
void sem_handler(int sig)
{
  print_flag = true;
} // end of print flag method
//==================================================================================================
// simple method to print out error depending on where code goes wrong
void error(const char * msg)
{
  perror(msg);
  exit(1);
}// end of error method
//==================================================================================================
int is_empty(const char *s) 
{
  while (*s != '\0') {
    if (!isspace(*s))
      return 0;
    s++;
  }
  return 1;
}// end of empty method
//==================================================================================================
fdnode* create_node(int fd){
	fdnode* ret = (fdnode *)malloc(sizeof(fdnode));
	ret->fd = fd;
	ret->next = NULL;
	return ret;
}
//==================================================================================================
fdnode* add(int fd){
	fdnode* new_node = create_node(fd);
	if(fdhead == NULL){
		fdhead = new_node;
		return fdhead;
	}
	fdnode* temp = fdhead->next;
	fdhead->next = new_node;
	new_node->next = temp;
	return new_node;
}
//==================================================================================================
int delete(int fd){
	fdnode* temp = fdhead;
	fdnode* next;
	if(temp != NULL){
		if(temp->fd == fd){
			fdhead = temp->next;
			free(temp);
			return 0;
		}
		next = temp->next;
	} else {
		return 0;
	}
	while(next != NULL){
		if(next->fd == fd){
			temp->next = next->next;
			fdnode* tempnext = next->next;
			free(next);
			next = tempnext;
		}
	}
	return 0;
}