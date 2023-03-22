#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getLeftPathHeight(Node* root);
int getRightPathHeight(Node* root);
bool LnRbalanced(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL)
        return true;
		if((getLeftPathHeight(root) != getRightPathHeight(root)) 
			&& (getLeftPathHeight(root) != 0 || getRightPathHeight(root)))
			return false;
		else 
			return equalPaths(root->left) && equalPaths(root->left);
}

int getLeftPathHeight(Node* root)
{
	if(root == NULL) return 0;
	if(root->left != NULL)
		return 1 + getLeftPathHeight(root->left);
	else
		return 1 + getLeftPathHeight(root->right);
}

int getRightPathHeight(Node* root)
{
	if(root == NULL) return 0;
	if(root->right != NULL)
		return 1 + getRightPathHeight(root->right);
	else
		return 1 + getRightPathHeight(root->left);
}



