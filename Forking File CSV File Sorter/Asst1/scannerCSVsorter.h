/*struct rowData* SortedMergeInt(struct rowData* a, struct rowData* b); 
void FrontBackSplitInt(struct rowData* source, struct rowData** frontRef, struct rowData** backRef); 
void MergeSortInt(struct rowData** headRef) ;

struct rowData* SortedMergeChar(struct rowData* a, struct rowData* b); 
void FrontBackSplitChar(struct rowData* source, struct rowData** frontRef, struct rowData** backRef); 
void MergeSortChar(struct rowData** headRef) ; */

//create struct 
typedef struct {
  char *headerName;
  char *charValue;
  int intValue;
  float floatValue;
  char *rowLine;
  struct rowData * next;
  } rowData;


//methods


//string tokenizer, removes commas from a read line
char *stringtokenizer(char *str, char *deliminator) 
{
	char *ptr;
	static char *pres = NULL;
  int preDelim;
	if(str != NULL)
	{
		pres = str;
	}
	if(pres == NULL || *pres == '\0')
	{
		return(NULL);
	}
	if (*pres != '\"')
	{	
		preDelim = strcspn(pres,deliminator);  //pre-delim characters
	ptr = pres; 					
	pres += preDelim;					
		if(*pres != '\0')
		{
			*pres++ = '\0';
		}
		pres--;
		if(*pres=='\n')
		{
			*pres='\0';
		}
		pres++;
	}
	else
	{
		pres++;//current is now one ahead of the "
		ptr=pres;
		while(pres!=NULL && *pres!='\"' && *pres!='\0')
		{
			pres++;
		}
		if(*pres=='\"')
		{
		*pres='\0';
		}
	pres+=2;//must have a "," or "\n" after quotation
	}
  //elinmate white space
  //leading space
  while(isspace(*ptr)) 
  {
    ptr++;
  }
  //ending space
  char* ends = ptr + strlen(ptr);
  while(isspace(*--ends));
  {
    *(ends+1) = '\0';
  } 
	return (ptr);
}


//if each header/column name has a number assigned to it, this code obtains the number pertaining to the headerName entered into the perameter
int columnNum(char* firstLine, char* headerName){
  
	char * token;
	int i=1;
  token = stringtokenizer(firstLine, ",");
	while (token != NULL )
	{
 
		if((strcmp(token, headerName) == 0)){
			return i;
		}
    token = stringtokenizer(NULL, ",");
		i++;
	}
 
   return -1;

}


//obtain the data in row for the specified column number obtained from headerNum method
  const char* getfield(char* line, int colNum)
{
	//THIS CODE WORKS
	char *token;
  token = stringtokenizer(line, ",");
	int i=1;
	while (token !=NULL && i<=colNum)
	{
		if(i==colNum){
			return token;
		}
		 i++;
   token = stringtokenizer(NULL, ",");
	
	}
}

int checkvalid(const char* path)
{
FILE * file;
file = fopen(path, "r");
 int valid;
 //check if valid csv
 char *line;
  line = (char*)malloc(sizeof(char)*5000);
 char*tokenline;
 char *token;
fgets(line,5000,file);
//printf("%s\n",line);
tokenline=strdup(line);
token = stringtokenizer(tokenline, ",");
int tokencounter=0;
while(token!=NULL)
{
tokencounter++;
token = stringtokenizer(NULL, ",");
}
while(fgets(line,5000,file)!=NULL)
{
int tokens=0;
tokenline=strdup(line);
//printf("%s\n", tokenline);
token = stringtokenizer(tokenline, ",");
while(token!=NULL)
{
tokens++;
token = stringtokenizer(NULL, ",");
}
//printf("tokens = %d\n", tokens);
//printf("%s\n",tokenline);
if (tokens!=tokencounter)
{
//printf("%s\n",tokenline);
//printf("error wrong format\n");
fclose(file);
return 0;
}
}
//printf("%d\n",tokencounter);
fclose(file);
return 1;//valid
}