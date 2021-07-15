#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$'	// end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode { //별다른 head없이 노드 하나만 있는것
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void); //빈 노드 하나 만드는 것, 초기화

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root); // 메모리 해제

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index);//하나의 문자열 삽입
//dic_index = 사전상의 인덱스 ->  이걸로 찾는다. +중복 체크 하도록!
//isupper, tolower 등의 추가 함수를 사용해서 소문자로 변환하여 적절하게 넣을 것! 영문자같은게 아니면 안되게 예외 처리
/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch(TRIE* root, char* str);
///완성 후, 주어진 문자열로 탐색하기


/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]);
//조회할 수 있는 모든 문자열들을 나타내어주는 것! 추후의 인덱스로 접근하는 것을 확인할 수 있도록!
//trie는 전위순회를 해야, 순서대로 나타남 (힌트)

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]);
//주어진 string까지는 따라 내려감 -> 거기서부터는 또 trieList를 사용해서 그 밑의 모든 것들을 리스트 형식으로 리턴하는?


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	int index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode();
	
	while (fscanf( fp, "%s", str) != EOF)
	{
		ret = trieInsert( trie, str, index);

		if (ret) 
			dic[index++] = strdup( str);//삽입에 성공한 경우에만 dic에 인덱스를 늘려가면서 삽입해주는 것(strdup으로 복사하여서!)
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search
		if (str[strlen(str)-1] == '*')
		{//음!
			str[strlen(str)-1] = 0;
			triePrefixList( trie, str, dic);
		}
		// keyword search
		else
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		
		printf( "\nQuery: ");
	}
	
	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	
	return 0;
}
// ./a.out words_ordered.txt 등 하면 된다.

/////////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE* trieCreateNode(void)
{
	TRIE* Trie = (TRIE*)malloc(sizeof(TRIE));
	if (!Trie)
	{
		return NULL;
	}

	else if (Trie)
	{
		Trie->index = -1;
		for (int i = 0; i < MAX_DEGREE; i++)
		{
			Trie->subtrees[i] = NULL;
		}
			
	}
	return Trie;
}
//빈 노드 하나 만드는 것, 초기화

/* Deletes all data in trie and recycles memory
*/
void trieDestroy(TRIE* root)
{
	for (int i = 0; i < MAX_DEGREE; i++)
	{//내부적 붕괴
		if (root->subtrees[i])
		{
			trieDestroy(root->subtrees[i]);
		}
			
	}
	free(root);//root까지 해제
}
// 메모리 해제

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert(TRIE* root, char* str, int dic_index)
{
	int result = trieSearch(root, str);//먼저 위치를 찾는다
	if (result != -1)
	{
		return 0;
	}

	if (!str[0])
	{
		root->index = dic_index;
		return 1;
	}
	else
	{
		if (isupper(str[0]))
		{//소문자변환
			str[0] = tolower(str[0]);
		}

		int INDEX = getIndex(str[0]);

		if ((INDEX < 0))
		{
			return 0;
		}
		if ((INDEX >= MAX_DEGREE))
		{
			return 0;
		}
		

		if (!root->subtrees[INDEX])
		{
			root->subtrees[INDEX] = trieCreateNode();
			if (!root->subtrees[INDEX])
			{
				return 0;
			}
		}
		int seguimente = trieInsert(root->subtrees[INDEX], str + 1, dic_index);
		if (!seguimente)
		{
			return 0;
		}
	}
	return 1;
}
//하나의 문자열 삽입
//dic_index = 사전상의 인덱스 ->  이걸로 찾는다. +중복 체크 하도록!
//isupper, tolower 등의 추가 함수를 사용해서 소문자로 변환하여 적절하게 넣을 것! 영문자같은게 아니면 안되게 예외 처리
/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch(TRIE* root, char* str)
{
	int iterator = 0;
	while (str[iterator])
	{
		if (isupper(str[iterator]))
		{
			str[iterator] = tolower(str[iterator]);
		}
		int INDEX = getIndex(str[iterator]);
		if (!root->subtrees[INDEX])
		{
			return -1;
		}
		if ((INDEX >= MAX_DEGREE) || (INDEX < 0))
		{
			return -1;
		}
		
		root = root->subtrees[INDEX];
		iterator+=1;
	}
	int ans = root->index;
	return ans;
}
///완성 후, 주어진 문자열로 탐색하기


/* prints all entries in trie using preorder traversal
*/
void trieList(TRIE* root, char* dic[])
{
	if (root->index != -1)
	{//standardout
		printf("%s\n", dic[root->index]);
	}
	for (int i = 0; i < MAX_DEGREE; i++)
	{
		if (root->subtrees[i])
		{
			trieList(root->subtrees[i], dic);
		}
	}
}
//조회할 수 있는 모든 문자열들을 나타내어주는 것! 추후의 인덱스로 접근하는 것을 확인할 수 있도록!
//trie는 전위순회를 해야, 순서대로 나타남 (힌트)

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList(TRIE* root, char* str, char* dic[])
{
	int iterator = 0;
	int trieIndex = -1;
	while (str[iterator])
	{//주어진 string까지는 따라 내려감 -> 거기서부터는 또 trieList를 사용해서 그 밑의 모든 것들을 리스트 형식으로 리턴하는?
		if (isupper(str[iterator]))
		{
			str[iterator] = tolower(str[iterator]);
		}
		trieIndex = getIndex(str[iterator]);
		if ((trieIndex < 0))
		{
			return;
		}
		if ((trieIndex >= MAX_DEGREE))
		{
			return;
		}
		if (!root->subtrees[trieIndex])
		{
			return;
		}
		root = root->subtrees[trieIndex];
		iterator+=1;
	}
	trieList(root, dic);
}



/////////////////////////////////////////////////////////////////