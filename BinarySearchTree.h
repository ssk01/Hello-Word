//bst by ssk, a little copy from lyp


#pragma once
#include <new>
#include <vector>
#include <stack>
#include <algorithm>


template<typename ValueType>
struct TreeNode
{
	ValueType value;
	TreeNode* leftPtr;
	TreeNode* rightPtr;
	TreeNode* parentPtr;
};
template<typename Type>
class BinarySearchTree
{
	
public:
	using value_type = Type;
	using pointer = value_type*;
	using reference = value_type&;
	using tree_type = BinarySearchTree<value_type>;
	using node_type = TreeNode<value_type>;
	using node_pointer = node_type*;
	using container_type = std::vector<value_type>;
	using container_iterator_type =
		typename container_type::const_iterator;
	
public:
	BinarySearchTree() : headPtr(nullptr) { }
	BinarySearchTree(node_pointer p) : headPtr(p) { }
	~BinarySearchTree()
	{
		/*Clear(headPtr);
		headPtr = nullptr;*/
	}
	BinarySearchTree(const tree_type& rhs)
	{
		headPtr = headPtr;
	}
	BinarySearchTree(const tree_type&& rhs)
	{
		headPtr = rhs.headPtr;
		rhs.headPtr = nullptr;
	}
	void swap(const tree_type& rhs)
	{
		std::swap(headPtr, rhs.headPtr);
	}
	//after copy two control one , how to deal with 

	node_pointer find(const value_type& _value)
	{
		auto findPtr = headPtr;
		while (findPtr != nullptr && findPtr->value != _value)
		{
			if (_value < findPtr->value)
				findPtr = findPtr->leftPtr;
			else
				findPtr = findPtr->rightPtr;
		}
		return findPtr;
	}


	node_pointer insert(const value_type& _value)
	{
		if (headPtr == nullptr)
		{
			headPtr = new node_type;
			headPtr->value = _value;
			headPtr->rightPtr = nullptr;
			headPtr->leftPtr = nullptr;
			headPtr->parentPtr = nullptr;
			return headPtr;
		}
		auto findPtr = headPtr;
		auto prePtr = findPtr;
		bool isLeft = false;
		while (findPtr != nullptr)
		{
			if (_value < findPtr->value)
			{
				prePtr = findPtr;
				findPtr = findPtr->leftPtr;
				isLeft = true;
			}
			else if (_value > findPtr->value)
			{
				prePtr = findPtr;
				findPtr = findPtr->rightPtr;
				isLeft = false;
			}
			else
			{
			return findPtr;
			}
		}
		findPtr = new node_type;
		findPtr->parentPtr = prePtr;
		findPtr->value = _value;
		findPtr->leftPtr = nullptr;
		findPtr->rightPtr = nullptr;
		if (isLeft)
		{
			prePtr->leftPtr = findPtr;
		}
		else
		{
			prePtr->rightPtr = findPtr;
		}
		return findPtr;
	}

	void erase(const value_type & _value)
	{
		auto deletePtr = find(_value);
		if (deletePtr == nullptr)
			return;
		if (deletePtr->leftPtr == nullptr)
		{
			transptr(deletePtr, deletePtr->rightPtr);
		}
		else if (deletePtr->rightPtr == nullptr)
		{
			transptr(deletePtr, deletePtr->leftPtr);
		}
		else
		{
			
			// two sons
			if (deletePtr == headPtr)
			{
				headPtr = deletePtr->rightPtr;
				headPtr->leftPtr = deletePtr->leftPtr;

			}
			else
			{
				auto left = deletePtr->leftPtr;
				auto right = deletePtr->rightPtr;
				auto minPtr = getMin(deletePtr->rightPtr);
				if (minPtr == deletePtr->rightPtr)
				{

					transptr(deletePtr, minPtr);
					minPtr->leftPtr = left;
				}
				else if (minPtr->rightPtr == nullptr)
				{
					minPtr->parentPtr->leftPtr = nullptr;
					transptr(deletePtr, minPtr);
					minPtr->rightPtr = right;
					minPtr->leftPtr = left;
				}
				else
				{
					minPtr->parentPtr->leftPtr = minPtr->rightPtr;
					transptr(deletePtr, minPtr);
					minPtr->rightPtr = right;
					minPtr->leftPtr = left;
				}
			}
			//value_type destructor;
			delete deletePtr;
		}
	}

	void insert(std::initializer_list<value_type> iList)
	{
		for (auto && v : iList)
		{
			insert(v);
		}
	}
	void couttt()
	{
		print(headPtr);
	}
	container_type preorderTreeWalk() const
	{
		container_type result;
		if (headPtr != nullptr)
		{
			node_pointer tempPtr;
			std::stack<node_pointer> ptrStack;
			ptrStack.push(headPtr);
			while (!ptrStack.empty())
			{
				tempPtr = ptrStack.top();
				ptrStack.pop();
				result.push_back(tempPtr->value);
				if (tempPtr->rightPtr != nullptr) ptrStack.push(tempPtr->rightPtr);
				if (tempPtr->leftPtr != nullptr) ptrStack.push(tempPtr->leftPtr);
			}
		}
		return result;
	}

	container_type inorderTreeWalk() const
	{
		container_type result;
		std::stack<node_pointer> ptrStack;
		node_pointer tempPtr = headPtr;
		while (!ptrStack.empty() || tempPtr != nullptr)
		{
			if (tempPtr != nullptr)
			{
				ptrStack.push(tempPtr);
				tempPtr = tempPtr->leftPtr;
			}
			else
			{
				tempPtr = ptrStack.top();
				ptrStack.pop();
				result.push_back(tempPtr->value);
				tempPtr = tempPtr->rightPtr;
			}
		}
		return result;
	}

	container_type  postorderTreeWalk() const
	{
		container_type result;
		std::stack<node_pointer> ptrStack;
		node_pointer tempPtr = headPtr;
		node_pointer prev;
		do {
			while (tempPtr != nullptr)
			{
				ptrStack.push(tempPtr);
				tempPtr = tempPtr->leftPtr;
			}
			prev = nullptr;
			while (!ptrStack.empty())
			{
				tempPtr = ptrStack.top();
				ptrStack.pop();
				if (tempPtr->rightPtr == prev)
				{
					result.push_back(tempPtr->value);
					prev = tempPtr;
				}
				else
				{
					ptrStack.push(tempPtr);
					tempPtr = tempPtr->rightPtr;
					break;
				}
			}
		} while (!ptrStack.empty());
			return result;
	}

	container_type morrisPreorderTraversal() const
	{
		container_type result;
		node_pointer prev, cur;
		cur = headPtr;
		while (cur != nullptr)
		{
			if (cur->leftPtr == nullptr)
			{
				result.push_back(cur->value);
				prev = cur;
				cur = cur->rightPtr;
			}
			else
			{
				node_pointer node = cur->leftPtr;
				while (node->rightPtr != nullptr && node != cur)
					node = node->rightPtr;
				if (node->rightPtr == nullptr)
				{
					result.push_back(cur->value);
					node->rightPtr = cur;
					prev = cur;
					cur = cur->leftPtr;
				}
				else
				{
					node->rightPtr = nullptr;
					cur = cur->rightPtr;
				}
			}
		}
		return result;
	}
private:
	node_pointer headPtr;
	//void Clear(node_pointer headPtr);
	node_pointer getMin(node_pointer a)
	{
		while (a->leftPtr != nullptr)
		{
			a = a->leftPtr;
		}
		return a;
	}
	int son(node_pointer a)
	{
		if (a->parentPtr == nullptr)
		{
			return 0;
		}
		if (a->parentPtr->leftPtr == a)
		{
			return -1;
		}
		return 1;

	}
	void transptr(node_pointer father, node_pointer son)
	{
		int a = BinarySearchTree::son(father);
		if (a == 1)
		{
			father->parentPtr->rightPtr = son;
		}
		else if (a == -1)
		{
			father->parentPtr->leftPtr = son;
		}
		else
		{
			headPtr = son;
			
		}
		son->parentPtr = father->parentPtr;
		delete father;
	}

	void print(node_pointer ptr)
	{

		if (ptr != nullptr)
		{
			print(ptr->leftPtr);
			std::cout << ptr->value << "  ";
			print(ptr->rightPtr);
		}

	};
};




















