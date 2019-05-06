#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include "modcsvSorter.c"

//global variables for pipe so new pipes aren't created when a recursive call is made
int myPipefd [2];
int depth = 0;
int i;



//void flagchecker(int argc, char * argv[]);
void listdir( char *name, char * outdir, char *headerName);


void listdir(char *name, char * outdir,  char * headerName)
{
 char path[1024]; // creates string to hold path name
    DIR *dir;  //struct directory pointer
    struct dirent *entry;  // pointer to each 'directory' entry
    //opens directory if initial directory entered is not a directory closes
    if (!(dir = opendir(name)))
    {
        printf("FATAL ERROR: search directory not found.\n");
        exit(1);
    }
    if(depth == 0)
      printf("Initial pid is %d\n",getpid());

    //reads directory until it it gets to the end
    while ((entry = readdir(dir)) != NULL) {
        //if entry in directory is another directory it checks if its the . or .. directory
        // if it is the . or .. it skips them
        if (entry->d_type == DT_DIR) {
          //  char path[1024]; // creates string to hold path name
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

           // if (strcmp
            //forks if it is a valid directory  
            
            int pid = fork();
            if(pid == 0)
            {
              //pid_t child = getpid();
              //if its the child then  it writes to my pipe and increments the depth
              write(myPipefd[1], &i,1);
	            depth += 1;
              //printf("Depth is now %d\n",depth);
              //concats initial name and new entry directory name into path in string/string format           
              snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
              //printf("%*s[%s]\n", indent, "", entry->d_name);
              //printf("Sub directory found path is: %s\n",path);
              listdir(path, outdir, headerName);
              return;
              // recurses back to the beginning of listdir to read all contents within sub dir
            }
            else if(pid > 0){ //when forked if it's the parent then it waits for the child to list its content
              //*(pids + depth) = pid;
              //printf("%d, ", pid);
              //fflush(stdout);
              wait(NULL);
            }
            
            
        }
        // if entry isn't a sub directory then just prints it out 
        else {
                int pid = fork();
                if(pid == 0)
                {
                  //pid_t child = getpid();
                  write(myPipefd[1], &i,1);
                  depth += 1;
                  int fileNameLen = strlen(entry->d_name);
				          if (fileNameLen > 4 && strcmp(entry->d_name + fileNameLen - 4, ".csv") == 0 &&strstr(entry->d_name,"-sorted-") == NULL){               
                    snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);

                 // printf("path: %s\n", path);
                 // printf("sorting: %s\n", headerName);
                 int valid=0;

                  valid = checkvalid(path);
                  
                  if (valid ==0)
                  {
                      
                      fprintf(stderr, "ERROR: Invalid CSV Format\n");
                      break;
                  }
                  csvSorter(path, outdir, entry->d_name, headerName);
                  

                  //printf("Depth is now %d\n",depth);
                  return;
                }
                else{
                // write(2,"Not CSV file.\n",14);
                    return;
                   /*printf("ga%d, ", pid);
                   fflush(stdout);
                   wait(NULL);*/
                }
              }
            else if(pid > 0){
                   printf("%d, ", pid);
                   fflush(stdout);
                   wait(NULL);
                } 
        }
    }
    //close pipe
    close(myPipefd[1]);
    // if initial process then it counts number of children spawned and prints total
   	if(depth == 0)
		{
			i = 0;
     // pid_t pidHolder;
			while(read(myPipefd[0], &depth,1) != 0)
      {
        //printf("%d ,",
        //printf("PIPE[%d]: %X\n", i, myPipefd[i]);
				i += 1;
      }
			printf("\nTotal number of processes: %d\n", i);
		}
    // close directory	
    closedir(dir);
}
// main method which takes in user input 
int main(int argc, char* argv[]) {
    // creates inital pipe to children 
    // created here because children will never get to this method
    
  //  flagchecker(argc, argv);
int thisdir =0;
char *outdir;
char *searchdir;
searchdir = (char *)malloc(sizeof(char) * 3);
char * headerName;
outdir = (char *)malloc(sizeof(char) * 3);
int dyes = 0;
int oyes =0;
int cyes=0;
int i;
//printf("\n");
if(argc!=3&&argc!=5&&argc!=7)
{
printf("FATAL ERROR: INCORRECT INPUT FORMAT\n");
exit(1);
}
for (i=1; i<argc;i+=2)
{
if (strcmp(argv[i],"-c") != 0 && strcmp(argv[i],"-d")!=0 &&
strcmp(argv[i],"-o")!=0)
{
printf("FATAL ERROR: INCORRECT INPUT FORMAT\n");
exit(1);
}
else if (strcmp(argv[i],"-c")==0)
{
  headerName = malloc(sizeof(argv[i+1])*5);
  strcpy(headerName,argv[i+1]);
  cyes++;
}
else if (strcmp(argv[i],"-d")==0)
{
searchdir = malloc(sizeof(argv[i+1])*5);
 strcpy(searchdir,argv[i+1]);
 dyes++;
}
else if (strcmp(argv[i],"-o")==0)
{
outdir = malloc(sizeof(argv[i+1])*5);
  strcpy(outdir,argv[i+1]) ;
  oyes++;
}
if (cyes!=1 ||dyes>1||oyes>1)
{
printf("FATAL ERROR: Duplicate Flags, Fatal Error\n");
exit(1);
}
}//end of for loop
if (oyes!=1 && dyes!=1)
{
searchdir = ".";
outdir = ".";
}
else if (oyes!=1)
{
outdir = ".";
}
else if (dyes!=1)
{
searchdir = ".";
}

//printf("search dir %s out dir: %s\n", searchdir, outdir);
DIR* dirCh = opendir(outdir);
if (dirCh)
{
    /* Directory exists. */
    closedir(dirCh);
}
else if (ENOENT == errno)
{
    printf("FATAL ERROR: Output Directory Does Not Exist\n");
    exit(1);
}
    pipe(myPipefd);
    printf("\nInitial PID: %d\n", getpid());
    printf("PIDS of all child processes: ");
    fflush(stdout);
      listdir(searchdir, outdir, headerName);

    return 0;
}
/*
void flagchecker(int argc, char * argv[])
{
  int thisdir =0;
  char *outdir;
  char  *searchdir;
  searchdir = (char *)malloc(sizeof(char) * 3);
  char * headerName;
  outdir = (char *)malloc(sizeof(char) * 3);
  int dyes = 0;
  int oyes =0;
  int cyes=0;
  int i;
  
  if(argc!=3&&argc!=5&&argc!=7)
  {
    fprintf(stderr, "\n");
    exit(1);
  }
  for (i=1; i<argc;i+=2)
  {
    if (strcmp(argv[i],"-c") != 0 && strcmp(argv[i],"-d")!=0 &&
strcmp(argv[i],"-o")!=0)
    {
      fprintf(stderr, "\n");
      exit(1);
    }
    else if (cyes!=1 && strcmp(argv[i],"-c")==0)
    {
      headerName = malloc(sizeof(argv[i+1])*5);
      strcpy(headerName,argv[i+1]);
      cyes =1;
    }
    else if (dyes!=1 && strcmp(argv[i],"-d")==0)
    {
      searchdir = malloc(sizeof(argv[i+1])*5);
      strcpy(searchdir,argv[i+1]);
      dyes=1;
    }
    else if (oyes!=1&&strcmp(argv[i],"-o")==0)
    {
      outdir = malloc(sizeof(argv[i+1])*5);
      strcpy(outdir,argv[i+1]) ;
      oyes=1;
    }

  }//end of for loop
  if (oyes!=1 && dyes!=1)
  {
    searchdir = ".";
    outdir = ".";
  }
printf("search %s\n", searchdir);
     printf("outdir %s\n", outdir);
      listdir(searchdir, outdir, headerName);

}*/