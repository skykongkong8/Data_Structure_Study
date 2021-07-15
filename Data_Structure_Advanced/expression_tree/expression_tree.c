#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50 //최대 스택 사이즈 정해놓음

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *createTree( void);

/* Deletes all data in tree and recycles memory
*/
void destroyTree( TREE *pTree);

static void _destroy( NODE *root);

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode( char ch);

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree( char *expr, TREE *pTree);

/* Print node in tree using inorder traversal
*/
void traverseTree( TREE *pTree);

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse( NODE *root);

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);

/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix( char *expr);

////////////////////////////////////////////////////////////////////////////////
void destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
}

//////////////////////////////////////////////////////////////////////////////
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

//////////////////////////////////////////////////////////////////////////////
void traverseTree( TREE *pTree)
{
	_traverse(pTree->root);
	
	return;
}

TREE* createTree(void) 
{
	//처음 트리 구조를 동적할당해서 만듬.
	TREE* newTree = (TREE*)malloc(sizeof(TREE));
	if (newTree) {
		newTree->root = NULL;//NULL초기화해주기! 메모리 누수위험 막음
	}
	return newTree;
}

static void _destroy(NODE* root) 
{
	if (root) {
		if (root->right) {
			_destroy(root->right);
		}
		if (root->left) {
			_destroy(root->left);
		}
		free(root); //이게 바로 RLE 순서인것인가..
	}
}

static NODE* _makeNode(char ch) 
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode) {
		//노드 구조 초기화해서 만들기
		newNode->data = ch;
		newNode->left = NULL;
		newNode->right = NULL;
	}
	return newNode;
}

int postfix2tree(char* expr, TREE* pTree)
{//스택에 넣고 빼면서 구현한다
	unsigned long stack[MAX_STACK_SIZE];//unsigned long
	int top = -1;//stack을 배열로 구현 - top을 조정하면서 다루면 된다. 실제로 값을 당장 조작할 필요없음.
	int i = 0;

	while (expr[i])//입력된 expression이 있는 한 - length가 없어서 for문말고 while문!
	{
		if (isdigit(expr[i]))//isdigit함수 : 숫자인지 구별
		{
			top++;
			if (top > MAX_STACK_SIZE)
			{//초과하면 안되고, INVALID EXPRESSION
				return 0;
			}
			stack[top] = expr[i];//숫자이면 스택에 넣는다
		}
		else
		{//숫자가 아니면 연산자인지 확인한다.
			if (expr[i] != '+' && expr[i] != '-' && expr[i] != '*' && expr[i] != '/')
			{
				return 0;
			}
			if (top < 1)
			{//top확인 - invalid항 상황
				return 0;
			}
			pTree->root = _makeNode(expr[i]);
			if (!(pTree->root))
			{//root가 일단 우선 있어야함!
				return 0;
			}
			//right
			//Ascii코드 47~58 : 숫자 0~9 (참고로 48 : 0이고, 57 : 9 임.)
			if ((stack[top] > 47) && (stack[top] < 58))//숫자이면 노드를 만들고
			{
				pTree->root->right = _makeNode(stack[top]);
				if (!pTree->root->right)
					return 0;
			}

			else
			{//숫자가아니면
				pTree->root->right = (NODE*)(unsigned long)stack[top];
			}
			top--;//처리

			//left
			if ((stack[top] > 47) && (stack[top] < 58))
			{
				pTree->root->left = _makeNode(stack[top]);
				if (!pTree->root->left)
					return 0;
			}
			else
			{
				pTree->root->left = (NODE*)(unsigned long)stack[top];
			}

			stack[top] = (unsigned long)(pTree->root);
		}
		i++;//한칸 다봤으니 넘어가기 (다음으로)
	}
	if (top != 0)
	{//다 종료됐는데 뭔가 이상이 있는 경우-- INVALID EXPRESSION!
		return 0;
	}
	return 1;
}
static void _traverse(NODE* root) 
{//괄호가 있는 형태로 출력하기
	if (root) {
		if (isdigit(root->data)) {
			printf("%c", root->data);
		}
		else {
			printf("( ");
			_traverse(root->left);
			printf(" %c ", root->data);
			_traverse(root->right);
			printf(" )");
		}
	}
}

static void _infix_print(NODE* root, int level) 
{//전반적인 생각?은 _traverse에서 차용해올수있지만,
	if (root->right) {
		_infix_print(root->right, level + 1);
	}
	for (int i = 0; i < level; i++) {
		//레벨별로 깊으면 탭을 그만큼 많이 줘야함 -- 옆으로 보면 트리처럼 보이도록!
		printf("\t");
	}
	printf("%c\n", root->data);
	if (root->left) {
		_infix_print(root->left, level + 1);
	}
}

float evalPostfix(char* expr) {
	int i = 0;
	float temp[MAX_STACK_SIZE];//temporary 한 fringe stack
	int top = -1;
	//실질적인 계산을 해줍니다.
	while (expr[i]) {
		if (isdigit(expr[i])) {//숫자인 경우
			top++;
			temp[top] = expr[i] - '0'; //char와 int가 같이 있을수있어 Ascii값에서 우리가 원하는 값만을 빼내오기 위함
		}
		else {//연산자인 경우: 연산
			if (expr[i] == '+') {
				temp[top - 1] = temp[top - 1] + temp[top];
				top--;
			}
			else if (expr[i] == '-') {
				temp[top - 1] = temp[top - 1] - temp[top];
				top--;
			}
			else if (expr[i] == '*'){
				temp[top-1] = temp[top-1]*temp[top];
				top--;
			}
			else if (expr[i] == '/') {
				temp[top-1] = temp[top-1]/temp[top];
				top--;
			}

		}
		i++;
	}
	return temp[top]++;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];
	
	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf( stdout, "invalid expression!\n");
			destroyTree( tree);
			fprintf( stdout, "\nInput an expression (postfix): ");
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree(tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}
