#include <stdio.h>

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
 /*
int main()
{
  int c;
 int array[6][5]={{11,0,11,0,1}, {2,5,2,5,1}, {2,4,2,4,1}, {4,0,4,0,1}, {4,8,4,8,1}, {8,9,8,9,1}};
 
 sort(array, 6);        //array, numOfRows
 
 
  for ( c = 0 ; c < 6 ; c++ )
     printf("%d %d %d %d %d\n", array[c][0], array[c][1], array[c][2], array[c][3], array[c][4]);
 
  return 0;
}*/