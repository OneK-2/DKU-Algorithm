//DKU 32183376 이원규
#pragma once
#include "node.h"
#include "B_Tree.h"
#include <map>
using namespace std;


class B_Tree_Node
{
	friend class B_Tree;
private:
	map<int, B_Tree_Node*> childs;	//key 보다 큰 key 값을 가진 자식 노드를 가리키고 key 값을 가진 노드
	map<int, node*> nodes;
	B_Tree_Node* parent;
	B_Tree_Node* split();		//현재 노드를 두개로 분할한 노드 반환
	void insert(B_Tree_Node* tree_node);	//tree 노드를 삽입
	B_Tree_Node* get_left_brother_or_nullptr();
	B_Tree_Node* get_right_brother_or_nullptr();
	int get_parent_key_with_left_brother(B_Tree_Node* left_near);
	int get_parent_key_with_right_brother(B_Tree_Node* right_near);
	B_Tree_Node* get_left_child_or_nullptr(int key);	//현재 노드에서 key값의 왼쪽
	B_Tree_Node* get_right_child_or_nullptr(int key);

public:
	B_Tree_Node(B_Tree_Node* parent);
	int get_key_count();
	bool add(int key);
	bool is_leaf_node();
	B_Tree_Node* get_child_or_nullptr(int key);
	void split_node();
	B_Tree_Node* get_parent_or_nullptr();
	void remove(int key);

};

