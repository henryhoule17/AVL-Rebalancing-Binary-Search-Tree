#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateRight(AVLNode<Key, Value>* gp);
    void rotateLeft(AVLNode<Key, Value>* gp);
		void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
		void removeFix(AVLNode<Key,Value>* n, int diff);
		void removeHelper(AVLNode<Key, Value>* toRemove);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
		if(!(this->root_))
		{
			AVLNode<Key, Value>* nNode	= new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
			this->root_ = nNode;
			nNode->setBalance(0);
			return;
		}

		AVLNode<Key, Value>* nNode = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
		nNode->setBalance(0);
		AVLNode<Key, Value>* temp = static_cast<AVLNode<Key, Value>*>(this->root_);
		while(temp){
			if(nNode->getKey() < temp->getKey()){
				if(temp->getLeft()){
					temp = temp->getLeft();
				} 
				else{
					nNode->setParent(temp);
					temp->setLeft(nNode);
					break;
				}
			} 
			else if(nNode->getKey() > temp->getKey()){
				if(temp->getRight()){
					temp = temp->getRight();
				} 
				else{
					nNode->setParent(temp);
					temp->setRight(nNode);
					break;
				}
			}
			else{
				temp->setValue(new_item.second);
				delete nNode;
				return;
			}
		}

		//Here temp is the parent of nNode
		if(temp->getBalance() == -1 || temp->getBalance() == 1){
			temp->setBalance(0);
			return;
		}
		else{ //temp's balance is 0
			if(temp->getLeft() == nNode)
				temp->updateBalance(-1);
			else
				temp->updateBalance(1);				
		}
		
		insertFix(temp, nNode);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n)
{
	if(p == NULL || p->getParent() == NULL)
		return;
	
	AVLNode<Key, Value>* gp = p->getParent();

	if(p == gp->getLeft()){
		gp->updateBalance(-1);
		int8_t gpBal = gp->getBalance();
		if(gpBal == 0)
			return;
		else if(gpBal == -1)
		{
			insertFix(gp, p);
			return;
		}
		else if(p->getLeft() == n){
			rotateRight(gp);
			p->setBalance(0);
			gp->setBalance(0);
		}
		else{
			rotateLeft(p);
			rotateRight(gp);

			if(n->getBalance() == -1)
			{
				p->setBalance(0);
				gp->setBalance(1);
			}
			else if(n->getBalance() == 0){
				p->setBalance(0);
        gp->setBalance(0);
			}
			else{
				p->setBalance(-1);
        gp->setBalance(0);
			}
			n->setBalance(0);
		}
	}
	else{//if p is a right child
		gp->updateBalance(1);
		int8_t gpBal = gp->getBalance();
		if(gpBal == 0)
			return;
		else if(gpBal == 1)
		{
			insertFix(gp, p);
			return;
		}
		else if(p->getRight() == n){
			rotateLeft(gp);
			p->setBalance(0);
			gp->setBalance(0);
		}
		else{
			rotateRight(p);
			rotateLeft(gp);

			if(n->getBalance() == 1)
			{
				p->setBalance(0);
				gp->setBalance(-1);
			}
			else if(n->getBalance() == 0){
				p->setBalance(0);
				gp->setBalance(0);
			}
			else{
				p->setBalance(1);
				gp->setBalance(0);
			}
			n->setBalance(0);
		}
	}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // TODO
	AVLNode<Key, Value>* toRemove = 
		dynamic_cast<AVLNode<Key, Value>*>(
		BinarySearchTree<Key, Value>::internalFind(key));
	int diff;
	if(!toRemove)
		return;
	
	//Handles the case where toRemove is the root
	if(toRemove == this->root_){
		if(toRemove->getLeft()){
			AVLNode<Key, Value>* pred = 
				dynamic_cast<AVLNode<Key, Value>*>(
				BinarySearchTree<Key, Value>::predecessor(toRemove));
			nodeSwap(toRemove, pred);
			this->root_ = pred;
		}
		else if(toRemove->getRight()){
			AVLNode<Key, Value>* suc = 
				dynamic_cast<AVLNode<Key, Value>*>(
				BinarySearchTree<Key, Value>::successor(toRemove));
			nodeSwap(toRemove, suc);
			this->root_ = suc;
		}
		else{
			delete toRemove;
			this->root_ = NULL;
			return;
		}
	}

	//Gets rid of case where there's 2 children
	if(toRemove->getRight() && toRemove->getLeft()){
		AVLNode<Key, Value>* pred = 
			dynamic_cast<AVLNode<Key, Value>*>(
			BinarySearchTree<Key, Value>::predecessor(toRemove));
		nodeSwap(toRemove, pred);
	}
	
	//Updating diff
	AVLNode<Key, Value>* p = toRemove->getParent();
	if(toRemove == p->getLeft())
		diff = 1;
	else
		diff = -1;

	//deletes the node
	removeHelper(toRemove);

	removeFix(p, diff);
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff)
{
	if(!n)
		return;
	AVLNode<Key, Value>* p = n->getParent();

	//sets diff for the next recursive call
	int ndiff;
	if(p){
		if(p->getLeft() == n)
			ndiff = 1;
		else
			ndiff = -1;
	}

	//Case where diff is -1
	if(diff == -1){
		if(n->getBalance() + diff == -2){
			AVLNode<Key, Value>* c = n->getLeft();
			//if left side of n is unbalanced
			if(c->getBalance() == -1){
				rotateRight(n);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p, ndiff);
			}
			else if(c->getBalance() == 0){
				rotateRight(n);
				n->setBalance(-1);
				c->setBalance(1);
				return;
			}
			else{
				AVLNode<Key, Value>* g = c->getRight();
				rotateLeft(c);
				rotateRight(n);
				if(g->getBalance() == 1){
					n->setBalance(0);
					c->setBalance(-1);
					g->setBalance(0);
				}
				else if(g->getBalance() == 0){
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else{
					n->setBalance(1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(p, ndiff);
			}
		}
		else if(n->getBalance() + diff == -1){
			n->setBalance(-1);
			return;
		}
		else{
			n->setBalance(0);
			removeFix(p, ndiff);
		}
	}
	//Case where diff is 1
	else{
		if(n->getBalance() + diff == 2){
			AVLNode<Key, Value>* c = n->getRight();
			//if left side of n is unbalanced
			if(c->getBalance() == 1){
				rotateLeft(n);
				n->setBalance(0);
				c->setBalance(0);
				removeFix(p, ndiff);
			}
			else if(c->getBalance() == 0){
				rotateLeft(n);
				n->setBalance(1);
				c->setBalance(-1);
				return;
			}
			else{
				AVLNode<Key, Value>* g = c->getLeft();
				rotateRight(c);
				rotateLeft(n);
				if(g->getBalance() == -1){
					n->setBalance(0);
					c->setBalance(1);
					g->setBalance(0);
				}
				else if(g->getBalance() == 0){
					n->setBalance(0);
					c->setBalance(0);
					g->setBalance(0);
				}
				else{
					n->setBalance(-1);
					c->setBalance(0);
					g->setBalance(0);
				}
				removeFix(p, ndiff);
			}
		}
		else if(n->getBalance() + diff == 1){
			n->setBalance(1);
			return;
		}
		else{
			n->setBalance(0);
			removeFix(p, ndiff);
		}
	}
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::removeHelper(AVLNode<Key, Value>* toRemove)
{
	if(toRemove->getRight() == NULL && toRemove->getLeft() == NULL){
		if(toRemove->getParent()->getLeft() == toRemove)
			toRemove->getParent()->setLeft(NULL);
		else
			toRemove->getParent()->setRight(NULL);

		delete toRemove;
		return;
	}
	else if(toRemove->getRight()){
		AVLNode<Key, Value>* suc = 
			dynamic_cast<AVLNode<Key, Value>*>(
			BinarySearchTree<Key, Value>::successor(toRemove));
		nodeSwap(toRemove, suc);
		removeHelper(toRemove);
	}
	else {
		AVLNode<Key, Value>* pred = 
			dynamic_cast<AVLNode<Key, Value>*>(
			BinarySearchTree<Key, Value>::predecessor(toRemove));
		nodeSwap(toRemove, pred);
		removeHelper(toRemove);
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* gp)
{
	//if(gp->getLeft()->getLeft() == NULL && gp->getLeft()->getRight() != NULL)
		//rotateLeft(gp->getLeft());

	AVLNode<Key,Value>* parent = gp->getLeft();
	AVLNode<Key,Value>* child = parent->getRight();
	AVLNode<Key,Value>* ggp = gp->getParent();

	gp->setLeft(child);
	parent->setRight(gp);
	gp->setParent(parent);
	if(child)
		child->setParent(gp);

	if(ggp){
		if(ggp->getRight() == gp)
				ggp->setRight(parent);
		else
				ggp->setLeft(parent);
		parent->setParent(ggp);
	}
	else{
		this->root_ = parent;
		parent->setParent(NULL);
	}
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* gp)
{
	//if(gp->getRight()->getRight() == NULL && gp->getRight()->getLeft() != NULL)
	//	rotateRight(gp->getRight());
		
	AVLNode<Key,Value>* parent = gp->getRight();
	AVLNode<Key,Value>* child = parent->getLeft();
	AVLNode<Key,Value>* ggp = gp->getParent();

	gp->setRight(child);
	parent->setLeft(gp);
	gp->setParent(parent);
	if(child)
		child->setParent(gp);

	if(ggp){
		if(ggp->getRight() == gp)
				ggp->setRight(parent);
		else
				ggp->setLeft(parent);
	}else{
		this->root_ = parent;
		parent->setParent(NULL);
	}

	parent->setParent(ggp);
}

/*
        if(gp->getParent()){
            AVLNode<Key,Value>* ggp = gp->getParent();

            if(ggp->getRight() == gp)
                ggp->setRight(parent);
            else
                ggp->setLeft(parent);

            parent->setParent(ggp);
        }
        else{
            
        }
        gp->setLeft(parent->getRight());
        parent->setRight(gp);
        gp->setParent(parent);
    }
    else if(gp->getLeft()->getRight()){
        rotateLeft(parent);
    }
    else{
        if(gp->getParent()){
            AVLNode<Key,Value>* ggp = gp->getParent();

            if(ggp->getRight() == gp)
                ggp->setRight(parent);
            else
                ggp->setLeft(parent);

            parent->setParent(ggp);
        }
        else{
            root_ = parent;
            parent->setParent(NULL);
        }
        gp->setLeft(NULL);
				parent->setRight(gp);
        gp->setParent(parent);
    }
}*/

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

#endif
