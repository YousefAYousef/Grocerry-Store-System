#include<iostream>
#include<fstream>
#include<Windows.h>
#include<string>
#include<vector>
using namespace std;
const float Banknotes[8] = { 200, 100, 50, 20, 10, 5, 1, 0.5 };

//Methods Decleration
vector<int> printMinBanknotes(float);
void viewInventory(); void viewReceipts();
int searchInventory(int); bool isIDUnique(int);
//void merge(int, int, int); void sortInventory(int l = 0, int r = inventory.size() - 1);

//Classes & Its Global Variables
int lastItemId = 1; //Item ID Counter 
class Item
{
public:
	int id;
	string name;
	float price;
	int quantity;
	//Constructor for Existing Items in File 
	Item(int id, string name, float price, int quantity = 1)
	{
		this->id = id;
		this->name = name;
		this->price = price;
		this->quantity = quantity;
	}
	//Display Item Info 
	void displayItem()
	{
		cout << "Item ID: " << id << "  Name: " << name << "  Price: " << price << "$" << "  Quantity: " << quantity << endl;
	}
};
vector<Item> inventory; //Inventory for all Items

int lastReceiptId = 1000; //Receipt ID Counter 
class Receipt
{
public:
	int id;
	int noOfItems;
	vector<Item> items;
	float discount = 0;
	double totalPrice = 0;

	//Default Constructor for new Receipts
	Receipt()
	{
		this->id = ++lastReceiptId;
		this->noOfItems = 0;
		this->discount = 0;
		this->totalPrice = 0;
	}
	//Constructor for Existing Receipts in File 
	Receipt(int id, float discount, double totalPrice)
	{
		this->id = id;
		this->discount = discount;
		this->totalPrice = totalPrice;
		this->noOfItems = 0;
	}

	//Add Item to Receipt
	void addItem(int itemIndex)
	{
		items.push_back(inventory[itemIndex]);
		noOfItems++;
	}
	//Remove Item from Receipt
	void RemoveItem(int itemIndex)
	{
		//Recipt has no items
		if(noOfItems == 0)
			return;
		//search and delete item
		items.erase(items.begin() + itemIndex);
		noOfItems--;
	}
	//Calculate the Total Price of Receipt
	double calcTotal()
	{
		for(int i = 0; i < items.size(); i++)
		{
			totalPrice += items[i].price;
		}
		totalPrice -= discount;
		return totalPrice;
	}
	//Display Item Info
	void displayReceipt(float paidAmount = 0)
	{
		cout << "Receipt ID: " << id << endl;
		cout << "Items: \n";
		for(int i = 0; i < items.size(); i++)
		{
			cout << "(" << i + 1 << ") " << items[i].id << " - " << items[i].name << " -Price: " << items[i].price << endl;
		}
		calcTotal();
		cout << "Discount: " << discount << "$" << endl;
		cout << "Total: " << totalPrice << "$" <<  endl;
		//if Receipt Confirmed Display Full Information
		if(paidAmount != 0)
		{
			cout << "Paid: " << paidAmount << "$" << endl;
			cout << "Exchange: " << paidAmount - totalPrice << "$" << endl;
			if((paidAmount - totalPrice) != 0)
			{
				cout << "Minimum number of Banknotes for Exchange:\n";
				vector<int> banknotesArr = printMinBanknotes(paidAmount - totalPrice);
				for(int i = 0; i < banknotesArr.size(); i++)
					cout << " " << banknotesArr[i];
			}
		}
		cout << "=============================================\n\n";
	}
};
vector<Receipt> receipts; //Receipts for all Receipts


//Load Data from DB Files
void loadItems()
{
	//Open Items File
	ifstream itemFile;
	itemFile.open("item.txt");
	//Temp Item Class Attributes
	int id;
	string name;
	float price;
	int quantity;
	//Get Items Info
	itemFile >> lastItemId;
	while(itemFile >> id)
	{		
		itemFile >> name;
		itemFile >> price;
		itemFile >> quantity;
		inventory.push_back(Item(id, name, price, quantity));
	}
	//Close File
	itemFile.close();
}
void loadReceipts()
{
	//Open Receipts File
	ifstream receiptFile;
	receiptFile.open("receipt.txt");
	//Temp Receipt Class Attributes
	int id;
	float discount;
	double totalPrice;
	int noOfItems;
	int itemIndex;
	//Get Receipts Info
	receiptFile >> lastReceiptId;
	while(receiptFile >> id)
	{
		receiptFile >> discount;
		receiptFile >> totalPrice;
		receipts.push_back(Receipt(id, discount, totalPrice));
		receiptFile >> noOfItems;
		for(int i = 0; i < noOfItems; i++)
		{
			receiptFile >> itemIndex;
			receipts.back().addItem(itemIndex);
		}
	}
	//Close File
	receiptFile.close();
}
//Save Data in DB Files
void saveItems()
{
	//Open Items File
	ofstream itemFile;
	itemFile.open("item.txt");

	//Save Items in File
	itemFile << lastItemId << endl;
	for(int i = 0; i <inventory.size();i++)
	{
		itemFile << inventory[i].id << " ";
		itemFile << inventory[i].name << " ";
		itemFile << inventory[i].price << " ";
		itemFile << inventory[i].quantity << "\n";
	}
	//Close File
	itemFile.close();
}
void saveReceipts()
{
	//Open Receipt File
	ofstream receiptFile;
	receiptFile.open("receipt.txt");

	//Save Receipts in File
	receiptFile << lastReceiptId << endl;
	for(int i = 0; i < inventory.size(); i++)
	{
		receiptFile << receipts[i].id << " ";
		receiptFile << receipts[i].discount << " ";
		receiptFile << receipts[i].totalPrice << " ";
		receiptFile << receipts[i].noOfItems << " ";
		for(int i = 0; i < receipts[i].noOfItems; i++)
			receiptFile << receipts[i].items[i].id << " ";
		cout << endl;
	}
	//Close File
	receiptFile.close();
}

//Greedy Algorithm 
//Print Minimum number Banknotes needed for Exchange
//Input: float (cash exchange value)
//Output: vector of int (minimum banknotes needed)
vector<int> printMinBanknotes(float cash)
{
	vector<int> banknotesArr;

	// Traverse through all Banknote values
	for(int i = 0; i < 8; i++)
	{
		// Find Maximum Banknotes valid
		while(cash >= Banknotes[i])
		{
			cash -= Banknotes[i];
			banknotesArr.push_back(Banknotes[i]);
		}
	}
	return banknotesArr;
}

//Brute-Force Algorithm
//View all Inventory Items
//Input: vector of Items (Inventory)
//Output: Print all Items Information
void viewInventory()
{
	if(inventory.empty())
		cout << "Inventory is Empty!\n\n";
	else
		for(int i = 0; i < inventory.size(); i++)
			inventory[i].displayItem();
}

//Brute-Force Algorithm
//View all Recipts & their Total Price
//Input: vector of Receipts
//Output: Print all Reciepts, Double (Total Price of receipts)
void viewReceipts()
{
	system("cls");
	if(receipts.empty())
		cout << "No Receipts yet!\n\n";
	else
	{
		double totalReceiptsPrice = 0;
		for(int i = 0; i < receipts.size(); i++)
		{
			receipts[i].displayReceipt();
			totalReceiptsPrice += receipts[i].totalPrice;
		}
		cout << "Total Price of Receipts = " << totalReceiptsPrice << "$" << endl;
		cout << "=============================================\n\n";
	}
	system("pause");
}

//Merge Sort (Divide & Conquer) Algorithm 
//Sort Inventory By Item ID 
//Input: vector of Items (Inventory), 3 int (index of first, middle, last elements in vector)
//Output: sorted vector of Items(Inventory) assendingly by item ID
void merge(int l, int m, int r)
{
	// Create temp vectors and their Sizes
	vector<Item> L;
	int Lsize = m - l + 1;
	vector<Item> R;
	int Rsize = r - m;

	// Copy data to temp vectors L and R
	for(int i = 0; i < Lsize; i++)
		L.push_back(inventory[l + i]);
	for(int j = 0; j < Rsize; j++)
		R.push_back(inventory[m + 1 + j]);

	// Index of L, R, Merged sub-vectors
	int i = 0, j = 0, k = l;

	while(i < Lsize && j < Rsize)
	{
		if(L[i].id <= R[j].id)
		{
			inventory[k] = L[i];
			i++;
		}
		else
		{
			inventory[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining objects of L and R sub-vectors
	while(i < Lsize)
	{
		inventory[k] = L[i];
		i++;
		k++;
	}
	while(j < Rsize)
	{
		inventory[k] = R[j];
		j++;
		k++;
	}
}
void sortInventory(int l = 0, int r = inventory.size() - 1)
{
	//Recursion Base Case
	if(l >= r)
		return;
	//Vector Middle Index
	int m = l + (r - l) / 2;
	//Sort Left Sub-Vector
	sortInventory(l, m);
	//Sort Right Sub-Vector
	sortInventory(m + 1, r);
	//Merge Sub-Vectors
	merge(l, m, r);
}

//Binary Search (Divide & Conquer) Algorithm 
//Search item in Inventory by ID
//Input: vector of Items (Inventory), int (itemId to search for)
//Output: int (itemIndex in Inventory)
int searchInventory(int itemId)
{
	sortInventory();

	int l = 0, r = inventory.size() - 1;
	while(l <= r)
	{
		int m = l + (r - l) / 2;

		// Check if item ID is present at mid
		if(inventory[m].id == itemId)
			return m;

		// If x greater, ignore left half
		if(inventory[m].id < itemId)
			l = m + 1;

		// If x is smaller, ignore right half
		else
			r = m - 1;
	}

	// if not found
	return -1;
}

//Transfer & Conquer Algorithm
//Check if Item ID is Unique in Inventory
//Input: vector of Items (Inventory), int (itemId)
//Output: bool (true if ID unique, else false)
bool isIDUnique(int itemId)
{
	sortInventory();
	for(int i = 0; i < inventory.size()-1; i++)
		if(inventory[i].id == itemId)
			return false;
	return true;
}

//Make New Purchase (New Receipt)
void makePurchase()
{
	Receipt newReceipt(0, 0, 0);
	int option = 0;
	do
	{
		//Make Purchase Menu UI
		system("cls");
		cout << "\n=============================================\n";
		cout << "============== Purchase Process =============\n";
		cout << "=============================================\n\n";
		//Display Receipt if not empty
		if(!newReceipt.items.empty())
			newReceipt.displayReceipt();
		cout << "\t1 - Add Item to Receipt\n\n";
		cout << "\t2 - Remove Item from Receipt\n\n";
		cout << "\t3 - Add Discount\n\n";
		cout << "\t4 - Confirm Purchase\n\n";
		cout << "\t0 - Cancel Purchase\n\n";
		cout << "\t---Enter Option Number--> ";
		cin >> option;
		cout << "\n=============================================\n\n";

		//Back to Menu
		if(option == 0)
			continue;
		//Add Item to Receipt
		else if(option == 1)
		{
			//Search for Item
			cout << "Item ID > ";
			int itemID;
			cin >> itemID;
			int itemIndex = searchInventory(itemID);

			//If Item not Found
			if(itemIndex == -1)
			{
				cout << "Item not Found!" << endl;
				//Pause Display
				system("pause");
				continue;
			}

			//Add to Receipt
			newReceipt.addItem(itemIndex);
		}
		//Remove Item from Receipt
		else if(option == 2)
		{
			//Search for Item
			cout << "Item Number in Receipt > ";
			int itemNum;
			cin >> itemNum;

			//If Item Number not in Receipt
			if(itemNum > newReceipt.items.size() || itemNum < 1)
			{
				cout << "Invalid Item Number! Please Choose from 1 to " << newReceipt.items.size() << endl;
				//Pause Display
				system("pause");

				continue;
			}

			//Remove from Receipt
			newReceipt.RemoveItem(itemNum - 1);
		}
		//Add Discount
		else if(option == 3)
		{
			cout << "Discount > ";
			float disc;
			cin >> disc;
			//Check if input valid
			if(disc < 0 || disc > newReceipt.totalPrice)
			{
				cout << "Invalid Discount Value!" << endl;
				//Pause Display
				system("pause");
				continue;
			}
			//Apply Discount to Receipt
			newReceipt.discount = disc;
		}
		//Confirm Purchase
		else if(option == 4)
		{
			//Get Customer Paid Amount
			cout << "Paid Amount > ";
			float paid;
			cin >> paid;
			//Check if paid is valid
			while(paid < newReceipt.totalPrice)
			{
				cout << "Insufficient Balance! Please Re-enter > ";
				cin >> paid;
			}

			system("cls");
			newReceipt.displayReceipt(paid);
			cout << "Thank You for Visiting Us!\n\n";
			receipts.push_back(newReceipt);
		}
		//Wrong Input
		else
		{
			cout << "Wrong Input! Please choose from 0 to 4" << endl;
			//Pause Display
			system("pause");
		}
	} while(option != 0 && option != 4);
	return;
}
//Add New Items in Inventory
void addInInventory()
{
	int option = 0;
	do
	{
		system("cls");
		cout << "\n=============================================\n";
		cout << "================== Add Item =================\n";
		cout << "=============================================\n\n";
		cout << "\t1 - Add New Item Information\n\n";
		cout << "\t2 - Add Quantity to Item\n\n";
		cout << "\t0 - Back to Main Menu\n\n";
		cout << "\t---Enter Option Number--> ";	
		cin >> option;
		cout << "\n=============================================\n\n";
		
		//Back to Menu
		if(option == 0)
			return;
		//Add New Item
		else if(option == 1)
		{
			cout << "New Item ID > ";
			int id;
			cin >> id;
			if(!isIDUnique(id))
			{
				cout << "Item Id is already Exist!" << endl;
				continue;
			}

			cout << "Name > ";
			string name;
			cin >> name;

			cout << "Price > ";
			float price;
			cin >> price;

			cout << "Quantity > ";
			int quantity;
			cin >> quantity;

			cout << "1-Save  0-Cancel  > ";
			int ch;
			cin >> ch;
			if(ch == 1)
			{			
				inventory.push_back(Item(id, name, price, quantity));
				cout << "Successfully Saved!" << endl;
				//Pause Display
				system("pause");
			}
		}
		//Add Quantity to Item
		else if(option == 2)
		{
			//Search for Item
			cout << "Item ID > ";
			int itemID;
			cin >> itemID;
			int itemIndex = searchInventory(itemID);

			//If Item not Found
			if(itemIndex == -1)
			{
				cout << "Item not Found!" << endl;
				//Pause Display
				system("pause");
				continue;
			}

			//Add Quantity to item
			cout << "New Quantity to Add > " << endl;
			int newQuantity;
			cin >> newQuantity;
			inventory[itemIndex].quantity += newQuantity;
		}
		//Wrong Input
		else
		{
			cout << "Wrong Input! Please choose from 0 to 2" << endl;
			//Pause Display
			system("pause");
		}
	} while(option != 0);
}
//View/Edit/Delete Items in Inventory
void manageInventory()
{
	int option = 0;
	do
	{
		//Manage Inventory Menu UI
		system("cls");
		cout << "\n=============================================\n";
		cout << "============== Manage Inventory =============\n";
		cout << "=============================================\n\n";
		cout << "\t1 - View all Items\n\n";
		cout << "\t2 - Edit Item\n\n";
		cout << "\t3 - Delete Item\n\n";
		cout << "\t0 - Back to Main Menu\n\n";
		cout << "\t---Enter Option Number--> ";
		cin >> option;
		cout << "\n=============================================\n\n";

		//Back to Menu
		if(option == 0)
			continue;
		//View Items
		else if(option == 1)
		{
			sortInventory();
			viewInventory();
			//Pause Display
			system("pause");
		}
		//Edit Item
		else if(option == 2)
		{
			//Search for Item
			cout << "Item ID > ";
			int itemID;
			cin >> itemID;
			int itemIndex = searchInventory(itemID);

			//If Item not Found
			if(itemIndex == -1)
			{
				cout << "Item not Found!" << endl;
				//Pause Display
				system("pause");

				continue;
			}
			cout << "\n============= Item Existing Data ============\n";
			inventory[itemIndex].displayItem();

			cout << "\n================ Item New Data ==============\n";
			cout << "Name > ";
			string name;
			cin >> name;

			cout << "Price > ";
			float price;
			cin >> price;

			cout << "Quantity > ";
			int quantity;
			cin >> quantity;

			cout << "\n1-Update  0-Cancel  > ";
			int ch;
			cin >> ch;
			if(ch == 1)
			{
				inventory[itemIndex].name = name;
				inventory[itemIndex].price = price;
				inventory[itemIndex].quantity = quantity;
				//update file
				cout << "Successfully Updated!" << endl;
				//Pause Display
				system("pause");
			}
		}
		//Delete Item
		else if(option == 3)
		{
			//Search for Item
			cout << "Item ID > ";
			int itemID;
			cin >> itemID;
			int itemIndex = searchInventory(itemID);

			//If Item not Found
			if(itemIndex == -1)
			{
				cout << "Item not Found!" << endl;
				//Pause Display
				system("pause");

				continue;
			}
			cout << "\n================== Item Data ================\n";
			inventory[itemIndex].displayItem();

			cout << "\n1-Delete  0-Cancel  > ";
			int ch;
			cin >> ch;
			if(ch == 1)
			{
				inventory.erase(inventory.begin() + itemIndex);
				//update file
				cout << "Successfully Deleted!" << endl;
				//Pause Display
				system("pause");
			}
		}
		//Wrong Input
		else
		{
			cout << "Wrong Input! Please choose from 0 to 3" << endl;
			//Pause Display
			system("pause");
		}

	} while(option != 0);
}

int main()
{
	//load data from files
	loadItems();
	
	int option = 0;
	do
	{
		//Main Menu UI
		system("cls");
		cout << "\n=================================\n";
		cout << "========== Grocery Store ==========\n";
		cout << "===================================\n\n";
		cout << "\t1 - Make Purchase\n\n";
		cout << "\t2 - Add New Item\n\n";
		cout << "\t3 - Manage Inventory\n\n";
		cout << "\t4 - View Receipts\n\n";
		cout << "\t0 - Exit\n\n";
		cout << "\t---Enter Option Number--> ";
		cin >> option;

		if(option == 1)
			makePurchase();
		else if(option == 2)
			addInInventory();
		else if(option == 3)
			manageInventory();
		else if(option == 4)
			viewReceipts();
		else if(option == 0)
		{
			cout << "\n=============================================\n";
			cout << "=================  Thank You  ===============\n";
			cout << "=============================================\n\n";
		}
		else
		{
			cout << "Wrong Input! Please choose from 0 to 4" << endl;
			//Pause Display
			system("pause");
		}
	} while(option != 0);
	
	//Save Data in files
	saveItems();
	saveReceipts();
	return 0;
}