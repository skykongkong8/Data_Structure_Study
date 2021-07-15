#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char* data;
	struct node* left;
	struct node* right;
	int			height;
} NODE;

typedef struct
{
	NODE* root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE* AVL_Create(void);

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy(AVL_TREE* pTree);
static void _destroy(NODE* root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert(AVL_TREE* pTree, char* data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE* _insert(NODE* root, NODE* newPtr);

static NODE* _makeNode(char* data);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char* AVL_Retrieve(AVL_TREE* pTree, char* key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, char* key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse(AVL_TREE* pTree);
static void _traverse(NODE* root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE* pTree);
/* internal traversal function
*/
static void _infix_print(NODE* root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE* root);

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE* rotateRight(NODE* root);

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE* rotateLeft(NODE* root);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	AVL_TREE* tree;
	char str[1024];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}

	// creates a null tree
	tree = AVL_Create();

	if (!tree)
	{
		fprintf(stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE* fp = fopen(argv[1], "rt");
	if (fp == NULL)
	{
		fprintf(stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while (fscanf(fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf(stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert(tree, str);

#if SHOW_STEP
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);
#endif
	}

	fclose(fp);

#if SHOW_STEP
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	AVL_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf(stdout, "Height of tree: %d\n", tree->root->height);
	fprintf(stdout, "# of nodes: %d\n", tree->count);

	// retrieval
	char* key;
	fprintf(stdout, "Query: ");
	while (fscanf(stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve(tree, str);

		if (key) fprintf(stdout, "%s found!\n", key);
		else fprintf(stdout, "%s NOT found!\n", str);

		fprintf(stdout, "Query: ");
	}

	// destroy tree
	AVL_Destroy(tree);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE* AVL_Create(void)
{
	AVL_TREE* avl_head = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	if (!avl_head)
	{
		return NULL;
	}
	else if (avl_head)
	{
		avl_head->count = 0;
		avl_head->root = NULL;
	}
	return avl_head;
}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy(AVL_TREE* pTree)
{
	_destroy(pTree->root);
	free(pTree); // destroy하고 head를 free
}

static void _destroy(NODE* root)
{//노드 free-ing function
	if (root->left)
	{
		_destroy(root->left);
	}
	if (root->right)
	{
		_destroy(root->right);
	}
	free(root->data);
	free(root);
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert(AVL_TREE* pTree, char* data)
{
	NODE* new_node = _makeNode(data);
	if (!new_node)
	{//overflow
		return 0;
	}
	pTree->root = _insert(pTree->root, new_node);
	pTree->count += 1;
	return 1;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE* _insert(NODE* root, NODE* newPtr)
{//기본생각: 우선 삽입 위치를 찾는다. AVL tree는 BST + balanced이므로 BST처럼 대소비교해서 좌우로 내려가는 재귀적호출 + 
	//balancing 상황 유형 = LL LR RL RR (unbalancing을 자처하고 있는 요소의 위치를 지정하는 표현이다.)
	if (!root)//예외처리
	{
		return newPtr;
	}


	if (strcmp(newPtr->data, root->data) >= 0)//root보다 현재의 데이터가 크다 : 오른쪽으로 하강
	{
		if (root->right)
		{//아직 오른쪽 자리가 남았다면 더 하강(이때 현재에서 왼쪽으로 갈지 오른쪽으로 갈지는 모르는 일이므로 재귀적 호출(다시 생각하도록))
			root->right = _insert(root->right, newPtr);
		}
		else
		{//오른쪽 자리의 끝을 찾았음 그 자리가 내자리이다.: 
			root->right = newPtr;
		}
		root->height = getHeight(root);//root에 추가되었으므로, height를 다시 책정 (balancing을 해야하는가 아닌가를 다시 따져보기 위해)

#if BALANCING
		if ((root->height > 2) && (!(root->left)))//balance가 안맞는 상황을 나열
		{//초반 상황: 한쪽에 삽입이 쏠린 경우
			if (root->right->left) {//LR인 상황 해당되면 LR이고 해당안되면 RR
				root->right = rotateRight(root->right);
			}
			return rotateLeft(root);//RR
		}
		else if ((root->left) && (root->right->height - root->left->height > 1))
		{//좀더 일반적인 상황 : 좌우의 높이 차가 1보다 크다
			if (root->right->right->height < root->right->left->height)
			{
				root->right = rotateRight(root->right);//LR
			}
			return rotateLeft(root);//RR
		}
#endif
	}

	else if (strcmp(newPtr->data, root->data) < 0)
	{//위의 VICE VERSA 왼쪽 하강
		if (root->left)
		{
			root->left = _insert(root->left, newPtr);
		}
		else
		{//이자리가 내자리일세~
			root->left = newPtr;
		}
		root->height = getHeight(root);
#if BALANCING
		if ((root->height > 2) && (!(root->right)))
		{//역시 초반에 한쪽으로 insert가 쏠린 경우:
			if (root->left->right)
			{//left의 right가 존재한다면 : = 영오로 번역하면  Right of Left인게 열받음;;
				root->left = rotateLeft(root->left);//RL

			}
			return rotateRight(root);//LL
		}
		else if ((root->right) && (root->left->height - root->right->height > 1))
		{//좌우의 차가 그 1보다 큰경우!
			if (root->left->left->height < root->left->right->height)
			{//그상태에서 LL높이와 RL 높이의 비교 -> RL이 더크다면, 
				root->left = rotateLeft(root->left);//RL 상황인 것이고
			}
			return rotateRight(root);//LL이 더 크다면 LL 상황인 것!
		}
#endif
	}

	return root;
}


static NODE* _makeNode(char* data)
{//받아온 data로 노드를 만드는 함수
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if (!new_node)
	{
		return NULL;
	}//overflow
	else if (new_node)
	{//값 지정
		new_node->height = 1;
		new_node->data = strdup(data);
		new_node->right = NULL;
		new_node->left = NULL;
	}

	return new_node;
}


/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/

char* AVL_Retrieve(AVL_TREE* pTree, char* key)
{
	if (_retrieve(pTree->root, key))
	{//값이 없으면 NULL을 리턴하므로 자연스럽게 끊기기 떄문에
		char* keyPtr = _retrieve(pTree->root, key)->data;//재귀적 호출
		return keyPtr;
	}
	else return NULL;
}


/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, char* key)
{//LR탐색
	while (strcmp(root->data, key))//==0이 아닌 동안은
	{
		if (strcmp(root->data, key) > 0)//key가 더 작으면 -> left하강 
		{
			if (root->left)
			{
				root = root->left;
			}
			else
			{//재귀 멈춤용 NULL
				return NULL;
			}
		}

		else if (strcmp(root->data, key) < 0)//key가 더 크면 right 하강
		{
			if (root->right)
			{
				root = root->right;
			}
			else
			{//없으면 NULL -> 이후 재귀를 멈추게 됨
				return NULL;
			}
		}

	}
	return root;
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse(AVL_TREE* pTree)
{
	if (pTree->count != 0)
	{
		_traverse(pTree->root); //재귀적 호출을 위해 굳이 내부함수가 더 용이한듯
	}
}

static void _traverse(NODE* root)
{
	if (root->left)
	{
		_traverse(root->left);
	}
	// LNR방식 travese
	fprintf(stdout, "%s ", root->data);

	if (root->right)
	{
		_traverse(root->right);
	}
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE* pTree)
{	//traverse와 같은 원리인데, 전체 시작점인 헤드구조부터
	if (pTree->count != 0)
	{
		_infix_print(pTree->root, 0);
	}

}
/* internal traversal function
*/
static void _infix_print(NODE* root, int level)
{
	if (root->right)
	{//RNL
		_infix_print(root->right, level + 1);
	}

	for (int i = 0; i < level; i++)
	{//level만큼 indenting
		fprintf(stdout, "\t");
	}

	fprintf(stdout, "%s\n", root->data);
	//출력
	if (root->left)
	{
		_infix_print(root->left, level + 1);
	}

}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE* root)
{
	int maxHeight;
	int rightHeight = 0;
	int leftHeight = 0;

	if (root->right)
	{
		rightHeight = root->right->height;
	}

	if (root->left)
	{
		leftHeight = root->left->height;
	}

	maxHeight = max(leftHeight, rightHeight) + 1;//맨 첫줄에 잡았던 max함수 (python-like?)
	return maxHeight;

}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/

//rotate의 실체 : 기존의 root가 자신의 left의 right로 들어가고, left는 새로운 root가 되는것.
//이때, left의 원래 right는 기존 root의 left로 들어가게 된다.
static NODE* rotateRight(NODE* root)
{
	if (root->left)
	{
		NODE* new_root = root->left;
		root->left = new_root->right; //자식을 미리 옮겨줌

		new_root->right = root;//오른쪽에 기존 root넣어주고
		root->height = getHeight(root);
		new_root->height = getHeight(new_root);
		return new_root;
	}

	else if (!root->left)
	{
		return root; //rotate할 수 없음
	}
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/

//rotateRight의 VICE VERSA 
static NODE* rotateLeft(NODE* root)
{
	if (root->right)
	{
		NODE* new_root = root->right;
		root->right = new_root->left;

		new_root->left = root;
		root->height = getHeight(root);
		new_root->height = getHeight(new_root);
		return new_root;
	}

	else if (!root->right)
	{
		return root;
	}
}