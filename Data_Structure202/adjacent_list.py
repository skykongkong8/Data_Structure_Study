from StationNode import*
def create_subway_graph(input_file):
    """외부 파일을 받아 딕셔너리 형태로 노드를 저장하는 함수"""
    stations = {}

    with open(input_file, encoding = "UTF-8") as stations_raw_file:
        for line in stations_raw_file:
            previous_station = None
            subway_line = line.strip().split("-") #띄어쓰기를 제하고, "-"를 기준으로 나누어 리스트에 저장

            for name in subway_line:
                station_name = name.strip()
                
                """지하철역명이 호선별로 쭉 나열되어 있는 데이터이다. 따라서 중복되는 역이 생기지 않도록 체크해주어야 한다."""
                #없다면, 추가해주고 전진한다.
                if station_name not in stations:
                    current_station = StationNode(station_name)
                    stations[station_name] = current_station
                
                #있다면, 한칸 전진만한다.
                else:
                    current_station = stations[station_name]
                
                #첫 번째 역은 이전역이 없으므로 제외하고, 서로 이웃한 역들을 넣어준다. 이러면 환승역도 다 포함해서 넣어줄 수 있게된다.(사실 데이터가 잘 정렬되어 있는 빨이다.)
                if previous_station is not None:
                    current_station.add_components_nondirection(previous_station)

                #이전 역 역시 전진시킨다.
                previous_station = current_station
    return stations

#파일 경로
stations = create_subway_graph("./station_line.txt")

for station in sorted(stations.keys()):
    print(f"{stations[station].station_name} : {' '.join([i.station_name for i in stations[station].adjacent_stations])}")
