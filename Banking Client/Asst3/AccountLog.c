#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct AccountLog{
  //int count;
  int Session_Flag;
  double Account_Balance;
  char * Account_Name;
}AccountLog;

AccountLog ** Create_Logger();
//AccountLog ** ReallocLog( AccountLog *log[], int count);
AccountLog * Create_Acc(char * name);
//void ** Print_Log(AccountLog * log[], int count);
void * Query(AccountLog * acc);
int If_Acc_Exist(AccountLog * log[], int count, char * name);
int Find_Account(AccountLog * log[], int count, char * name);
int Deposit(AccountLog * log[], char * name,int count, double amount);
int Withdraw(AccountLog * log[],char * name,int count, double amount);

//int BankSize = 10;
//====================================================================================================
AccountLog ** Create_Logger()
{
  AccountLog ** AccLog = (AccountLog **)malloc(50 * sizeof(AccountLog *));
  AccountLog ** temp = AccLog;
  int i;
  
  for(i = 0; i < 50; i ++)
  {
    *temp = NULL;
  }
  return AccLog;
}
//====================================================================================================
AccountLog * Create_Acc(char * name)
{
  AccountLog * newAcc = (AccountLog *)malloc(sizeof(AccountLog));
  newAcc->Account_Name = (char *)malloc(256);
  
  strcpy(newAcc->Account_Name,name);
  newAcc->Account_Balance = 0.00;
  newAcc->Session_Flag = 1;
  return newAcc;
}
//====================================================================================================
AccountLog * Add_To_Log(AccountLog ** log, int count, char *name)
{
      
    if(count == 50){
      return NULL;
    } 
     
    AccountLog * acc = Create_Acc(name);
    AccountLog ** newLog = log;
    int i =0;
    for(i = 0; i < 50; i++){
      if(log[i]== NULL)
      {
        log[i] = acc;
        return acc;
      }
      else{
        newLog ++;
      }  
    }
     
}
//====================================================================================================
int If_Acc_Exist(AccountLog * log[], int count, char * name)
{
  int i;
  if(count == 0)
    return 0;
  for(i = 0; i < 50; i++)
  {
   // printf("%d\t%d\n",i,count);
    if(log[i] != NULL && (strcmp(log[i]->Account_Name,name) == 0))
    {
      printf("index[%d] account:%s\t%s\n",i,log[i]->Account_Name,name);
      return 1;
    } 
  }
  return 0;
}//====================================================================================================
int Find_Account(AccountLog * log[], int count, char * name)
{
  int i;
  if(count == 0)
    return -1;
  for(i = 0; i < 50; i++)
  {
   // printf("%d\t%d\n",i,count);
    if(log[i] != NULL && (strcmp(log[i]->Account_Name,name) == 0))
    {
      //printf("index[%d] account:%s\t%s\n",i,log[i]->Account_Name,name);
      return i;
    } 
  }
  return -1;
}
//====================================================================================================

void * Query(AccountLog * acc)
{
  if(acc->Session_Flag == 1)
     printf("%s\t $%.2f\tOFFLINE\n",acc->Account_Name,acc->Account_Balance);
  else
    printf("%s\t $%.2f\tIN SESSION\n",acc->Account_Name,acc->Account_Balance);
}
//====================================================================================================
int Deposit(AccountLog *log[],char * name,int count,double amount)
{
  if(!name)
    return -1;
  int index = Find_Account(log,count,name);
  
  log[index]->Account_Balance += amount;
  return 0;
}
//====================================================================================================
int Withdraw(AccountLog *log[],char * name, int count,double amount)
{
  if(!name)
    return -1;
  int index = Find_Account(log,count,name);
  
  if(log[index]->Account_Balance < amount)
    return -1;
  
  log[index]->Account_Balance -= amount;
  return 0;
}


