#include <stdio.h>
#include <stdlib.h>
#include "scannerCSVsorter.h"
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

 void csvSorter(char * path, char * outdir, char * fileName, char * headerName)
{

FILE * csvOut;
int ab;
char fileName1[500];
char outDirA[500];
char * headerName1 = strdup(headerName);
strcpy(outDirA, outdir);
strncpy(fileName1, fileName,strlen(fileName)-4);
//strcpy(fileName1,"hello");
//fileName1 = "hello";
strcat(fileName1,"-sorted-");
strcat(fileName1,headerName1);
strcat(fileName1,".csv");
strcat(outDirA,"/");
strcat(outDirA, fileName1);
//printf("FILENAME1 = %s\n", fileName1);
//printf("OUTDIR %s\n", outDirA);




/*if (strcmp(path, "")!=0)
{
strncat(path, "/",2);
}
//printf("\npath: %s\n",path);
//printf("\nENTRYNAME: %s\n",entryname);
  strncat(path, entryname,50);*/
 // printf("\nPATH:: %s\n", path);
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
// char line[5000];
 //char *headerLine = &line;
 //size_t charNum; 
fgets(line,5000,file);
char* firstLine = strdup(line);

//free(file);
//return;
//}//end of void for now
 size_t len = strlen(firstLine);//replace new line character with null terminator
 firstLine[len-1] = '\0';
 firstLine[len-2] = '\0';
  
  
  int headerNumber = columnNum(firstLine, headerName);
  
  if (headerNumber==-1)
  {
  fprintf(stderr, "No column name found with entry\n");
  return;
  }
csvOut = fopen(outDirA, "w");
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


int as=0;
//if the column data is of type integer
 if (isIntHeaderValue ==1 &&isCharHeaderValue ==0 &&isFloatHeaderValue==0){
MergeSortInt(&head);
rowData * ptr = head ;
while(ptr!=NULL)//while(ptr!=NULL) 
{ 
fprintf(csvOut, "%s", ptr->rowLine); 
ptr = ptr->next;
as++; 
} 
}

//if the column data is of type char
else if (isCharHeaderValue ==1 && isIntHeaderValue==0&&isFloatHeaderValue==0){
MergeSortChar(&head);
rowData * ptr = head ;

while(ptr!=NULL) 
{ 
fprintf(csvOut, "%s", ptr->rowLine); 
ptr = ptr->next; 

as++; 
}
}

else if (isFloatHeaderValue==1 &&isCharHeaderValue ==0 && isIntHeaderValue==0){
MergeSortFloat(&head);
rowData * ptr = head ;
while(ptr!=NULL) 
{ 
fprintf(csvOut, "%s", ptr->rowLine); 
ptr = ptr->next; 

as++; 
}
}

//free(path);
//free(line);
//free(searchdir);
//free(outdir);
fclose(csvOut);
fclose(file);
return;
}