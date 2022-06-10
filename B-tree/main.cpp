#include <iostream>
#include "B_Tree.h"
#include <crtdbg.h>
#include <time.h>
using namespace std;

/*****************************
*   2022 / 6 / 10
*   DKU 32183376 이원규
*   wonk0424@gmail.com
*   B-Tree 구현
*   중복된 키는 들어가지 않음
*   차수가 D 인 B-tree 를 만들기 위해선 
*   B_Tree btree(D) 로 선언
*   Minimum degree t = 5, 7, 9 일때 시간측정 과제
*   Minimum degree => t'- 1 => 5, 7, 9
*   D = 2t'-1 => 11, 13, 15
*   D는 반드시 홀수로 사용
*   travel() -> 왼쪽 자식으로 이동하며 전위 순회
*       출력형태
*           Level: (현재 탐색 중인 레벨)
*           (현재 노드에 있는 key들)
*   add(i) -> 키 i 를 삽입, 성공시 true, 실패시 false 반환
*   remove(i) -> 키 i 를 삭제, 성공시 true, 실패시 false 반환
*   find(i) -> 키 i 가 존재하는지 확인, 성공시 true, 실패시 false 반환
*******************************/
int main(void)
{
    clock_t start, end;
    start = clock();
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);   //메모리 누수 확인용
    B_Tree btree(15);
    for (int i = 1; i <= 1000; ++i) {
        btree.add(i);
    }
    int cnt(0);
    for (int i = 500; i >= 1; --i) {
        cnt+=btree.remove(i);
    }
    end = clock();
    cout << "Total remove Count: " << cnt << endl;
    cout << " remove end\n";
    btree.Travel();
    cout << "time: " << (end-start)<<"ms";
    return 0;
}