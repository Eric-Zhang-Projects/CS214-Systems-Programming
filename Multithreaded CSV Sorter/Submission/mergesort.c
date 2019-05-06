//Declaration of Struct Node
struct rowData{
  char *headerName;
  char *charValue;
  int intValue;
  float floatValue;
  char *rowLine;
  struct rowData * next;
  } ;


//MERGE SORT ON AN INT DATA TYPE -------------------------------------------
//prototypes
struct rowData* SortedMergeInt(struct rowData* a, struct rowData* b); 
void FrontBackSplitInt(struct rowData* originalD, 
        struct rowData** frontPtr, struct rowData** backPtr); 
  
//sort
void MergeSortInt(struct rowData** headRef) 
{ 
struct rowData* head = *headRef; 
struct rowData* a; 
struct rowData* b; 
  
//base case
if ((head == NULL) || (head->next == NULL)) 
{ 
    return; 
} 
  
//see method below
FrontBackSplitInt(head, &a, &b); 

//recursively sort
MergeSortInt(&a); 
MergeSortInt(&b); 
  
//recombine to sort

*headRef = SortedMergeInt(a, b); 

} 

struct rowData* SortedMergeInt(struct rowData* a, struct rowData* b) 
{ 
struct rowData* result = NULL;
  
//base case
if (a == NULL) 
    return(b); 
else if (b==NULL) 
    return(a); 
  
//comparisons
if (a->intValue <= b->intValue) 
{ 
    result = a; 
    result->next = SortedMergeInt(a->next, b); 
} 
else
{ 
    result = b; 
    result->next = SortedMergeInt(a, b->next); 
} 
return(result); 
} 
  
    //split linked list into first half and back half
void FrontBackSplitInt(struct rowData* originalD, 
        struct rowData** frontPtr, struct rowData** backPtr) 
{ 
        struct rowData* quickV; 
        struct rowData* pacedV; 
    pacedV = originalD; 
    quickV = originalD->next; 
  
    // Advance 'quickV' two nodes, and advance 'pacedV' one node 
    while (quickV != NULL) 
    { 
    quickV = quickV->next; 
    if (quickV != NULL) 
    { 
        pacedV = pacedV->next; 
        quickV = quickV->next; 
    } 
    } 
  
    //'pacedV' is before the midpoint in the list, so split it in two at that point. 
    *frontPtr = originalD; 
    *backPtr = pacedV->next; 
    pacedV->next = NULL; 
} 


  

//MERGE SORT ON A CHAR DATA TYPE -------------------------------------------




//prototypes
struct rowData* SortedMergeChar(struct rowData* a, struct rowData* b); 
void FrontBackSplitChar(struct rowData* originalD, 
        struct rowData** frontPtr, struct rowData** backPtr); 
  
//sort
void MergeSortChar(struct rowData** headRef) 
{ 
struct rowData* head = *headRef; 
struct rowData* a; 
struct rowData* b; 
  
//base case
if ((head == NULL) || (head->next == NULL)) 
{ 
    return; 
} 
  
//see method below
FrontBackSplitChar(head, &a, &b);  
  
//recursively sort
MergeSortChar(&a); 
MergeSortChar(&b); 
  
//recombine to sort
*headRef = SortedMergeChar(a, b); 
} 

struct rowData* SortedMergeChar(struct rowData* a, struct rowData* b) 
{ 
struct rowData* result = NULL; 
  
//base case
if (a == NULL) 
    return(b); 
else if (b==NULL) 
    return(a); 
  
//comparisons
if (strcmp(a->charValue, b->charValue)<0) 
{ 
    result = a; 
    result->next = SortedMergeChar(a->next, b); 
} 
else
{ 
    result = b; 
    result->next = SortedMergeChar(a, b->next); 
} 
return(result); 
} 

    //split linked list into first half and back half
void FrontBackSplitChar(struct rowData* originalD, 
        struct rowData** frontPtr, struct rowData** backPtr) 
{ 
        struct rowData* quickV; 
        struct rowData* pacedV; 
    pacedV = originalD; 
    quickV = originalD->next; 
  
    // Advance 'quickV' two nodes, and advance 'pacedV' one node 
    while (quickV != NULL) 
    { 
    quickV = quickV->next; 
    if (quickV != NULL) 
    { 
        pacedV = pacedV->next; 
        quickV = quickV->next; 
    } 
    } 
  
  //   'pacedV' is before the midpoint in the list, so split it in two at that point. 
    *frontPtr = originalD; 
    *backPtr = pacedV->next; 
    pacedV->next = NULL; 
} 

//DOUBLE -----------------------------------------------
struct rowData* SortedMergeFloat(struct rowData* a, struct rowData* b); 
void FrontBackSplitFloat(struct rowData* originalD, 
        struct rowData** frontPtr, struct rowData** backPtr); 
  
//sort
void MergeSortFloat(struct rowData** headRef) 
{ 
struct rowData* head = *headRef; 
struct rowData* a; 
struct rowData* b; 
  
//base case
if ((head == NULL) || (head->next == NULL)) 
{ 
    return; 
} 
  
//see method below
FrontBackSplitFloat(head, &a, &b);  
  
//recursively sort
MergeSortFloat(&a); 
MergeSortFloat(&b); 
  
//recombine to sort
*headRef = SortedMergeFloat(a, b); 
} 

struct rowData* SortedMergeFloat(struct rowData* a, struct rowData* b) 
{ 
struct rowData* result = NULL; 
  
//base case
if (a == NULL) 
    return(b); 
else if (b==NULL) 
    return(a); 
  
//comparisons
if (a->floatValue<= b->floatValue) 
{ 
    result = a; 
    result->next = SortedMergeFloat(a->next, b); 
} 
else
{ 
    result = b; 
    result->next = SortedMergeFloat(a, b->next); 
} 
return(result); 
} 

    //split linked list into first half and back half
void FrontBackSplitFloat(struct rowData* originalD, 
        struct rowData** frontPtr, struct rowData** backPtr) 
{ 
        struct rowData* quickV; 
        struct rowData* pacedV; 
    pacedV = originalD; 
    quickV = originalD->next; 
  
    // Advance 'quickV' two nodes, and advance 'pacedV' one node 
    while (quickV != NULL) 
    { 
    quickV = quickV->next; 
    if (quickV != NULL) 
    { 
        pacedV = pacedV->next; 
        quickV = quickV->next; 
    } 
    } 
  
  //   'pacedV' is before the midpoint in the list, so split it in two at that point. 
    *frontPtr = originalD; 
    *backPtr = pacedV->next; 
    pacedV->next = NULL; 
} 

 













