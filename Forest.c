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



pthread_mutex_t myDataMutex;
pthread_mutex_t recievedDataMutex;
pthread_mutex_t mpiMutex;
pthread_mutex_t queueMutex;
int bunnyCount = 10;
int teddyCount = 5;
int meadowCount = 5;

int numOfThreads,tid;
MPI_Status status;


int * meadows;


int animal[5];
//0 - animal id;
//1 - animal size;
//2 - meadow id;
//3 - lampost
//4 - just in case

bool shouldIStartProgram()
{
	bool startProgram = true;

	
	return false;;
}
void initialize(int tid){



}

void broadcastRequests(){

	int i = 0;
	for(i = 0; i < numOfThreads; i++) {
		if(i == tid)
			continue;		

		MPI_Request request;
		pthread_mutex_lock(&mpiMutex);
		MPI_Isend(animal, 5, MPI_INT, i, TAG_JOIN,
              MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		pthread_mutex_unlock(&myDataMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
			int flag;
			MPI_Test(&request, &flag, &status);
			if(flag > 0){
				pthread_mutex_unlock(&mpiMutex);
				break;
			}
			pthread_mutex_unlock(&mpiMutex);
			usleep(100000+ rand() % 1000);
		}
		sleep(1);
	}

}

void sendConfirmation(int toWhom){

}

void iWannaParty(){
		srand(time(NULL));

	pthread_mutex_lock(&myDataMutex);
	animal[3] = animal[3] + 1;
	animal[2] = rand() % meadowCount;

	pthread_mutex_lock(&queueMutex);
	/////////////////////////////////////////////////TODO : IMPLEMENT FUNCTION!!!
	//addToQueue(animal);
	//////////////////////////////////////////////////////////////////////////////////////
	pthread_mutex_unlock(&queueMutex);
	broadcastRequests();

}


void *handleMsgRecieve() {

while(1) {	
		int *received = (int *) malloc(5 * sizeof(int));
		MPI_Request request;
		pthread_mutex_lock(&mpiMutex);
			MPI_Irecv(received , 5, MPI_INT, MPI_ANY_SOURCE, 
				MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
				int flag;
				MPI_Test(&request, &flag, &status);
				if(flag > 0) {
					pthread_mutex_unlock(&mpiMutex);
					break;
				}
			pthread_mutex_unlock(&mpiMutex);
			usleep(1000000);
		}	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		//addToQueue(received);
		
		animal[3] = animal[3] + 1;	

		printf("my tid = %d, recieved msg from %d with meadow %d tag %d and clock %d \n", animal[0], received[0], received[2], status.MPI_TAG, received[3]);

		sendConfirmation(received[0]);
	}
	pthread_exit(NULL);
}









int main(int argc, char **argv)
{



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
		printf("Argv conversion unsuccessful!!!!!");
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
			//broadcasting meadows ro all but tid = 0
			for (thNum = 1; thNum < numOfThreads; thNum++)
			{
				MPI_Send(&meadows, meadowCount, MPI_INT, thNum, 100, MPI_COMM_WORLD);		  
			}
			  
		  }
		else //if not tid 0
		{
			MPI_Recv(&meadows, meadowCount, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	  
		}
		

		//0 - animal id;
//1 - animal size;
//2 - meadow id;
//3 - lampost
//4 - just in case
		if (tid < bunnyCount)
		{
		   animal[1] = 1;
		}
		else
		{
		   animal[1] = 4;
		}	
		animal[0] = tid;
		animal[3] = 1;
		animal[4] = 0;

		//create new thread with 
		pthread_t id;
	  	pthread_create(&id, NULL, handleMsgRecieve, 
	  		NULL);


		while(1) {
			printf("tid:%d: I'm sleeping\n", tid );
			usleep(rand() % 5000000 + 2000000);
			printf("tid:%d: I wanna party\n", tid);
			iWannaParty();
		}

    	pthread_exit(&id);
	
    }


    MPI_Finalize();
}



