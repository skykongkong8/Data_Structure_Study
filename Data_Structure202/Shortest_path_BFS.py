"""BFS를 활용한 최단 경로 알고리즘 - 비가중치 그래프 구조에만 해당"""
from collections import deque

class StationNode:
    def __init__(self, station_name):
        self.station_name = station_name
        self.adjacent_stations = []
        self.visited = False
        self.predecessor = None
    
    #비가중치, 무방향
    def add_connection(self, other_station):
        self.adjacent_stations.append(other_station)
        other_station.adjacent_stations.append(self)
    
def create_station_graph(input_file):
    stations = {}

    with open(input_file, encoding="UTF-8") as stations_raw_file:
        for line in stations_raw_file:
            previous_station = None
            raw_data = line.strip().split("-")

            for name in raw_data:
                station_name = name.strip()

                if station_name not in stations:
                    current_station = StationNode(station_name)
                    stations[station_name] = current_station
                else:
                    current_station = stations[station_name]

                if previous_station is not None:
                    current_station.add_connection(previous_station)

                previous_station = current_station #이중 반복문이 아니고서도 이런 방식으로 이웃한 두 자료끼리 이동하며 상호작용하게 할 수 있다!
    return stations

#start_node가 최단경로의 목적지가 되는 노드! 목적지를 바꾸고 싶다면 그때마다 다른 노드로 bfs하면 된다!
def bfs(graph, start_node):
    queue = deque()
    start_node.visited = True
    queue.append(start_node)

    while len(queue) > 0:
        current_node = queue.popleft()
        for node in current_node.adjacent_stations:
            if node.visited is False:
                node.predecessor = current_node
                node.visited = True
                queue.append(node)

def backtrack(node):
    cnt = 0
    res_str = ""
    iterator = node

    """주의! iterator.predecessor is not None 이라고 하면, predecessor를 None으로 갖는 start_node는 반복문 연산에 해당이 안되어 빠진다!"""
    #헷갈리지 말기ㅎ
    while iterator is not None:
        cnt +=1
        # res_str += f" {iterator.station_name}"
        res_str = f"{iterator.station_name} {res_str}"
        iterator = iterator.predecessor
    return res_str+f", 최단 경로는 {cnt}입니다."

stations = create_station_graph("C:/Users/공성식/Desktop/WORKSTATION/Python Workplace/codeit/datascience_machinelearning_codeit/station_line.txt")  # stations.txt 파일로 그래프를 만든다

bfs(stations, stations["을지로3가"])  # 지하철 그래프에서 을지로3가역을 시작 노드로 bfs 실행
print(backtrack(stations["강동구청"]))  # 을지로3가에서 강동구청역까지 최단 경로 출력