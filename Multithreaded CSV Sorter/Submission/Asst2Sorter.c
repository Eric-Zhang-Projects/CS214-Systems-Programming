#include <stdio.h>
#include <stdlib.h>
#include "threadSorter.h"
#include "mergesort.c"
#include <string.h>

/*int columnNum(char* firstLine, char* headerName);
const char* getfield(char* line, int colNum);
char *stringtokenizer(char *c, char *d);
struct rowData* SortedMergeInt(struct rowData* a, struct rowData* b); 
void FrontBackSplitInt(struct rowData* source, struct rowData** frontRef, struct rowData** backRef); 
void MergeSortInt(struct rowData** headRef) ;

struct rowData* SortedMergeChar(struct rowData* a, struct rowData* b); 
void FrontBackSplitChar(struct rowData* source, struct rowData** frontRef, struct rowData** backRef); 
void MergeSortChar(struct rowData** headRef) ; */

rowData* csvSorter(char * path, char * outdir, char * fileName, char * headerName)
{
//printf("%s, %s, %s, %s\n", path, outdir, fileName, headerName);
FILE * csvOut;
int ab;
char outdirA[500];

strcpy(outdirA, outdir);
strcat(outdirA,"/AllFiles-sorted-<");
strcat(outdirA, headerName);
strcat(outdirA, ">.csv");
printf("OUTDIR: %s\n", outdirA);

 char *line; //Line stored in this variable-----------------
 FILE * file = fopen(path, "r");
  file = fopen(path, "r");
  if (file ==NULL)
  {
  fprintf(stderr, "File not found\n");
  fclose(file);
  return;
  }
  size_t lineSize = 5000;
  line = (char *)malloc(lineSize * sizeof(char)); 
 
fgets(line,5000,file);
char* firstLine = strdup(line);


 size_t len = strlen(firstLine);//replace new line character with null terminator
 firstLine[len-1] = '\0';
 firstLine[len-2] = '\0';
  
  
  int headerNumber = columnNum(firstLine, headerName);
  
  if (headerNumber==-1)
  {
  fprintf(stderr, "No column name found with entry\n");
  return;
  }
csvOut = fopen(outdirA, "w");
fprintf(csvOut, "%s", line);
 rowData * temp;
 rowData * current;
 rowData * head;
 rowData * prev;
int rowCounter = 0; 
int isIntHeaderValue = 0;
int isCharHeaderValue =0;
int isFloatHeaderValue =0;
//charNum = fgets(line,5000,file);
//fgets(line,5000,file);
//while (line!=NULL)//charnum!=NULL

while(fgets(line,5000,file)!=NULL)
{
  if(rowCounter == 0)
  {
   char* allRow = strdup(line); 

 
   char * charValueChar = getfield(allRow, headerNumber);
  
   head = malloc(sizeof(rowData));
   head->headerName = headerName;
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
if (strcmp (headerName, "imdb_score") ==0 ||
    strcmp (headerName, "aspect_ratio") ==0)
    {
       head->floatValue = atof(charValueChar);
   head->charValue = NULL;
   head->intValue=0;
   isFloatHeaderValue = 1;
}
else{
   head->intValue = atoi(charValueChar);
   head->charValue = NULL;
   head->floatValue=0;
   isIntHeaderValue = 1;
 } 
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
      head->intValue = 0;
      head->floatValue=0;
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
 
  temp->headerName = headerName;
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
if (strcmp (headerName, "imdb_score") ==0 ||
    strcmp (headerName, "aspect_ratio") ==0)
    {
    temp->floatValue = atof(charValueChar);
   temp->charValue = NULL;
   temp->intValue=0;
    isFloatHeaderValue = 1;
    
    }
    else
    {
   temp->intValue = atoi(charValueChar);
   temp->charValue = NULL;
   temp->floatValue=0;
    isIntHeaderValue = 1;
  }
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
      temp->intValue = 0;
      temp->floatValue=0;
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
//charNum = fgets(line,lineSize,file);

}//end of while loop
//free(path);
//free(line);
//free(searchdir);
//free(outdir);

fclose(csvOut);
fclose(file);
return head;
}


void printing(rowData head, char * outdir, char * headerName)
{
char outdirA[500];
FILE * csvOut;
strcpy(outdirA, outdir);
strcat(outdirA,"/AllFiles-sorted-<");
strcat(outdirA, headerName);
strcat(outdirA, ">.csv");
//printf("OUTDIR: %s\n", outdirA);
csvOut = fopen(outdirA, "w");
rowData * ptr = &head;
while(ptr!=NULL)//while(ptr!=NULL)
{ 
fprintf(csvOut, "%s", ptr->rowLine); 
ptr = ptr->next;
}
fclose(csvOut); 
}