//DKU 32183376 이원규
#pragma once
#include "B_Tree_Node.h"

class B_Tree_Node;

class B_Tree
{
private:
	int degree;
	B_Tree_Node* root;
	B_Tree_Node* find_B_Tree_node_by_key(int key);
	void remove_leaf_node(B_Tree_Node* leaf_node, int key);
	void merge(B_Tree_Node* now_node);	//now_node 를 기준으로 좌우 형제 B_Tree_Node와 부모 key_node들과 merge
	void merge_at_left_brother(B_Tree_Node* now_node, B_Tree_Node* left_node);	//now_node에 저장된 메모리는 함수 종료시 삭제됨, left_node 로 merge
	B_Tree_Node* get_right_child_min_B_Tree_node(B_Tree_Node* sub_root, int key);
	B_Tree_Node* get_left_child_max_B_Tree_node(B_Tree_Node* sub_root, int key);
	void rotate_left_to_right(B_Tree_Node* left, B_Tree_Node* right, B_Tree_Node* parent);
	void rotate_right_to_left(B_Tree_Node* left, B_Tree_Node* right, B_Tree_Node* parent);
	void Travel(B_Tree_Node* now_root, int level);
public:
	B_Tree(int degree);
	bool add(int key);
	bool find(int key);
	bool remove(int key);
	bool is_empty();
	void Travel();
};

