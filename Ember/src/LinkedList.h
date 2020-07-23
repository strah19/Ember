#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

template <class T>
class LinkedList;

template <class T>
class Node
{
public:
	Node(T* nodeValue);
private:
	friend LinkedList<T>;
	T* m_NodePointer;
	Node<T>* m_NextNode;
	Node<T>* m_PrevNode;
};

template <class T>
Node<T>::Node(T* nodeValue)
	: m_NodePointer(nodeValue), m_NextNode(nullptr), m_PrevNode(nullptr)
{
}

template <class T>
class LinkedList
{
public:
	LinkedList();
	virtual ~LinkedList();
	int add_node(T* nodeValue);
	int remove_node(T* nodeValue);
	void clear();
	inline size_t size() { return m_Counter; }
	T* get(unsigned int index);

	T& operator[](unsigned int index);
private:
	Node<T>* get_last_element();
	Node<T>* get_element(T* nodeValue);

	Node<T>* m_HeadNode;
	size_t m_Counter;
};

template <class T>
T& LinkedList<T>::operator[](unsigned int index)
{
	return *get(index);
}

template <class T>
LinkedList<T>::LinkedList()
	:m_HeadNode(nullptr), m_Counter(0)
{
}

template <class T>
LinkedList<T>::~LinkedList()
{
	clear();
}

template <class T>
T* LinkedList<T>::get(unsigned int index)
{
	Node<T>* iterator = m_HeadNode;
	unsigned int counter = 0;
	if (index > m_Counter - 1)
	{
		return 0;
	}
	while (iterator)
	{
		if (counter == index)
		{
			std::cout << "Found index.\n";
			return iterator->m_NodePointer;
		}
		iterator = iterator->m_NextNode;
		counter++;
	}
	return 0;
}

template <class T>
void LinkedList<T>::clear()
{
	Node<T>* iterator;
	while (m_HeadNode)
	{
		iterator = m_HeadNode->m_NextNode;

		delete m_HeadNode;

		if (iterator)
		{
			iterator->m_PrevNode = 0;
		}
		m_HeadNode = iterator;

		std::cout << "Deleting node " << m_Counter << std::endl;
		m_Counter--;
	}
}

template <class T>
int LinkedList<T>::add_node(T* nodeValue)
{
	Node<T>* TempNode = new Node<T>(nodeValue);
	if (!TempNode)
	{
		return -1;
	}
	if (!m_HeadNode)
	{
		std::cout << "Creating head for list.\n";
		m_HeadNode = TempNode;
		m_HeadNode->m_PrevNode = nullptr;
	}
	else
	{
		Node<T>* lastNode = get_last_element();
		TempNode->m_PrevNode = lastNode;
		lastNode->m_NextNode = TempNode;
		TempNode->m_NextNode = nullptr;
		std::cout << "Creating new node for list at " << m_Counter << std::endl;
	}
	m_Counter++;
	return 0;
}

template <class T>
int LinkedList<T>::remove_node(T* nodeValue)
{
	std::cout << "Removing node.\n";
	Node<T>* removeNode = get_element(nodeValue);

	if (removeNode == m_HeadNode)
	{
		std::cout << "Removing m_HeadNode.\n";
		m_HeadNode = removeNode->m_NextNode;
		if (m_HeadNode)
		{
			m_HeadNode->m_PrevNode = nullptr;
		}
		delete removeNode;
	}
	else
	{
		std::cout << "Removing middle node.\n";
		Node<T>* item1 = removeNode->m_PrevNode;
		Node<T>* item2 = removeNode->m_NextNode;

		if (item1)
			item1->m_NextNode = item2;
		if (item2)
			item2->m_PrevNode = item1;

		delete removeNode;
	}

	m_Counter--;
	return 0;
}

template <class T>
Node<T>* LinkedList<T>::get_last_element()
{
	Node<T>* TempNode = m_HeadNode;
	while (TempNode && (TempNode->m_NextNode))
	{
		std::cout << "Searching for node...\n";
		TempNode = TempNode->m_NextNode;
	}
	return TempNode;
}

template <class T>
Node<T>* LinkedList<T>::get_element(T* nodeValue)
{
	Node<T>* TempNode = m_HeadNode;
	while (TempNode)
	{
		if (TempNode->m_NodePointer == nodeValue)
		{
			return TempNode;
		}
		TempNode = TempNode->m_NextNode;
	}
	return nullptr;
}

#endif 
