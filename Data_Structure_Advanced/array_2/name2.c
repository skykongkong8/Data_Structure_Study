#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_YEAR_DURATION	10	// 기간

// 구조체 선언
typedef struct {
	char	name[20];		// 이름
	char	sex;			// 성별 M or F
	int		freq[MAX_YEAR_DURATION]; // 연도별 빈도
} tName;

typedef struct {
	int		len;		// 배열에 저장된 이름의 수
	int		capacity;	// 배열의 용량 (배열에 저장 가능한 이름의 수)
	tName* data;		// 이름 배열의 포인터
} tNames;

// 함수 원형 선언

// 연도별 입력 파일을 읽어 이름 정보(연도, 이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// 주의사항: 정렬 리스트(ordered list)를 유지해야 함 (qsort 함수 사용하지 않음)
// 이미 등장한 이름인지 검사하기 위해 bsearch 함수를 사용
// 새로운 이름을 저장할 메모리 공간을 확보하기 위해 memmove 함수를 이용하여 메모리에 저장된 내용을 복사
// names->capacity는 2배씩 증가
int compare(const void* n1, const void* n2);
int binary_search(const void* key, const void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*));
void load_names(FILE* fp, int start_year, tNames* names) {
	int line;
	char n[20];
	char s;
	int f;
	int Y;
	while (1) {
		line = fscanf(fp, "%d\t%s\t%c\t%d\n", &Y, n, &s, &f);
		int year_index = Y - start_year;
		if (line == EOF) {
			break;
		}
		if (names->len == 0) {
			//첫 번째 입력!
			strcpy(names->data->name, n);
			names->data->sex = s;
			memset(names->data->freq, 0, 10 * sizeof(int));
			names->data->freq[year_index] = f;
			names->len++;
		}
		else {
			//#1이미 있는지 아닌지(빈도수만 첨가) #2용량 부족인지 아닌지  #3인덱스의 위치가 다 밀어줘야하는 위치인지 아닌지
			//어쨌든 위 상황을 고려하기위해, binary_search로 들어가야할 인덱스를 찾는다.
			//비교할 구조체
			tName* tmp_name = (tName*)malloc(sizeof(tName));
			strcpy(tmp_name->name, n);
			tmp_name->sex = s;
			memset(tmp_name->freq, 0, 10 * sizeof(int));
			tmp_name->freq[year_index] = f;

			int place = binary_search(tmp_name, names, (names->len) * sizeof(tName), sizeof(tName), compare);
			free(tmp_name);
			//#1이미 있는지 아닌지
			if (strcmp(n, (names->data + place)->name) == 0 && (names->data + place)->sex == s) {
				(names->data + place)->freq[year_index] = f;
			}
			else {//새로 만들어야 하는 경우
				//#3 인덱스의 위치가 밀어야하는지 아닌지
				if (place > (names->len)) {
					//#2용량 부족인지 아닌지
					if ((names->len) == (names->capacity)) {//용량 부족 메모리 두배 재할당
						names->capacity *= 2;
						names->data = (tName*)realloc(names->data, (names->capacity) * sizeof(tName));
						for (int i = names->len; i < names->capacity; i++) {
							memset((names->data + i)->freq, 0, 10 * sizeof(int));
						}
					}
					strcpy((names->data + place)->name, n);
					(names->data + place)->sex = s;
					(names->data + place)->freq[year_index] = f;
				}
				else {//한 줄 씩 밀어주기
					//#2용량 부족인지 아닌지
					if ((names->len) == (names->capacity)) {//용량 부족 메모리 두배 재할당
						names->capacity *= 2;
						names->data = (tName*)realloc(names->data, (names->capacity) * sizeof(tName));
						for (int i = names->len; i < names->capacity; i++) {
							memset((names->data + i)->freq, 0, 10 * sizeof(int));
						}
					}
					memmove(names->data + place + 1, names->data + place, sizeof(tName) * ((names->len) - (place)));
					memset((names->data + place)->freq, 0, 10 * sizeof(int));
					strcpy((names->data + place)->name, n);
					(names->data + place)->sex = s;
					(names->data + place)->freq[year_index] = f;
				}
				names->len++;//어쨌든 한번 들어감
			}
		}
	}
}

// 구조체 배열을 화면에 출력
void print_names(tNames* names, int num_year) {
	for (int i = 0; i < names->len; i++) {
		printf("%s\t%c\t", (names->data + i)->name, (names->data + i)->sex);
		for (int j = 0; j < num_year; j++) {
			printf("%d\t", (names->data + i)->freq[j]);
		}
		printf("\n");
	}
}

// bsearch를 위한 비교 함수
int compare(const void* n1, const void* n2) {
	//이름, 성별이 모두 같아야 0
	tName* name1 = (tName*)n1;
	tName* name2 = (tName*)n2;
	int cmp = strcmp(name1->name, name2->name);
	if (cmp != 0) {
		return cmp;
	}
	else {
		if (name1->sex == name2->sex) {
			return 0;
		}
		else if (name1->sex > name2->sex) {
			return 1;
		}
		else return -1;
	}
}

// 이진탐색 함수
// return value: key가 발견되는 경우, 배열의 인덱스
//				key가 발견되지 않는 경우, key가 삽입되어야 할 배열의 인덱스
int binary_search(const void* key, const void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*)) {
	tName* target = (tName*)key;
	tNames* list = (tNames*)base;
	int left = 0;
	int right = (list->len) - 1;
	while (left <= right) {
		int mid = (int)(left + right) / 2;
		if (compare(target, (list->data + mid)) > 0) {
			left = mid + 1;
		}
		else if (compare(target, list->data + mid) < 0) {
			right = mid - 1;
		}
		else return mid;
	}
	return left;
}

// 함수 정의

// 이름 구조체 초기화
// len를 0으로, capacity를 1로 초기화
// return : 구조체 포인터
tNames* create_names(void)
{
	tNames* pnames = (tNames*)malloc(sizeof(tNames));

	pnames->len = 0;
	pnames->capacity = 1;
	pnames->data = (tName*)malloc(pnames->capacity * sizeof(tName));

	return pnames;
}

// 이름 구조체에 할당된 메모리를 해제
void destroy_names(tNames* pnames)
{
	free(pnames->data);
	pnames->len = 0;
	pnames->capacity = 0;

	free(pnames);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	tNames* names;
	FILE* fp;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s FILE\n\n", argv[0]);
		return 1;
	}

	// 이름 구조체 초기화
	names = create_names();

	fp = fopen(argv[1], "r");
	if (!fp)
	{
		fprintf(stderr, "cannot open file : %s\n", argv[1]);
		return 1;
	}

	fprintf(stderr, "Processing [%s]..\n", argv[1]);

	// 연도별 입력 파일(이름 정보)을 구조체에 저장
	load_names(fp, 2009, names);

	fclose(fp);

	// 이름 구조체를 화면에 출력
	print_names(names, MAX_YEAR_DURATION);

	// 이름 구조체 해제
	destroy_names(names);

	return 0;
}










