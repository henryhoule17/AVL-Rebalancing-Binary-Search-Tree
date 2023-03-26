#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('f',1));
    bt.insert(std::make_pair('b',2));
    bt.insert(std::make_pair('a',3));
    bt.insert(std::make_pair('c',3));
    bt.insert(std::make_pair('l',4));
    bt.insert(std::make_pair('k',5));
    bt.insert(std::make_pair('f',6));
    bt.insert(std::make_pair('c',5));
    bt.insert(std::make_pair('n',5));
    bt.insert(std::make_pair('o',5));
    bt.insert(std::make_pair('m',5));
    bt.insert(std::make_pair('j',5));
    bt.insert(std::make_pair('g',5));
    //bt.print();
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
	bt.print();
    cout << "The tree height is " << bt.getHeight() << endl;
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('j');
    cout << "removed j" << endl;
    bt.remove('l');
    cout << "removed l" << endl;

    bt.print();
    cout << "Made it to AVL part" << endl;

    // AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    at.insert(std::make_pair('b',2));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');

    return 0;
}
