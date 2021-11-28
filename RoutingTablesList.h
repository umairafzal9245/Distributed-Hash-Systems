#include <iostream>
using namespace std;

template <class T>
struct RoutingTableNode
{
	T *data;
	RoutingTableNode<T> *next;
	RoutingTableNode<T> *previous;

	RoutingTableNode()
	{
		data = nullptr;
		next = nullptr;
		previous = nullptr;
	}
	RoutingTableNode(T *dat)
	{
		data = dat;
		next = nullptr;
		previous = nullptr;
	}
};

/*Doubly linkedlist to store the address of each machine of ring and to construct the routing tables*/
template <class T>
class RoutingTablesList
{
private:
	RoutingTableNode<T> *head;
	long long int items;

public:
	RoutingTablesList()
	{
		head = nullptr;
		items = 0;
	}
	RoutingTableNode<T> *gethead()
	{
		return head;
	}
	void insert(T *data)
	{
		RoutingTableNode<T> *newnode = new RoutingTableNode<T>;
		newnode->data = data;
		newnode->next = nullptr;
		newnode->previous = nullptr;

		if (head == nullptr)
		{
			head = newnode;
			items++;
		}
		else
		{
			RoutingTableNode<T> *temp = head;
			while (temp->next != nullptr)
			{
				temp = temp->next;
			}
			temp->next = newnode;
			newnode->previous = temp;
			items++;
		}
	}

	void Remove(T *data)
	{
		RoutingTableNode<T> *temp = head;

		if (temp != nullptr)
		{
			if (temp->data == data)
			{
				head = temp->next;
				temp->next->previous = nullptr;
				temp->next = nullptr;
				delete temp;
			}
			else
			{
				while (temp->data != data)
				{
					temp = temp->next;
				}
				if (temp->next == nullptr)
				{
					temp->previous->next = nullptr;
					temp->previous = nullptr;
					delete temp;
				}
				else
				{
					temp->previous->next = temp->next;
					temp->next->previous = temp->previous;
					temp->next = nullptr;
					temp->previous = nullptr;
					delete temp;
				}
			}
		}
	}
	void Display()
	{
		RoutingTableNode<T> *temp = head;
		if (temp == nullptr)
		{
			cout << "list is empty\n\n";
		}
		else
		{
			long long int number = 1;
			while (temp != nullptr)
			{
				cout << "\t\t" << number << ". " << temp->data << "	ID : " << temp->data->machineid << endl;
				number++;
				temp = temp->next;
			}
		}
	}
};
