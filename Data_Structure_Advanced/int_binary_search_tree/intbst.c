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
	if (root->left)
	{
		_destroy(root->left);
	}
	if (root->right)
	{
		_destroy(root->right);
	}//left와 right가 모두 없는 leaf부터 역순으로 재귀적 해제
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
		pTree->root = new_node;//아예 처음 인 경우, root로 설정함
	}
	else
		_insert(pTree->root, new_node);//이외에는 내부함수를 따른다.
	return 1;
}


/* internal function (not mandatory)
*/
static void _insert(NODE* root, NODE* newPtr)
{
	if (root->data > newPtr->data)//함수에 들어온 값 = root임 그보다 작다면, 
	{
		if (root->left)
			_insert(root->left, newPtr);//root->left가 없을 때까지 left로 파고들어서 넣는다.
		else
			root->left = newPtr;
	}
	else
	{
		if (root->right)//root보다 크다면 root->right가 없을 때까지 right로 파고 들어서 넣는다.
			_insert(root->right, newPtr);
		else
			root->right = newPtr;
	}//그 파고드는 방향을 함수로 재귀적으로 호출하면서 매번 결정하도록 한다.
}

NODE* _makeNode(int data)
{
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	if (new_node)//overflow처리..!
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
	int success = 0;//성공 여부를 결정지어주는 변수 success
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
	//아예없으면? 아래의 모든 고려사항이 불필요할 수 있음
	NODE* dltLoc = NULL;
	dltLoc = _retrieve(root, dltKey);
	if (dltLoc == NULL)
	{
		*success = 0;
		return root;
	}


	NODE* preLoc = NULL;
	NODE* target_Loc = root;//삭제할 노드와 그 부모를 탐색하는 부분
	while (target_Loc->data != dltKey)
	{
		preLoc = target_Loc;//대소정렬이 되어있으므로 크기차이를 이용해서 크면 오른쪽, 작으면 왼쪽으로 간다.
		if (target_Loc->data < dltKey)
			target_Loc = target_Loc->right;
		else if (target_Loc->data > dltKey)
			target_Loc = target_Loc->left;
	}

	//삭제할 위치를 찾았으면, 이제 삭제를 한 후의 행동에 대한 방식을 생각해보자.

	//삭제를 한 후, 삭제 노드의 자식들과 부모를 어떻게 연결지을 것인가?  (대소의 질서를 지키면서도)
	//자식이 아예없/왼쪽만/오른쪽만/둘다 있는 경우
	NODE* new_root = target_Loc;
	if (target_Loc->left == NULL)//왼쪽 자식이 없는 경우, 부모에게 오른쪽 자식(혹은 그가 root인 subtree전체)를 연결시켜주면 된다.
	{
		new_root = target_Loc->right;
	}
	else if (target_Loc->right == NULL)//오른쪽 자식이 없는 경우, 위의 vice versa.
	{
		new_root = target_Loc->left;
	}//아예없는 경우는 애초에 NULL이 연결되는 식으로 되기 때문에 상관이 없다.


	else//양쪽 자식이 모두 있는 경우!! : 최소를 긁어와서 이어붙인다.
	{
		NODE* prenew_root = new_root;
		new_root = prenew_root->right;//오른쪽 subtree의 가장 left를 찾는다. = 최솟값
		while (new_root->left != NULL)
		{
			prenew_root = new_root;
			new_root = prenew_root->left;
		}
		
		//삭제할 노드가 root인 subtree에서의 최솟값노드 = newRoot, 그 부모노드 = prenew_root인 상황

		//최솟값노드에 right 항이 있을 경우
		if (new_root->right != NULL)
		{
			if (prenew_root != target_Loc)
			{
				prenew_root->left = new_root->right;//new_root의 자리를 대체시켜주는것임.
			}
			else//깊이가 1층(?)짜리였던것 right를 바로 부모의 right로 이어줌.
				prenew_root->right = new_root->right;
		}

		//right가 바로 이어지는 것이 아니라면
		if (target_Loc->right != new_root)
		{
			new_root->right = target_Loc->right;
		}
		else
			new_root->right = NULL;//바로면 그냥 NULL처리해줌

		//left가 바로 이어지는 것이 아니라면
		if (target_Loc->left != new_root)
		{
			new_root->left = target_Loc->left;
		}
		else
			new_root->left = NULL;//바로면 그냥 NULL처리해줌
	}


	if (preLoc == NULL)//root삭제였던 것
	{
		free(target_Loc);
		*success = 1;//삭제 성공!
		return new_root;//기존 root가 바뀌었으니 새로운 root를 리턴해줌 -> Delete함수에서 바로 이 리턴값을 새 root로 지정해주므로
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
		*success = 1;//삭제 성공!
		return root;//이 경우 root는 바뀌지 않았으므로
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
	while (root->data != key) {//key와 맞는것을 찾을것임 -> 언제든지 같으면 반복문 탐색이 끝남
		if (root->data < key) {
			if (root->right) {//크면 오른쪽
				root = root->right;
			}
			else return NULL;//right의 끝까지 간것 = NULL
		}
		else if (root->data > key) {
			if (root->left) {
				root = root->left;
			}
			else return NULL;//위와 같다.
		}
	}
	return root;//탐색한 노드
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
	printf("%d ", root->data);//기본적으로 계속 같은 탐색 순서방식의 논리임! LNR방식
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
	printf("%d\n", root->data); // 기본적으로 계속 같은 탐색 순서방식의 논리임!LNR방식 + level만큼 tab을 출력하는 방식(재귀적으로 늘려가며)
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













