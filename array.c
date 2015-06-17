#include <stdio.h>
#include <string.h>
int main()
{
// Define matrix as an array of pointers
int** xyz;
int n = 3, m;
int i, j;

// Allocate the columns
xyz = (int**)calloc(n, sizeof(int*));

// Allocate the rows
for (i = 0; i < n; i++)
xyz[i] = (int*)calloc(n, sizeof(int));

// Initialize the element(s)
for(i = 0; i < n; i++)
for(j = 0; j < n; j++)
xyz[i][j] = (i+1);

// Display whole matrix
for(i = 0; i < n; i++)
{
putchar('\n');
for(j = 0; j < n; j++)
printf("%i\t", xyz[i][j]);
}

// Increase number of row(s) and column(s)
m = 2;

// Reallocate columns
xyz = (int**)realloc(xyz, (n + m)*sizeof(int*));

// The new column's pointer must be initialised to NULL
for(i = n; i < (n+m); i++)
xyz[i] = NULL;

// Reallocate rows
for (i = 0; i < (n+m); i++)
xyz[i] = (int*)realloc(xyz[i], (n + m)*sizeof(int));

// Display whole matrix
for(i = 0; i < (n+m); i++)
{
putchar('\n');
for(j = 0; j < (n+m); j++)
printf("%d\t", xyz[i][j]);
}

// Deallocate the rows
for (i = 0 ; i < (n+m) ; ++i)
free(xyz[i]);

// Deallocate the columns
free(xyz);

return(0);
}