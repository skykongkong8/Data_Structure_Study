#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define SEARCH			4
#define DELETE			5
#define COUNT			6

// User structure type definition
typedef struct
{
	char* name;
	int		freq;
} tName;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	void* dataPtr;	//tName		*dataPtr;
	struct node* llink;
	struct node* rlink;
} NODE;

typedef struct
{
	int		count;
	NODE* pos; // unused
	NODE* head;
	NODE* rear;
	int		(*compare)(const void*, const void*); // used in _search function
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST* createList(int (*compare)(const void*, const void*));

/* Deletes all data in list and recycles memory
*/
void destroyList(LIST* pList, void (*callback)(void *));

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode(LIST* pList, void* dataInPtr, void (*callback)(const void*, const void*)); //tName

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode(LIST* pList, void* keyPtr, void** dataOut); //tName

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList(LIST* pList, void* pArgu, void** dataOutPtr); //tName 데이터노드를 변화시키지 않는것

/* returns number of nodes in list
*/
int countList(LIST* pList);

/* returns	1 empty
			0 list has data
*/
int emptyList(LIST* pList);//실제로 사용하지는 않으므로 정의할 필요없음!

/* traverses data from list (forward)
*/
void traverseList(LIST* pList, void (*callback)(const void*));//printList의 업그레이드 버전 +callback함수 : 이름 자료구조를 직접 받아서 갖고오는것 //print_naem을 사용하기만 하면 된다.

/* traverses data from list (backward)
*/
void traverseListR(LIST* pList, void (*callback)(const void*));

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert(LIST* pList, NODE* pPre, void* dataInPtr); //tName _붙은 것은 _스태틱 내부함수. 위의 함수가 사용하는 함수.

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST* pList, NODE* pPre, NODE* pLoc, void** dataOutPtr); //tName 이중포인터로 선언한이유! - 변화를 반영하기 위해!!

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search(LIST* pList, NODE** pPre, NODE** pLoc, void* pArgu);

////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName* createName(char* str, int freq); //이름 문자열을 받는데 strdup(name)이라는 함수를 사용하면 메모리를 새롭게 동적할당 받아서 변수로 넘겨줌 다만 이를 또 free해줘야함

/* Deletes all data in name structure and recycles memory
*/
void destroyName(void* pNode);


////////////////////////////////////////////////////다양한 콜백함수를 미리 선언해놓았음. 상황에 따라 적절한 콜백함수를 사용할것
// compares two names in name structures
// for createList function
int cmpName(const void* pName1, const void* pName2)
{
	return strcmp(((tName*)pName1)->name, ((tName*)pName2)->name);//아까 그 보이드포인터를 어떻게 사용하는지 HINT : 꼭 괄호를 써서 캐스팅이 먼저 되도록 해야함!
}

// prints name and freq in name structure
// for traverseList and traverseListR functions
void print_name(const void* dataPtr)
{
	printf("%s\t%d\n", ((tName*)dataPtr)->name, ((tName*)dataPtr)->freq);
}

// increases freq in name structure
// for addNode function
void increse_freq(const void* dataOutPtr, const void* dataInPtr) //addnode에서 중복이 발생했을 때 빈도수를 합하기!
{
	((tName*)dataOutPtr)->freq += ((tName*)dataInPtr)->freq;
}

////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// 함수 전치선언

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST* createList(int (*compare)(const void*, const void*)) {
	LIST* pList = (LIST*)malloc(sizeof(LIST));
	pList->count = 0;
	pList->head = NULL;
	pList->rear = NULL;
	pList->pos = NULL; //필요X
	pList->compare = compare;

	return pList;
}

/* Deletes all data in list and recycles memory
*/
void destroyList(LIST* pList, void (*callback)(void *)) {
	//LIST안의 요소들을 차례로 해제
	for (int i = 0; i < pList->count; i++) {
		NODE* pLoc = pList->head;
		pList->head = pLoc->rlink;

		callback(((tName*)(pLoc->dataPtr)));
		//free(((tName*)pLoc->dataPtr)->name);//dataPtr이 void임을 잊지말자..
		//free((tName*)(pLoc->dataPtr));
		free(pLoc);
	}
	//마지막으로
	free(pList);
}

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
//
int addNode(LIST* pList, void* dataInPtr, void (*callback)(const void*, const void*)) {
	//callback이 여기서는 Increasefrequency
	// 빈 리스트 : 처음 넣는 경우
	NODE* pPre = NULL;
	NODE* pLoc = NULL;
	if (pList->head == NULL) {
		NODE* new_node = (NODE*)malloc(sizeof(NODE));
		if (new_node == NULL) {
			return 0;//동적 메모리할당 실패 -> memory overflow
		}
		new_node->dataPtr = (tName*)dataInPtr;
		new_node->llink = NULL;
		new_node->rlink = NULL;
		pList->rear = new_node;
		pList->head = new_node;
		//처음 넣는 경우는 중복될 경우가 없음!
		pList->count += 1;
		/*int first = _insert(pList, pPre, dataInPtr);
		if (first == 0) {
			return 0;
		}*/
		return 1;
	}
	/*if (pList->count == 1) {

		return 1;
	}*/

	//중복이 있을 수 있으므로 탐색을 실시한다.
	int found = _search(pList, &pPre, &pLoc, (((tName*)dataInPtr)));
	if (found == 0) {
		int IN = _insert(pList, pPre, dataInPtr);
		if (IN == 0) {//삽입에 실패 - overflow였기 때문
			return 0;
		}
		else return 1;
	}
	else {//찾음 : 1.빈도수 덧셈, 2."""필요없는 동적메모리 해제""" ->이건 main함수에서 해줌
		//dataInPtr : 데이터시트에서 받아온 데이터로 만든 tName
		//pLoc : _search 함수로 찾은 dataInPtr과 똑같은 name인스턴스를 가진 이미 있는, 더블리링크드리스트 상의 노드의 포인터
		callback((tName*)(pLoc->dataPtr), (tName*)dataInPtr);
		return 2;
	}
}//tName

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode(LIST* pList, void* keyPtr, void** dataOut) {
	NODE* pPre = NULL;
	NODE* pLoc = NULL;

	int found = _search(pList, &pPre, &pLoc, ((tName*)keyPtr));//keyPtr : *name
	if (found == 0) {
		return 0;
	}
	else {
		_delete(pList, pPre, pLoc, dataOut);
		return 1;
	}
} //tName

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList(LIST* pList, void* pArgu, void** dataOutPtr) {
	NODE* pPre = NULL;
	NODE* pLoc = NULL;
	int found = _search(pList, &pPre, &pLoc, ((tName*)pArgu));
	if (found == 0) {
		return 0;
	}
	else {
		*dataOutPtr = (tName*)(pLoc->dataPtr);
		return 1;
	}
} //tName 데이터노드를 변화시키지 않는것

/* returns number of nodes in list
*/
int countList(LIST* pList) {
	return pList->count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList(LIST* pList) {
	//그래도 위의 설명을 따르면:
	if (pList->count == 0) {
		return 1;
	}
	else return 0;
}//실제로 사용하지는 않으므로 정의할 필요없음!

/* traverses data from list (forward)
*/
void traverseList(LIST* pList, void (*callback)(const void*)) {
	//print_name은 tName* 을 파라미터로 받으므로 거기까지 접근하는건 여기서 해줘야한다.
	NODE* pLoc = pList->head;
	while (pLoc != NULL) {
		callback((tName*)(pLoc->dataPtr));
		pLoc = pLoc->rlink;
	}
	/*for (int i = 0; i < pList->count; i++) {
		callback((tName*)(pLoc->dataPtr));
		pLoc = pLoc->rlink;
	}*/
	//free(pLoc);
}//이전 과제의 printList의 업그레이드 버전 +callback함수 : 이름 자료구조를 직접 받아서 갖고오는것 //print_name을 사용하기만 하면 된다.

/* traverses data from list (backward)
*/
void traverseListR(LIST* pList, void (*callback)(const void*)) {
	NODE* pLoc = pList->rear;
	while (pLoc != NULL) {
		callback((tName*)(pLoc->dataPtr));
		pLoc = pLoc->llink;
	}
	/*for (int i = 0; i < pList->count; i++) {
		callback((tName*)(pLoc->dataPtr));
		pLoc = pLoc->llink;
	}*/
}

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert(LIST* pList, NODE* pPre, void* dataInPtr) {
	//dataInPtr 은 tName*임
	if (pList == NULL) {
		return 0;//애초에 pList동적메모리 할당에 실패했었다면 - overflow
	}
	//순서를 알아야함..?
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if (new_node == NULL) {
		return 0;//동적 메모리할당 실패 -> memory overflow
	}
	new_node->dataPtr = (tName*)dataInPtr;//역참조이므로 자료형을 붙여준다.
	new_node->llink = pPre;
	new_node->rlink = NULL;
	//new_nodellink처리는 원래 되어있음 new_node는 r만 정하면 되는것

	//head를 바꿀 경우
	if (pPre == NULL) {
		new_node->rlink = (pList->head);
		(pList->head)->llink = new_node;
		pList->head = new_node;
	}
	//중간에 넣을 경우
	else
	{
		new_node->rlink = pPre->rlink;
		//근데 마지막이 NULL이면 rear이므로
		if (pPre->rlink == NULL)
			pList->rear = new_node;
		else
			pPre->rlink->llink = new_node;

		pPre->rlink = new_node;
	}
	pList->count += 1;
	return 1;
} //tName _붙은 것은 _스태틱 내부함수. 위의 함수가 사용하는 함수.

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST* pList, NODE* pPre, NODE* pLoc, void** dataOutPtr) {
	*dataOutPtr = pLoc->dataPtr;
	//1개남은것마저 삭제(빈 리스트가 된다.)
	if (pList->count == 1) {
		pList->head = NULL;
		pList->rear = NULL;
	}
	//head를 삭제할 경우
	else if (pPre == NULL) {
		pLoc->rlink->llink = pPre;
		pList->head = pLoc->rlink;
	}
	//rear를 삭제할 경우
	else if (pLoc->rlink == NULL) {
		pPre->rlink = NULL;
		pList->rear = pPre;
	}
	//중간을 삭제할 경우
	else {
		pPre->rlink = pLoc->rlink;
		pLoc->rlink->llink = pPre;
	}
	//삭제한 노드는 어떻게 free해주지? 
	free(pLoc);
	pList->count -= 1;
} //tName 이중포인터로 선언한이유! - 변화를 반영하기 위해!!

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search(LIST* pList, NODE** pPre, NODE** pLoc, void* pArgu) {
	//탐색 위치를 pLoc에 저장할 것임
	*pLoc = pList->head;
	for (int i = 0; i < (pList->count); i++) {
		*pPre = (*pLoc)->llink;
		int cmpname = (pList->compare((tName*)pArgu, ((tName*)((*pLoc)->dataPtr))));
		//printf("%d\n", cmpname);
		if (cmpname == 0)
		{
			return 1;
		}
		else if (cmpname < 0)
		{
			return 0;
		}
		if ((*pLoc)->rlink) {
			*pLoc = (*pLoc)->rlink;
		}
		else if ((*pLoc)->rlink == NULL) {
			*pPre = (*pLoc);
			return 0;
		}
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
/* Allocates dynamic memory for a name structure, initialize fields(name, freq) and returns its address to caller
	return	name structure pointer
			NULL if overflow
*/
tName* createName(char* str, int freq) {
	tName* pName = (tName*)malloc(sizeof(tName));
	if (pName == NULL) {
		return NULL;
	}
	pName->name = strdup(str);//여기서 할당한 동적 메모리(strdup)는 1.addNode중에 중복이 확인되었을 경우 : destroyName으로 해제됨 OR 2.프로그램 종료시 : destroyList로 해제됨.
	pName->freq = freq;
	return pName;
} //이름 문자열을 받는데 strdup(name)이라는 함수를 사용하면 메모리를 새롭게 동적할당 받아서 변수로 넘겨줌 다만 이를 또 free해줘야함

/* Deletes all data in name structure and recycles memory
*/
void destroyName(void* pNode) {
	free(((tName*)pNode)->name);
	free((tName*)pNode);
}

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf("%c", &ch);
	ch = toupper(ch);//대문자 변환!
	switch (ch)
	{
	case 'Q':
		return QUIT;
	case 'F':
		return FORWARD_PRINT;
	case 'B':
		return BACKWARD_PRINT;
	case 'S':
		return SEARCH;
	case 'D':
		return DELETE;
	case 'C':
		return COUNT;
	}
	return 0; // undefined action
}


int main(int argc, char** argv)
{
	LIST* list;

	char str[1024];
	int freq;

	tName* pName;
	int ret;
	FILE* fp;

	if (argc != 2) {
		fprintf(stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}

	fp = fopen(argv[1], "rt");
	if (!fp)
	{
		fprintf(stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}

	// creates an empty list
	list = createList(cmpName);
	if (!list)
	{
		printf("Cannot create list\n");
		return 100;
	}

	void* p;

	while (fscanf(fp, "%*d\t%s\t%*c\t%d", str, &freq) != EOF) //*를붙여 우리는 이름과 빈도정보만 읽어오도록 하였음!
	{
		pName = createName(str, freq);

		ret = addNode(list, pName, increse_freq); //넣어주는데 기존에 있는 이름이면 ->increase freq하는것 --중복인지 아닌지 알아야하므로 그 결과를

		if (ret == 2) // duplicated
		{
			destroyName(pName);//중복일 경우, 새노드만든걸 바로 해제해준다.
		}
	}

	//while (fscanf(fp, "%*d\t%s\t%*c\t%d", str, &freq) != EOF) //*를붙여 우리는 이름과 빈도정보만 읽어오도록 하였음!
	//{
	//	pName = createName(str, freq);

	//	ret = addNode(list, pName, increse_freq); //넣어주는데 기존에 있는 이름이면 ->increase freq하는것 --중복인지 아닌지 알아야하므로 그 결과를

	//	if (ret == 2) // duplicated
	//	{
	//		destroyName(pName);//중복일 경우, 새노드만든걸 바로 해제해준다.
	//	}
	//	if (list->count > 1) {
	//		break;
	//	}
	//}

	fclose(fp);

	fprintf(stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");

	while (1)//옵션처리하는 부분
	{
		void* p;
		int action = get_action();

		switch (action)
		{
		case QUIT:
			destroyList(list, destroyName);
			return 0;

		case FORWARD_PRINT:
			traverseList(list, print_name);
			break;

		case BACKWARD_PRINT:
			traverseListR(list, print_name);
			break;

		case SEARCH:
			fprintf(stderr, "Input a string to find: ");
			fscanf(stdin, "%s", str);

			pName = createName(str, 0);

			if (searchList(list, pName, &p))
			{
				print_name(p);
			}
			else fprintf(stdout, "%s not found\n", str);

			destroyName(pName);
			break;

		case DELETE:
			fprintf(stderr, "Input a string to delete: ");
			fscanf(stdin, "%s", str);

			pName = createName(str, 0);

			if (removeNode(list, pName, &p))
			{
				fprintf(stdout, "(%s, %d) deleted\n", ((tName*)p)->name, ((tName*)p)->freq);
				destroyName((tName*)p);
			}
			else fprintf(stdout, "%s not found\n", str);

			destroyName(pName);
			break;

		case COUNT:
			fprintf(stdout, "%d\n", countList(list));
			break;
		}

		if (action) fprintf(stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
