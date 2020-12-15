# #binary_tree

# # class Node:
# #     def __init__(self, data):
# #         self.data = data
# #         self.left_child = None
# #         self.right_child = None

# # root_node = Node("A");node_b = Node("B");node_c = Node("C");node_d = Node("D");node_e= Node("E");node_f = Node("F");node_g = Node("G");node_h = Node("H")

# # root_node.left_child = node_b
# # root_node.right_child = node_c

# # node_b.left_child = node_d
# # node_b.right_child = node_e

# # node_e.left_child = node_g
# # node_e.right_child = node_h

# # node_c.right_child = node_f


# #perfect_binary_tree--by list

# def get_parent_index(complete_binary_tree, index):
#     """배열로 구현한 완전 이진 트리에서 index번째 노드의 부모 노드의 인덱스를 리턴하는 함수"""
#     # 코드를 쓰세요
#     return index//2


   
# def get_left_child_index(complete_binary_tree, index):
#     """배열로 구현한 완전 이진 트리에서 index번째 노드의 왼쪽 자식 노드의 인덱스를 리턴하는 함수"""
#     # 코드를 쓰세요
#     return index*2

# def get_right_child_index(complete_binary_tree, index):
#     """배열로 구현한 완전 이진 트리에서 index번째 노드의 오른쪽 자식 노드의 인덱스를 리턴하는 함수"""
#     # 코드를 쓰세요
#     return index*2 +1

# # 실행 코드
# root_node_index = 1 # root 노드

# tree = [None, 1, 5, 12, 11, 9, 10, 14, 2, 10]  # 과제 이미지에 있는 완전 이진 트리



# # in-order 순회 재귀함수 구현(non-perfect-binary-tree)---node클래스는 위에 이미 있음

# def traverse_inorder(node):
#     while node != None:
#         if node.left_child is None:
#             print(node.data)
#             if node.right_child is None:
#                 return 
#             else:
#                 return traverse_inorder(node.right_child)
#         else:
#             return traverse_inorder(node.left_child), print(node.data), traverse_inorder(node.right_child)


# node_A = Node("A")
# node_B = Node("B")
# node_C = Node("C")
# node_D = Node("D")
# node_E = Node("E")
# node_F = Node("F")
# node_G = Node("G")
# node_H = Node("H")
# node_I = Node("I")

# # 생성한 노드 인스턴스들 연결
# node_F.left_child = node_B
# node_F.right_child = node_G

# node_B.left_child = node_A
# node_B.right_child = node_D

# node_D.left_child = node_C
# node_D.right_child = node_E

# node_G.right_child = node_I

# node_I.left_child = node_H

# # 노드 F를 root 노드로 만든다
# root_node = node_F

# # 만들어 놓은 트리를 in-order로 순회한다
# traverse_inorder(root_node)




###이진 탐색 트리-Complete binary tree에 해당되지 않으므로 리스트가 아닌 노드로 다룸

class Node:
    """이진 탐색 트리 노드 클래스"""
    def __init__(self, data):
        self.data = data
        self.parent = None
        self.right_child = None
        self.left_child = None


def print_inorder(node):
    """주어진 노드를 in-order로 출력해주는 함수"""
    if node is not None:
        print_inorder(node.left_child)
        print(node.data)
        print_inorder(node.right_child)


class BinarySearchTree:
    """이진 탐색 트리 클래스"""
    def __init__(self):
        self.root_node = None


    def insert(self, data):
        new_node = Node(data)
        if self.root_node is None:
            self.root_node = new_node
            return
        else:
            iterator = self.root_node
            iterator_just_before = None
            while iterator is not None:
                if new_node.data > iterator.data:
                    iterator_just_before = iterator
                    iterator = iterator.right_child
                else:
                    iterator_just_before = iterator
                    iterator = iterator.left_child

            if iterator_just_before.data < new_node.data:
                iterator_just_before.right_child = new_node
                new_node.parent = iterator_just_before
            else:
                iterator_just_before.left_child = new_node
                new_node.parent = iterator_just_before

        # 코드를 쓰세요

    def search(self, data):
        iterator = self.root_node
        while iterator is not None:
            if iterator.data > data:
                iterator = iterator.right_child
            elif iterator.data < data:
                iterator = iterator.left_child
            elif iterator.data == data:
                return iterator.data
        else:
            return None


    def print_sorted_tree(self):
        """이진 탐색 트리 내의 데이터를 정렬된 순서로 출력해주는 메소드"""
        print_inorder(self.root_node)  # root 노드를 in-order로 출력한다


# 빈 이진 탐색 트리 생성
bst = BinarySearchTree()

# 데이터 삽입
bst.insert(7)
bst.insert(11)
bst.insert(9)
bst.insert(17)
bst.insert(8)
bst.insert(5)
bst.insert(19)
bst.insert(3)
bst.insert(2)
bst.insert(4)
bst.insert(14)

# 이진 탐색 트리 출력
bst.print_sorted_tree()