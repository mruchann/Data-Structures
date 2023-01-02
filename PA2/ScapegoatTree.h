#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>

#include "Node.h"
#include "NoSuchItemException.h"

enum TraversalMethod {preorder, inorder, postorder};

template<class T>
class ScapegoatTree {
public: // DO NOT CHANGE THIS PART.
    ScapegoatTree();

    ScapegoatTree(const ScapegoatTree<T> &obj);

    ~ScapegoatTree();

    bool isEmpty() const;

    int getHeight() const;

    int getSize() const;

    bool insert(const T &element);

    bool remove(const T &element);

    void removeAllNodes();

    const T &get(const T &element) const;

    void print(TraversalMethod tp=inorder) const;

    void printPretty() const;

    ScapegoatTree<T> &operator=(const ScapegoatTree<T> &rhs);

    void balance();

    const T &getCeiling(const T &element) const;

    const T &getFloor(const T &element) const;

    const T &getMin() const;

    const T &getMax() const;

    const T &getNext(const T &element) const;

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(Node<T> *node, TraversalMethod tp) const;
    
    void printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const;
    
    int getHeightHelper(Node<T> *root) const;
    
    const T& getHelper(Node<T> *root, const T &element) const;
    
    int getSizeHelper(Node<T> *root) const;
    
    const T & getCeilingHelper(Node<T> *root, const T& element) const; 
    
    const T& getFloorHelper(Node<T> *root, const T &element) const;
    
    const T & getMinHelper(Node<T> *root) const;
    
    const T & getMaxHelper(Node<T> *root) const;
    
    const T& getNextHelper(Node<T> *root, const T &element) const;
    
    Node<T> *removeHelper(Node<T> *root, const T &element, bool &result);
    
    Node<T>* insertHelper(Node<T> *root, const T& element, bool &result);
    
    Node<T>* constructTree(Node<T>* root, Node<T>* rhsroot);
    
    void putIntoArray(Node<T> *root, T *arr, int& i);
    
    void putIntoTree(Node<T> *&root, T *arr, int l, int r);
    
    void removeAllNodesHelper(Node<T>* root);
    
    void balanceHelper(Node<T>* scapegoat);
    
    Node<T> *trace(Node<T>* root, const T &element);
    
    Node<T>* findParentOfScapegoatNode(Node<T>* root, Node<T>* scapegoat);
    

private: // DO NOT CHANGE THIS PART.
    Node<T> *root;

    int upperBound;
};

#endif //TREE_H

template<class T>
ScapegoatTree<T>::ScapegoatTree()
{
    root = NULL;
    upperBound = 0;
}

template<class T>
ScapegoatTree<T>::ScapegoatTree(const ScapegoatTree<T> &obj)
{
    root = constructTree(root, obj.root);
    upperBound = obj.upperBound;
}

template<class T>
Node<T>* ScapegoatTree<T>::constructTree(Node<T>* root, Node<T>* rhsroot)
{
    if (rhsroot != NULL)
    {
        root = new Node<T>(rhsroot->element, NULL, NULL);
        root->left = constructTree(root->left, rhsroot->left);
        root->right = constructTree(root->right, rhsroot->right);
    }
    else
        root = NULL;
    return root;
}

template<class T>
ScapegoatTree<T>::~ScapegoatTree()
{
    removeAllNodes();
}

template<class T>
bool ScapegoatTree<T>::isEmpty() const
{
    return root == NULL;
}

template<class T>
int ScapegoatTree<T>::getHeightHelper(Node<T> *root) const
{
    if (root == NULL)
        return -1;
    else
        return 1 + std::max(getHeightHelper(root->left), getHeightHelper(root->right));
}

template<class T>
int ScapegoatTree<T>::getHeight() const
{
    return getHeightHelper(root);
}

template<class T>
int ScapegoatTree<T>::getSizeHelper(Node<T> *root) const
{
    if (root == NULL)
        return 0;
    else
        return 1 + getSizeHelper(root->left) + getSizeHelper(root->right);
}

template<class T>
int ScapegoatTree<T>::getSize() const
{
    return getSizeHelper(root);
}

template<class T>
bool ScapegoatTree<T>::insert(const T &element) 
{
    bool result = true;
    root = insertHelper(root, element, result);
    
    if (result == true)
        upperBound++;
    
    bool heightCondition = pow(1.5, getHeight()) <= upperBound; 

    if (heightCondition == false) 
    {
        if (result == true)
        {
            Node<T>* scapegoat = trace(root, element);
            balanceHelper(scapegoat);
        }
    }
    
    return result;
}

template<class T>
Node<T>* ScapegoatTree<T>::trace(Node<T>* root, const T &element)
{
    Node<T>*scapegoat = root;
    Node<T>*temp = root;
    while(temp->element != element)
    {
        if (temp->element > element) // left
        {
            if (3 * getSizeHelper(temp->left) > 2 * getSizeHelper(temp))
                scapegoat = temp;
                
            temp = temp->left;
        }

        else if (temp->element < element) // right
        {
            if (3 * getSizeHelper(temp->right) > 2 * getSizeHelper(temp))
                scapegoat = temp;            
                
            temp = temp->right; 
        }
    }
    return scapegoat;
}

template<class T>
Node<T>* ScapegoatTree<T>::insertHelper(Node<T> *root, const T& element, bool &result)
{
    if (root == NULL)
        root =  new Node<T>(element, NULL, NULL);
        
    else if (root->element > element) // left
        root->left = insertHelper(root->left, element, result);
    
    else if (root->element < element) // right
        root->right = insertHelper(root->right, element, result);
    else    
        result = false;    
    return root;
}

template<class T>
bool ScapegoatTree<T>::remove(const T &element) 
{
    bool result = false;
    root = removeHelper(root, element, result);
    
    bool upperBoundCondition = upperBound/2.0 <= getSize() && getSize() <= upperBound;
    
    if (upperBoundCondition == false)
    {
        balance();
        upperBound = getSize();
    }
    
    return result;
}

template<class T>
Node<T>* ScapegoatTree<T>::removeHelper(Node<T> *root, const T &element, bool &result)
{      
    if (root == NULL) // empty or not found
        return root; 
    
    else if (element < root->element)
        root->left = removeHelper(root->left, element, result);
    
    else if (root->element < element)
        root->right = removeHelper(root->right, element, result);
    
    else // found
    {
        
        if (root->left == NULL && root->right == NULL) // leaf node
        {
            delete root; 
            root = NULL;
        }
        
        else if (root->left == NULL || root->right == NULL) // one child
        {
            Node<T> *temp = root;
            if (root->left == NULL)
                root = root->right;
            else if (root->right == NULL)
                root = root->left;
            delete temp; 
            temp = NULL;
        }
        
        else // two child
        {
            Node<T> *temp = root->left;
            while (temp->right)
                temp = temp->right;
            root->element = temp->element;
            root->left = removeHelper(root->left, root->element, result);
        }     
        result = true;
    }
    return root;
}        

template<class T>
void ScapegoatTree<T>::removeAllNodes()
{
    while (!isEmpty())
        remove(root->element);
}

template<class T>
void ScapegoatTree<T>::removeAllNodesHelper(Node<T>* root)
{
    if (root != NULL)
    {
        removeAllNodesHelper(root->left);
        removeAllNodesHelper(root->right);
        delete root;
    }
}

template<class T>
const T &ScapegoatTree<T>::get(const T &element) const
{
    return getHelper(root, element);
}

template<class T>
const T &ScapegoatTree<T>::getHelper(Node<T> *root, const T &element) const
{
    if (root != NULL)
    {
        if (root->element == element)
            return root->element;
        else if (root->element > element) // left
            return getHelper(root->left, element);
        else if (root->element < element) //right
            return getHelper(root->right, element);       
    }
    else
        throw NoSuchItemException();
}

template<class T>
void ScapegoatTree<T>::print(TraversalMethod tp) const
{

    if (tp == preorder) 
    {
        if (isEmpty())
            std::cout << "BST_preorder{}" << std::endl;
        else
        {
            std::cout << "BST_preorder{" << std::endl;
            print(root, tp);
            std::cout << std::endl << "}" << std::endl;  
        }
    } 
    
    else if (tp == inorder)
    {
        if (isEmpty()) 
            std::cout << "BST_inorder{}" << std::endl;
        else
        {
            std::cout << "BST_inorder{" << std::endl;
            print(root, tp);
            std::cout << std::endl << "}" << std::endl;            
        }
    } 
    
    else if (tp == postorder) 
    {
        if (isEmpty()) 
            std::cout << "BST_postorder{}" << std::endl;
        else
        {
            std::cout << "BST_postorder{" << std::endl;
            print(root, tp);
            std::cout << std::endl << "}" << std::endl;            
        }
    }
}

template<class T>
void ScapegoatTree<T>::print(Node<T> *node, TraversalMethod tp) const
{

    if (tp == preorder) 
    {
        if (node != NULL)
        {
            std::cout << "\t" << node->element;
            if (node->left)
                std::cout << "," << std::endl;
            print(node->left, preorder);
            if (node->right)
                std::cout << "," << std::endl;
            print(node->right, preorder);
        }
    } 
    
    else if (tp == inorder) 
    {
        if (node != NULL)
        {
            print(node->left, inorder);
            if (node->left)
                std::cout << "," << std::endl;
            std::cout << "\t" << node->element;
            if (node->right)
                std::cout << "," << std::endl;
            print(node->right, inorder);        
        }
    } 
    
    else if (tp == postorder) 
    {
        if (node != NULL)
        {
            print(node->left, postorder);
            if (node->left)
                std::cout << "," << std::endl;
            print(node->right, postorder);
            if (node->right)
                std::cout << "," << std::endl;
            std::cout << "\t" << node->element;
        }
    }
}

template<class T>
void ScapegoatTree<T>::printPretty() const
{

    // check if the tree is empty?
    if (isEmpty()) {
        // the tree is empty.
        std::cout << "BST_pretty{}" << std::endl;
        return;
    }

    // the tree is not empty.

    // recursively output the tree.
    std::cout << "BST_pretty{" << std::endl;
    printPretty(root, 0, false);
    std::cout << "}" << std::endl;

}

template<class T>
void ScapegoatTree<T>::printPretty(Node<T> *node, int indentLevel, bool isLeftChild) const
{
    // check if the node is NULL?
    if (node == NULL)
        return;

    // output the indentation and the node.
    std::cout << "\t";
    for (int i = 0; i < indentLevel; ++i) {
        std::cout << "---";
    }
    std::cout << (indentLevel == 0 ? "root:" : (isLeftChild ? "l:" : "r:")) << node->element << std::endl;

    // first, output left subtree with one more indentation level.
    printPretty(node->left, indentLevel + 1, true);

    // then, output right subtree with one more indentation level.
    printPretty(node->right, indentLevel + 1, false);
}

template<class T>
ScapegoatTree<T> &ScapegoatTree<T>::operator=(const ScapegoatTree<T> &rhs) 
{
    if (this != &rhs)
    {
        removeAllNodes();
        root = constructTree(root, rhs.root);
        upperBound = rhs.upperBound;    
    }
}

template<class T>
void ScapegoatTree<T>::balance() 
{
    int upperBound = this->upperBound;
    T* arr = new T[getSize()];
    int i = 0;
    putIntoArray(root, arr, i);
    
    int l = 0, r = getSize()-1;
    removeAllNodes();
    this->upperBound = upperBound;
    putIntoTree(root, arr, l, r);

    delete[] arr;
}
template<class T>
Node<T>* ScapegoatTree<T>::findParentOfScapegoatNode(Node<T>* root, Node<T>* scapegoat)
{
    T element = scapegoat->element;
    
    if (root->element > element) // left
    {
        if (root->left->element == element)
            return root;
        else
            return findParentOfScapegoatNode(root->left, scapegoat);
    }
    else if (root->element < element) // right
    {
        if (root->right->element == element)
            return root;
        else
            return findParentOfScapegoatNode(root->right, scapegoat);
    }
    else
        return NULL;
}


template<class T>
void ScapegoatTree<T>::balanceHelper(Node<T>* scapegoat) 
{
    Node<T> *parent = findParentOfScapegoatNode(root, scapegoat); 
    T* arr = new T[getSizeHelper(scapegoat)];
    int i = 0;
    putIntoArray(scapegoat, arr, i);
    
    int l = 0, r = getSizeHelper(scapegoat)-1;
    removeAllNodesHelper(scapegoat);
    scapegoat = NULL;
    putIntoTree(scapegoat, arr, l, r);
    if (parent != NULL)
    {
        if (parent->element > scapegoat->element)
            parent->left = scapegoat;
        
        else if (parent->element < scapegoat->element)
            parent->right = scapegoat;
    }
    else
        root = scapegoat;
    delete[] arr;
}

template<class T>
void ScapegoatTree<T>::putIntoTree(Node<T> *&root, T *arr, int l, int r)
{
    if (l <= r)
    {
        int mid = (l+r)/2;
        
        root = new Node<T>(arr[mid], NULL, NULL);
        
        putIntoTree(root->left, arr, l, mid-1);
        putIntoTree(root->right, arr, mid+1, r);
    }
}
template<class T>
void ScapegoatTree<T>::putIntoArray(Node<T> *root, T *arr, int& i)
{
    if (root != NULL)
    {
        putIntoArray(root->left, arr, i);
        arr[i++] = root->element;
        putIntoArray(root->right, arr, i);
    }
}

template<class T>
const T &ScapegoatTree<T>::getCeiling(const T &element) const
{
    if (isEmpty() || element > getMax())
        throw NoSuchItemException();
    else
        return getCeilingHelper(root, element);
}

template<class T>
const T& ScapegoatTree<T>::getCeilingHelper(Node<T> *root, const T &element) const
{   
    if (root != NULL)
    {
        if (root->element == element)
            return root->element;
        
        else if (root->element > element) // left
        {
            if (root->left == NULL)
                return root->element;
            else if (getCeilingHelper(root->left, element) < element)
                return root->element;
            else    
                return getCeilingHelper(root->left, element);   
        }  
        else if (root->element < element) // right
        {
            if (root->right == NULL)
                return root->element;
            else
                return getCeilingHelper(root->right, element);
        }
    }
}

template<class T>
const T &ScapegoatTree<T>::getFloor(const T &element) const
{
    if (isEmpty() || element < getMin())
        throw NoSuchItemException();
    else
        return getFloorHelper(root, element);
}

template<class T>
const T& ScapegoatTree<T>::getFloorHelper(Node<T> *root, const T &element) const
{
    if (root != NULL)
    {
        if (root->element == element)
            return root->element;
            
        else if (root->element < element) // right
        {
            if (root->right == NULL)
                return root->element;
            else if (getFloorHelper(root->right, element) > element)    
                return root->element;
            else
                return getFloorHelper(root->right, element);
        }    
        
        else if (root->element > element) // left
        {
            if (root->left == NULL)
                return root->element;
            else
                return getFloorHelper(root->left, element);
        }  
    }
}

template<class T>
const T &ScapegoatTree<T>::getMin() const
{
    if (isEmpty())
        throw NoSuchItemException();
    else
        return getMinHelper(root);
}

template<class T>
const T &ScapegoatTree<T>::getMinHelper(Node<T> *root) const
{
    if (root != NULL)
    {
        if (root->left)
            return getMinHelper(root->left);
        else
            return root->element;
    } 
}

template<class T>
const T &ScapegoatTree<T>::getMax() const
{
    if (isEmpty())
        throw NoSuchItemException();
    else
        return getMaxHelper(root);
}

template<class T>
const T &ScapegoatTree<T>::getMaxHelper(Node<T> *root) const
{
    if (root != NULL)
    {
        if (root->right)
            return getMaxHelper(root->right);
        else
            return root->element;
    } 
}

template<class T>
const T &ScapegoatTree<T>::getNext(const T &element) const
{
    if (isEmpty() || element >= getMax())
        throw NoSuchItemException();
    else
        return getNextHelper(root, element);
}

template<class T>
const T& ScapegoatTree<T>::getNextHelper(Node<T> *root, const T &element) const
{   
    if (root != NULL)
    {
        if (root->element > element) // left
        {
            if (root->left == NULL)
                return root->element;
            else if (getNextHelper(root->left, element) <= element)
                return root->element;
            else    
                return getNextHelper(root->left, element);   
        }  
        else if (root->element <= element) // right
        {
            if (root->right == NULL)
                return root->element;
            else
                return getNextHelper(root->right, element);
        }
    }
}