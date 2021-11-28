#include <iostream>
using namespace std;

template<class T>
struct QNODE
{
	T data;
	QNODE<T>* Next;
};

template<class T>
class queue
{
private:

	QNODE<T>* Front;
	QNODE<T>* Rear;

public:
	queue()
	{
		Front = nullptr;
		Rear = nullptr;
	}
	T front()
	{
		return Front->data;
	}
	T back()
	{
		return Rear->data;
	}
	void enqueue(T cc)
	{
		QNODE<T>* newnode = new QNODE<T>;
		newnode->data = cc;
		newnode->Next = nullptr;
		if (is_empty())
		{
			Front = newnode;
			Rear = newnode;
		}
		else
		{
			Rear->Next = newnode;
			Rear = newnode;
		}
	}
	T dequeue()
	{
		T data;
		if (!is_empty())
		{
			QNODE<T>* temp = Front;
			data = temp->data;
			Front = Front->Next;
			temp->Next = nullptr;
			if (Front == NULL)
				Rear = NULL;
		}
		return data;
	}

	bool is_empty()
	{
		bool flag = false;
		if (Front == nullptr && Rear == nullptr)
			flag = true;
		return flag;
	}

	void Display()
	{
		QNODE<T>* temp = Front;
		while (temp != nullptr)
		{
			cout << temp->data.id;
			temp = temp->Next;
		}
	}
	void Makeempty()
	{
		while (!is_empty())
		{
			dequeue();
		}
	}
};