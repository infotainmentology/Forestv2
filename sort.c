#include <stdio.h>
#include <string.h>
int sizeArray=1;
int oneMeadowSizeArray = 0;

 void sort(int ** array, int n, int first, int second)             //first=2, second =3
 {
   int c, d, swap[5], iter;
  for (c = 1 ; c < n; c++){
    for (d = 0 ; d < n - 1; d++)
    { 
      if (array[d][first] > array[d+1][first]) 
      {
	for (iter = 0 ; iter < 5; iter++){
	 int t = array[d][iter];
	 array[d][iter] = array[d+1][iter];
	 array[d+1][iter] = t;
	}
      }
      if (array[d][first] == array[d+1][first]) 
      {
	if (array[d][second] > array[d+1][second]) 
	{
	  for (iter = 0 ; iter < 5; iter++){
	 int t = array[d][iter];
	 array[d][iter] = array[d+1][iter];
	 array[d+1][iter] = t;
	}
	}
      }
    }
  }
 }
int numberOfOneMeadow(int meadow, int ** arrayIn, int n){
   int i, j = 0;
   
   for(i = 0; i < n; i++)
   {
     if(meadow == arrayIn[i][2])
     {
	j++;
     }
   }
   oneMeadowSizeArray = j;
   return j;
 }
int ** animalsOfOneMeadow(int meadow, int ** arrayIn, int n) {  
   int **arrayOut;
    int i, j, kk, licz=0;
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
       int iter;
	for (iter = 0 ; iter < 5; iter++){
	 arrayOut[licz][iter] = arrayIn[i][iter];
	}
	licz++;
     }
   }
    for(i=0; i<j; i++){
   for(kk=0; kk<5; kk++){
     //printf("%d ", arrayOut[i][kk]);
   }
   //printf("\n");
  }
  
  sort(arrayOut, j, 3,0);
  return arrayOut;
 }
 int delete(int ** array, int n, int m, int del)
 {
   int i, k, p;
   
   for(i = del; i < n-1; i++)        // m - liczba kolumn
   {
     for(k = 0; k < m; k++)
   {
	  array[i][k] = array[i+1][k];
   }
   }
   n--;
   return n;
 }

 int** addToPartyLine(int addingArray[5], int **array){
   int i, c, k, j;
   int *next;
   int ncolumns = 5;
  if(sizeArray == 1)
  {    
    array = malloc(sizeArray * sizeof(int *));
	for(i = 0; i < sizeArray; i++)
		{
		array[i] = malloc(ncolumns * sizeof(int));
		}
	for(i = 0; i < sizeArray; i++){
	  for(j = 0; j < ncolumns; j++){
	    array[i][j] = addingArray[j];
	    //printf("%d ", array[i][j]);
	    
	  }
	  //printf("\n");
	}
    sizeArray++;    
  } else{ 
    
array = (int**)realloc(array, (sizeArray)*sizeof(int*));

for(i = (sizeArray-1); i < (sizeArray); i++)
array[i] = NULL;


for (i = 0; i < (sizeArray); i++)
array[i] = (int*)realloc(array[i], (ncolumns)*sizeof(int));

for(i = (sizeArray-1); i < sizeArray; i++){
	  for(j = 0; j < ncolumns; j++){
	    array[i][j] = addingArray[j];
	    //printf("%d ", array[i][j]);
	    
	  }
	  //printf("\n");
	}
    
      
    sizeArray++;
      
  }
  
   return array;
 }
 