#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>

template <class T> struct bintree {

	bintree<T> *parent;
	T data;
	int level;
	bintree<T> *left, *right;

};

template <class T> class bs_tree {

	bintree<T> *root;
	std::map<int, int> levelmap;

	bool delete_data(bintree<T> *node, T data);
	bool decrease_levelmap(int level);
	bintree<T> *create_node();

public:

	enum TRAVERSAL {
		VLR,
		LRV,
		LVR
	};

	bs_tree<T>();

	bool insert_data(T data);


	bool delete_data(T data);

	bintree<T> *get_inorder_successor(bintree<T> *node);

	void display_tree(int trav_type = TRAVERSAL::LVR);

	void display_tree(bintree<T> *temp, int trav_type = TRAVERSAL::LVR);

	size_t get_level();
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

template <class T> bs_tree<T>::bs_tree()
{
	root = nullptr;
}

template <class T> bool bs_tree<T>::decrease_levelmap(int level)
{
	if (levelmap.count(level) > 0)
	{
		if (levelmap[level] == 1)
		{
			levelmap.erase(level);
		}
		else
		{
			levelmap[level]--;
		}
		return true;
	}
	return false;
}

template <class T> bool bs_tree<T>::insert_data(T data)
{
	//If not used double pointer,  due to single pointer, the changes made on the address of tree gets removed after the function exits. Becoz i wud b changing the value of pointer if not for double pointer. which means
	//its like changing the value of a non pointer used data type (primitive). More info in the pic....
	bool done = false;
	int tempcounter = -1;
	if (root == nullptr)
	{
		root = create_node();
		root->data = data;
		
		tempcounter++;
		root->level = tempcounter;

		levelmap[0] = 1;

		done = true;
	}
	else
	{
		bintree<T> *tree = root;
		while (true)
		{
			tempcounter++;
			if (data < tree->data)
			{
				if (tree->left == nullptr)
				{
					tree->left = create_node();
					tree->left->data = data;
					tree->left->parent = tree;
					
					tempcounter++;
					tree->left->level = tempcounter;
					
					if (levelmap.count(tempcounter) == 0)
					{
						levelmap[tempcounter] = 1;
					}
					else
					{
						levelmap[tempcounter]++;
					}

					done = true;
					break;
				}
				else
				{
					tree = tree->left;
				}
			}
			else
			{
				if (tree->right == nullptr)
				{
					tree->right = create_node();
					tree->right->data = data;
					tree->right->parent = tree;

					tempcounter++;
					tree->right->level = tempcounter;

					if (levelmap.count(tempcounter) == 0)
					{
						levelmap[tempcounter] = 1;
					}
					else
					{
						levelmap[tempcounter]++;
					}

					done = true;
					break;
				}
				else
				{
					tree = tree->right;
				}
			}
		}
	}
	return done;
}

template <class T> bintree<T> *bs_tree<T>::create_node()
{
	bintree<T> *temp = new bintree<T>;
	
	temp->left = nullptr;
	temp->right = nullptr;
	temp->parent = nullptr;
	temp->level = -1;

	return temp;
}

template <class T> bool bs_tree<T>::delete_data(T data)
{
	return delete_data(root, data);
}

template <typename T> bool bs_tree<T>::delete_data(bintree<T> *node, T data)
{
	bintree<T> *walker = node;
	if (walker == nullptr) return false;
	if (walker->data == data)
	{
		if (walker->right == nullptr)
		{
			if (walker->left == nullptr)
			{
				if (walker->parent->left == walker)
				{
					walker->parent->left = nullptr;
				}
				else
				{
					walker->parent->right = nullptr;
				}
				if (levelmap[walker->level] == 1)
				{
					levelmap.erase(walker->level);
				}
				else
				{
					levelmap[walker->level]--;
				}
				delete walker;

				return true;
			}
			else
			{
				walker->data = walker->left->data;
				delete_data(walker->left, walker->left->data);

				return true;
			}
		}
		else
		{
			bintree<T> *del = get_inorder_successor(walker);
			if (del != nullptr)
			{
				walker->data = del->data;
				delete_data(del, del->data);

				return true;
			}
		}
		return false;
	}
	else
	{
		if (data < walker->data && walker->left != nullptr) delete_data(walker->left, data);
		else if (walker->right != nullptr) delete_data(walker->right, data);

		return true;
	}
}

template <class T> bintree<T> *bs_tree<T>::get_inorder_successor(bintree<T> *node)
{
	bintree<T> *orig = node, *newvar = nullptr;
	if (node == nullptr) return nullptr;
	if (node->left == nullptr && node->right == nullptr)
	{
		return nullptr;
	}
	while (node != nullptr)
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			newvar = node;
			break;
		}
		else
		{
			if (node->left != nullptr && node != orig)
			{
				if (node->left->left == nullptr)
				{
					newvar = node->left;
					break;
				}
				else
					node = node->left;
			}
			else if (node->right != nullptr && node == orig)
			{
				if (node->right->left != nullptr)
					node = node->right->left;
				else
				{
					newvar = node->right;
					break;
				}
			}
			else
			{
				newvar = node;
				break;
			}
		}
	}
	if (orig == newvar) return nullptr;
	return newvar;
}

template <class T> void bs_tree<T>::display_tree(int trav_type)
{
	display_tree(root, trav_type);
}

template <class T> void bs_tree<T>::display_tree(bintree<T> *temp, int trav_type)
{
	if (trav_type == TRAVERSAL::VLR)
	{
		if (temp != nullptr)
		{
			if (temp->parent != nullptr)
			{
				if (temp->parent->left == temp)
					std::cout << "Left child of Parent = " << temp->parent->data << "\tData = " << temp->data;
				else
					std::cout << "Right child of Parent = " << temp->parent->data << "\tData = " << temp->data;
			}
			else
			{
				std::cout << "Root, Data = " << temp->data;
			}
			std::cout << "\n";
			display_tree(temp->left, trav_type);
			display_tree(temp->right, trav_type);
		}
	}
	else if (trav_type == TRAVERSAL::LRV)
	{
		if (temp != nullptr)
		{
			display_tree(temp->left, trav_type);
			display_tree(temp->right, trav_type);
			if (temp->parent != nullptr)
			{
				if (temp->parent->left == temp)
					std::cout << "Left child of Parent = " << temp->parent->data << "\tData = " << temp->data;
				else
					std::cout << "Right child of Parent = " << temp->parent->data << "\tData = " << temp->data;
			}
			else
			{
				std::cout << "Root, Data = " << temp->data;
			}
			std::cout << "\n";
		}
	}
	else
	{
		if (temp != nullptr)
		{
			display_tree(temp->left, trav_type);
			if (temp->parent != nullptr)
			{
				if (temp->parent->left == temp)
					std::cout << "Left child of Parent = " << temp->parent->data << "\tData = " << temp->data;
				else
					std::cout << "Right child of Parent = " << temp->parent->data << "\tData = " << temp->data;
			}
			else
			{
				std::cout << "Root, Data = " << temp->data;
			}
			std::cout << "\n";
			display_tree(temp->right, trav_type);
		}
	}
}

template <class T> size_t bs_tree<T>::get_level()
{
	return levelmap.size() - 1; // - 1 because the size does not count from zero.
}