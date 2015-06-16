#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <pthread.h>
#include <stdbool.h>

#define maxMeadowCapacity 20

#define TAG_JOIN 1
#define TAG_ENTER 2
#define TAG_LEAVE 3
#define PARTY_NO 0
#define PARTY_YES 1



pthread_mutex_t dataMutex;
pthread_mutex_t mpiMutex;
int bunnyCount = 10;
int teddyCount = 5;
int meadowCount = 5;


struct animal{
   int id;
   int meadowId;
   int size;
   int lamport;
   int partying;
};

int * meadows;

struct animal me;

bool shouldIStartProgram()
{
	bool startProgram = true;

	
	return false;;
}
void initialize(int tid){



}

int main(int argc, char **argv)
{


    int numOfThreads,tid;
     bool startProgram = true;
	
    MPI_Status status;
    MPI_Init(&argc, &argv); 

    MPI_Comm_size( MPI_COMM_WORLD, &numOfThreads );
    MPI_Comm_rank( MPI_COMM_WORLD, &tid );
    

	if (argc >=4)
    { 
      	bunnyCount = atoi(argv[1]);
		teddyCount = atoi(argv[2]);
		meadowCount = atoi(argv[3]);
	
	if ( ((strcmp(argv[1], "0") != 0) && bunnyCount == 0) ||
	    ((strcmp(argv[2], "0") != 0) && teddyCount == 0) ||
	    ((strcmp(argv[3], "0") != 0) && meadowCount == 0) )
	{
	    if (tid == 0)
	    {
		printf("Argv conversion unsuccessful");
		//MPI_Abort(MPI_COMM_WORLD, -1);
	    }
	    startProgram = false;
	}
    }

    if ( (numOfThreads != bunnyCount + teddyCount) || (bunnyCount < 0) ||
	(teddyCount < 0) || (meadowCount <1) )
    {
		if (tid == 0)
		{
	    	printf("Argv conversion unsuccessful");
		}
	startProgram = false;
    }
    // startProgram = shouldIStartProgram();
    
    //if no errors were made during initialization
    if (startProgram)
    {
    	meadows = (int *) malloc(meadowCount * sizeof(int));
	//if first bunny
	if (tid == 0)
	{
		srand(time(NULL));
		int i;
		for (i = 0; i < meadowCount; ++i)
		{
		  meadows[i] = rand() % maxMeadowCapacity;
	
		}
		int thNum;
		//broadcasting meadows
		for (thNum = 1; thNum < numOfThreads; thNum++)
		{
			MPI_Send(&meadows, meadowCount, MPI_INT, thNum, 100, MPI_COMM_WORLD);		  
		}
		  
	  }
	else //if not tid 0
	{
		MPI_Recv(&meadows, meadowCount, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	  
	}
	
	if (tid < bunnyCount)
	{
	   ; 
	}
	else
	{
	   ; 
	}	

	
    }

    MPI_Finalize();
}



