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
	int last = heap->last;//������ ����, ������ ���� last�� ���ؼ� ����, last�� ���ؼ� ���ٴ� ���� �� �� �ִ�!

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
		heap->last = -1;//�迭�� ���� -> cursor

	}
	return heap;
}

/* Free memory for heap
*/
void heapDestroy(HEAP* heap)
{//���� �ʱ�ȭ
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
	}//�ƿ� ���� ��� : ������ cursor �ε����� capacity-1 (������ �ε�����)

	heap->last += 1;
	(heap->heapArr)[heap->last] = data;
	if (heap->last > 0)//ù ��° ���Խÿ��� heapify���� �ʿ� ����
	{
		_reheapUp(heap, heap->last);
	}//root�� �־��� �� heapify
	return 1;	
}


/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP* heap, int index)
{
	int parent = (index - 1) / 2;
	int tmp;

	while ((heap->heapArr)[index] > (heap->heapArr)[parent])
	{//���԰��� �� Ŀ�� �������� ��� swap
		tmp = (heap->heapArr)[parent];
		(heap->heapArr)[parent] = (heap->heapArr)[index];
		(heap->heapArr)[index] = tmp;

		index = parent;
		if (index <= 0)//root�� �׸�
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
	}//�����Ͱ� �ƿ� ���� ��� last == -1�̰ų� �� ����


	// ������ ������Out ����, root�� �� ������ �� �����ͼ� heapify�ϱ� (heap���� ������ �׻� root������ �Ͼ�� ������)
	*data = (heap->heapArr)[0];
	(heap->heapArr)[0] = (heap->heapArr)[heap->last];

	//������ ���������� �ʴ´�. cursor�� �ű�� �迭������ ������ ��ó�� �����ȴ�.
	(heap->heapArr)[heap->last] = *data;//�� ���� ��ǻ� ���°�.(�ֹ���) �׷��� heapPrint�� last�� ���� ����ϱ� ������ last�� ������
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
	//heap�� start�� 0�ϰ���� �ڽ� �ε���
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	//���� �ٷ� �����ϴ� ���� iterator
	int iterator = (heap->heapArr)[index];


	if ((iterator < (heap->heapArr)[left]) || (iterator < (heap->heapArr)[right]))
	{//iterator�� �׻� ū ���·� heapify + ���� ��ο���! == �׷��� ���� ������ ��͹� (�򰥸���..)
		if ((heap->heapArr)[left] > (heap->heapArr)[right])
		{//�� ū ������ �������� -> �������� ��������
			(heap->heapArr)[index] = (heap->heapArr)[left];//���� �ڸ����� ū ���� �־��ְ�
			index = left;
			(heap->heapArr)[left] = iterator; //�� ��� update
		}
		else
		{//�� ū ������ �������� -> ������
			(heap->heapArr)[index] = (heap->heapArr)[right];
			index = right;
			(heap->heapArr)[right] = iterator;
		}

		if (2 * index + 1 <= heap->last)
			_reheapDown(heap, index);//������Ʈ�� ��� �ϰ�
	}

	//���ٸ��� ���
	else if (right > heap->last)
	{
		if (iterator < (heap->heapArr)[left])
		{
			(heap->heapArr)[index] = (heap->heapArr)[left];
			(heap->heapArr)[left] = iterator;
		}
	}
}