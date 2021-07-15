#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node* left;
	struct node* right;
} NODE;

typedef struct
{
	NODE* root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE* BST_Create(void);

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy(TREE* pTree);

/* internal function (not mandatory)
*/
static void _destroy(NODE* root);

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert(TREE* pTree, int data);

/* internal function (not mandatory)
*/
static void _insert(NODE* root, NODE* newPtr);

NODE* _makeNode(int data);

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE* pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE* _delete(NODE* root, int dltKey, int* success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int* BST_Retrieve(TREE* pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse(TREE* pTree);
static void _traverse(NODE* root);

/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE* pTree);
/* internal traversal function
*/
static void _infix_print(NODE* root, int level);

/*
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE* pTree);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	TREE* tree;
	int data;

	// creates a null tree
	tree = BST_Create();

	if (!tree)
	{
		printf("Cannot create a tree!\n");
		return 100;
	}

	fprintf(stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf("%d", &numbers);

	fprintf(stdout, "Inserting: ");

	srand(time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers * 3) + 1; // random number (1 ~ numbers * 3)

		fprintf(stdout, "%d ", data);

		// insert funtion call
		int ret = BST_Insert(tree, data);
		if (!ret) break;
	}
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	BST_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);

	while (1)
	{
		fprintf(stdout, "Input a number to delete: ");
		int num;
		int ret = scanf("%d", &num);
		if (ret != 1) break;

		ret = BST_Delete(tree, num);
		if (!ret)
		{
			fprintf(stdout, "%d not found\n", num);
			continue;
		}

		// print tree with right-to-left infix traversal
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);

		if (BST_Empty(tree))
		{
			fprintf(stdout, "Empty tree!\n");
			break;
		}
	}

	BST_Destroy(tree);

	return 0;
}




/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE* BST_Create(void)
{//새 TREE 생성, root를 NULL로 초기화
	TREE* newTree = (TREE*)malloc(sizeof(TREE));
	newTree->root = NULL;
	return newTree;
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy(TREE* pTree)
{
	_destroy(pTree->root);
	free(pTree);//최종적으로 근본까지 메모리 해제
}

/* internal function (not mandatory)
*/
static void _destroy(NODE* root)
{
	if (root->left) {
		_destroy(root->left);
	}
	if (root->right) {
		_destroy(root->right);
	}
	free(root);
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert(TREE* pTree, int data)
{
	NODE* newNode = _makeNode(data);
	if (new_node == NULL) return 0;
	if (pTree->root == NULL) {
		pTree->root = newNode;
	}
	else {
		_insert(root, newNode);
	}
	return 1;
}


/* internal function (not mandatory)
*/
static void _insert(NODE* root, NODE* newPtr)
{
	if (root->data < newPtr->data) {
		if (root->left) {
			_insert(root->left, newPtr);
		}
		else
			root->left = newPtr;
	}
	else {
		if (root->right) {
			_insert(root->right, newPtr);
		}
		else
			root->right = newPtr;
	}
}

NODE* _makeNode(int data)
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (newNode) {
		newNode->data = data;
		newNode->left = NULL;
		newNode->right = NULL;
	}
	return newNode;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE* pTree, int dltKey)
{
	int success = 0;
	NODE* new_root = _delete(pTree->root, dltKey, &success);
	pTree->root = new_root;
	return success;
}



/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE* _delete(NODE* root, int dltKey, int* success)
{
	NODE* dltLoc = NULL;
	dltLoc = _retrieve(root, dltKey);
	if (dltLoc == NULL) {
		*success = 0;
		return root;
	}

	NODE* preLoc = NULL;
	NODE* targetLoc = root;
	while (targetLoc->data != dltKey) {
		preLoc = targetLoc;
		if (targetLoc->data < dltKeu) {
			targetLoc = targetLoc->right;
		}
		else if (targetLoc->data > dltKey) {
			targetLoc = targetLoc->left;
		}
	}

	NODE* new_root = targetLoc;
	if (targetLoc->left == NULL) {
		new_root = targetLoc->right;
	}
	else if (targetLoc->right = NULL) {
		new_root = targetLoc->left;
	}


}




/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int* BST_Retrieve(TREE* pTree, int key)
{
	NODE* keyLoc = _retrieve(pTree->root, key);
	return &(keyLoc->data);
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE* _retrieve(NODE* root, int key)
{
	while (root->data != key) {
		if (root->data < key) {
			if (root->right) {
				root = root->right;
			}
			else return NULL;
		}
		else if (root->data > key) {
			if (root->left) {
				root = root->left;
			}
			else return NULL;
		}
	}
	return root;
}


/* prints tree using inorder traversal
*/
void BST_Traverse(TREE* pTree)
{
	if (BST_Empty(pTree) == 0) {
		_traverse(pTree->root);
	}
}
static void _traverse(NODE* root)
{
	if (root->left) {
		_traverse(root->left);
	}
	printf("%d", root->data);
	if (root->right) {
		_traverse(root->right);
	}
}


/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE* pTree)
{
	if (BST_Tree(pTree) == 0) {
		_infix_print(pTree->root, 0);
	}
}
/* internal traversal function
*/
static void _infix_print(NODE* root, int level)
{
	if (root->right) {
		_infix_print(root->right, level + 1);
	}
	for (int i = 0; i++; i < level) {
		printf("\t");
	}
	printf("%d\n", root->data);
	if (root->left) {
		_infix_print(root->left, level + 1);
	}
}

/*
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE* pTree)
{
	if (pTree->root == NULL) return 1;
	else return 0;
}













