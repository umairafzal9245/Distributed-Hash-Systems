#include <iostream>
#include "Queue.h"
using namespace std;

struct Treenode
{
	long long int id;
	string key;
	string filepath;
	long long int linenumber;
	long long int height;
	Treenode *left;
	Treenode *right;

	Treenode()
	{
		id = 0;
		height = 0;
		key = "";
		filepath = "";
		linenumber = 0;
		left = nullptr;
		right = nullptr;
	}
};

class AVL
{
private:
	Treenode *root;

public:
	AVL()
	{
		root = NULL;
	}

	void insert(long long int x, string key, string filepath, long long int linenumber)
	{
		root = insert(x, key, filepath, linenumber, root);
	}

	void remove(long long int x)
	{
		root = remove(x, root);
	}

	void display()
	{
		inorder(root);
		cout << endl;
	}

	/* Function used to get the height of avl tree*/
	long long int height(Treenode *temp)
	{
		return (temp == NULL ? -1 : temp->height);
	}

	/* Function used to insert the data in avl tree and perform rotation on it to balance the tree*/
	Treenode *insert(long long int x, string key, string filepath, long long int linenumber, Treenode *temp)
	{
		if (temp == NULL)
		{
			temp = new Treenode;
			temp->id = x;
			temp->key = key;
			temp->filepath = filepath;
			temp->linenumber = linenumber;
			temp->height = 0;
			temp->left = temp->right = NULL;
		}
		else if (x < temp->id)
		{
			temp->left = insert(x, key, filepath, linenumber, temp->left);

			if (height(temp->left) - height(temp->right) == 2)
			{
				if (x < temp->left->id)
					temp = singleRightRotate(temp);
				else
					temp = doubleRightRotate(temp);
			}
		}
		else if (x > temp->id)
		{
			temp->right = insert(x, key, filepath, linenumber, temp->right);

			if (height(temp->right) - height(temp->left) == 2)
			{
				if (x > temp->right->id)
					temp = singleLeftRotate(temp);
				else
					temp = doubleLeftRotate(temp);
			}
		}
		temp->height = max(height(temp->left), height(temp->right)) + 1;
		return temp;
	}

	Treenode *singleRightRotate(Treenode *&temp)
	{
		Treenode *u = temp->left;
		temp->left = u->right;
		u->right = temp;

		temp->height = max(height(temp->left), height(temp->right)) + 1;
		u->height = max(height(u->left), temp->height) + 1;

		return u;
	}

	Treenode *singleLeftRotate(Treenode *&temp)
	{
		Treenode *u = temp->right;
		temp->right = u->left;
		u->left = temp;

		temp->height = max(height(temp->left), height(temp->right)) + 1;
		u->height = max(height(temp->right), temp->height) + 1;

		return u;
	}

	Treenode *doubleLeftRotate(Treenode *&t)
	{
		t->right = singleRightRotate(t->right);
		return singleLeftRotate(t);
	}

	Treenode *doubleRightRotate(Treenode *&t)
	{
		t->left = singleLeftRotate(t->left);
		return singleRightRotate(t);
	}
	/*Function used the find the minimum node of tree*/
	Treenode *findMin(Treenode *temp)
	{
		if (temp == NULL)
			return NULL;
		else if (temp->left == NULL)
			return temp;
		else
			return findMin(temp->left);
	}
	/* Function used to remove the data in avl tree and perform rotation on it to balance the tree*/
	Treenode *remove(long long int x, Treenode *t)
	{
		Treenode *temp;

		if (t == NULL)
			return NULL;

		else if (x < t->id)
			t->left = remove(x, t->left);
		else if (x > t->id)
			t->right = remove(x, t->right);

		else if (t->left && t->right)
		{
			temp = findMin(t->right);
			t->id = temp->id;
			t->right = remove(t->id, t->right);
		}
		else
		{
			temp = t;
			if (t->left == NULL)
				t = t->right;
			else if (t->right == NULL)
				t = t->left;
			delete temp;
		}

		if (t == NULL)
			return t;

		t->height = max(height(t->left), height(t->right)) + 1;

		if (height(t->left) - height(t->right) == -2)
		{
			if (height(t->right->right) - height(t->right->left) == 1)
				return singleLeftRotate(t);
			else
				return doubleLeftRotate(t);
		}
		else if (height(t->right) - height(t->left) == 2)
		{
			if (height(t->left->left) - height(t->left->right) == 1)
				return singleRightRotate(t);
			else
				return doubleRightRotate(t);
		}
		return t;
	}
	Treenode *getroot()
	{
		return root;
	}
	/*Function used to print the data in inorder form*/
	void inorder(Treenode *temp)
	{
		if (temp == NULL)
			return;
		inorder(temp->left);
		cout << "\n		ID         : " << temp->id << endl;
		cout << "		KEY	   : " << temp->key << endl;
		cout << "		FILEPATH   : " << temp->filepath << endl;
		cout << "		LINENUMBER : " << temp->linenumber << endl;
		int i = 0;
		fstream object;
		string value;
		object.open(temp->filepath, ios::in);
		while (i < temp->linenumber)
		{
			getline(object, value);
			i++;
		}
		cout << "		Value	   : " << value << endl;
		inorder(temp->right);
	}
	/*Function used to get the data of whole tree in queue*/
	void getdata(Treenode *temp, queue<Treenode> &q)
	{
		if (root != nullptr)
		{
			if (temp == NULL)
				return;
			getdata(temp->left, q);
			q.enqueue(*temp);
			getdata(temp->right, q);
		}
	}
};
