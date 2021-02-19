from collections import deque
class StationNode:
    """지하철 역을 나타내는 역"""
    def __init__(self, station_name):
        self.station_name = station_name
        self.adjacent_stations = []
        self.visited = 0 #0 : 방문한 적 없음 1: 스택에 있음 2: 방문함

    def add_connection(self, station):
        """파라미터로 받은 역과 엣지를 만들어주는 메소드""" 
        self.adjacent_stations.append(station)
        station.adjacent_stations.append(self)
def create_station_graph(input_file):
    stations = {}

    # 일반 텍스트 파일을 여는 코드
    with open(input_file, encoding = "UTF-8") as stations_raw_file:
        for line in stations_raw_file:  # 파일을 한 줄씩 받아온다
            previous_station = None  # 엣지를 저장하기 위한 변수
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

                previous_station = current_station

    return stations

def dfs(graph, start_node):
    stack = deque()

    start_node.visited = 1
    stack.append(start_node)

    while len(stack)>0:
        present_node = stack.pop()
        present_node.visited = 2

        for node in present_node.adjacent_stations:
            if node.visited == 0:
                stack.append(node)
                node.visited = 1
            #없어도 되지만 형식상 넣은 부분
            # else:
            #     pass

#파일 경로
stations = create_station_graph("./station_line.txt")

gangnam_station = stations["강남"]

# 강남역과 경로를 통해 연결된 모든 노드를 탐색
dfs(stations, gangnam_station)

# 강남역과 서울 지하철 역들 연결됐는지 확인
print(stations["강동구청"].visited)
print(stations["평촌"].visited)
print(stations["송도"].visited)
print(stations["개화산"].visited)

# 강남역과 대전 지하철 역들 연결됐는지 확인
print(stations["반석"].visited)
print(stations["지족"].visited)
print(stations["노은"].visited)
print(stations["(대전)신흥"].visited)

