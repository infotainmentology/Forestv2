#include <stdio.h>
#include <string.h>
int sizeArray=1;
 void sort(int array[][5], int n)
 {
   int c, d, swap[5];
  for (c = 0 ; c < ( n - 1 ); c++)
  {
    for (d = 0 ; d < n - c - 1; d++)
    {
      if (array[d][2] > array[d+1][2]) 
      {
  memcpy(swap, array[d], sizeof(array[d]));
  memcpy(array[d], array[d+1], sizeof(array[d+1]));
  memcpy(array[d+1], swap, sizeof(swap));
      }
      if (array[d][2] == array[d+1][2]) 
      {
    if (array[d][3] > array[d+1][3]) 
    {
      memcpy(swap, array[d], sizeof(array[d]));
      memcpy(array[d], array[d+1], sizeof(array[d+1]));
      memcpy(array[d+1], swap, sizeof(swap));
    }
      }
    }
  }
 
 }
 
 int delete(int array[][5], int n, int del)
 {
   int i;
   
   for(i = del; i < n-1; i++)
   {
      memcpy(array[i], array[i+1], sizeof(array[i+1]));
   }
   n--;
   return n;
 }
 int numberOfOneMeadow(int meadow, int arrayIn[][5], int n){
   int i, j = 0;
   
   for(i = 0; i < n; i++)
   {
     if(meadow == arrayIn[i][2])
     {
  j++;
     }
   }
   return j;
 }
 int** animalsOfOneMeadow(int meadow, int arrayIn[][5], int **arrayOut, int n)
 {
    int i, j, licz=0, c, d, swap[5];
    j=numberOfOneMeadow(meadow, arrayIn, n);
   
   arrayOut = malloc(j * sizeof(int *));
  
  for(i = 0; i < j; i++)
    {
    arrayOut[i] = malloc(5 * sizeof(int));
    
    }
   
   for(i = 0; i < n; i++)
   {
     if(meadow == arrayIn[i][2])
     {
  memcpy(arrayOut[licz], arrayIn[i], sizeof(arrayIn[i]));
  licz++;
     }
   }
   
    
    
  for (c = 0 ; c < ( j - 1 ); c++)
  {
    for (d = 0 ; d < j - c - 1; d++)
    {
      if (arrayOut[d][3] > arrayOut[d+1][3]) 
      {
  memcpy(swap, arrayOut[d], sizeof(arrayOut[d]));
  memcpy(arrayOut[d], arrayOut[d+1], sizeof(arrayOut[d+1]));
  memcpy(arrayOut[d+1], swap, sizeof(swap));
      }
      if (arrayOut[d][3] == arrayOut[d+1][3]) 
      {
    if (arrayOut[d][0] > arrayOut[d+1][0]) 
    {
      memcpy(swap, arrayOut[d], sizeof(arrayOut[d]));
      memcpy(arrayOut[d], arrayOut[d+1], sizeof(arrayOut[d+1]));
      memcpy(arrayOut[d+1], swap, sizeof(swap));
    }
      }
    }
  }
  
  return arrayOut;
 }


 int** addToPartyLine(int addingArray[5], int **array){
   int i, c, k;
   int *next;
  if(array == NULL)
  {
      array = malloc(sizeArray*sizeof(int *));
      
      for(i = 0; i < sizeArray; i++)
    {
    array[i] = malloc(5 * sizeof(int));
    
      
    }
    for(k = 0; k < 5; k++){
      array[0][k]=addingArray[k];
     // printf(addingArray[k]);
    }
    sizeArray++;    
  } else{ 
    printf("pppppp");
    array = (int**) realloc (array, (sizeArray) * sizeof(int *));
    
      array[sizeArray-1] = (int *)realloc(array[sizeArray-1], sizeof(int)*5);   
    for(k = 0; k < 5; k++){
      array[sizeArray-1][k]=addingArray[k];
      //printf(addingArray[k]);
    }
    sizeArray++;
      
  }
  
   return array;
 }
 
 
 /*
int main()
{
  int c, n=6, n1=1;
  int **arrayOut, **arr;
 int array[6][5]={{11,0,11,0,1}, {2,5,2,5,1}, {2,4,2,4,1}, {4,0,4,0,1}, {4,8,4,8,1}, {8,9,8,9,1}};
 
 sort(array, 6);        //array, numOfRows
 
 
  for ( c = 0 ; c < 6 ; c++ )
     printf("%d %d %d %d %d\n", array[c][0], array[c][1], array[c][2], array[c][3], array[c][4]);
 printf("%d\n", sizeof(array));
 n=delete(array, n, 2);
 for ( c = 0 ; c < n ; c++ )
     printf("%d %d %d %d %d\n", array[c][0], array[c][1], array[c][2], array[c][3], array[c][4]);
 
 arrayOut=animalsOfOneMeadow(2, array,arrayOut, 6);
 
 
 for ( c = 0 ; c < 2 ; c++ )
     printf("%d %d %d %d %d\n", arrayOut[c][0], arrayOut[c][1], arrayOut[c][2], arrayOut[c][3], arrayOut[c][4]);
 arr=addToPartyLine(array[0], arr);
 for ( c = 0 ; c < 1 ; c++ )
     printf("%d %d %d %d %d\n", arr[c][0], arr[c][1], arr[c][2], arr[c][3], arr[c][4]);
 arr=addToPartyLine(array[0], arr);
  arr=addToPartyLine(array[0], arr);
  for ( c = 0 ; c < 3 ; c++ )
     printf("%d %d %d %d %d\n", arr[c][0], arr[c][1], arr[c][2], arr[c][3], arr[c][4]);
  return 0;
}

*/