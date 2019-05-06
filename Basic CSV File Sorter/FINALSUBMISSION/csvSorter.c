#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csvSorter.h"
#include "mergesort.c"

int columnNum(char* firstLine, char* headerName);
const char* getfield(char* line, int colNum);
char *stringtokenizer(char *c, char *d);
struct rowData* SortedMergeInt(struct rowData* a, struct rowData* b); 
void FrontBackSplitInt(struct rowData* source, struct rowData** frontRef, struct rowData** backRef); 
void MergeSortInt(struct rowData** headRef) ;

struct rowData* SortedMergeChar(struct rowData* a, struct rowData* b); 
void FrontBackSplitChar(struct rowData* source, struct rowData** frontRef, struct rowData** backRef); 
void MergeSortChar(struct rowData** headRef) ;        

//main

int main(int argc, char *argv[]){
  
  char* headerName = argv[2];

 	//get individual lines of CSV
  //Store each line
  char *line; //Line stored in this variable
  
  size_t lineSize = 5000;
  line = (char *)malloc(lineSize * sizeof(char)); 
  size_t charNum; 
getline(&line,&lineSize,stdin);
if (stdin ==NULL){
  fprintf(stderr, "\n");
exit(1);
}
char* firstLine = strdup(line);
printf("%s", line);
 size_t len = strlen(firstLine);//replace new line character with null terminator
 firstLine[len-1] = '\0';
  firstLine[len-2] = '\0';
  
  
  int headerNumber = columnNum(firstLine, headerName);
  
  if (headerNumber==-1)
  {
  fprintf(stderr, "No column name found with entry\n");
  exit(1);
  }


 rowData * temp;
 rowData * current;
 rowData * head;
 rowData * prev;
int rowCounter = 0; 
int isIntHeaderValue = 0;
int isCharHeaderValue =0;
 
charNum = getline(&line,&lineSize,stdin);

while (charNum!=-1)
{
  if(rowCounter == 0)
  {
   char* allRow = strdup(line); 

 
   char * charValueChar = getfield(allRow, headerNumber);
  
   head = malloc(sizeof(rowData));
   head->headerName = argv[2];
   head->rowLine = strdup(line);
   
      if (strcmp (headerName, "num_critic_for_reviews") ==0 || //check if int
    strcmp (headerName, "duration") ==0 ||
    strcmp (headerName, "director_facebook_likes") ==0 ||
    strcmp (headerName, "actor_3_facebook_likes") ==0 ||
    strcmp (headerName, "actor_1_facebook_likes") ==0 ||
    strcmp (headerName, "gross") ==0 ||
    strcmp (headerName, "num_voted_users") ==0 ||
    strcmp (headerName, "cast_total_facebook_likes") ==0 ||
    strcmp (headerName, "facenumber_in_poster") ==0 ||
    strcmp (headerName, "num_user_for_reviews") ==0 ||
    strcmp (headerName, "budget") ==0 ||
    strcmp (headerName, "title_year") ==0 ||
    strcmp (headerName, "actor_2_facebook_likes") ==0 ||
    strcmp (headerName, "imdb_score") ==0 ||
    strcmp (headerName, "aspect_ratio") ==0 ||
    strcmp (headerName, "movie_facebook_likes") ==0)
{ 
   head->intValue = atoi(charValueChar);
   head->charValue = NULL;
   isIntHeaderValue = 1;
  
}
else if ((strcmp (headerName, "num_critic_for_reviews") !=0 && //check if string
    strcmp (headerName, "duration") !=0 &&
    strcmp (headerName, "director_facebook_likes") !=0 &&
    strcmp (headerName, "actor_3_facebook_likes") !=0 &&
    strcmp (headerName, "actor_1_facebook_likes") !=0 &&
    strcmp (headerName, "gross") !=0 &&
    strcmp (headerName, "num_voted_users") !=0 &&
    strcmp (headerName, "cast_total_facebook_likes") !=0 &&
    strcmp (headerName, "facenumber_in_poster") !=0 &&
    strcmp (headerName, "num_user_for_reviews") !=0 &&
    strcmp (headerName, "budget") !=0 &&
    strcmp (headerName, "title_year") !=0 &&
    strcmp (headerName, "actor_2_facebook_likes") !=0 &&
    strcmp (headerName, "imdb_score") !=0 &&
    strcmp (headerName, "aspect_ratio") !=0 &&
    strcmp (headerName, "movie_facebook_likes") !=0))
    {
      head->charValue = charValueChar;
      head->intValue = NULL;
       isCharHeaderValue = 1;
     
    }
      
   head->next = NULL;
   current = head;
   prev=head;


rowCounter ++;
 }//end of if rowcounter=0

else {

  temp = malloc(sizeof(rowData));
  char* allRow = strdup(line);
  char * charValueChar = getfield(allRow, headerNumber);
 
  temp->headerName = argv[2];
  temp->rowLine = strdup(line);

     if (strcmp (headerName, "num_critic_for_reviews") ==0 || //check if int
    strcmp (headerName, "duration") ==0 ||
    strcmp (headerName, "director_facebook_likes") ==0 ||
    strcmp (headerName, "actor_3_facebook_likes") ==0 ||
    strcmp (headerName, "actor_1_facebook_likes") ==0 ||
    strcmp (headerName, "gross") ==0 ||
    strcmp (headerName, "num_voted_users") ==0 ||
    strcmp (headerName, "cast_total_facebook_likes") ==0 ||
    strcmp (headerName, "facenumber_in_poster") ==0 ||
    strcmp (headerName, "num_user_for_reviews") ==0 ||
    strcmp (headerName, "budget") ==0 ||
    strcmp (headerName, "title_year") ==0 ||
    strcmp (headerName, "actor_2_facebook_likes") ==0 ||
    strcmp (headerName, "imdb_score") ==0 ||
    strcmp (headerName, "aspect_ratio") ==0 ||
    strcmp (headerName, "movie_facebook_likes") ==0)
{ 
   temp->intValue = atoi(charValueChar);
   temp->charValue = NULL;
    isIntHeaderValue = 1;
  
}
else if ((strcmp (headerName, "num_critic_for_reviews") !=0 && //check if string
    strcmp (headerName, "duration") !=0 &&
    strcmp (headerName, "director_facebook_likes") !=0 &&
    strcmp (headerName, "actor_3_facebook_likes") !=0 &&
    strcmp (headerName, "actor_1_facebook_likes") !=0 &&
    strcmp (headerName, "gross") !=0 &&
    strcmp (headerName, "num_voted_users") !=0 &&
    strcmp (headerName, "cast_total_facebook_likes") !=0 &&
    strcmp (headerName, "facenumber_in_poster") !=0 &&
    strcmp (headerName, "num_user_for_reviews") !=0 &&
    strcmp (headerName, "budget") !=0 &&
    strcmp (headerName, "title_year") !=0 &&
    strcmp (headerName, "actor_2_facebook_likes") !=0 &&
    strcmp (headerName, "imdb_score") !=0 &&
    strcmp (headerName, "aspect_ratio") !=0 &&
    strcmp (headerName, "movie_facebook_likes") !=0))
    {
      temp->charValue = charValueChar;
      temp->intValue = NULL;
       isCharHeaderValue = 1;
   
    }
//traverse the linked list to add the current struct data onto the end of the linked list
  while (prev->next!=NULL)
    {
      prev = prev->next;
    }
  current = temp;
  prev->next = current;
  prev = temp;
  rowCounter++;
 }//end of else
charNum = getline(&line,&lineSize,stdin);
}//end of while loop


int as=0;
//if the column data is of type integer
 if (isIntHeaderValue ==1 &&isCharHeaderValue ==0){
MergeSortInt(&head);
rowData * ptr = head ;
while(ptr!=NULL)//while(ptr!=NULL) 
{ 
printf("%s ", ptr->rowLine); 
ptr = ptr->next;
as++; 
} 
}

//if the column data is of type char
else if (isCharHeaderValue ==1 && isIntHeaderValue==0){
MergeSortChar(&head);
rowData * ptr = head ;
while(ptr!=NULL) 
{ 
printf("%s ", ptr->rowLine); 
ptr = ptr->next; 

as++; 
} 
}
return 0;
 
}








 
	






