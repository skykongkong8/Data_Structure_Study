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
{//�� TREE ����, root�� NULL�� �ʱ�ȭ
	TREE* newTree = (TREE*)malloc(sizeof(TREE));
	newTree->root = NULL;
	return newTree;
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy(TREE* pTree)
{
	_destroy(pTree->root);
	free(pTree);//���������� �ٺ����� �޸� ����
}

/* internal function (not mandatory)
*/
static void _destroy(NODE* root)
{
	if (root->left)
	{
		_destroy(root->left);
	}
	if (root->right)
	{
		_destroy(root->right);
	}//left�� right�� ��� ���� leaf���� �������� ����� ����
	free(root);
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert(TREE* pTree, int data)
{
	NODE* new_node = _makeNode(data);
	if (new_node == NULL) return 0;
	if (pTree->root == NULL) {
		pTree->root = new_node;//�ƿ� ó�� �� ���, root�� ������
	}
	else
		_insert(pTree->root, new_node);//�̿ܿ��� �����Լ��� ������.
	return 1;
}


/* internal function (not mandatory)
*/
static void _insert(NODE* root, NODE* newPtr)
{
	if (root->data > newPtr->data)//�Լ��� ���� �� = root�� �׺��� �۴ٸ�, 
	{
		if (root->left)
			_insert(root->left, newPtr);//root->left�� ���� ������ left�� �İ�� �ִ´�.
		else
			root->left = newPtr;
	}
	else
	{
		if (root->right)//root���� ũ�ٸ� root->right�� ���� ������ right�� �İ� �� �ִ´�.
			_insert(root->right, newPtr);
		else
			root->right = newPtr;
	}//�� �İ��� ������ �Լ��� ��������� ȣ���ϸ鼭 �Ź� �����ϵ��� �Ѵ�.
}

NODE* _makeNode(int data)
{
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if (new_node)//overflowó��..!
	{
		new_node->data = data;
		new_node->left = NULL;
		new_node->right = NULL;
	}
	return new_node;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE* pTree, int dltKey)
{
	int success = 0;//���� ���θ� ���������ִ� ���� success
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
	//�ƿ�������? �Ʒ��� ��� ��������� ���ʿ��� �� ����
	NODE* dltLoc = NULL;
	dltLoc = _retrieve(root, dltKey);
	if (dltLoc == NULL)
	{
		*success = 0;
		return root;
	}


	NODE* preLoc = NULL;
	NODE* target_Loc = root;//������ ���� �� �θ� Ž���ϴ� �κ�
	while (target_Loc->data != dltKey)
	{
		preLoc = target_Loc;//��������� �Ǿ������Ƿ� ũ�����̸� �̿��ؼ� ũ�� ������, ������ �������� ����.
		if (target_Loc->data < dltKey)
			target_Loc = target_Loc->right;
		else if (target_Loc->data > dltKey)
			target_Loc = target_Loc->left;
	}

	//������ ��ġ�� ã������, ���� ������ �� ���� �ൿ�� ���� ����� �����غ���.

	//������ �� ��, ���� ����� �ڽĵ�� �θ� ��� �������� ���ΰ�?  (����� ������ ��Ű�鼭��)
	//�ڽ��� �ƿ���/���ʸ�/�����ʸ�/�Ѵ� �ִ� ���
	NODE* new_root = target_Loc;
	if (target_Loc->left == NULL)//���� �ڽ��� ���� ���, �θ𿡰� ������ �ڽ�(Ȥ�� �װ� root�� subtree��ü)�� ��������ָ� �ȴ�.
	{
		new_root = target_Loc->right;
	}
	else if (target_Loc->right == NULL)//������ �ڽ��� ���� ���, ���� vice versa.
	{
		new_root = target_Loc->left;
	}//�ƿ����� ���� ���ʿ� NULL�� ����Ǵ� ������ �Ǳ� ������ ����� ����.


	else//���� �ڽ��� ��� �ִ� ���!! : �ּҸ� �ܾ�ͼ� �̾���δ�.
	{
		NODE* prenew_root = new_root;
		new_root = prenew_root->right;//������ subtree�� ���� left�� ã�´�. = �ּڰ�
		while (new_root->left != NULL)
		{
			prenew_root = new_root;
			new_root = prenew_root->left;
		}
		
		//������ ��尡 root�� subtree������ �ּڰ���� = newRoot, �� �θ��� = prenew_root�� ��Ȳ

		//�ּڰ���忡 right ���� ���� ���
		if (new_root->right != NULL)
		{
			if (prenew_root != target_Loc)
			{
				prenew_root->left = new_root->right;//new_root�� �ڸ��� ��ü�����ִ°���.
			}
			else//���̰� 1��(?)¥�������� right�� �ٷ� �θ��� right�� �̾���.
				prenew_root->right = new_root->right;
		}

		//right�� �ٷ� �̾����� ���� �ƴ϶��
		if (target_Loc->right != new_root)
		{
			new_root->right = target_Loc->right;
		}
		else
			new_root->right = NULL;//�ٷθ� �׳� NULLó������

		//left�� �ٷ� �̾����� ���� �ƴ϶��
		if (target_Loc->left != new_root)
		{
			new_root->left = target_Loc->left;
		}
		else
			new_root->left = NULL;//�ٷθ� �׳� NULLó������
	}


	if (preLoc == NULL)//root�������� ��
	{
		free(target_Loc);
		*success = 1;//���� ����!
		return new_root;//���� root�� �ٲ������ ���ο� root�� �������� -> Delete�Լ����� �ٷ� �� ���ϰ��� �� root�� �������ֹǷ�
	}
	else
	{
		if (preLoc->data > new_root->data)
		{
			preLoc->left = new_root;
		}
		else
		{
			preLoc->right = new_root;
		}
		free(target_Loc);
		*success = 1;//���� ����!
		return root;//�� ��� root�� �ٲ��� �ʾ����Ƿ�
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
	while (root->data != key) {//key�� �´°��� ã������ -> �������� ������ �ݺ��� Ž���� ����
		if (root->data < key) {
			if (root->right) {//ũ�� ������
				root = root->right;
			}
			else return NULL;//right�� ������ ���� = NULL
		}
		else if (root->data > key) {
			if (root->left) {
				root = root->left;
			}
			else return NULL;//���� ����.
		}
	}
	return root;//Ž���� ���
}


/* prints tree using inorder traversal
*/
void BST_Traverse(TREE* pTree)
{
	if (BST_Empty(pTree) == 0) 
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
	printf("%d ", root->data);//�⺻������ ��� ���� Ž�� ��������� ����! LNR���
	if (root->right)
	{
		_traverse(root->right);
	}
}


/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE* pTree)
{
	if (BST_Empty(pTree)==0)
		_infix_print(pTree->root, 0);
}
/* internal traversal function
*/
static void _infix_print(NODE* root, int level)
{
	if (root->right) {
		_infix_print(root->right, level + 1);
	}
	for (int i = 0; i < level; i++) {
		printf("\t");
	}
	printf("%d\n", root->data); // �⺻������ ��� ���� Ž�� ��������� ����!LNR��� + level��ŭ tab�� ����ϴ� ���(��������� �÷�����)
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













