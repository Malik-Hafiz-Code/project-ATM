#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;
const string ClientFileName = "Clients.txt";
void ShowMainMenu();
void ShowTransactionsMenu();
struct stClient
{
	string AccountNumber, Phone, Name, PinCode;
	double AccountBalance;
	bool MarkForDelete = false;
};
enum enMainMenuOptions
{
	Listclients = 1, AddNewCLient = 2,
	DeleteClient = 3, UpDateClient = 4,
	 FindClient = 5,Transaction=6, Exit = 7
};
enum enTransaction {
	Deposit=1,Withdraw=2,TotalBalance=3,MainMenu=4
};
vector <string> SplitString(string s, string sep = "#//#") {
	vector <string> vString;
	string sword = "";
	short pos = 0;
	while ((pos = s.find(sep)) != std::string::npos) {
		sword = s.substr(0, pos);
		if (sword != "") {
			vString.push_back(sword);
		}
		s.erase(0, pos + sep.length());
	}
	if (s != "") {
		vString.push_back(s);
	}
	return vString;
}
string ConvertRecordToLine(stClient client, string sep = "#//#") {
	string line = "";
	line += client.AccountNumber + sep;
	line += client.PinCode + sep;
	line += client.Name + sep;
	line += client.Phone + sep;
	line += to_string(client.AccountBalance);
	return line;
}
stClient ConvertLineToRecord(string s, string sep = "#//#") {
	vector <string> vString;
	stClient client;
	vString = SplitString(s, sep);
	client.AccountNumber = vString[0];
	client.PinCode = vString[1];
	client.Name = vString[2];
	client.Phone = vString[3];
	client.AccountBalance = stof(vString[4]);
	return client;
}
bool ClientExistByAccountNumber(string AccountNumber, string FileName) {
	vector <stClient> vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string line;
		stClient client;
		while (getline(MyFile, line)) {
			client = ConvertLineToRecord(line);
			if (client.AccountNumber == AccountNumber) {
				MyFile.close();
				return true;
			}
			vClient.push_back(client);
		}
		MyFile.close();
	}
	return false;
}
stClient ReadClients() {
	stClient client;
	cout << "enter AccountNumber: ";
	getline(cin >> ws, client.AccountNumber);
	while (ClientExistByAccountNumber(client.AccountNumber, ClientFileName)) {
		cout << "\nClient With [" << client.AccountNumber << "] already exists,"
			"enter another Account Number: ";
		getline(cin >> ws, client.AccountNumber);
	}
	cout << "enter PinCode: ";
	getline(cin, client.PinCode);
	cout << "enter Name: ";
	getline(cin, client.Name);
	cout << "enter Phone: ";
	getline(cin, client.Phone);
	cout << "enter AccountBalance: ";
	cin >> client.AccountBalance;
	return client;
}
vector <stClient> LoadDataClient(string FileName) {
	fstream MyFile;
	vector <stClient> vClient;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string line;
		stClient client;
		while (getline(MyFile, line)) {
			client = ConvertLineToRecord(line);
			vClient.push_back(client);
		}
		MyFile.close();
	}
	return vClient;
}
void PrintClientRecord(stClient client) {
	cout << "|" << setw(15) << left << client.AccountNumber;
	cout << "|" << setw(10) << left << client.PinCode;
	cout << "|" << setw(40) << left << client.Name;
	cout << "|" << setw(12) << left << client.Phone;
	cout << "|" << setw(12) << left << client.AccountBalance;
}
void ShowAllClientsScreen() {
	vector <stClient> vClients = LoadDataClient(ClientFileName);
	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
	cout << "________________________________________________________________";
	cout << "_____________________________________\n\n";
	cout << "|" << setw(15) << left << "Account Number";
	cout << "|" << setw(10) << left << "Pin Code";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "Phone";
	cout << "|" << setw(12) << left << "Account Balance";
	cout << "\n________________________________________________________________";
	cout << "_____________________________________\n\n";
	for (stClient client : vClients) {
		PrintClientRecord(client);
		cout << endl;
	}
	cout << "________________________________________________________________";
	cout << "_____________________________________\n\n";
}
void PrintClientCard(stClient client) {
	cout << "the following are the client details:\n";
	cout << "\nAccount Number : " << client.AccountNumber;
	cout << "\nPin Code       : " << client.PinCode;
	cout << "\nName           : " << client.Name;
	cout << "\nPhone          : " << client.Phone;
	cout << "\nAccount Balance: " << client.AccountBalance;
}
bool FindClientByAccountNumber(string AccountNumber, stClient& client,
	vector <stClient> vClient) {
	for (stClient c : vClient) {
		if (AccountNumber == c.AccountNumber) {
			client = c;
			return true;
		}
	}
	cout << "\nclient with Account Number (" << AccountNumber << ") is not found.";
	return false;
}
stClient ChangeClient(string AccountNumber) {
	stClient client;
	client.AccountNumber = AccountNumber;
	cout << "enter PinCode: ";
	getline(cin >> ws, client.PinCode);
	cout << "enter Name: ";
	getline(cin, client.Name);
	cout << "enter Phone: ";
	getline(cin, client.Phone);
	cout << "enter AccountBalance: ";
	cin >> client.AccountBalance;
	return client;
}
bool MarkForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClient) {
	for (stClient& c : vClient) {
		if (AccountNumber == c.AccountNumber) {
			c.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
vector <stClient> SaveClientDateToFile(string FileName, vector <stClient> vClient) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open()) {
		string DataLine;
		for (stClient c : vClient) {
			if (c.MarkForDelete == false) {
				DataLine = ConvertRecordToLine(c);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClient;
}
void AddDatalineToFile(string FileName, string stDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open()) {
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}
void AddNewClient() {
	stClient client;
	client = ReadClients();
	AddDatalineToFile(ClientFileName, ConvertRecordToLine(client));
}
void AddNewClients() {
	char add;
	do {
		//system("cls");
		cout << "adding new Clients:\n\n";
		AddNewClient();
		cout << "client added succesfully, do you wnat to add more Client? Y/N ";
		cin >> add;
	} while (toupper(add) == 'Y');
}
bool DeleteClients(string AccountNumber, vector <stClient>& vClient) {
	stClient client;
	char answer;
	if (FindClientByAccountNumber(AccountNumber, client, vClient)) {
		PrintClientCard(client);
		cout << "\nAre you sure do delete this Client Y/N ? ";
		cin >> answer;
		if (answer == 'Y' || answer == 'y') {
			MarkForDeleteByAccountNumber(AccountNumber, vClient);
			SaveClientDateToFile(ClientFileName, vClient);
			vClient = LoadDataClient(ClientFileName);
			cout << "\n\nclient delete successfully.";
			return true;
		}
	}
	else {
		return false;
	}
}
bool UpdateClients(string AccountNumber, vector <stClient>& vClient) {
	stClient client;
	char answer;
	if (FindClientByAccountNumber(AccountNumber, client, vClient)) {
		PrintClientCard(client);
		cout << "\nAre you sure do UpDate this Client Y/N ? ";
		cin >> answer;
		if (answer == 'Y' || answer == 'y') {
			for (stClient& c : vClient) {
				if (c.AccountNumber == AccountNumber) {
					c = ChangeClient(AccountNumber);
					break;
				}
			}
			SaveClientDateToFile(ClientFileName, vClient);
			cout << "\n\nclient UpDate successfully.";
			return true;
		}
	}
	else {
		cout << "\nclient with Account Number (" << AccountNumber << ") is not found.";
		return false;
	}
}
string ReadAccountNumber() {
	string AccountNumber;
	cout << "enter AccountNumber: ";
	cin >> AccountNumber;
	return AccountNumber;
}
void ShowDeleteClientsScreen() {
	cout << "\n\n---------------------------------------\n";
	cout << "\t\tDelete Clients Screen\n";
	cout << "\n---------------------------------------\n";
	vector <stClient> vClient = LoadDataClient(ClientFileName);
	string AccountNumber = ReadAccountNumber();
	DeleteClients(AccountNumber, vClient);
}
void ShowUpDateClientsScreen() {
	cout << "\n\n-----------------------------------------\n";
	cout << "\t\tUpDate Clients  Info Screen\n";
	cout << "\n------------------------------------------\n";
	vector <stClient> vClient = LoadDataClient(ClientFileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClients(AccountNumber, vClient);
}
void ShowAddNewClientsScreen() {
	cout << "\n\n--------------------------------------------\n";
	cout << "\t\tAdd New Clients Screen\n";
	cout << "\n--------------------------------------------\n";
	AddNewClients();
}
void ShowFindClientScreen() {
	cout << "\n\n-------------------------------------------\n";
	cout << "\t\tFind Clients Screen\n";
	cout << "\n-------------------------------------------\n";
	vector <stClient> vClient = LoadDataClient(ClientFileName);
	stClient client;
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, client, vClient))
		PrintClientCard(client);
	else
		cout << "\nclient with Account Number (" << AccountNumber << ") is not found.";
}
void ShowEndScreen() {
	cout << "\n\n-------------------------------------------\n";
	cout << "\t\tProgram Ends   :-)\n";
	cout << "\n-------------------------------------------\n";
}
void GoBackToMainMenu() {
	cout << "\n\nPress any key to go back to main menu....";
	system("pause>0");
	ShowMainMenu();
}
short ReadMainMenuOption() {
	short choice;
	cout << "choose what do you wnat to do [1 to 7] ? ";
	cin >> choice;
	return choice;
}
short ReadTransactionsOption() {
	short choice;
	cout << "choose what do you wnat to do [1 to 4] ? ";
	cin >> choice;
	return choice;
}
void GoBackToTransactionsMenu() {
	cout << "\n\nPress any key to go back to transactions menu....";
	system("pause>0");
	ShowTransactionsMenu();
}
void PrintTransactions(stClient client) {
	cout << "|" << setw(15) << left << client.AccountNumber;
	cout << "|" << setw(40) << left << client.Name;
	cout << "|" << setw(12) << left << client.AccountBalance;
}
bool DepositBalance(string AccountNumber, double amount, vector <stClient>& vClient) {
	char answer;
		cout << "\nAre you sure to sub AccountBalance Y/N ? ";
		cin >> answer;
		if (answer == 'Y' || answer == 'y') {
			for (stClient& c : vClient) {
				if (c.AccountNumber == AccountNumber) {
					c.AccountBalance += amount;
					SaveClientDateToFile(ClientFileName,vClient);
					cout << "\n\nDone Successfully, New balance is: " << c.AccountBalance;
					return true;
				}
			}
		}
	return false;
}
void ShowDepositeScreen() {
	cout << "\n\n-------------------------------------------\n";
	cout << "\t\tDeposite Screen\n";
	cout << "\n-------------------------------------------\n";
	stClient client;
	vector <stClient> vClient = LoadDataClient(ClientFileName);
	string AccountNumber = ReadAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, client, vClient)) {
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClientCard(client);
	double amount = 0;
	cout << "\n\nenter deposit amount: ";
	cin >> amount;
	DepositBalance(AccountNumber, amount, vClient);
}
void ShowWithdrawScreen() {
	cout << "\n\n-------------------------------------------\n";
	cout << "\t\tWithdraw Screen\n";
	cout << "\n-------------------------------------------\n";
	stClient client;
	vector <stClient> vClient = LoadDataClient(ClientFileName);
	string AccountNumber = ReadAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, client, vClient)) {
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClientCard(client);
	double amount = 0;
	cout << "\nenter withdraw amount: ";
	cin >> amount;
	while (amount > client.AccountBalance) {
		cout << "\namount exceeds the balance, you can withdraw up to: " <<
			client.AccountBalance << endl;
		cout << "\n\nenter another amount: ";
		cin >> amount;
	}
	DepositBalance(AccountNumber, amount*-1, vClient);
}
void ShowTotlaBalances() {
	vector <stClient> vClients = LoadDataClient(ClientFileName);
	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).\n";
	cout << "________________________________________________________________";
	cout << "_____________________________________\n\n";
	cout << "|" << setw(15) << left << "Account Number";
	cout << "|" << setw(40) << left << "Name";
	cout << "|" << setw(12) << left << "Account Balance";
	cout << "\n________________________________________________________________";
	cout << "_____________________________________\n\n";
	double TotalBalances = 0;
	for (stClient client : vClients) {
		PrintTransactions(client);
		TotalBalances += client.AccountBalance;
		cout << endl;
	}
	cout << "________________________________________________________________";
	cout << "_____________________________________\n\n";
	cout << "\t\t\t\t\tToatal Balances = " << TotalBalances << "\n\n";
}
void ShowTotalBalancesScreen() {
	ShowTotlaBalances();
}
void PerFromTransactionsMenuOption(enTransaction transactions) {
	switch (transactions) {
	case enTransaction::Deposit:
		system("cls");
		ShowDepositeScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransaction::Withdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransaction::TotalBalance:
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenu();
		break;
	case enTransaction::MainMenu:
		system("cls");
		ShowMainMenu();
		break;
	}
}
void PerFromMainMenuOption(enMainMenuOptions MainMenuOptions) {
	switch (MainMenuOptions) {
	case enMainMenuOptions::Listclients:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::AddNewCLient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::DeleteClient:
		system("cls");
		ShowDeleteClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::UpDateClient:
		system("cls");
		ShowUpDateClientsScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::FindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	case enMainMenuOptions::Transaction:
		system("cls");
		ShowTransactionsMenu();
		break;
	case enMainMenuOptions::Exit:
		system("cls");
		ShowEndScreen();
		break;
	}
}
void ShowTransactionsMenu() {
	system("cls");
	cout << "\n=====================================================";
	cout << "==============================\n\n";
	cout << "\t\t\t\t\tTransactions Menu Screen\n";
	cout << "\n=====================================================";
	cout << "==============================\n\n";
	cout << "\t [1] Deposite:\n";
	cout << "\t [2] WithDraw:\n";
	cout << "\t [3] TotalBalance:\n";
	cout << "\t [4] Main Menu:\n";
	cout << "\n=====================================================";
	cout << "==============================\n\n";
	PerFromTransactionsMenuOption(enTransaction(ReadTransactionsOption()));
}
void ShowMainMenu() {
	system("cls");
	cout << "\n=====================================================";
	cout << "==============================\n\n";
	cout << "\t\t\t\t\tMain Menu Screen\n";
	cout << "\n=====================================================";
	cout << "==============================\n\n";
	cout << "\t [1] Show Clients List:\n";
	cout << "\t [2] Add New Clients:\n";
	cout << "\t [3] Delete Clients:\n";
	cout << "\t [4] UpDate Clients:\n";
	cout << "\t [5] Find Clients:\n";
	cout << "\t [6] Transactions:\n";
	cout << "\t [7] Exit:\n";
	cout << "\n=====================================================";
	cout << "==============================\n\n";
	PerFromMainMenuOption(enMainMenuOptions(ReadMainMenuOption()));
}
int main() {
	ShowMainMenu();
	system("pause>0");
}