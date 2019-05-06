typedef struct fdnode{
	int fd;
	struct fdnode *next;	
} fdnode;

int delete(int fd);
fdnode* add(int fd);
fdnode* create_node(int fd);

// headers for main program
void * listen_connect(void * args);
void *get_in_addr(struct sockaddr *sa);
void * client_thread(void *args);
void * Print_All_Accounts(void * arg);
void sem_handler(int sig);
void sigint_handler(int s);
void Print_Log();
void error(const char * msg);
//void Print_All_Accounts(void * args);
void client_communication(char *op, char *ar, char *acc_name,char buf[],int fd);
//int Serve_Acc(char *op, char *arg, char *name, char buff[]);
int is_empty(const char *s);
