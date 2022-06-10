//DKU 32183376 이원규
#include "B_Tree_Node.h"
#include  <cassert>
#include <iostream>
using namespace std;
B_Tree_Node::B_Tree_Node(B_Tree_Node* parent) {
	this->parent = parent;
}

bool B_Tree_Node::add(int key) {
	nodes[key] = new node(key);
	return true;
}

bool B_Tree_Node::is_leaf_node() {
	return childs.size() == 0;
}

B_Tree_Node* B_Tree_Node::get_child_or_nullptr(int key) {
	//key 가 있는 자식 노드를 반환
	if (is_leaf_node()) {
		return nullptr;
	}
	auto child = childs.lower_bound(key);
	if (child != childs.begin()) {
		--child;
	}
	return child->second;
}

int B_Tree_Node::get_key_count() {
	return nodes.size();
}

void B_Tree_Node::split_node() {
	B_Tree_Node* splited_node = split();
	if (parent != nullptr) {
		parent->insert(splited_node);
	}
	delete splited_node;
}

B_Tree_Node* B_Tree_Node::split() {
	B_Tree_Node* splited=new B_Tree_Node(nullptr);
	int mid_key;
	int degree = nodes.size() ;	//분할이 일어나는 순간은 nodes가 최대차수를 넘겼을 때	3차 트리면 -> 3개가 되는 순간이 최대 이므로 degree=2
	int i = 0;
	auto iter = begin(nodes);
	B_Tree_Node* left=splited->childs[-1] = new B_Tree_Node(splited);
	left->parent = splited;
	while (i < degree / 2) {
		left->nodes[iter->first] = iter->second;
		++iter;
		++i;
	}
	mid_key = iter->first;
	splited->nodes[mid_key] = iter->second;
	B_Tree_Node* right = splited->childs[mid_key] = new B_Tree_Node(splited);
	while (++iter != end(nodes)) {
		right->nodes[iter->first] = iter->second;
	}
	if (is_leaf_node()==false) {
		i = 0;
		auto child_iter = begin(childs);
		while (i <= degree / 2) {
			left->childs.insert(*child_iter);
			child_iter->second->parent = left;
			++child_iter;
			++i;
		}
		right->childs[-1] = child_iter->second;
		right->childs[-1]->parent = right;
		while (++child_iter != end(childs)) {
			right->childs.insert(*child_iter);
			child_iter->second->parent = right;
		}
	}
	return splited;
}

void B_Tree_Node::insert(B_Tree_Node* tree_node) {
	tree_node->parent = this->parent;
	int insert_node_key = tree_node->nodes.rbegin()->first;
	auto iter=this->childs.lower_bound(insert_node_key);
	if (iter != begin(childs)) {
		--iter;
	}
	iter->second = tree_node->childs.begin()->second;
	for (auto iter : tree_node->childs) {
		iter.second->parent = this;
	}
	nodes[insert_node_key] = tree_node->nodes[insert_node_key];
	this->childs[insert_node_key] = tree_node->childs.rbegin()->second;
}

B_Tree_Node* B_Tree_Node::get_left_brother_or_nullptr() {
	B_Tree_Node* parent = this->parent;
	if (parent == nullptr) {
		return nullptr;
	}
	for (auto iter = parent->childs.begin(); iter != end(parent->childs); ++iter) {
		if (iter->second == this) {
			if (iter == begin(parent->childs)) {
				return nullptr;
			}
			--iter;
			return iter->second;
		}
	}
	return nullptr;
}

B_Tree_Node* B_Tree_Node::get_right_brother_or_nullptr() {
	B_Tree_Node* parent = this->parent;
	if (parent == nullptr) {
		return nullptr;
	}
	for (auto iter = begin(parent->childs); iter != end(parent->childs); ++iter) {
		if (iter->second == this) {
			++iter;
			if (iter == end(parent->childs)) {
				return nullptr;
			}
			return iter->second;
		}
	}
	return nullptr;
}

int B_Tree_Node::get_parent_key_with_left_brother(B_Tree_Node* left_near) {
	assert(left_near != nullptr);
	B_Tree_Node* parent = left_near->parent;
	if (parent == nullptr) {
		return -1;
	}
	for (auto iter = begin(parent->childs); iter != end(parent->childs); ++iter) {
		if (iter->second == left_near) {
			++iter;
			return iter->first;
		}
	}
	return -1;
}

int B_Tree_Node::get_parent_key_with_right_brother(B_Tree_Node* rigth_near) {
	assert(rigth_near != nullptr);
	B_Tree_Node* parent = rigth_near->parent;
	if (parent == nullptr) {
		return -1;
	}
	for (auto iter = begin(parent->childs); iter != end(parent->childs); ++iter) {
		if (iter->second == rigth_near) {
			return iter->first;
		}
	}
	return -1;
}

B_Tree_Node* B_Tree_Node::get_left_child_or_nullptr(int key) {
	auto iter = childs.lower_bound(key);
	if (iter != end(childs) && iter->first == key) {
		--iter;
		return iter->second;
	}
	else {
		return nullptr;
	}
}

B_Tree_Node* B_Tree_Node::get_right_child_or_nullptr(int key) {
	auto iter = childs.lower_bound(key);
	if (iter != end(childs) && iter->first == key) {
		return iter->second;
	}
	else {
		return nullptr;
	}
}

B_Tree_Node* B_Tree_Node::get_parent_or_nullptr() {
	return this->parent;
}

void B_Tree_Node::remove(int key) {
	childs.erase(key);
	nodes.erase(key);
}