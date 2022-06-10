//DKU 32183376 이원규
#include "node.h"

node::node(int key) {
	this->key = key;
}

int node::get_key() {
	return this->key;
}