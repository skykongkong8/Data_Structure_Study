#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#define MAX_ELEM	20

typedef struct
{
	void **heapArr;
	int	last;
	int	capacity;
	int (*compare) (void *arg1, void *arg2);
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heapCreate( int capacity, int (*compare) (void *arg1, void *arg2));

/* Free memory for heap
*/
void heapDestroy( HEAP *heap);

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert( HEAP *heap, void *dataPtr);

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index);

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete( HEAP *heap, void **dataOutPtr);

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index);

/* user-defined compare function */
int compare(void *arg1, void *arg2)
{
	int *a1 = (int *)arg1;
	int *a2 = (int *)arg2;
	
	return *a1 - *a2;
}

/* Print heap array */
void heapPrint( HEAP *heap)
{
	int i;
	int last = heap->last;
	
	for( i = 0; i <= last; i++)
	{
		int *value = (int *)(heap->heapArr[i]);
		printf("%6d", *value);
	}
	printf( "\n");
}

int main(void)
{
	HEAP *heap;
	int data;
	int *dataPtr;
	int i;
	
	//heap = heapCreate(MAX_ELEM, compare);
	
	srand( time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		int *newdata = (int *)malloc( sizeof(int));
		*newdata = data;
		
		// insert function call
		//heapInsert( heap, newdata);
		
		//heapPrint( heap);
 	}

	//while (heap->last >= 0)
	{
		// delete function call
		//heapDelete( heap, (void **)&dataPtr);

		//printf( "Deleting %d: ", *(int *)dataPtr);

		//free(dataPtr);

		//heapPrint( heap);
 	}
	
	//heapDestroy( heap);
	
	return 0;
}

