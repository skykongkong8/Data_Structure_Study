"""가중치 그래프 구조의 최단 경로 알고리즘""" 
#간단한 가중치 그래프 자료를 만들어서 실험해보자.

class StationNode:
    def __init__(self, station_name):
        self.station_name = station_name
        self.distance = float('inf')
        self.predecessor = None
        self.complete = False
        self.adjacent_stations = {}
    
    # 가중치, 무방향
    def add_connection(self, node, weight):
        self.adjacent_stations[f"{node.station_name}"] = (node, weight)
        node.adjacent_stations[f"{self.station_name}"] = (self, weight)

# def create_station_graph(input_file):
#원래는 이렇게 파일을 받아오는 거지만 임의로 다섯 개의 역을 만들어보자

#클래스 객체 생성
stations = {}
A = StationNode("A");B = StationNode("B");C = StationNode("C");D = StationNode("D");E = StationNode("E")
stations["A"] = A
stations["B"] = B
stations["C"] = C
stations["D"] = D
stations["E"] = E

#가중치 그래프 구조 형성-노가다
A.add_connection(B, 4)
A.add_connection(C, 1)
B.add_connection(C, 2)
B.add_connection(D, 2)
B.add_connection(E, 3)
C.add_connection(D, 1)
D.add_connection(E, 3)

#모두다 complete 상태인지 아닌지 검사하는 함수 check_complete
def check_complete(graph):
    for node in graph.values():
        if node.complete == False:
            return False
    else:
        return True

def pick_node(graph):
    tmp = StationNode('tmp')
    for node in graph.values():
        if node.complete == False:
            if tmp.distance >= node.distance:
                tmp = node
    return tmp            


def dijkstra(graph, start_node):
    """최단 경로 distance를 아예 정해놓는 dijkstra 알고리즘"""
    start_node.distance = 0
    start_node.complete = True
    current_node = start_node

    while True:
        for info in current_node.adjacent_stations.values():
            if info[0].complete is False:
                current_distance = current_node.distance + info[1]
                if info[0].distance > current_distance:
                    info[0].distance = current_distance
                    info[0].predecessor = current_node
        
        current_node.complete = True
        if not check_complete(graph):
            current_node = pick_node(graph)
        else:
            break

def backtracking(node):
    """Dijkstra는 거리를 바로 알 수 있지만, 경로 자체를 보고 싶다면 백트래킹이 필요하다"""
    iterator = node
    res_str = ""
    while iterator is not None:
        res_str = f"{iterator.station_name} {res_str}"
        iterator = iterator.predecessor
    return res_str

"""작동 부분"""
dijkstra(stations, stations["A"])

#1 Dijkstra 확인
print(stations["E"].distance)
#2 백트래킹 확인
print(backtracking(stations["E"]))