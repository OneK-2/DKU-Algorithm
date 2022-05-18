/*
*	DKU Algorithm HW#3
*	    Student id : 32183376
*	    Student name : Wonkyu-Lee
*       Date: 2022.05.18
*   dijkstra.cpp
*/
#include <iostream>
#include <vector>
#include <queue>

#define INF 9999999

using namespace std;

vector<int> dijkstra(int start, int vertex, vector<pair<int, int>> arr[]) {
	vector<int> dist(vertex, INF);    // 모두 INF로 초기화 
	priority_queue<pair<int, int> > pq;

	dist[start] = 0;
	pq.push(make_pair(0, start));    // 시작 노드

	while (!pq.empty()) {
		int cost = -pq.top().first;    // 방문한 노드의 dist 값 
		int cur = pq.top().second;    // 현재 위치한 노드 
		pq.pop();

		for (int i = 0; i < arr[cur].size(); i++) {    // 현재 위치한 노드의 주변 노드 모두 조사 
			int next = arr[cur][i].first;    // 조사할 다음 노드 
			int nCost = cost + arr[cur][i].second;    // 현재 위치한 노드를 거쳐 다음 노드로 갈때의 비용 
			if (nCost < dist[next]) {     // 최소비용 찾기
				dist[next] = nCost;
				pq.push(make_pair(-nCost, next));    
			}
		}
	}
	return dist;
}
int main()
{
	int vertex, edge;
	vector<pair<int, int>> arr[100];    //최대 노드 개수 100으로 설정
	cout << "노드의 개수 입력 : ";
	cin >> vertex;
	cout << "엣지의 개수 입력 : ";
	cin >> edge;

	for (int i = 0; i < edge; i++) {
		int from, to, cost;
		cout << "그래프 입력 [노드 노드 가중치]: ";
		cin >> from >> to >> cost;
		arr[from].push_back(make_pair(to, cost));    
		arr[to].push_back(make_pair(from, cost));
	}

	printf("\n===다익스트라 결과===\n");
	vector<int> dist = dijkstra(0, vertex, arr);
	for (int i = 0; i < vertex; i++) {
		printf("0번 정점에서 %d번 정점까지 최단거리 : %d\n", i, dist[i]);
	}
	return 0;
}