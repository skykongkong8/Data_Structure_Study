class StationNode:
    def __init__(self, station_name):
        self.station_name = station_name
        self.adjacent_stations = []

    #1 무방향 그래프
    def add_components_nondirection(self, other_station):
        self.adjacent_stations.append(other_station)
        other_station.adjacent_stations.append(self)
    
    #2 방향 그래프
    def add_components_direction(self, other_station):
        self.adjacent_stations.append(other_station)

    #3 가중치 그래프 - weight이 정의되지 않은 관계로 주석처리하였음.
    # def add_components_weight(self, other_station):
    #     self.adjacent_stations.append((other_station, weight))
    #     self.other_station.adjacent_stations.append((self, weight))

K = {"A" : (1,0), "B" : (2,1)}

for i in K.values():
    print(i)