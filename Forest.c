#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <pthread.h>
#include <string.h>

#include "sort.c"

#define maxMeadowCapacity 20

#define TAG_JOIN 1
#define TAG_ENTER 2
#define TAG_LEAVE 3
#define TAG_RESPONSE 4

#define MSG_RESPONSE 0
#define MSG_REQUEST 1

#define MIN_ANIMAL_NUM 3



pthread_mutex_t myDataMutex;
pthread_mutex_t meadowsMutex;
pthread_mutex_t mpiMutex;
pthread_mutex_t queueMutex;
int bunnyCount = 10;
int teddyCount = 5;
int meadowCount = 5;

int numOfThreads,tid;
MPI_Status status;


int * meadows;
int * initMeadows;
int  **partyLine;


int animal[5];
//0 - animal id;
//1 - animal size;
//2 - meadow id;
//3 - lampost
//4 - just in case





void broadcastRequests(){

	MPI_Status rcvStatus;
	int i = 0;
	for(i = 0; i < numOfThreads; i++) {
		if(i == tid)
			continue;		

		MPI_Request request;
		pthread_mutex_lock(&mpiMutex);
		MPI_Isend(animal, 5, MPI_INT, i, TAG_JOIN,
              MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
			int flag;
			MPI_Test(&request, &flag, &rcvStatus);
			if(flag > 0){
				pthread_mutex_unlock(&mpiMutex);
				break;
			}
			pthread_mutex_unlock(&mpiMutex);
			usleep(100000);
		}
	}

}

void sendConfirmation(int toWhom){

	MPI_Status rcvStatus;
	
	MPI_Request request;

	pthread_mutex_lock(&myDataMutex);
	animal[3] = animal[3] + 1;
	animal[4] = 0;

	pthread_mutex_lock(&mpiMutex);
	int ret = MPI_Isend(animal, 5, MPI_INT, toWhom, TAG_RESPONSE, MPI_COMM_WORLD, &request);
	pthread_mutex_unlock(&mpiMutex);
	animal[4] = 1;

	pthread_mutex_unlock(&myDataMutex);
	while(1) {
		pthread_mutex_lock(&mpiMutex);				
		int flag;
		MPI_Test(&request, &flag, &rcvStatus);
		if(flag > 0 && ret == MPI_SUCCESS ) {
			pthread_mutex_unlock(&mpiMutex);
			break;
		}
		pthread_mutex_unlock(&mpiMutex);
		usleep(100000);
	}
}

broadcastMeadowInOut(int tag){

	int i = 0;
	for(i = 0; i < numOfThreads; i++) {
		if(i == tid)
			continue;		

		MPI_Request request;
		//pthread_mutex_lock(&myDataMutex);
		pthread_mutex_lock(&mpiMutex);
		MPI_Isend(animal, 5, MPI_INT, i, tag,
              MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		//pthread_mutex_unlock(&myDataMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
			int flag;
			MPI_Test(&request, &flag, &status);
			if(flag > 0){
				pthread_mutex_unlock(&mpiMutex);
				break;
			}
			pthread_mutex_unlock(&mpiMutex);
			usleep(100000);
		}
	}
}

/**
*method determines whether an animal can enter a meadow and party
*
* @return 1 if animal can party and 0 itherwise
*/
int tryParty(){

	if (tid == 2)
	{
		printf("tryPartyLabel 1\n");
	}
	//if all people haven't left the meadow
	pthread_mutex_lock(&meadowsMutex);
	if(meadows[animal[2]] != initMeadows[animal[2]]){
		pthread_mutex_unlock(&meadowsMutex);
		usleep(100000);
		if (tid == 2)
		{
			printf("tryPartyLabel 2\n");
		}
		return 0;
	}
	pthread_mutex_unlock(&meadowsMutex);

if (tid == 2)
{
	printf("tryPartyLabel 3\n");
}
	//int ** subArray;
	//int subArray[6][5]={{1,0,0,0,1}, {2,5,2,5,1}, {2,4,2,4,1}, {4,0,4,0,1}, {4,8,4,8,1}, {8,9,2,9,1}};
/*
	if(tid == 0){
		int nr = 2;
		nr = numberOfOneMeadow(animal[2], subArray, sizeArray);
		printf("nr of meadow 2 is %d\n", nr);
		int ** arrayOut = animalsOfOneMeadow(2, subArray, arrayOut, 6);
		int c;
		for (c=0; c < nr; c++){
			printf("%d %d %d %d %d\n", arrayOut[c][0], arrayOut[c][1], arrayOut[c][2], arrayOut[c][3], arrayOut[c][4]);
		}

		int aOutSize = (int) sizeof(&arrayOut);
		printf("size of array out is %d\n", aOutSize);

//int ** arrayOut = animalsOfOneMeadow(2, subArray, arrayOut, 6);
	}
*/

	while (1){
		pthread_mutex_lock(&queueMutex);
		if (sizeArray - 1 >= MIN_ANIMAL_NUM){
			pthread_mutex_unlock(&queueMutex);
			if (tid == 2)
			{
				printf("tryPartyLabel 4, sa = %d\n", sizeArray);
			}
		 	break;
		 }
		 pthread_mutex_unlock(&queueMutex);
		 usleep(100000);

	}

	usleep(3000000);
	pthread_mutex_lock(&queueMutex);
	int ** subArray = animalsOfOneMeadow(animal[2], partyLine, subArray, sizeArray -1 );
	int tmpPosition = oneMeadowSizeArray;
	pthread_mutex_unlock(&queueMutex);

	if (tid == 2)
	{
		printf("tryPartyLabel 5, oneMeadowSizeArray = %d\n", oneMeadowSizeArray);
		int zzz = 0;
		for (zzz = 0; zzz < oneMeadowSizeArray; zzz++){
			printf("zzz = %d, %d, %d, %d, %d \n", zzz, subArray[zzz][0], subArray[zzz][1], subArray[zzz][2], subArray[zzz][3]);
		}
	}


	//int subArrayS = (int) sizeof(subArray) / ( 5 * sizeof(int));
	//printf("sizeArray = %d\n", tmpPosition  - 1);

	int position = 0;
	int sumWeights = animal[1];

			if (tid == 2)
				printf("sum weights == %d\n", animal[1]);

	for (position = 0; position < tmpPosition - 1; position++){
			if (tid == 2)
				printf("it = %d, sum weights == %d\n", position, sumWeights);
		
		if (subArray[position][0] == tid){
			if (tid == 2){
				printf("tag 8, subArray[position][0] == %d, position == %d, sumWeights == %d \n", subArray[position][0], position, sumWeights);
			}
			break;
		}
		sumWeights += subArray[position][1];
	}
	pthread_mutex_lock(&meadowsMutex);

	// TODO: previous
	//if (sumWeights > meadows[animal[1]]){
	if (sumWeights > animal[1]){

		if (tid == 2)
		{
			printf("tryPartyLabel 6\n %d, %d\n", sumWeights, meadows[animal[1]]);
		}
		pthread_mutex_unlock(&meadowsMutex);
		return 0;
	}
	else{
		pthread_mutex_unlock(&meadowsMutex);
		if (tid == 0)
			printf("tryPartyLabel 7\n %d, %d\n", sumWeights, meadows[animal[1]]);
		return 1;
	}

	return -1;
}

void party() {

	printf("tid:%d: partying on meadow %d!\n", tid, animal[2]);

	int it;
			for (it = 0; it < sizeArray -1; it++)
			{
				if (partyLine[it][0] == animal[0]){
					sizeArray = delete(partyLine, sizeArray -1, it);
					sizeArray--;
					break;
				}

				}





	broadcastMeadowInOut(TAG_ENTER);
	usleep(rand() % 6000000+2000000);
	printf("tid:%d: leaving meadow %d!\n", tid, animal[2]);
	broadcastMeadowInOut(TAG_LEAVE);
}

void iWannaParty(){
		

	srand(time(NULL));
	pthread_mutex_lock(&myDataMutex);
	//increment lamport
	animal[3] = animal[3] + 1;
	animal[2] = rand() % meadowCount;
	pthread_mutex_lock(&queueMutex);
	partyLine=addToPartyLine(animal, partyLine);
	pthread_mutex_unlock(&myDataMutex);
	pthread_mutex_unlock(&queueMutex);
	broadcastRequests();
 	//printf("%d %d %d %d %d\n", partyLine[0][0], partyLine[0][1], partyLine[0][2], partyLine[0][3], partyLine[0][4]);
	while(tryParty() != 1){
	;
	}

	party();
}


void *handleMsgRecieve() {

	while(1) {	
		MPI_Status rcvStatus;
		int *received = (int *) malloc(5 * sizeof(int));
		MPI_Request request;
		pthread_mutex_lock(&mpiMutex);
			MPI_Irecv(received , 5, MPI_INT, MPI_ANY_SOURCE, 
				MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		pthread_mutex_unlock(&mpiMutex);
		while(1) {
			pthread_mutex_lock(&mpiMutex);
				int flag;
				MPI_Test(&request, &flag, &rcvStatus);
				if(flag > 0) {
					pthread_mutex_unlock(&mpiMutex);
					break;
				}
			pthread_mutex_unlock(&mpiMutex);
			usleep(1000000 + rand() % 1000);
		}	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		
		//increment lamport clock
		animal[3] = animal[3] + 1;	

		printf("my tid = %d, recieved msg from %d with meadow %d tag %d clock %d and [4] %d\n", animal[0], received[0], received[2], rcvStatus.MPI_TAG, received[3], received[4]);

		if (rcvStatus.MPI_TAG == TAG_JOIN){

			pthread_mutex_lock(&queueMutex);
			partyLine=addToPartyLine(received , partyLine);
			pthread_mutex_unlock(&queueMutex);

			sendConfirmation(received[0]);
		}
		else if (rcvStatus.MPI_TAG == TAG_ENTER){
			pthread_mutex_lock(&meadowsMutex);
			meadows[received[2]] -= received[1];

			int it;
			pthread_mutex_lock(&queueMutex);
			for (it = 0; it < sizeArray -1; it++)
			{
				if (partyLine[it][0] == received[0]){
					sizeArray = delete(partyLine, sizeArray -1, it);
					sizeArray--;
					pthread_mutex_unlock(&queueMutex);
					break;

				}
			}
			pthread_mutex_unlock(&queueMutex);
			
			pthread_mutex_unlock(&meadowsMutex);
		}
		else if (rcvStatus.MPI_TAG == TAG_LEAVE){
			pthread_mutex_lock(&meadowsMutex);
			meadows[received[2]] += received[1];
			pthread_mutex_unlock(&meadowsMutex);
		}
		else if (rcvStatus.MPI_TAG == TAG_RESPONSE){
			
		}
	
	}
	pthread_exit(NULL);
}



int main(int argc, char **argv)
{

    int startProgram = 1;
	
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
		    }
		    startProgram = 0;
		}
    }

    if ( (numOfThreads != bunnyCount + teddyCount) || (bunnyCount < 0) ||
	(teddyCount < 0) || (meadowCount <1) )
    {
		if (tid == 0)
		{
	    	printf("Argv conversion unsuccessful");
		}
	startProgram = 0;
    }

    
    //if no errors were made during initialization
    if (startProgram == 1)
    {
    	meadows = (int *) malloc(meadowCount * sizeof(int));
		//if first bunny
		if (tid == 0)
		{
			srand(time(NULL));
			int i;
			for (i = 0; i < meadowCount; ++i)
			{
			  //meadows[i] = rand() % maxMeadowCapacity + 13;
				meadows[i] = 5;
		
			}			

     		memcpy(&initMeadows, &meadows, meadowCount * sizeof(int));
/*
     		int c;
			for ( c = 0 ; c < meadowCount ; c++ )
     			printf("meadow %d size =  %d\n", c, meadows[c]);

     		for ( c = 0 ; c < meadowCount ; c++ )
     			printf("meadow %d size =  %d\n", c, initMeadows[c]);
*/

			int thNum;
			//broadcasting meadows to all but tid = 0
			for (thNum = 1; thNum < numOfThreads; thNum++)
			{

				int i=0;
				for (i=0; i< meadowCount; i++)
		  		{
		    
				    int toSend;
				    toSend = meadows[i];
				    MPI_Send(&toSend, 1, MPI_INT, thNum, 100, MPI_COMM_WORLD);
				}		  
			}
			  
		  }
		else //if not tid 0
		{

			int i=0;
			int toRecieve = 0;
		    for (i=0; i< meadowCount; i++)
		    {
				MPI_Recv(&toRecieve, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);	  
				meadows[i] = toRecieve;
		    }
     		memcpy(&initMeadows, &meadows, meadowCount * sizeof(int));
		}

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
		animal[4] = 1;

		//create new thread responsible gor handling mpi_recv
		pthread_t id;
	  	pthread_create(&id, NULL, handleMsgRecieve, 
	  		NULL);


		while(1) {
			printf("tid:%d: I'm sleeping\n", tid );
			//usleep(rand() % 5000000 + 2000000);
			printf("tid:%d: I wanna party\n", tid);
			iWannaParty();
		}

    	pthread_exit(&id);
	
    }


    MPI_Finalize();
}



