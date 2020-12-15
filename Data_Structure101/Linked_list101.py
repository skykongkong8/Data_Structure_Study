# class Node:
#     """노드 클래스"""
#     def __init__(self, data):
#         self.data = data
#         self.next = None
#     """생성 시의 next 관계는 None으로 한다."""

# """Node 클래스 인스턴스 생성"""
# head_node = Node(2)
# node1 = Node(3)
# tail_node =Node(5)

# """링크드 리스트 next 관계 형성"""
# head_node.next = node1
# node1.next = tail_node

# """연쇄적 출력"""
# iterator = head_node
# while iterator is not None:
#     print(iterator.data)
#     iterator = iterator.next

# class Linked_List:
#     """링크드 리스트 클래스"""
#     def __init__(self):
#         self.head=None
#         self.tail = None

#     def __str__(self):
#         """예쁘게 str으로 출력해주는 str메소드"""
#         res_str = "|"
#         iterator = self.head
#         while iterator is not None:
#             res_str += f"{iterator.data}|"
#             iterator = iterator.next
#         return res_str

#     def append(self, data):
#         """새로운 링크드 리스트를 형성할 때"""
#         new_node = Node(data)
#         """링크드 리스트가 비어있을 경우: 하나만 넣으므로 처음과 끝이 같을 것임"""
#         if self.head is None:
#             self.head = new_node
#             self.tail = new_node
#         else:
#             """링크드 리스트가 비어있지 않을 경우, 먼저 tailnode를 와 newnode를 이어준 후, tail node의 위치를 바꾸어준다"""
#             self.tail.next = new_node
#             self.tail = new_node
    
#     def find_node_at(self, index):
#         """접근하는 메소드"""
#         iterator = self.head
#         for i in range(index):
#             iterator = iterator.next
#         return iterator

#     def find_node_with_data(self, data):
#         """인덱스가 아닌 데이터로 접근하는 메소드"""
#         iterator = self.head
#         while iterator.data is not data:
#             iterator = iterator.next
#             if iterator is None:
#                 return None
#         return iterator

#     def insert_after(self, previous_node, data):
#         """특정 인덱스에 삽입하는 메소드-생성하는것이므로 new_node를 만들어준다"""
#         new_node = Node(data)
#         """처음 아예 비어있는 경우"""
#         if previous_node is self.tail:
#             self.tail.next = new_node
#             self.tail = new_node
#             """중간에 넣는 경우"""
#         else:
#             new_node.next = previous_node.next
#             previous_node.next = new_node

#     def prepend(self, data):
#         """만약 삽입이 맨 처음 위치에 이루어진다면(previous_node가 없는것이므로)+(역시 생성이므로 new_node를 만든다)"""
#         new_node = Node(data)
#         """역시 생성이므로 맨 처음 비어있을 경우를 생각한다."""
#         if self.head is None:
#             self.head = new_node
#             self.tail = new_node
#             """일반적인 경우"""
#         else:
#             new_node.next = self.head
#             self.head = new_node
    
#     def delete_after(self, previous_node):
#         """특정 부분 노드를 삭제하는 메소드"""
#         data = previous_node.next.data
#         """self.tail을 지우는 경우"""
#         if previous_node.next is self.tail:
#             previous_node.next = None
#             self.tail = previous_node
#             """중간에 있는 값을 지우는 경우(일반적인 경우)"""
#         else:
#             previous_node.next = previous_node.next.next
#         """관례적으로 삭제 관련 메소드는 삭제한 값을 리턴해줌(이 메소드 자체를 pr int했을 때 삭제값을 받을 수 있도록)"""
#         return data

#     def pop_left(self):
#         data = self.head
        # if self.head is self.tail:
        #     self.head = None
        #     self.tail = None
        # else:
        #     self.head = self.head.next
        # return data


            
            



# """링크드 리스트의 연쇄적 출력하기"""
# my_list = Linked_List()

# my_list.append(2)
# my_list.append(3)
# my_list.append(5)

# print(my_list)

# next_should_be_numero_4 = my_list.find_node_at(1)
# print(next_should_be_numero_4) #뭐야 그냥 data위치로 나오네? 뭐지

# my_list.insert_after(next_should_be_numero_4,4)

# print(my_list)





#더블리 링크드 리스트의 경우:(위쪽은 싱글리 링크드 리스트에 관한 코드임)

class Node:
    def __init__(self, data):
        """더블리 링크드 리스트의 init메소드에 prev를 추가한다"""
        self.data = data
        self.next = None
        self.prev = None

class LinkedList:
    """링크드 리스트 클래스에 일단 싱글리 링크드 리스트와 같은 메소드들"""
    """보통 previous_node 등을 사용하지 않는 경우이다"""
    def __init__(self):
        self.head = None
        self.tail = None
    
    def __str__(self):
        rev_str = "|"
        iterator = self.head
        while iterator is not None:
            rev_str+=f"{iterator.data}|"
            iterator = iterator.next
        return rev_str

    def find_node_at(self, index):
        iterator = self.head
        for i in range(index):
            iterator = iterator.next
        return iterator

    def find_node_with_data(self, data):
        iterator = self.head
        while iterator.data is not data:
            iterator = iterator.next
            if iterator is None:
                return None
        return iterator 
    #iterator 라는 값을 설정해서 탐색하는 사고 방식 또한 배워가자.
    """prev 신설로 더블리 링크드 리스트에 새로 생겨난 메소드들"""
    
    def append(self, data):
        """리스트의 마지막에 새로운 노드를 '추가'하는 메소드"""
        new_node = Node(data)
        #링크드 리스트가 비어있는 경우:
        if self.head is None:
            self.head = new_node
            self.tail= new_node
        #일반적인 리스트에 추가하는 경우:
        else:
            self.tail.next = new_node
            new_node.prev = self.tail
            self.tail = new_node

    def insert_after(self,previous_node, data):#after라는 딕션이 있을 때부터 before에 아무것도 없는 경우에 사용할 메소드가 존재할 것이라는 생각을 해야한다
        """특정 위치에 리스트를 '삽입'하는 메소드"""
        new_node = Node(data)
        #맨 뒤에 넣을 경우
        if previous_node is self.tail:
            self.tail.next = new_node
            new_node.prev = self.tail
            self.tail = new_node
        else:#일반적인 경우
            new_node.next = previous_node.next
            previous_node.next.prev = new_node
            new_node.prev = previous_node
            previous_node.next = new_node
    
    def prepend(self, data):
        #previous_node 같은 것이 없는 가장 처음에 넣는 경우
        new_node = Node(data)
        #링크드 리스트가 비어있는 경우:
        if self.head is None:
            self.head = new_node
            self.tail= new_node
        else:
            #일반적인 경우
            new_node.next = self.head
            self.head.prev = new_node
            self.head = new_node
    def delete(self, deleting_node):
        data = deleting_node.data
        if self.head is self.tail:
            self.head = None
            self.tail = None
        elif deleting_node is self.head:
            self.head = deleting_node.next
            self.head.prev = None
        elif deleting_node is self.tail:
            self.tail = deleting_node.prev
            self.tail.next = None
        else:
            deleting_node.prev.next = deleting_node.next
            deleting_node.next.prev = deleting_node.prev
        return data
    


my_list2 = LinkedList()

my_list2.append(2)
my_list2.append(3)
my_list2.append(5)
my_list2.append(7)

print(my_list2)

my_list2.prepend(0)
print(my_list2)

A=my_list2.find_node_at(1)

B=my_list2.find_node_with_data(7)

my_list2.delete(A)
print(my_list2)
my_list2.delete(B)
print(my_list2)



