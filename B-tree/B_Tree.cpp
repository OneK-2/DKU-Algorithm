//DKU 32183376 이원규
#include "B_Tree.h"
#include <cassert>
#include <iostream>
using namespace std;

bool B_Tree::add(int key) {
	if (find(key)) {
		return false;
	}
	if (is_empty()) {
		root = new B_Tree_Node(nullptr);
	}
	B_Tree_Node* inserted_node = root;
	while (inserted_node->is_leaf_node() == false) {
		inserted_node = inserted_node->get_child_or_nullptr(key);
	}
	bool insert_success=inserted_node->add(key);
	if (insert_success == false) {
		return false;
	}
	B_Tree_Node* now_node = inserted_node;
	while (now_node->get_key_count() >= degree&& now_node != root) {
		//삽입된 B_Tree_Node의 용량 초과
		B_Tree_Node* now_node_parent = now_node->get_parent_or_nullptr();
		//노드 분할
		now_node->split_node();
		//원본 노드는 사용되지 않음
		delete now_node;
		now_node = now_node_parent;
	}
	if (root->get_key_count() >= degree) {
		//root 분할
		B_Tree_Node* del_root = root;
		root = root->split();
		delete del_root;
	}
	return true;
}

bool B_Tree::find(int key) {
	if (is_empty()) {
		return false;
	}
	B_Tree_Node* travel_node = root;
	while (travel_node != nullptr && travel_node->nodes.find(key) == travel_node->nodes.end()) {
		travel_node = travel_node->get_child_or_nullptr(key);
	}
	return travel_node != nullptr;
}

void B_Tree::remove_leaf_node(B_Tree_Node* leaf_node, int key) {
	assert(leaf_node != nullptr);
	if (leaf_node == root) {
		node* del_node = root->nodes[key];
		root->nodes.erase(key);
		delete del_node;
		if (root->get_key_count() == 0) {
			delete root;
			root = nullptr;
		}
	}
	else {
		auto key_node_iter = leaf_node->nodes.find(key);
		if (leaf_node->is_leaf_node() && key_node_iter != leaf_node->nodes.end()) {
			node* key_node = key_node_iter->second;
			//데이터 삭제
			leaf_node->nodes.erase(key);
			delete key_node;
			int node_cnt = leaf_node->get_key_count();
			//case 1. leaf_node의 key 개수가 최소 개수이상 많을 때
			if (node_cnt >= degree / 2) {
				return;
			}
			else {
				//case 2 최소개수일때

				//case 2.1 왼쪽이나 오른쪽 형제에 최소개수 보다 많은 경우
				B_Tree_Node* left_brother = leaf_node->get_left_brother_or_nullptr();
				B_Tree_Node* right_brother = leaf_node->get_right_brother_or_nullptr();
				B_Tree_Node* parent_node = leaf_node->get_parent_or_nullptr();
				if (left_brother != nullptr && left_brother->get_key_count() > degree / 2) {
					//왼쪽 형제 노드의 키의 개수가 최소 개수 초과일때
					//왼쪽 형제에서 노드 가져옴
					rotate_left_to_right(left_brother, leaf_node, parent_node);
					return;
				}
				else if (right_brother != nullptr && right_brother->get_key_count() > degree / 2) {
					//오른쪽 형제 노드의 키의 개수가 최소개수 초과일때
					rotate_right_to_left(leaf_node, right_brother, parent_node);
					return;
				}
				else if (parent_node != nullptr && parent_node->get_key_count() > degree / 2) {
					//부모 노드의 키의 개수가 최소개수 초과일때
					//병합
					//왼쪽 형제로 merge
					if (left_brother == nullptr) {
						merge_at_left_brother(right_brother, leaf_node);
						//왼쪽 형제로 merge 불가능
						//오른쪽 형제를 현재 노드에merge
					}
					else {
						merge_at_left_brother(leaf_node, left_brother);
					}
				}
				else {
					//부모, 형제 노드 모두 최소 개수
					merge(leaf_node);
				}
			}
		}
	}
}

void B_Tree::merge(B_Tree_Node* now_node) {
	//now_node에서 merge 작업
	B_Tree_Node* parent = now_node->get_parent_or_nullptr();
	B_Tree_Node* left_brother = now_node->get_left_brother_or_nullptr();
	B_Tree_Node* right_brother = now_node->get_right_brother_or_nullptr();
	//왼쪽으로 merge 가능
	if (left_brother != nullptr && left_brother->nodes.size() > degree / 2) {
		//왼쪽 형제 노드가 최소개수보다 많으면 왼쪽에서 끌어와도 무방
		rotate_left_to_right(left_brother, now_node, parent);
	}
	else  if (right_brother != nullptr && right_brother->get_key_count() > degree / 2) {
		//반대 경우
		rotate_right_to_left(now_node, right_brother, parent);
	}
	else if(left_brother!=nullptr){
		//끌어올 수 없다면 병합
		merge_at_left_brother(now_node, left_brother);
		if (left_brother->nodes.size() >= degree) {
			left_brother->split_node();
			delete left_brother;
		}
		if (parent != root && parent->get_key_count() < degree / 2) {
			//병합을 했는데 부모에 있는 key의 개수가 최소 개수보다 적음
			merge(parent);
		}
		else if (parent == root && root->get_key_count() == 0) {
			//병합을 했는데 루트가 비게된 경우
			delete root;	//기존 루트 삭제
			root = left_brother;
			for (auto& child : root->childs) {
				child.second->parent = root;
			}
			root->parent = nullptr;
		}
	}
	else if(right_brother!=nullptr) {
		merge_at_left_brother(right_brother, now_node);
		if (now_node->get_key_count() > degree ) {
			now_node->split_node();
			delete now_node;
		}
		else if (parent != root && parent->get_key_count() < degree / 2) {
			merge(parent);
		}
		else if (parent == root && root->get_key_count() == 0) {
			delete root;
			root = now_node;
			for (auto& child : root->childs) {
				child.second->parent = root;
			}
			root->parent = nullptr;
		}
	}
}

bool B_Tree::remove(int key) {
	B_Tree_Node* finded_node = find_B_Tree_node_by_key(key);
	if (finded_node==nullptr) {
		return false;
	}
	if (finded_node->is_leaf_node()) {
		//리프노드를 삭제하면 따로 처리
		remove_leaf_node(finded_node,key);
	}
	else {
		//리프노드가 아닌  노드 삭제
		B_Tree_Node* left_child = finded_node->get_left_child_or_nullptr(key);
		B_Tree_Node* right_child = finded_node->get_right_child_or_nullptr(key);
		node* del_node = finded_node->nodes[key];
		delete del_node;
		if (left_child != nullptr ) {
			//왼쪽 서브 트리에서 가장 큰 키 값을  가진 것과 키 교환
			B_Tree_Node* left_child_max = get_left_child_max_B_Tree_node(left_child, key);
			int max_key = left_child_max->nodes.rbegin()->first;
			node* max_node = left_child_max->nodes[max_key];
			finded_node->nodes[max_key] = max_node;
			finded_node->childs[max_key] = finded_node->childs[key];
			finded_node->remove(key);
			left_child_max->remove(max_key);
			if (left_child_max->get_key_count() < degree / 2) {
				merge(left_child_max);
			}
		}
		else if (right_child != nullptr) {
			//오른쪽 서브 트리에서 가장 작은 키 값과 교환
			B_Tree_Node* right_child_min = get_right_child_min_B_Tree_node(right_child,key);
			int min_key = right_child_min->nodes.begin()->first;
			node* min_node = right_child_min->nodes[min_key];
			finded_node->nodes[min_key] = min_node;
			finded_node->childs[min_key] = finded_node->childs[key];
			finded_node->remove(key);
			right_child_min->nodes.erase(min_key); 
			if (right_child_min->get_key_count() < degree / 2) {
				merge(right_child_min);
			}
		}
	}
	return true;
}

void B_Tree::merge_at_left_brother(B_Tree_Node* now_node, B_Tree_Node* left_node) {
	int parent_key = left_node->get_parent_key_with_right_brother(now_node);
	assert(now_node->parent == left_node->parent);
	bool is_leaf_node_merge = now_node->is_leaf_node();
	B_Tree_Node* parent = now_node->parent;
	//부모 key 삽입
	auto parent_node = parent->nodes.find(parent_key);
	left_node->nodes.insert(*parent_node);
	//now_node 에 있는 것들을 left_node에 옮김
	auto now_node_iter = now_node->nodes.begin();
	while (now_node_iter!=now_node->nodes.end()){
		left_node->nodes.insert(*now_node_iter);
		++now_node_iter;
	}
	parent->remove(parent_key);
	if (is_leaf_node_merge == false) {
		auto now_childs_iter = now_node->childs.begin();
		left_node->childs[parent_key] = now_childs_iter->second;
		now_childs_iter->second->parent = left_node;
		while (++now_childs_iter != now_node->childs.end()) {
			left_node->childs.insert(*now_childs_iter);
			now_childs_iter->second->parent = left_node;
		}
	}
	delete now_node;
}
B_Tree_Node* B_Tree::find_B_Tree_node_by_key(int key) {
	B_Tree_Node* travel_node = root;
	while (travel_node != nullptr && travel_node->nodes.find(key) == travel_node->nodes.end()) {
		travel_node = travel_node->get_child_or_nullptr(key);
	}
	return travel_node;
}

bool B_Tree::is_empty() {
	return root == nullptr;
}

B_Tree_Node* B_Tree::get_left_child_max_B_Tree_node(B_Tree_Node* sub_root, int key) {
	B_Tree_Node* travel_node = sub_root;
	while (travel_node->is_leaf_node() == false) {
		travel_node = travel_node->childs.rbegin()->second;
	}
	return travel_node;
}

B_Tree_Node* B_Tree::get_right_child_min_B_Tree_node(B_Tree_Node* sub_root, int key) {
	B_Tree_Node* travel_node = sub_root;
	while (travel_node->is_leaf_node() == false) {
		travel_node = travel_node->childs.begin()->second;
	}
	return travel_node;
}

void B_Tree::rotate_left_to_right(B_Tree_Node* left, B_Tree_Node* right, B_Tree_Node* parent) {
	int parent_key = left->get_parent_key_with_right_brother(right);
	assert(parent_key != -1);
	node* parent_node = parent->nodes[parent_key];
	int left_max_key = left->nodes.rbegin()->first;
	node* left_max_node = left->nodes.rbegin()->second;
	
	right->nodes[parent_key] = parent_node;
	parent->nodes[left_max_key] = left_max_node;
	if (left->is_leaf_node() == false && right->is_leaf_node() == false) {
		B_Tree_Node* left_max_Tree_node = left->childs.rbegin()->second;
		B_Tree_Node* right_min_tree_nonde = right->childs[-1];
		right->childs[parent_key] = right_min_tree_nonde;
		right->childs[-1] = left_max_Tree_node;
		left_max_Tree_node->parent = right;
		parent->childs[left_max_key] = parent->childs[parent_key];
	}
	left->remove(left_max_key);
	parent->remove(parent_key);
}

void B_Tree::rotate_right_to_left(B_Tree_Node* left, B_Tree_Node* right, B_Tree_Node* parent) {
	int parent_key = left->get_parent_key_with_right_brother(right);
	assert(parent_key != -1);
	node* parent_node = parent->nodes[parent_key];
	node* right_min_node = right->nodes.begin()->second;
	int right_min_key = right_min_node->get_key();
	left->nodes[parent_key] = parent_node;
	parent->nodes[right_min_key] = right_min_node;
	parent->childs[right_min_key] = right;
	if (left->is_leaf_node() == false && right->is_leaf_node()==false) {
		left->childs[parent_key] = right->childs[-1];
		left->childs[parent_key]->parent = left;
		right->childs[-1] = right->childs[right_min_key];
	}
	right->remove(right_min_key);
	parent->remove(parent_key);

}

void B_Tree::Travel() {
	if (root != nullptr) {
		Travel(root, 0);
	}
}

void B_Tree::Travel(B_Tree_Node* now_root, int level) {
	cout << "level: "<<level << "\n";
	for (auto node : now_root->nodes) {
		cout << node.first << " ";
	}
	cout << endl;
	if (now_root->is_leaf_node()) {
		cout << "Leaf Node\n";
	}
	else {
		for (auto child : now_root->childs) {
			Travel(child.second, level + 1);
		}
	}
}

B_Tree::B_Tree(int degree) {
	this->degree = degree;
	root = nullptr;
}