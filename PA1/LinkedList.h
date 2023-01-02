#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template <class T>
class LinkedList
{
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);
    ~LinkedList();

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

    int getSize() const;
    bool isEmpty() const;
    bool containsNode(Node<T> *node) const;
    int getIndex(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void append(const T &data);
    void prepend(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAtIndex(const T &data, int index);
    void moveToIndex(int currentIndex, int newIndex);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeNodeAtIndex(int index);
    void removeAllNodes();

    void print() const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;
    int size;
};

template<class T>
LinkedList<T>::LinkedList() // constructor with no arguments
{
    head = NULL;
    size = 0;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj) // copy constructor - copy initialization
{
    head = NULL;
    size = 0;
    Node<T> *temp = obj.head;

    for (int i = 0; i < obj.size; i++)
    {
        append(temp->data);
        temp = temp->next;
    }
}

template<class T>
LinkedList<T>::~LinkedList() // destructor
{
    removeAllNodes();
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs) // '=' operator overloading
{
    if (this != &rhs)
    {
        removeAllNodes();
        Node<T> *temp = rhs.head;

        for (int i = 0; i < rhs.size; i++)
        {
            append(temp->data);
            temp = temp->next;
        }
    }
    return *this;
}

template<class T>
int LinkedList<T>::getSize() const
{
    return size;
}

template<class T>
bool LinkedList<T>::isEmpty() const
{
    return head == NULL && size == 0;
}

template<class T>
bool LinkedList<T>::containsNode(Node<T> *node) const
{
    if (isEmpty())
        return false;

    Node<T> *temp = head;

    do {
        if (temp == node)
            return true;
        temp = temp->next;
    } while (temp != head);

    return false;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const
{
    if (isEmpty())
        return NULL;

    return head;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const
{
    if (isEmpty())
        return NULL;

    return head->prev;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const // uses overload == operator when T is a class
{
    if (isEmpty())
        return NULL;

    Node<T> *temp = head;

    do {
        if (temp->data == data)
            return temp;
        temp = temp->next;
    } while (temp != head);

    return NULL;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const
{
    if (isEmpty() || index < 0 || index >= size) // invalid index
        return NULL;

    Node<T> *temp = head;

    for (int i = 0; i < index; i++)
        temp = temp->next;

    return temp;
}

template<class T>
void LinkedList<T>::append(const T &data)
{
    if (isEmpty())
    {
        head = new Node<T>(data);
        head->prev = head;
        head->next = head;
    }
    else // after head->prev == last node
    {
        Node<T> *newNode = new Node<T>(data, head->prev, head);
        head->prev->next = newNode;
        head->prev = newNode;
    }
    size++;
}

template<class T>
void LinkedList<T>::prepend(const T &data)
{
    append(data);
    head = head->prev; // changes the head
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node)
{
    if (isEmpty()) 
    {
        if (node == NULL) // if both are NULL
            append(data);
    }
    else
    {
        Node<T> *temp = head;

        do {
            if (temp == node)
            {
                Node<T> *newNode = new Node<T>(data, node, node->next);
                node->next->prev = newNode;
                node->next = newNode;
                size++;
                return;
            }
            temp = temp->next;

        } while (temp != head);
    }
}

template<class T>
void LinkedList<T>::insertAtIndex(const T &data, int index)
{
    if (index >= 0 && index <= size) // valid index
    {
        if (index == 0)
            prepend(data);
        else
        {
            Node<T> *temp = head;

            for (int i = 0; i < index - 1; i++) // stops before the place, so add after temp
                temp = temp->next;

            Node<T> *newNode = new Node<T>(data, temp, temp->next);
            temp->next->prev = newNode;
            temp->next = newNode;
            size++;
        }
    }
}

template<class T>
int LinkedList<T>::getIndex(Node<T> *node) const
{
    if (isEmpty())
        return -1;

    int index = 0;

    Node<T> *temp = head;

    do {
        if (temp == node)
            return index;
        temp = temp->next;
        index++;
    } while (temp != head);

    return -1;
}

    /* GUARANTEED:
    1. head is not NULL.
    2. currentIndex is valid.
    3. newIndex is valid.
    4. currentIndex and newIndex are different.
    5. size is not 1.
    */

template<class T>
void LinkedList<T>::moveToIndex(int currentIndex, int newIndex)
{
    if (currentIndex >= 0 && currentIndex < size && newIndex >= 0 && currentIndex != newIndex && size != 1)
    {
        bool forward = false, backward = false;
        if (currentIndex > newIndex)
            backward = true;
        else if (newIndex > currentIndex)
            forward = true;
        
        Node<T> *currentNode = head;
        Node<T> *temp = head;
        
        for (int i = 0; i < currentIndex; i++)
            currentNode = currentNode->next;
            
        for (int i = 0; i < newIndex; i++)
            temp = temp->next; 
            
        if (currentIndex == 0)
            head = head->next; 
            
        currentNode->next->prev = currentNode->prev;
        currentNode->prev->next = currentNode->next;                
            
        if (newIndex >= size - 1 || newIndex == 0) // forward, end or beginning, to avoid code repetition
        {
            currentNode->next = head;
            currentNode->prev = head->prev;
            head->prev->next = currentNode;
            head->prev = currentNode;     
            if (newIndex == 0) // beginning
                head = head->prev;                
        }
        else // middle and currentIndex == 0
        {
            if (forward)
            {
                currentNode->next = temp->next;
                currentNode->prev = temp;
                temp->next->prev = currentNode;
                temp->next = currentNode;
            }
            else if (backward)
            {
                currentNode->next = temp;
                currentNode->prev = temp->prev;
                temp->prev->next = currentNode;
                temp->prev = currentNode;                
            }
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    if (!isEmpty())
    {
        if (node == head)
        {
            if (size == 1)
            {
                delete node;
                size--; // 0
                head = NULL; 
                
            }
            else
            {
                head = head->next;
                head->prev = node->prev;
                node->prev->next = head;
                delete node;
                size--;
            }
        }
        else
        {
            Node<T> *temp = head->next;
            while (temp != head)
            {
                if (temp == node)
                {
                    node->next->prev = node->prev;
                    node->prev->next = node->next;
                    delete node;
                    size--;
                    return;
                }
                temp = temp->next;
            }
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data)
{
    Node<T> *temp = head;
    
    for (int i = 0; i < size; i++)
    {
        if (temp->data == data)
        {
            temp = temp->next;
            removeNode(temp->prev);
        }
        else
            temp = temp->next;  
    }
}

template<class T>
void LinkedList<T>::removeNodeAtIndex(int index)
{
    if (index >= 0 && index < size)
    {
        Node<T> *temp = head;
        
        for (int i = 0; i < index; i++)
            temp = temp->next;
        
        removeNode(temp);
    }
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    if (!isEmpty())
    {
        Node<T> *firstHead = head;
        Node<T> *temp = head;

        do {
            head = head->next;
            delete temp;
            temp = head;

        } while (temp != firstHead);
        
        head = NULL;
        size = 0;   
    }
}

template<class T>
void LinkedList<T>::print() const
{
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    Node<T> *node = this->getFirstNode();

    do {
        std::cout << *node << std::endl;
        node = node->next;
    }
    while (node != this->getFirstNode());
}

#endif //LINKEDLIST_H
