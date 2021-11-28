#include "AvlTree.h"
#include "RoutingTablesList.h"
#include <iostream>
using namespace std;

struct machineNode
{

	AVL *tree;
	RoutingTablesList<machineNode> *Routingtable;
	long long int machineid;

	machineNode *next;

	machineNode()
	{
		tree = new AVL;
		Routingtable = new RoutingTablesList<machineNode>;

		machineid = 0;
		next = nullptr;
	}
	machineNode(long long int ID)
	{
		tree = new AVL;
		Routingtable = new RoutingTablesList<machineNode>;

		machineid = ID;
		next = nullptr;
	}
};

class MachinesList //	circular linkedlist to store the machines every node is a machine
{
private:
	machineNode *head;
	long long int numberofnodes;
	long long int length;

public:
	MachinesList()
	{
		head = nullptr;
		numberofnodes = 0;
		length = 0;
	}
	void setlength(long long int length)
	{
		this->length = length;
	}
	machineNode *gethead()
	{
		return head;
	}
	/* searching if the give id machine is present or not */
	bool search(long long int id)
	{
		machineNode *temp = head;
		do
		{
			if (temp == NULL)
				return false;

			if (temp->machineid == id)
				return true;

			temp = temp->next;
		} while (temp != head);

		return false;
	}
	/* This function is used to insert the machine in circular linkedlist*/
	bool insert(long long int id)
	{
		machineNode *newnode = new machineNode{id};
		if (!search(id))
		{
			if (numberofnodes < length)
			{
				machineNode *current = head;

				if (current == nullptr)
				{
					newnode->next = newnode;
					head = newnode;
					numberofnodes++;
					return true;
				}
				else if (current->machineid >= newnode->machineid)
				{
					while (current->next != head)
						current = current->next;

					current->next = newnode;
					fstream object;
					newnode->next = head;
					head = newnode;
					numberofnodes++;
					return true;
				}
				else
				{
					while (current->next != head && current->next->machineid < newnode->machineid)
						current = current->next;

					newnode->next = current->next;
					current->next = newnode;
					numberofnodes++;
					return true;
				}
			}
			else
			{
				cout << "\nIdentifier space is full cant add more data" << endl;
				return false;
			}
		}
		else
		{
			cout << "\n\nThe machine is already present in the ring\n\n";
			return false;
		}
	}
	/*Function used for removal of machine from ring and to return its avl tree*/
	AVL *Remove(long long int id)
	{
		AVL *tree = nullptr;
		if (search(id))
		{
			machineNode *temp = head;
			if (temp != nullptr)
			{
				if (temp->machineid == id)
				{
					machineNode *temp2 = head;
					while (temp2->next != head)
					{
						temp2 = temp2->next;
					}
					head = temp->next;
					tree = temp->tree;
					delete temp;
					temp2->next = head;
					numberofnodes--;
				}
				else
				{
					machineNode *temp2 = head;
					while (temp->machineid != id)
					{
						temp = temp->next;
					}
					while (temp2->next->machineid != id)
					{
						temp2 = temp2->next;
					}
					if (temp2->next->next == head)
					{
						tree = temp->tree;
						temp2->next = head;
						numberofnodes--;
					}
					else
					{
						tree = temp->tree;
						temp2->next = temp->next;
						delete temp;
						numberofnodes--;
					}
				}
			}
		}
		else
		{
			cout << "\n\nYour specified machine is not present in ring\n\n";
			return nullptr;
		}
		return tree;
	}

	void Display()
	{
		machineNode *temp = head;
		if (temp == nullptr || numberofnodes == 0)
		{
			cout << "\nIdentifier space is empty\nEnter the machines first\n";
		}
		else
		{
			do
			{
				cout << "___________________________________________________________________\n\n";
				cout << "\nMachine Id : " << temp->machineid << endl;
				cout << "\nMachine Adress : " << temp << endl
					 << endl;
				cout << "Machine id " << temp->machineid << " Tree data : " << endl
					 << endl;
				temp->tree->display();
				cout << "\nRouting Table : \n";
				temp->Routingtable->Display();
				temp = temp->next;
			} while (temp != head);
		}
	}
};