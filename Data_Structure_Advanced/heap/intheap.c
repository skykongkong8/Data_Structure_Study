#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time

#define MAX_ELEM	20

typedef struct
{
	int* heapArr;
	int	last;
	int	capacity;
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP* heapCreate(int capacity);

/* Free memory for heap
*/
void heapDestroy(HEAP* heap);

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert(HEAP* heap, int data);

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP* heap, int index);

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete(HEAP* heap, int* dataOut);

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown(HEAP* heap, int index);

/* Print heap array */
void heapPrint(HEAP* heap)
{
	int i;
	int last = heap->last;//삭제할 때도, 삽입할 때도 last를 통해서 들어가고, last를 통해서 들어간다는 것을 알 수 있다!

	for (i = 0; i <= last; i++)
	{
		printf("%6d", (heap->heapArr)[i]);
	}
	printf("\n");
}

int main(void)
{
	HEAP* heap;
	int data;
	int i;

	heap = heapCreate(MAX_ELEM);

	srand(time(NULL));

	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // 1 ~ MAX_ELEM*3 random number

		fprintf(stdout, "Inserting %d: ", data);

		// insert function call
		heapInsert(heap, data);

		heapPrint(heap);
	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete(heap, &data);

		printf("Deleting %d: ", data);

		heapPrint(heap);
	}

	heapDestroy(heap);

	return 0;
}


/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP* heapCreate(int capacity)
{
	HEAP* heap = (HEAP*)malloc(sizeof(HEAP));
	if (heap)
	{
		heap->capacity = capacity;
		heap->heapArr = (int*)malloc(capacity * sizeof(int));
		heap->last = -1;//배열로 구현 -> cursor

	}
	return heap;
}

/* Free memory for heap
*/
void heapDestroy(HEAP* heap)
{//완전 초기화
	heap->last = -1;
	heap->capacity = 0;
	free(heap->heapArr);
	free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert(HEAP* heap, int data)
{
	if (heap->last == heap->capacity - 1)
	{
		return 0;
	}//아예 꽉찬 경우 : 현재의 cursor 인덱스가 capacity-1 (마지막 인덱스임)

	heap->last += 1;
	(heap->heapArr)[heap->last] = data;
	if (heap->last > 0)//첫 번째 삽입시에는 heapify해줄 필요 없음
	{
		_reheapUp(heap, heap->last);
	}//root에 넣어준 후 heapify
	return 1;	
}


/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP* heap, int index)
{
	int parent = (index - 1) / 2;
	int tmp;

	while ((heap->heapArr)[index] > (heap->heapArr)[parent])
	{//삽입값이 더 커질 때까지는 계속 swap
		tmp = (heap->heapArr)[parent];
		(heap->heapArr)[parent] = (heap->heapArr)[index];
		(heap->heapArr)[index] = tmp;

		index = parent;
		if (index <= 0)//root면 그만
		{
			break;
		}
		parent = (index - 1) / 2;//parent update
	}
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete(HEAP* heap, int* data)
{
	if (heap->last < 0)
	{
		return 0;
	}//데이터가 아예 없을 경우 last == -1이거나 그 이하


	// 삭제할 데이터Out 지정, root에 맨 마지막 값 가져와서 heapify하기 (heap에서 삭제는 항상 root에서만 일어나기 때문에)
	*data = (heap->heapArr)[0];
	(heap->heapArr)[0] = (heap->heapArr)[heap->last];

	//실제로 삭제하지는 않는다. cursor를 옮기면 배열에서는 삭제한 것처럼 구현된다.
	(heap->heapArr)[heap->last] = *data;//이 값은 사실상 없는것.(휘발적) 그러나 heapPrint때 last를 기준 출력하기 때문에 last에 몰아줌
	heap->last -= 1;
	if (heap->last > 0)
	{
		_reheapDown(heap, 0);
	}

	return 1;
}

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown(HEAP* heap, int index)
{
	//heap의 start가 0일경우의 자식 인덱스
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	//값을 바로 지정하는 변수 iterator
	int iterator = (heap->heapArr)[index];


	if ((iterator < (heap->heapArr)[left]) || (iterator < (heap->heapArr)[right]))
	{//iterator가 항상 큰 상태로 heapify + 양쪽 모두에게! == 그렇지 않은 동안의 재귀문 (헷갈린다..)
		if ((heap->heapArr)[left] > (heap->heapArr)[right])
		{//더 큰 쪽으로 내려가기 -> 왼쪽으로 내려가기
			(heap->heapArr)[index] = (heap->heapArr)[left];//원래 자리에는 큰 값을 넣어주고
			index = left;
			(heap->heapArr)[left] = iterator; //비교 대상 update
		}
		else
		{//더 큰 쪽으로 내려가기 -> 오른쪽
			(heap->heapArr)[index] = (heap->heapArr)[right];
			index = right;
			(heap->heapArr)[right] = iterator;
		}

		if (2 * index + 1 <= heap->last)
			_reheapDown(heap, index);//업데이트된 재귀 하강
	}

	//꼬다리일 경우
	else if (right > heap->last)
	{
		if (iterator < (heap->heapArr)[left])
		{
			(heap->heapArr)[index] = (heap->heapArr)[left];
			(heap->heapArr)[left] = iterator;
		}
	}
}