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
int searchList(LIST* pList, void* pArgu, void** dataOutPtr); //tName �����ͳ�带 ��ȭ��Ű�� �ʴ°�

/* returns number of nodes in list
*/
int countList(LIST* pList);

/* returns	1 empty
			0 list has data
*/
int emptyList(LIST* pList);//������ ��������� �����Ƿ� ������ �ʿ����!

/* traverses data from list (forward)
*/
void traverseList(LIST* pList, void (*callback)(const void*));//printList�� ���׷��̵� ���� +callback�Լ� : �̸� �ڷᱸ���� ���� �޾Ƽ� ������°� //print_naem�� ����ϱ⸸ �ϸ� �ȴ�.

/* traverses data from list (backward)
*/
void traverseListR(LIST* pList, void (*callback)(const void*));

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert(LIST* pList, NODE* pPre, void* dataInPtr); //tName _���� ���� _����ƽ �����Լ�. ���� �Լ��� ����ϴ� �Լ�.

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST* pList, NODE* pPre, NODE* pLoc, void** dataOutPtr); //tName ���������ͷ� ����������! - ��ȭ�� �ݿ��ϱ� ����!!

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
tName* createName(char* str, int freq); //�̸� ���ڿ��� �޴µ� strdup(name)�̶�� �Լ��� ����ϸ� �޸𸮸� ���Ӱ� �����Ҵ� �޾Ƽ� ������ �Ѱ��� �ٸ� �̸� �� free�������

/* Deletes all data in name structure and recycles memory
*/
void destroyName(void* pNode);


////////////////////////////////////////////////////�پ��� �ݹ��Լ��� �̸� �����س�����. ��Ȳ�� ���� ������ �ݹ��Լ��� ����Ұ�
// compares two names in name structures
// for createList function
int cmpName(const void* pName1, const void* pName2)
{
	return strcmp(((tName*)pName1)->name, ((tName*)pName2)->name);//�Ʊ� �� ���̵������͸� ��� ����ϴ��� HINT : �� ��ȣ�� �Ἥ ĳ������ ���� �ǵ��� �ؾ���!
}

// prints name and freq in name structure
// for traverseList and traverseListR functions
void print_name(const void* dataPtr)
{
	printf("%s\t%d\n", ((tName*)dataPtr)->name, ((tName*)dataPtr)->freq);
}

// increases freq in name structure
// for addNode function
void increse_freq(const void* dataOutPtr, const void* dataInPtr) //addnode���� �ߺ��� �߻����� �� �󵵼��� ���ϱ�!
{
	((tName*)dataOutPtr)->freq += ((tName*)dataInPtr)->freq;
}

////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// �Լ� ��ġ����

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST* createList(int (*compare)(const void*, const void*)) {
	LIST* pList = (LIST*)malloc(sizeof(LIST));
	pList->count = 0;
	pList->head = NULL;
	pList->rear = NULL;
	pList->pos = NULL; //�ʿ�X
	pList->compare = compare;

	return pList;
}

/* Deletes all data in list and recycles memory
*/
void destroyList(LIST* pList, void (*callback)(void *)) {
	//LIST���� ��ҵ��� ���ʷ� ����
	for (int i = 0; i < pList->count; i++) {
		NODE* pLoc = pList->head;
		pList->head = pLoc->rlink;

		callback(((tName*)(pLoc->dataPtr)));
		//free(((tName*)pLoc->dataPtr)->name);//dataPtr�� void���� ��������..
		//free((tName*)(pLoc->dataPtr));
		free(pLoc);
	}
	//����������
	free(pList);
}

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
//
int addNode(LIST* pList, void* dataInPtr, void (*callback)(const void*, const void*)) {
	//callback�� ���⼭�� Increasefrequency
	// �� ����Ʈ : ó�� �ִ� ���
	NODE* pPre = NULL;
	NODE* pLoc = NULL;
	if (pList->head == NULL) {
		NODE* new_node = (NODE*)malloc(sizeof(NODE));
		if (new_node == NULL) {
			return 0;//���� �޸��Ҵ� ���� -> memory overflow
		}
		new_node->dataPtr = (tName*)dataInPtr;
		new_node->llink = NULL;
		new_node->rlink = NULL;
		pList->rear = new_node;
		pList->head = new_node;
		//ó�� �ִ� ���� �ߺ��� ��찡 ����!
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

	//�ߺ��� ���� �� �����Ƿ� Ž���� �ǽ��Ѵ�.
	int found = _search(pList, &pPre, &pLoc, (((tName*)dataInPtr)));
	if (found == 0) {
		int IN = _insert(pList, pPre, dataInPtr);
		if (IN == 0) {//���Կ� ���� - overflow���� ����
			return 0;
		}
		else return 1;
	}
	else {//ã�� : 1.�󵵼� ����, 2."""�ʿ���� �����޸� ����""" ->�̰� main�Լ����� ����
		//dataInPtr : �����ͽ�Ʈ���� �޾ƿ� �����ͷ� ���� tName
		//pLoc : _search �Լ��� ã�� dataInPtr�� �Ȱ��� name�ν��Ͻ��� ���� �̹� �ִ�, ������ũ�帮��Ʈ ���� ����� ������
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
} //tName �����ͳ�带 ��ȭ��Ű�� �ʴ°�

/* returns number of nodes in list
*/
int countList(LIST* pList) {
	return pList->count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList(LIST* pList) {
	//�׷��� ���� ������ ������:
	if (pList->count == 0) {
		return 1;
	}
	else return 0;
}//������ ��������� �����Ƿ� ������ �ʿ����!

/* traverses data from list (forward)
*/
void traverseList(LIST* pList, void (*callback)(const void*)) {
	//print_name�� tName* �� �Ķ���ͷ� �����Ƿ� �ű���� �����ϴ°� ���⼭ ������Ѵ�.
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
}//���� ������ printList�� ���׷��̵� ���� +callback�Լ� : �̸� �ڷᱸ���� ���� �޾Ƽ� ������°� //print_name�� ����ϱ⸸ �ϸ� �ȴ�.

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
	//dataInPtr �� tName*��
	if (pList == NULL) {
		return 0;//���ʿ� pList�����޸� �Ҵ翡 �����߾��ٸ� - overflow
	}
	//������ �˾ƾ���..?
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if (new_node == NULL) {
		return 0;//���� �޸��Ҵ� ���� -> memory overflow
	}
	new_node->dataPtr = (tName*)dataInPtr;//�������̹Ƿ� �ڷ����� �ٿ��ش�.
	new_node->llink = pPre;
	new_node->rlink = NULL;
	//new_nodellinkó���� ���� �Ǿ����� new_node�� r�� ���ϸ� �Ǵ°�

	//head�� �ٲ� ���
	if (pPre == NULL) {
		new_node->rlink = (pList->head);
		(pList->head)->llink = new_node;
		pList->head = new_node;
	}
	//�߰��� ���� ���
	else
	{
		new_node->rlink = pPre->rlink;
		//�ٵ� �������� NULL�̸� rear�̹Ƿ�
		if (pPre->rlink == NULL)
			pList->rear = new_node;
		else
			pPre->rlink->llink = new_node;

		pPre->rlink = new_node;
	}
	pList->count += 1;
	return 1;
} //tName _���� ���� _����ƽ �����Լ�. ���� �Լ��� ����ϴ� �Լ�.

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST* pList, NODE* pPre, NODE* pLoc, void** dataOutPtr) {
	*dataOutPtr = pLoc->dataPtr;
	//1�������͸��� ����(�� ����Ʈ�� �ȴ�.)
	if (pList->count == 1) {
		pList->head = NULL;
		pList->rear = NULL;
	}
	//head�� ������ ���
	else if (pPre == NULL) {
		pLoc->rlink->llink = pPre;
		pList->head = pLoc->rlink;
	}
	//rear�� ������ ���
	else if (pLoc->rlink == NULL) {
		pPre->rlink = NULL;
		pList->rear = pPre;
	}
	//�߰��� ������ ���
	else {
		pPre->rlink = pLoc->rlink;
		pLoc->rlink->llink = pPre;
	}
	//������ ���� ��� free������? 
	free(pLoc);
	pList->count -= 1;
} //tName ���������ͷ� ����������! - ��ȭ�� �ݿ��ϱ� ����!!

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search(LIST* pList, NODE** pPre, NODE** pLoc, void* pArgu) {
	//Ž�� ��ġ�� pLoc�� ������ ����
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
	pName->name = strdup(str);//���⼭ �Ҵ��� ���� �޸�(strdup)�� 1.addNode�߿� �ߺ��� Ȯ�εǾ��� ��� : destroyName���� ������ OR 2.���α׷� ����� : destroyList�� ������.
	pName->freq = freq;
	return pName;
} //�̸� ���ڿ��� �޴µ� strdup(name)�̶�� �Լ��� ����ϸ� �޸𸮸� ���Ӱ� �����Ҵ� �޾Ƽ� ������ �Ѱ��� �ٸ� �̸� �� free�������

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
	ch = toupper(ch);//�빮�� ��ȯ!
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

	while (fscanf(fp, "%*d\t%s\t%*c\t%d", str, &freq) != EOF) //*���ٿ� �츮�� �̸��� �������� �о������ �Ͽ���!
	{
		pName = createName(str, freq);

		ret = addNode(list, pName, increse_freq); //�־��ִµ� ������ �ִ� �̸��̸� ->increase freq�ϴ°� --�ߺ����� �ƴ��� �˾ƾ��ϹǷ� �� �����

		if (ret == 2) // duplicated
		{
			destroyName(pName);//�ߺ��� ���, ����常��� �ٷ� �������ش�.
		}
	}

	//while (fscanf(fp, "%*d\t%s\t%*c\t%d", str, &freq) != EOF) //*���ٿ� �츮�� �̸��� �������� �о������ �Ͽ���!
	//{
	//	pName = createName(str, freq);

	//	ret = addNode(list, pName, increse_freq); //�־��ִµ� ������ �ִ� �̸��̸� ->increase freq�ϴ°� --�ߺ����� �ƴ��� �˾ƾ��ϹǷ� �� �����

	//	if (ret == 2) // duplicated
	//	{
	//		destroyName(pName);//�ߺ��� ���, ����常��� �ٷ� �������ش�.
	//	}
	//	if (list->count > 1) {
	//		break;
	//	}
	//}

	fclose(fp);

	fprintf(stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");

	while (1)//�ɼ�ó���ϴ� �κ�
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
