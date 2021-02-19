class StationNode:
    """지하철의 이름을 저장하는 노드"""
    def __init__(self, station_name):
        self.station_name = station_name

def create_station_nodes(input_file):
    """외부 파일을 받아서 지하철 노드를 만든 후, 딕셔너리에 저장하는 함수"""
    stations= {}
    with open(input_file, encoding="UTF-8") as stations_raw_file:
        for line in stations_raw_file:
            subway_line = line.strip().split("-")

            for name in subway_line:
                station_name = name.strip()
                stations[station_name] = StationNode(station_name)
    return stations

#파일 경로
stations = create_station_nodes("C:/Users/공성식/Desktop/WORKSTATION/Python Workplace/codeit/datascience_machinelearning_codeit/station_line.txt")

#ㄱㄴㄷ순 지하철역이름 정렬 후 출력
for station in sorted(stations.keys()):
    print(stations[station].station_name)