import numpy as np
class Friend:
    """이름을 저장하는 노드 Friends"""
    def __init__(self, name):
        self.name = name
        self.adj_friend = []

#각 객체를 리스트에 저장 - 객체마다 고유 인덱스가 부여된다.
Friends = [Friend("영훈"), Friend("현승"), Friend("동욱"), Friend("지웅"), Friend("규리"), Friend("소원")]

"""연결 관계"""
Friends[0].adj_friend.extend([Friends[1], Friends[2]])
Friends[1].adj_friend.extend([Friends[0], Friends[3], Friends[5]])
Friends[2].adj_friend.extend([Friends[0], Friends[5]])
Friends[3].adj_friend.extend([Friends[1], Friends[5], Friends[4]])
Friends[4].adj_friend.extend([Friends[3], Friends[5]])
Friends[5].adj_friend.extend([Friends[1], Friends[2], Friends[3], Friends[4]])

#친구 관계 연결
adj_matrix = np.zeros((6,6), dtype=np.int8) #팁- dtype을 통해 float(0.0)에서 int(0)로 바꿔줄수있다.

"""의도와 다르게 방향 그래프 그래프 구조가 되었다만, 결과 자체는 같다!"""
# for i in range(len(Friends)):
#     for j in range(len(Friends)):
#         if Friends[j] in Friends[i].adj_friend and Friends[i] in Friends[j].adj_friend:
#             adj_matrix[i][j] = 1
#             adj_matrix[j][i] = 1

for i in range(len(Friends)):
    for j in range(len(Friends)):
        if Friends[j] in Friends[i].adj_friend:
            adj_matrix[i][j] = 1

print(adj_matrix)