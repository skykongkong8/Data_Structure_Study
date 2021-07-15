#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50 //�ִ� ���� ������ ���س���

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
	//ó�� Ʈ�� ������ �����Ҵ��ؼ� ����.
	TREE* newTree = (TREE*)malloc(sizeof(TREE));
	if (newTree) {
		newTree->root = NULL;//NULL�ʱ�ȭ���ֱ�! �޸� �������� ����
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
		free(root); //�̰� �ٷ� RLE �����ΰ��ΰ�..
	}
}

static NODE* _makeNode(char ch) 
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode) {
		//��� ���� �ʱ�ȭ�ؼ� �����
		newNode->data = ch;
		newNode->left = NULL;
		newNode->right = NULL;
	}
	return newNode;
}

int postfix2tree(char* expr, TREE* pTree)
{//���ÿ� �ְ� ���鼭 �����Ѵ�
	unsigned long stack[MAX_STACK_SIZE];//unsigned long
	int top = -1;//stack�� �迭�� ���� - top�� �����ϸ鼭 �ٷ�� �ȴ�. ������ ���� ���� ������ �ʿ����.
	int i = 0;

	while (expr[i])//�Էµ� expression�� �ִ� �� - length�� ��� for������ while��!
	{
		if (isdigit(expr[i]))//isdigit�Լ� : �������� ����
		{
			top++;
			if (top > MAX_STACK_SIZE)
			{//�ʰ��ϸ� �ȵǰ�, INVALID EXPRESSION
				return 0;
			}
			stack[top] = expr[i];//�����̸� ���ÿ� �ִ´�
		}
		else
		{//���ڰ� �ƴϸ� ���������� Ȯ���Ѵ�.
			if (expr[i] != '+' && expr[i] != '-' && expr[i] != '*' && expr[i] != '/')
			{
				return 0;
			}
			if (top < 1)
			{//topȮ�� - invalid�� ��Ȳ
				return 0;
			}
			pTree->root = _makeNode(expr[i]);
			if (!(pTree->root))
			{//root�� �ϴ� �켱 �־����!
				return 0;
			}
			//right
			//Ascii�ڵ� 47~58 : ���� 0~9 (����� 48 : 0�̰�, 57 : 9 ��.)
			if ((stack[top] > 47) && (stack[top] < 58))//�����̸� ��带 �����
			{
				pTree->root->right = _makeNode(stack[top]);
				if (!pTree->root->right)
					return 0;
			}

			else
			{//���ڰ��ƴϸ�
				pTree->root->right = (NODE*)(unsigned long)stack[top];
			}
			top--;//ó��

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
		i++;//��ĭ �ٺ����� �Ѿ�� (��������)
	}
	if (top != 0)
	{//�� ����ƴµ� ���� �̻��� �ִ� ���-- INVALID EXPRESSION!
		return 0;
	}
	return 1;
}
static void _traverse(NODE* root) 
{//��ȣ�� �ִ� ���·� ����ϱ�
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
{//�������� ����?�� _traverse���� �����ؿü�������,
	if (root->right) {
		_infix_print(root->right, level + 1);
	}
	for (int i = 0; i < level; i++) {
		//�������� ������ ���� �׸�ŭ ���� ����� -- ������ ���� Ʈ��ó�� ���̵���!
		printf("\t");
	}
	printf("%c\n", root->data);
	if (root->left) {
		_infix_print(root->left, level + 1);
	}
}

float evalPostfix(char* expr) {
	int i = 0;
	float temp[MAX_STACK_SIZE];//temporary �� fringe stack
	int top = -1;
	//�������� ����� ���ݴϴ�.
	while (expr[i]) {
		if (isdigit(expr[i])) {//������ ���
			top++;
			temp[top] = expr[i] - '0'; //char�� int�� ���� �������־� Ascii������ �츮�� ���ϴ� ������ �������� ����
		}
		else {//�������� ���: ����
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
