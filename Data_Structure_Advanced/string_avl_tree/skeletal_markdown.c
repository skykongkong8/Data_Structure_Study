#define SHOW_STEP 0 //모드를 미리 지정해놓았음..!
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y)) //이건 무엇이죠?

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
	//AVL_Traverse( tree);
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
	if (avl_head)
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
	free(pTree);
}

static void _destroy(NODE* root)
{//노드 free-ing function
	if (root->left) {
		_destroy(root->left);
	}
	if (root->right) {
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
	NODE* newNode = _makeNode(data);
	if (!newNode)
	{
		return 0;
	}
	pTree->count += 1;
	pTree->root = _insert(pTree->root, newNode);
	return 1;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE* _insert(NODE* root, NODE* newPtr)
{
	if (!root)
	{
		return newPtr;
	}

	if (strcmp(newPtr->data, root->data) < 0)
	{
		if (root->left)
		{
			root->left = _insert(root->left, newPtr);
		}
		else
		{
			root->left = newPtr;
		}
		root->height = getHeight(root);
#if	BALANCING //삽입으로 인하여 balance가 안맞았을 경우 (LL LR RL RR)
		if ((!root->right) && (root->height > 2))
		{
			if (root->left->right)
			{
				root->left = rotateLeft(root->left);
				return rotateRight(root);
			}
		}
		else if ((root->right) && (root->left->height - root->right->height > 1))
		{
			if ((root->right && (root->left->hieght - root->right->height > 1)))
			{
				root->left = rotateLeft(root->left);
				return rotateRight(root);
			}
		}
#endif
	}
	else
	{
		if (root->right)
		{
			root->right = _insert(root->right, newPtr);
		}
		else
		{
			root->right = newPtr;
		}
		root->height = getHeight(root);
#if BALANCING
		if (!(root->left) && (root->height > 2))
		{
			if (root->right->left)
			{
				root->right = rotateRight(root->right);
				return rotateLeft(root);
			}
		}
		else if ((root->left) && (root->right->height - root->left->height > 1))
		{
			if (root->right->right->height < root->right->left->height)
			{
				root->right = rotateRight(root->right);
			}
			return rotateLeft(root);
		}
#endif
	}
	return root;
}


static NODE* _makeNode(char* data)
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode) {
		newNode->data = strdup(data);
		newNode->left = NULL;
		newNode->right = NULL;
		newNode->height = 1;
	}
	return newNode;
}


/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char* AVL_Retrieve(AVL_TREE* pTree, char* key)
{
	char* keyLoc = NULL;
	if (_retrieve(pTree->root, key))
	{
		keyLoc = _retrieve(pTree->root, key)->data;
	}
	return keyLoc;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, char* key)
{
	while (strcmp(root->data, key)==1)
	{
		if (strcmp(root->data, key) < 0)
		{
			if (root->right)
			{
				root = root->right;
			}
			else
			{
				return NULL;
			}
		}
		else if (strcmp(root->data, key) > 0)
		{
			if (root->left)
			{
				root = root->left;
			}
			else
			{
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
		_traverse(pTree->root);
	}
}
static void _traverse(NODE* root)
{
	if (root->left)
	{
		_traverse(root->left);
	}
	fprintf(stdout, "%s ", root->data);
	if (root->right)
	{
		_traverse(root->right);
	}
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE* pTree)
{
	if (pTree->count > 0)
	{
		_infix_print(pTree->root, 0);
	}
}
/* internal traversal function
*/
static void _infix_print(NODE* root, int level)
{
	if (root->right)
	{
		_infix_print(root->right, level + 1);
	}
	for (int i = 0; i < level; i++)
	{
		fprintf(stdout, "\t");
	}
	fprintf(stdout, "%s\n", root->data);
	if (root->left)
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE* root)
{
	
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE* rotateRight(NODE* root)
{

}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE* rotateLeft(NODE* root)
{

}