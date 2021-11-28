#include <cmath>
#include "hash.h"
#include "MachinesList.h"

/* A function used to compute hash value of give string */
string ComputeShahash(string source)
{
	SHA1 object;
	object.update(source);
	string hash = object.final();
	transform(hash.begin(), hash.end(), hash.begin(), ::toupper);
	string newhash(hash, 26, 40);
	return newhash;
}

/* A function used to convert hexadecimal string to decimal value*/
long long int hexadecimalToDecimal(string hexVal)
{

	long long int len = hexVal.length();
	long long int base = 1;
	long long int dec_val = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		if (hexVal[i] >= '0' && hexVal[i] <= '9')
		{
			dec_val += (hexVal[i] - 48) * base;
			base = base * 16;
		}
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
		{
			dec_val += (hexVal[i] - 55) * base;
			base = base * 16;
		}
	}
	return dec_val;
}

/* Distributed Hash System class*/
class DHT
{

private:
	MachinesList *ring;
	int bits;
	long long int size;
	long long int count;

public:
	DHT()
	{
		ring = nullptr;
		bits = 0;
		size = 0;
		count = 0;
	}
	/* Function used to create identifier space*/
	void createidentifierspace()
	{
		cout << "Enter the number of bits of system : ";
		cin >> bits;
		cout << "\n\nIdentifier space of " << bits << " bits is created !!!!\n";
		size = pow(2, bits);
		ring = new MachinesList;
		ring->setlength(size);
	}
	/* Function used to add more than one machine*/
	void specifynumberofmachines(int nu)
	{
		while (nu < 0 || nu > size)
		{
			cout << "Enter the number in range from 1 to " << size << " : ";
			cin >> nu;
		}
		int option = 0;
		cout << "\n		1. Do you want to assign id manually \n";
		cout << "		2. Do you want to assign id automatically \n";
		cout << "\nEnter the option : ";
		cin >> option;
		while (option > 2 || option < 1)
		{
			cout << "\n\nEnter the correct option : ";
			cin >> option;
		}
		if (option == 1)
		{
			for (int i = 0; i < nu; i++)
			{
				long long int id;
				cout << "\nEnter the id of machine : ";
				cin >> id;
				while (id < 0 || id >= size)
				{
					cout << "Enter the valid id in range from 0 to " << size - 1 << " : ";
					cin >> id;
				}
				if (ring->insert(id))
				{
					cout << "\nMachine with id " << id << " is added in identifier space successfully!!!\n";
					constructroutingtables();
					count++;
				}
			}
		}
		else if (option == 2)
		{
			for (int i = 0; i < nu; i++)
			{
				long long int id;
				string ipaddress;
				cout << "\nEnter the ip address of machine : ";
				cin >> ipaddress;
				string hashvalue = ComputeShahash(ipaddress);
				long long int hashindecimal = hexadecimalToDecimal(hashvalue); //integer problem
				id = hashindecimal % size;
				if (ring->insert(id))
				{
					cout << "\nMachine with id " << id << " is added in identifier space successfully!!!\n";
					constructroutingtables();
					count++;
				}
			}
		}
	}
	/* A function used to add one machine*/
	void addmachine()
	{
		int option = 0;
		cout << "\n		1. Do you want to assign id manually \n";
		cout << "		2. Do you want to assign id automatically \n";
		cout << "\nEnter the option : ";
		cin >> option;
		while (option > 2 || option < 1)
		{
			cout << "\nEnter the correct option : ";
			cin >> option;
		}
		if (option == 1)
		{
			long long int id;
			cout << "\nEnter the id of machine : ";
			cin >> id;
			while (id < 0 || id >= size)
			{
				cout << "Enter the valid id in range from 0 to " << size - 1 << " : ";
				cin >> id;
			}
			if (ring->insert(id))
			{
				cout << "\nMachine with id " << id << " is added in identifier space successfully!!!\n";
				count++;
			}
		}
		else if (option == 2)
		{
			string ipaddress;
			cout << "\nEnter the ip address of machine : ";
			cin >> ipaddress;
			string hashvalue = ComputeShahash(ipaddress);
			long long int hashindecimal = hexadecimalToDecimal(hashvalue);
			long long int id = hashindecimal % size;
			if (ring->insert(id))
			{
				cout << "\nMachine with id " << id << " is added in identifier space successfully!!!\n";
				count++;
			}
		}
	}
	/* Function for search algorithm with routing tables*/
	machineNode *search(string key, int specifiedmachineid)
	{
		string hex = ComputeShahash(key);
		long long int hexd = hexadecimalToDecimal(hex);

		if (hexd < 0)
			hexd = -hexd;

		long long int id = hexd % size;

		cout << id << endl;

		bool found = false;

		machineNode *temp = ring->gethead();
		do
		{
			if (temp->machineid == specifiedmachineid)
			{
				found = true;
				break;
			}
			temp = temp->next;
		} while (temp != ring->gethead());

		machineNode *last = ring->gethead();
		do
		{
			last = last->next;
		} while (last->next != ring->gethead());

		if (found)
		{
			cout << "CURRENT MACHINE : " << temp->machineid << endl;
			cout << "LAST MACHINE : " << last->machineid << endl;
			cout << "PATH IS : \n";
			while (true)
			{
				cout << temp->machineid << endl;
				RoutingTableNode<machineNode> *rt = temp->Routingtable->gethead();
				if (id > last->machineid)
				{
					return ring->gethead();
				}
				else if (id < ring->gethead()->machineid)
				{
					return ring->gethead();
				}
				else if (temp->machineid == id)
				{
					cout << "first condition" << endl;
					return temp;
				}
				else if (temp->machineid < id && id <= rt->data->machineid)
				{
					cout << "second condition" << endl;
					temp = rt->data;
					if (temp->machineid > id)
					{
						return temp;
					}
				}
				else
				{
					bool flag = true;
					for (int i = 0; i < bits - 1; i++)
					{
						if (rt->data->machineid < id && id <= rt->next->data->machineid)
						{
							cout << "third condition" << endl;
							flag = false;
							temp = rt->data;
						}
						rt = rt->next;
					}
					if (flag)
					{
						cout << "fourth condition" << endl;
						temp = rt->data;
						if (temp->machineid > id)
						{
							return temp;
						}
					}
				}
			}
		}
		else
		{
			cout << "\n\nYour specified machine is not present in ring\n\n";
			return nullptr;
		}
		return temp;
	}
	/* Function used to add data in the form of key value pair*/
	void adddata(string key, string filepath, int linenumber, long long int specifiedmachineid)
	{
		if (ring->gethead() != nullptr)
		{
			string hex = ComputeShahash(key);
			long long int hexd = hexadecimalToDecimal(hex);

			if (hexd < 0)
				hexd = -hexd;

			long long int id = hexd % size;
			machineNode *searchvalue = search(key, specifiedmachineid);
			if (searchvalue != nullptr)
			{
				cout << "search pointer is at " << searchvalue->machineid << endl;
				searchvalue->tree->insert(id, key, filepath, linenumber);
				cout << "\nData Added in machine with id " << searchvalue->machineid << " successfully!!!\n";
			}
		}
		else
		{
			cout << "\nEnter the machine first then enter the data\n";
		}
	}
	/* Function used to redestribute data after addition or removal of machine*/
	void redestributedata()
	{
		machineNode *temp = ring->gethead();
		queue<Treenode> array;
		do
		{
			temp->tree->getdata(temp->tree->getroot(), array);
			temp->tree = new AVL;
			temp = temp->next;
		} while (temp != ring->gethead());

		while (!array.is_empty())
		{
			Treenode node = array.dequeue();
			string key = node.key;
			string value = node.filepath;
			long long int lin = node.linenumber;
			adddata(key, value, lin, ring->gethead()->machineid);
		}
	}
	/* Function used to remove machine*/
	queue<Treenode> removemachine(long long int id)
	{

		AVL *Tree = new AVL;
		Tree = ring->Remove(id);
		queue<Treenode> q;
		if (Tree == nullptr)
		{
			return q;
		}
		else
		{
			cout << "\n\nMachine is removed successfully!!!!!\n\n";
			Tree->getdata(Tree->getroot(), q);
		}
		return q;
	}
	/* Function used to add data again after removal of machine*/
	void adddataagainafterremoval(queue<Treenode> q)
	{
		while (!q.is_empty())
		{
			Treenode obj = q.dequeue();
			adddata(obj.key, obj.filepath, obj.linenumber, ring->gethead()->machineid);
		}
	}
	/*Function used to remove data*/
	void Removedata(string key, long long int specifiedid)
	{
		string hex = ComputeShahash(key);
		long long int hexd = hexadecimalToDecimal(hex);

		if (hexd < 0)
			hexd = -hexd;

		long long int id = hexd % size;
		machineNode *searchvalue = search(key, specifiedid);
		searchvalue->tree->remove(id);

		cout << "\nData Removed From machine with id " << searchvalue->machineid << " successfully!!!\n";
	}
	void printavltree(long long int id)
	{
		if (ring->search(id))
		{
			machineNode *temp = ring->gethead();
			do
			{
				temp = temp->next;
			} while (temp->machineid != id);
			temp->tree->display();
		}
		else
		{
			cout << "\n\nYour machine is not present in ring enter the valid id\n\n";
		}
	}
	void printroutingtable(long long int id)
	{
		if (ring->search(id))
		{
			machineNode *temp = ring->gethead();
			do
			{
				if (temp->machineid != id)
					temp = temp->next;
			} while (temp->machineid != id);
			temp->Routingtable->Display();
		}
		else
		{
			cout << "\n\nYour machine is not present in ring enter the valid id\n\n";
		}
	}
	/* Function used to find the successor or responsible machine of data id*/
	machineNode *Succ(long long int id)
	{
		bool flag = true, flag1 = true;
		machineNode *temp = ring->gethead();
		do
		{
			if (id <= temp->machineid)
			{
				flag = false;
				break;
			}
			temp = temp->next;
		} while (temp != ring->gethead());
		if (flag)
		{
			return ring->gethead();
		}
		return temp;
	}
	/*Function used to construct the routing tables*/
	void constructroutingtables()
	{
		machineNode *temp = ring->gethead();
		if (temp != nullptr)
		{
			do
			{
				temp->Routingtable = new RoutingTablesList<machineNode>;
				for (int i = 1; i <= bits; i++)
				{
					int formula = 0;
					formula = temp->machineid + pow(2, i - 1);
					formula = formula % size;
					temp->Routingtable->insert(Succ(formula));
				}
				temp = temp->next;
			} while (temp != ring->gethead());
		}
	}
	void menu()
	{
		cout << "\n\n\n";
		cout << "				WELCOME TO DISTRIBUTED HASH SYSTEMS\n\n\n";
		cout << "___________________________________________________________________\n\n";

		createidentifierspace();
		bool flag = true;

		while (flag)
		{
			int option = 0;
			cout << "___________________________________________________________________\n\n";

			cout << "	1. Specify number of machines\n";
			cout << "	2. Add Machines\n";
			cout << "	3. Add Data\n";
			cout << "	4. Remove Data\n";
			cout << "	5. Print routing table of any machine\n";
			cout << "	6. Print AVL tree of any machine\n";
			cout << "	7. Print Total Data\n";
			cout << "	8. Remove Machine\n";
			cout << "	9. Exit\n\n";
			cout << "Enter the option : ";
			cin >> option;
			while (option < 1 || option > 9)
			{
				cout << "\n\nEnter the correct option : ";
				cin >> option;
			}
			if (option == 1)
			{
				long long int nu;
				cout << "Enter the number of machines : ";
				cin >> nu;
				specifynumberofmachines(nu);
			}
			else if (option == 2)
			{
				addmachine();
				constructroutingtables();
				redestributedata();
			}
			else if (option == 3)
			{
				string key = "", value = "", file = "";
				long long int linenumber = 0;
				cout << "\nEnter the key : ";
				cin >> key;
				cout << "Enter the Filepath : ";
				cin >> file;
				cout << "Enter the linenumber : ";
				cin >> linenumber;
				cout << "Enter the value : ";
				cin >> value;
				cout << "Enter the machine id from where you want to insert data : ";
				long long int id;
				cin >> id;
				while (id < 0 || id >= size)
				{
					cout << "\nEnter the valid id in range 0 to " << size - 1 << " : ";
					cin >> id;
				}
				fstream object;
				object.open(file, ios::app);
				object << value << "\n";
				adddata(key, file, linenumber, id);
			}
			else if (option == 4)
			{
				string key = "";
				cout << "\nEnter the key : ";
				cin >> key;
				cout << "Enter the machine id from where you want to delete data : ";
				long long int id;
				cin >> id;
				while (id < 0 || id >= size)
				{
					cout << "\nEnter the valid id in range 0 to " << size - 1 << " : ";
					cin >> id;
				}
				Removedata(key, id);
			}
			else if (option == 5)
			{
				long long int id;
				cout << "Enter the id of machine : ";
				cin >> id;
				while (id < 0 || id >= size)
				{
					cout << "\nEnter the valid id in range 0 to " << size - 1 << " : ";
					cin >> id;
				}
				printroutingtable(id);
			}
			else if (option == 6)
			{
				long long int id;
				cout << "Enter the id of machine : ";
				cin >> id;
				while (id < 0 || id >= size)
				{
					cout << "\nEnter the valid id in range 0 to " << size - 1 << " : ";
					cin >> id;
				}
				printavltree(id);
			}
			else if (option == 7)
			{
				ring->Display();
			}
			else if (option == 8)
			{
				long long int id;
				cout << "Enter the id of machine : ";
				cin >> id;
				while (id < 0 || id >= size)
				{
					cout << "\nEnter the valid id in range 0 to " << size - 1 << " : ";
					cin >> id;
				}
				queue<Treenode> q = removemachine(id);
				constructroutingtables();
				adddataagainafterremoval(q);
			}
			else if (option == 9)
			{
				flag = false;
			}
		}
	}
};