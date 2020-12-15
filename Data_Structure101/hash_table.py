from HDLL import LinkedList  # 해시 테이블에서 사용할 링크드 리스트 임포트

class HashTable:
    """해시 테이블 클래스"""
    
    def __init__(self, capacity):
        self._capacity = capacity  # 파이썬 리스트 수용 크기 저장
        self._table = [LinkedList() for _ in range(self._capacity)]  # 파이썬 리스트 인덱스에 반 링크드 리스트 저장

    def _hash_function(self, key): 
        """
        주어진 key에 나누기 방법을 사용해서 해시된 값을 리턴하는 메소드
        주의: key는 파이썬 불변 타입이여야 한다.
        """
        return hash(key) % self._capacity
    
    def get_linked_list_for_key(self,key):
        hashed_index = self._hash_function(key)
        return self._table[hashed_index]

    def look_up_node(self, key):
        """
        주어진 key에 해당하는 데이터를 리턴하는 메소드
        """
        my_linked_list = self.get_linked_list_for_key(key)
        return my_linked_list.find_node_with_key(key)
    
    def look_up_value(self, key):
        return self.look_up_node(key).value
        # 코드를 쓰세요
            
    def insert(self, key, value):
        """
        새로운 key - value 쌍을 삽입시켜주는 메소드
        이미 해당 key에 저장된 데이터가 있으면 해당 key에 해당하는 데이터를 바꿔준다
        """
        
        if self.look_up_node(key) is not None:
            self.look_up_node(key).value = value
        else:
            self.get_linked_list_for_key(key).append(key, value)
            
        # 코드를 쓰세요

    def __str__(self):
        """해시 테이블 문자열 메소드"""
        res_str = ""

        for linked_list in self._table:
            res_str += str(linked_list)

        return res_str[:-1]

    
 
test_scores = HashTable(50)  # 시험 점수를 담을 해시 테이블 인스턴스 생성

# 여러 학생들 이름과 시험 점수 삽입
test_scores.insert("현승", 85)
test_scores.insert("영훈", 90)
test_scores.insert("동욱", 87)
test_scores.insert("지웅", 99)
test_scores.insert("신의", 88)
test_scores.insert("규식", 97)
test_scores.insert("태호", 90)

print(test_scores)

# key인 이름으로 특정 학생 시험 점수 검색
print(test_scores.look_up_value("현승"))
print(test_scores.look_up_value("태호"))
print(test_scores.look_up_value("영훈"))

# 학생들 시험 점수 수정
test_scores.insert("현승", 10)
test_scores.insert("태호", 20)
test_scores.insert("영훈", 30)

print(test_scores)

