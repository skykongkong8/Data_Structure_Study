#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_YEAR_DURATION	10	// 기간
#define LINEAR_SEARCH 0
#define BINARY_SEARCH 1

//FROM LECTURE:
//"""힌트 1 : 데이터를 load해서 중복을 search할 때, 한번 search할 때마다 새로 sort하지 말고 년도 파일이 하나 끝날 때마다 sort할 것.\어차피 년도파일 내부에는 중복이 없으므로"""
//"""힌트 2 : 아예 데이터값이 없어서(실제로는 5회 이하라서) 메모리에 없는 것들을 0으로 초기화해라-memset 사용"""
//"""힌트 3 : 첫 해는 중복이 없으므로 비교할 필요가 없으니, 이를 처리하는 별도의 실행코드를 짜라"""

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

////////////////////////////////////////////////////////////////////////////////
// 함수 원형 선언(declaration)

// 연도별 입력 파일을 읽어 이름 정보(이름, 성별, 빈도)를 이름 구조체에 저장
// 이미 구조체에 존재하는(저장된) 이름은 해당 연도의 빈도만 저장
// 새로 등장한 이름은 구조체에 추가
// 주의사항: 동일 이름이 남/여 각각 사용될 수 있으므로, 이름과 성별을 구별해야 함
// names->capacity는 2배씩 증가
// 선형탐색(linear search) 버전

//linear search는 그냥 반복문을 돌면서 하나하나 살펴본다는 뜻이다
void load_names_lsearch(FILE* fp, int year_index, tNames* names) {
	char NAME[20];
	char SEX;
	int FREQ;
	int ans;//while 반복문 할때마다 선언하지말고

	//파일이 끝날 때까지 무한 루프
	while (1) {
		ans = fscanf(fp, "%[^,],%c,%d\n", NAME, &SEX, &FREQ);
		if (ans == EOF) {
			break; //반복문 탈출방법 : 파일이 끝나기 -> 이후 main함수에서 새로운 파일로 또 load_names_lsearch를 실행하는 구조
		}
		//첫 년도일때 - 기존에 있었는지 확인할 필요가 없음
		if (year_index == 0) {
			//궁금증 : calloc 과 memset 0 으로해주는 것은 어떤 차이가 있을까??
			//용량이 충분
			if ((names->len) < (names->capacity)) {
				memset((names->data + names->len)->freq, 0, 10 * sizeof(int));
				(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
				strcpy((names->data + names->len)->name, NAME);//이름
				(names->data + names->len)->sex = SEX;//성별
				names->len++;//개수
			}
			else {
				//용량이 충분하지 않으면, 2배 확장 후 재할당
				names->data = (tName*)realloc(names->data, (names->capacity) * 2 * (sizeof(tName)));
				names->capacity *= 2;

				for (int i = (names->len); i < (names->capacity); i++) {
					memset((names->data + i)->freq, 0, 10 * sizeof(int));//0초기화 - 혹시 없을 값들을 위한(여기도 새롭게 생성된 freq이므로 0 초기화 해준다)
				}
				(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
				strcpy((names->data + names->len)->name, NAME);//이름
				(names->data + names->len)->sex = SEX;//성별
				names->len++;//개수
			}
		}
		else {
			//이외 : 기존에 있었는지 확인해야함. 이때 확인하는 방식은 linear search
			//search 부분 : 있는지 없는지를 확인해주는 Boolean 변수 check: 있으면 그 인덱스(이름과 성별이 모두 같아야함!) 없으면 0
			int check = 0;
			for (int i = 0; i < names->len; i++) {
				if ((strcmp((names->data + i)->name, NAME) == 0) && ((names->data + i)->sex == SEX)) {
					check = i;
					break;
				}
			}

			//#1 없었다(완전히 새로운 이름) ++ 용량 capacity도 따져야함
			if (check == 0) {
				//용량이 충분
				if ((names->len) < (names->capacity)) {
					(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
					strcpy((names->data + names->len)->name, NAME);//이름
					(names->data + names->len)->sex = SEX;//성별
					names->len++;//개수
				}
				else {
					//용량이 충분하지 않으면, 2배 확장 후 재할당
					names->data = (tName*)realloc(names->data, (names->capacity) * 2 * (sizeof(tName)));
					names->capacity *= 2;

					for (int i = (names->len); i < (names->capacity); i++) {
						memset((names->data + i)->freq, 0, 10 * sizeof(int));//0초기화 - 혹시 없을 값들을 위한(여기도 새롭게 생성된 freq이므로 0 초기화 해준다)
					}
					(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
					strcpy((names->data + names->len)->name, NAME);//이름
					(names->data + names->len)->sex = SEX;//성별
					names->len++;//개수
				}
			}

			//#2 있었다 - capacity 따질 필요없음
			else {
				(names->data + check)->freq[year_index] = FREQ;//중복된 이름이 있는 경우 : freq만 더 채워주면 된다
			}
		}
	}

}

// 이진탐색(binary search) 버전 --논리는 linear search랑 똑같은데 탐색을 할 때 binary search를 사용하는것임!
// C언어 내장함수 bsearch를 사용하자 + compare 전치선언
int compare(const void* n1, const void* n2);
int compare_b(const void* n1, const void* n2);
tNames* create_names();
void destroy_names(tNames* pnames);
void load_names_bsearch(FILE* fp, int year_index, int formal_len,  tNames* names, tName* tmp_name1) {
	char NAME[20];
	char SEX;
	int FREQ;
	int ans;//while 반복문 할때마다 선언하지말고
	//파일이 끝날 때까지 무한 루프
	while (1) {
		ans = fscanf(fp, "%[^,],%c,%d\n", NAME, &SEX, &FREQ);
		if (ans == EOF) {
			break; //반복문 탈출방법 : 파일이 끝나기 -> 이후 main함수에서 새로운 파일로 또 load_names_lsearch를 실행하는 구조
		}
		//첫 년도일때 - 기존에 있었는지 확인할 필요가 없음
		if (year_index == 0) {
			//궁금증 : calloc 과 memset 0 으로해주는 것은 어떤 차이가 있을까??
			//용량이 충분
			if ((names->len) < (names->capacity)) {
				memset((names->data + names->len)->freq, 0, 10 * sizeof(int));
				(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
				strcpy((names->data + names->len)->name, NAME);//이름
				(names->data + names->len)->sex = SEX;//성별
				names->len++;//개수
			}
			else {
				//용량이 충분하지 않으면, 2배 확장 후 재할당
				names->data = (tName*)realloc(names->data, (names->capacity) * 2 * (sizeof(tName)));
				names->capacity *= 2;

				for (int i = (names->len); i < (names->capacity); i++) {
					memset((names->data + i)->freq, 0, 10 * sizeof(int));//0초기화 - 혹시 없을 값들을 위한(여기도 새롭게 생성된 freq이므로 0 초기화 해준다)
				}
				(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
				strcpy((names->data + names->len)->name, NAME);//이름
				(names->data + names->len)->sex = SEX;//성별
				names->len++;//개수
			}
		}
		else {
			//이외 : 기존에 있었는지 확인해야함. 이때 확인하는 방식은 BINARY search
			//내장 bsearch는 포인터를 리턴하는데, 현 상황에서는 그렇게 좋지 않은 것 같다(인덱스로 접근하는 방식을 고수하고 있으므로)
			//구조체를 주어야해서, 가상 구조체를 만들어서 비교한다

			tName tmp_name;
			strcpy(tmp_name.name, NAME);
			tmp_name.sex = SEX;
			tName* BSCH = (tName*)bsearch(&(tmp_name), tmp_name1, formal_len, sizeof(tName), compare_b);
			//#1 있다 찾은 포인터값이 tName구조체의 포인터이므로 그대로 접근?
			if (BSCH != NULL) {
				BSCH->freq[year_index] = FREQ;
			}

			//#2 없었다(완전히 새로운 이름) ++ 아예 새로운 구조체 포인터에 저장 후 병합++용량 capacity도 따져야함
			else {
				//임시 구조체 관리자 tmp_names에 저장 -> 이후 EOF에서 names와 병합해야함
				//용량이 충분
				if ((names->len) < (names->capacity)) {
					(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
					strcpy((names->data + names->len)->name, NAME);//이름
					(names->data + names->len)->sex = SEX;//성별
					names->len++;//개수
				}
				else {
					//용량이 충분하지 않으면, 2배 확장 후 재할당
					names->data = (tName*)realloc(names->data, (names->capacity) * 2 * (sizeof(tName)));
					names->capacity *= 2;

					for (int i = (names->len); i < (names->capacity); i++) {
						memset((names->data + i)->freq, 0, 10 * sizeof(int));//0초기화 - 혹시 없을 값들을 위한(여기도 새롭게 생성된 freq이므로 0 초기화 해준다)
					}
					(names->data + names->len)->freq[year_index] = FREQ;//이번 년도(이름 개수번째) 빈도수
					strcpy((names->data + names->len)->name, NAME);//이름
					(names->data + names->len)->sex = SEX;//성별
					names->len++;//개수
				}
			}
		}
	}
}


// 구조체 배열을 화면에 출력
void print_names(tNames* names, int num_year) {
	for (int i = 0; i < names->len; i++) {
		printf("%s\t%c\t", (names->data + i)->name, (names->data + i)->sex);
		//freq는 연도별 빈도수가 저장되어있는 배열이므로 배열의 요소를 개별적으로 출력하는 처리.
		for (int j = 0; j < num_year - 1; j++) {
			printf("%d\t", (names->data + i)->freq[j]);
		}
		printf("%d\n", (names->data + i)->freq[num_year - 1]);//마지막 \t를 없애주고, 개행문자\n
	}
}

int compare_b(const void* n1, const void* n2) {
	const tName* NAME_1 = (const tName*)n1;
	const tName* NAME_2 = (const tName*)n2;

	int std = strcmp(NAME_1->name, NAME_2->name);
	if ((std == 0) && (NAME_1->sex == NAME_2->sex)) return 0;
	else {
		if (std == 0) {
			if (NAME_1->sex > NAME_2->sex) {
				return 1;
			}
			else return -1;
		}
		else {
			return std;
		}
	}
}

// qsort를 위한 비교 함수
// 정렬 기준 : 이름(1순위), 성별(2순위) +Ascii 코드 기준 : 두 구조체를 비교한다.
int compare(const void* n1, const void* n2) {
	const tName* NAME_1 = (const tName*)n1;
	const tName* NAME_2 = (const tName*)n2;

	int std = strcmp(NAME_1->name, NAME_2->name);
	if (std != 0) {
		return std; //이름이 같지 않으면, 이름순
	}
	else {//이름이 같으면, 성별순(Ascii 코드 기준)
		if (NAME_1->sex > NAME_2->sex) {
			return 1;
		}
		else {
			return -1;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// 함수 정의 (definition)

// 이름 구조체를 초기화
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
	//구조체 배열선언
	tNames* names;
	int mode;

	//파일포인터, 년수(추후에 이 횟수만큼 반복문을 돌리기 위한)
	FILE* fp;
	int num_year = 0;

	//리눅스 환경에서 argument count가 제대로 주이지지 않은 경우
	if (argc <= 2)
	{
		fprintf(stderr, "Usage: %s mode FILE...\n\n", argv[0]);
		fprintf(stderr, "mode\n\t-l\n\t\twith linear search\n\t-b\n\t\twith binary search\n");
		return 1;
	}

	//선형 탐색과 이진탐색의 모드를 나누는 코드 + 예외 처리
	if (strcmp(argv[1], "-l") == 0) mode = LINEAR_SEARCH;
	else if (strcmp(argv[1], "-b") == 0) mode = BINARY_SEARCH;
	else {
		fprintf(stderr, "unknown mode : %s\n", argv[1]);
		return 1;
	}

	// 이름 구조체 초기화
	names = create_names();

	// 첫 연도 알아내기 "yob2009.txt" -> 2009
	int start_year = atoi(&argv[2][strlen(argv[2]) - 8]); //atoi string인 숫자를 int형 문자로 바꿔주는 함수

	for (int i = 2; i < argc; i++)
	{
		num_year++;//횟수 증가 : 추후에 인덱스로 사용
		fp = fopen(argv[i], "r");//반복문 돌며 차례로 파일 열기
		if (!fp) {
			fprintf(stderr, "cannot open file : %s\n", argv[i]);
			return 1;
		}

		int year = atoi(&argv[i][strlen(argv[i]) - 8]); // ex) "yob2009.txt" -> 2009

		fprintf(stderr, "Processing [%s]..\n", argv[i]);

		if (mode == LINEAR_SEARCH)//open한 파일에 대해 search와 load 실행!
		{
			// 연도별 입력 파일(이름 정보)을 구조체에 저장
			// 선형탐색 모드 : year_index자리에 현재 년도와 시작년도의 차이를 넣어준다! -> freq 배열의 인덱스로 사용될 것임
			load_names_lsearch(fp, year - start_year, names);

		}
		else // (mode == BINARY_SEARCH)
		{
			//이전 파일까지의 결과물을 복사해두고, 복사본과 bsearch를 실행하자
			tName* tmp_name1 = (tName*)malloc((names->capacity) * sizeof(tName));
			if (year - start_year != 0) {
				memcpy(tmp_name1, names->data, (names->len) * sizeof(tName));
			}
			int formal_len = (names->capacity);
			// 이진탐색 모드
			load_names_bsearch(fp, year - start_year, formal_len, names, tmp_name1);
			// 정렬 (이름순 (이름이 같은 경우 성별순))
			qsort(names->data, names->len, sizeof(tName), compare);
			//printf("%s\t%s\t%s\n", tmp_name1->name, (tmp_name1 + 1)->name, (tmp_name1)->name);
			free(tmp_name1);
		}
		fclose(fp);
	}

	// 정렬 (이름순 (이름이 같은 경우 성별순))
	qsort(names->data, names->len, sizeof(tName), compare);

	// 이름 구조체를 화면에 출력
	print_names(names, num_year);
	
	// 이름 구조체 해제
	destroy_names(names);

	return 0;
}