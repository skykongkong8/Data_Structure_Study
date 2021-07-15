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
	free(pTree); // destroy�ϰ� head�� free
}

static void _destroy(NODE* root)
{//��� free-ing function
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
{//�⺻����: �켱 ���� ��ġ�� ã�´�. AVL tree�� BST + balanced�̹Ƿ� BSTó�� ��Һ��ؼ� �¿�� �������� �����ȣ�� + 
	//balancing ��Ȳ ���� = LL LR RL RR (unbalancing�� ��ó�ϰ� �ִ� ����� ��ġ�� �����ϴ� ǥ���̴�.)
	if (!root)//����ó��
	{
		return newPtr;
	}


	if (strcmp(newPtr->data, root->data) >= 0)//root���� ������ �����Ͱ� ũ�� : ���������� �ϰ�
	{
		if (root->right)
		{//���� ������ �ڸ��� ���Ҵٸ� �� �ϰ�(�̶� ���翡�� �������� ���� ���������� ������ �𸣴� ���̹Ƿ� ����� ȣ��(�ٽ� �����ϵ���))
			root->right = _insert(root->right, newPtr);
		}
		else
		{//������ �ڸ��� ���� ã���� �� �ڸ��� ���ڸ��̴�.: 
			root->right = newPtr;
		}
		root->height = getHeight(root);//root�� �߰��Ǿ����Ƿ�, height�� �ٽ� å�� (balancing�� �ؾ��ϴ°� �ƴѰ��� �ٽ� �������� ����)

#if BALANCING
		if ((root->height > 2) && (!(root->left)))//balance�� �ȸ´� ��Ȳ�� ����
		{//�ʹ� ��Ȳ: ���ʿ� ������ �� ���
			if (root->right->left) {//LR�� ��Ȳ �ش�Ǹ� LR�̰� �ش�ȵǸ� RR
				root->right = rotateRight(root->right);
			}
			return rotateLeft(root);//RR
		}
		else if ((root->left) && (root->right->height - root->left->height > 1))
		{//���� �Ϲ����� ��Ȳ : �¿��� ���� ���� 1���� ũ��
			if (root->right->right->height < root->right->left->height)
			{
				root->right = rotateRight(root->right);//LR
			}
			return rotateLeft(root);//RR
		}
#endif
	}

	else if (strcmp(newPtr->data, root->data) < 0)
	{//���� VICE VERSA ���� �ϰ�
		if (root->left)
		{
			root->left = _insert(root->left, newPtr);
		}
		else
		{//���ڸ��� ���ڸ��ϼ�~
			root->left = newPtr;
		}
		root->height = getHeight(root);
#if BALANCING
		if ((root->height > 2) && (!(root->right)))
		{//���� �ʹݿ� �������� insert�� �� ���:
			if (root->left->right)
			{//left�� right�� �����Ѵٸ� : = ������ �����ϸ�  Right of Left�ΰ� ������;;
				root->left = rotateLeft(root->left);//RL

			}
			return rotateRight(root);//LL
		}
		else if ((root->right) && (root->left->height - root->right->height > 1))
		{//�¿��� ���� �� 1���� ū���!
			if (root->left->left->height < root->left->right->height)
			{//�׻��¿��� LL���̿� RL ������ �� -> RL�� ��ũ�ٸ�, 
				root->left = rotateLeft(root->left);//RL ��Ȳ�� ���̰�
			}
			return rotateRight(root);//LL�� �� ũ�ٸ� LL ��Ȳ�� ��!
		}
#endif
	}

	return root;
}


static NODE* _makeNode(char* data)
{//�޾ƿ� data�� ��带 ����� �Լ�
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if (!new_node)
	{
		return NULL;
	}//overflow
	else if (new_node)
	{//�� ����
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
	{//���� ������ NULL�� �����ϹǷ� �ڿ������� ����� ������
		char* keyPtr = _retrieve(pTree->root, key)->data;//����� ȣ��
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
{//LRŽ��
	while (strcmp(root->data, key))//==0�� �ƴ� ������
	{
		if (strcmp(root->data, key) > 0)//key�� �� ������ -> left�ϰ� 
		{
			if (root->left)
			{
				root = root->left;
			}
			else
			{//��� ����� NULL
				return NULL;
			}
		}

		else if (strcmp(root->data, key) < 0)//key�� �� ũ�� right �ϰ�
		{
			if (root->right)
			{
				root = root->right;
			}
			else
			{//������ NULL -> ���� ��͸� ���߰� ��
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
		_traverse(pTree->root); //����� ȣ���� ���� ���� �����Լ��� �� �����ѵ�
	}
}

static void _traverse(NODE* root)
{
	if (root->left)
	{
		_traverse(root->left);
	}
	// LNR��� travese
	fprintf(stdout, "%s ", root->data);

	if (root->right)
	{
		_traverse(root->right);
	}
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE* pTree)
{	//traverse�� ���� �����ε�, ��ü �������� ��屸������
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
	{//level��ŭ indenting
		fprintf(stdout, "\t");
	}

	fprintf(stdout, "%s\n", root->data);
	//���
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

	maxHeight = max(leftHeight, rightHeight) + 1;//�� ù�ٿ� ��Ҵ� max�Լ� (python-like?)
	return maxHeight;

}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/

//rotate�� ��ü : ������ root�� �ڽ��� left�� right�� ����, left�� ���ο� root�� �Ǵ°�.
//�̶�, left�� ���� right�� ���� root�� left�� ���� �ȴ�.
static NODE* rotateRight(NODE* root)
{
	if (root->left)
	{
		NODE* new_root = root->left;
		root->left = new_root->right; //�ڽ��� �̸� �Ű���

		new_root->right = root;//�����ʿ� ���� root�־��ְ�
		root->height = getHeight(root);
		new_root->height = getHeight(new_root);
		return new_root;
	}

	else if (!root->left)
	{
		return root; //rotate�� �� ����
	}
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/

//rotateRight�� VICE VERSA 
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