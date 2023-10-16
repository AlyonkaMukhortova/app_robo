#pragma once
#include "rbtree.h"
#include <iostream>


/**
 * Template class for map.
 */
template<typename key_t, typename data_t>
class MyMap {
private:
	node_t<key_t, data_t>* tree = nullptr;
	int mysize = 0;
public:
	class iterator;
	//iterator enditer;

	MyMap() {}														//tested

	MyMap(iterator first, iterator last) {							//tested
		this->insert(first, last);
	}

	MyMap(MyMap&& one) {
		tree = one.tree;
		mysize = one.mysize;
	}

	MyMap(const MyMap& one) {										//tested
		for (auto it = one.begin(); it != one.end(); it++) {
			insert(std::make_pair(it.cur->key, it.cur->data));
		}
	}

	MyMap operator=(const MyMap& one) {
		tree->RBTreeDealloc(&tree);
		tree = nullptr;
		for (auto it = one.begin(); it != one.end(); it++) {
			insert(std::make_pair(it.cur->key, it.cur->data));
		}
		return *this;
	}

	MyMap operator=(MyMap&& one) {
		tree->RBTreeDealloc(&tree);
		tree = nullptr;
		tree = one.tree;
		mysize = one.mysize;
		return *this;
	}

	std::pair<iterator, bool> insert(std::pair<key_t, data_t>&& pair) {		//tested
		auto a = std::move(pair);
		node_t<key_t, data_t>* newone = tree->RBTreeInsert(tree, a.first, a.second);
		bool success = newone != nullptr;
		if (success) mysize++;
		return std::make_pair(iterator(newone), success);
	}

	std::pair<iterator, bool> insert(const std::pair<key_t, data_t>& pair) {		//tested
		node_t<key_t, data_t>* newone = tree->RBTreeInsert(tree, pair.first, pair.second);
		bool success = newone != nullptr;
		if (success) mysize++;
		return std::make_pair(iterator(newone), success);
	}


	std::pair<iterator, bool> insert(MyMap& newone) {							//tested
		node_t<key_t, data_t>* node = nullptr, *first = nullptr;
		bool success = 0, maxsuccess = 0;
		for (auto it = newone.begin(); it != newone.end(); it++) {
			node = this->insert(std::make_pair(it.cur->key, it.cur->data)).first.cur;
			if (it == newone.begin() || !maxsuccess) first = node;
			success = !success ? 0 : node;
			if (success) mysize++;
			maxsuccess = maxsuccess ? 1 : node != nullptr;
		}
		return std::make_pair(iterator(first), success);
	}

	std::pair<iterator, bool> insert(iterator& firstone, iterator& last) {		//tested
		node_t<key_t, data_t>* node = nullptr, *first = nullptr;
		bool success = 0, maxsuccess = 0;
		for (auto it = firstone; it != last; it++) {
			node = this->insert(std::make_pair(it.cur->key, it.cur->data)).first.cur;
			if (it == firstone || !maxsuccess) first = node;
			success = !success ? 0 : node != nullptr;
			if (success) mysize++;
			maxsuccess = maxsuccess ? 1 : node != nullptr;
		}
		return std::make_pair(iterator(first), success);

	}

	std::pair<iterator, bool> emplace(key_t&& key, data_t&& data) {				//tested
		auto ptr = tree->RBTreeFind(tree, key);
		if (!ptr) {
			return insert(std::make_pair(key, data));
		}
		return std::pair<iterator, bool>(iterator(ptr), false);
	}

	std::pair<iterator, bool> emplace(std::pair<key_t&&, data_t&&> pair) {		//tested
		auto ptr = tree->RBTreeFind(tree, pair.first);
		if (!ptr) {
			return insert(std::move(pair));
		}
		return std::pair<iterator, bool> (iterator(ptr), false);
	}

	iterator find(key_t&& key) {													//tested
		return iterator(tree->RBTreeFind(tree, std::move(key)));
	}

	iterator find(const key_t& key) {												//tested
		return iterator(tree->RBTreeFind(tree, key));
	}

	iterator erase(iterator pos) {													//tested
		auto node = tree->RBTreeFindNext(pos.cur);
		bool a = tree->RBTreeRemove(tree, pos.cur->key);
		if (a) mysize--;
		return iterator(node);
	}

	iterator erase(iterator first, iterator last) {									//tested
		iterator itres;
		for (auto it = first; it != last; it++) {
			itres = erase(it);
		}
		return itres;
	}

	iterator erase(key_t&& key) {													//tested
		auto pos = find(std::move(key));
		auto pos1 = pos;
		pos1++;
		if (pos == end()) return end();
		erase(pos);
		return pos1;
	}

	iterator erase(const key_t& key) {												//tested
		auto pos = find(key);
		auto pos1 = pos;
		pos1++;
		if (pos == end()) return end();
		erase(pos);
		return pos1;
	}

	iterator begin() const{															//tested
		node_t<key_t, data_t>* node = tree->RBTreeFindMin(tree);
		return iterator(node);
	}

	iterator end() const{															//tested
		return iterator(nullptr);
	}

	data_t& operator[](const key_t& key){											//tested
		return (tree->RBTreeFind(tree, key))->data;
	}

	data_t& operator[](key_t&& key) {												//tested
		if (tree->RBTreeFind(tree, key)) {
			return (tree->RBTreeFind(tree, key))->data;
		}
		else {
			tree->RBTreeInsert(tree, key, data_t());
			mysize++;
			return (tree->RBTreeFind(tree, key))->data;
		}
	}

	int size() { 
		return mysize; 
	}


	class iterator {
	public:
		node_t<key_t, data_t>* cur = nullptr;

		iterator(key_t key, data_t data) : cur(key, data) {
		}
		
		iterator() {}																//tested
		
		iterator(node_t<key_t, data_t>* n) {										//tested
			cur = n;
		}
		
		bool operator==(iterator a) {												//tested
			return this == &a;
		}

		iterator operator--(int i) {												//tested
			*this = iterator(cur->RBTreeFindPrevious(cur));
			return *this;
		}

		iterator operator++(int i) {												//tested
			*this = iterator(cur->RBTreeFindNext(cur));
			return *this;
		}

		iterator operator--() {														//tested
			auto ptr = *this;
			*this = iterator(cur->RBTreeFindPrevious(cur));
			return ptr;
		}						

		iterator operator++() {														//tested
			auto ptr = *this;
			*this = iterator(cur->RBTreeFindNext(cur));
			return ptr;
		}						

		bool operator !=(iterator it) {												//tested
			bool notnull = it.cur && this->cur;
			bool check = notnull ? it.cur->key != this->cur->key : !((!it.cur) && (!this->cur));
			return check;// it.cur->key != this->cur->key;
		}

		node_t<key_t, data_t>* operator*() {										//tested
			return cur;
		}

		node_t<key_t, data_t>** operator->() {
			return &cur;
		}

		void operator=(iterator i) {
			cur = i.cur;
		}
	};
};
