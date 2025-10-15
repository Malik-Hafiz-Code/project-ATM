#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;
enum enATMMenueOptions {
    eQuickWithdraw=1,eNormalWithdraw = 2,
    eDeposit = 3, eCheckBalance = 4, eLogout = 5
};
enum enQuiWithdraw {
    e20 = 1, e50 = 2, e100 = 3, e200 = 4, e400 = 5, e600 = 6, e800 = 7, e1000 = 8, eExit = 9
};
const string ClientsFileName = "Clients.txt";
void ShowATMMenue();
void Login();
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    int AccountBalance;
};
sClient CurrentClient;
vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  
    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());  // erase() until positon and move to next word. 
    }
    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }
    return vString;
}
sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stoi(vClientData[4]);//cast string to double
    return Client;
}
string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}
vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}
bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
bool FindClientByAccountNumberAndPinCode(string AccountNumber,string PinCode,
    vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {
        if (C.AccountNumber == AccountNumber&&C.PinCode==PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}
vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite
    string DataLine;
    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
        }
        MyFile.close();
    }
    return vClients;
}
bool DepositBalanceToClientByAccountNumber( double Amount, vector <sClient>& vClients)
{
    char Answer = 'n';
    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        for (sClient& C : vClients)
        {
                C.AccountBalance += Amount;
                SaveCleintsDataToFile(ClientsFileName, vClients);
                CurrentClient.AccountBalance = C.AccountBalance;
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}
string ReadClientPinCode()
{
    string PinCode = "";
    cout << "\nPlease enter PinCode? ";
    cin >> PinCode;
    return PinCode;
}
void ShowDepositScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-----------------------------------\n";
    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    int Amount = 0;
    cout << "\nPlease enter a positive deposit amount? ";
    cin >> Amount;
    DepositBalanceToClientByAccountNumber(Amount, vClients);
}
void ShowNormalWithDrawScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tNormal Withdraw Screen";
    cout << "\n-----------------------------------\n";
    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    int Amount = 0;
    cout << "\nPlease enter an amount multiple of 5? ";
    cin >> Amount;
    while (Amount % 5 != 0) {
        cout << "\nPlease enter an amount multiple of 5? ";
        cin >> Amount;
    }
    while (Amount > CurrentClient.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : "
            <<CurrentClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> Amount;
    }
    DepositBalanceToClientByAccountNumber(Amount * -1, vClients);
}
void ShowBalanceScreen()
{
    cout << "\n-----------------------------------\n";
    cout << "\tCheck Balance Screen";
    cout << "\n-----------------------------------\n";
    cout << "\nyour Balance is: " << CurrentClient.AccountBalance << endl;
}
void GoBackToATMMenue()
{
    cout << "\n\nPress any key to go back to ATM Menue...";
    system("pause>0");
    ShowATMMenue();
}
short ReadATMMenueOption()
{
    cout << "Choose what do you want to do? [1 to 5]? ";
    short Choice;
    cin >> Choice;
    return Choice;
}
short ReadQuickWithdrawOption()
{
    cout << "Choose what do you want to do? [1 to 9]? ";
    short Choice;
    cin >> Choice;
    return Choice;
}
bool  LoadClientInfo(string AccountNumber, string PinCode,vector <sClient> vClient)
{
    sClient client;
    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, vClient, client)) {
        CurrentClient=client;
        return true;
    }
    else
        return false;
}
bool IsChoiceMoreThanBalance(int Amount) {
    return CurrentClient.AccountBalance < Amount;
}
void ShowQuickWithdrawScreen() {
    cout << "=====================================\n";
    cout << "\tQuick Withdraw\n";
    cout << "=====================================\n";
    cout << " [1] 20 \t[2] 50\n";
    cout << " [3] 100 \t[4] 200\n";
    cout << " [5] 400 \t[6] 600\n";
    cout << " [7] 800 \t[8] 1000\n";
    cout << " [9] Exit\n";
    cout << "=====================================\n";
    cout << "your Balance is: " << CurrentClient.AccountBalance << endl;
    int choice = ReadQuickWithdrawOption();
    if (choice == 9)
        GoBackToATMMenue();
    int arr[8] = { 20,50,100,200,400,600,800,1000 };
    int withdraw = arr[choice - 1];
    while (IsChoiceMoreThanBalance(withdraw)) {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : "
            << CurrentClient.AccountBalance << endl;
        cout << "Please enter another amount? ";
        choice = ReadQuickWithdrawOption();
        if (choice == 9)
            GoBackToATMMenue();
        withdraw = arr[choice - 1];
    }
    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(withdraw * -1, vClient);
}
void PerfromATMOption(enATMMenueOptions ATMOptions) {
    switch (ATMOptions) {
    case enATMMenueOptions::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToATMMenue();
        break;
    case enATMMenueOptions::eNormalWithdraw:
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToATMMenue();
        break;
    case enATMMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToATMMenue();
        break;
    case enATMMenueOptions::eCheckBalance:
        system("cls");
        ShowBalanceScreen();
        GoBackToATMMenue();
        break;
    case enATMMenueOptions::eLogout:
        system("cls");
        Login();
    }
}
void ShowATMMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tATM Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Quick Withdraw.\n";
    cout << "\t[2] Normal Withdraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] Check Balance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromATMOption(enATMMenueOptions(ReadATMMenueOption()));
}
void Login()
{
    vector <sClient> vClient = LoadCleintsDataFromFile(ClientsFileName);
    bool LoginFaild = false;
    string AccountNumber, PinCode;
    do
    {
        system("cls");
        cout << "\n---------------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n---------------------------------\n";
        if (LoginFaild)
        {
            cout << "Invlaid AccountNumber/PinCode!\n";
        }
        cout << "Enter AccountNumber? ";
        cin >> AccountNumber;
        cout << "Enter PinCode? ";
        cin >> PinCode;
        LoginFaild = !LoadClientInfo(AccountNumber,PinCode,vClient);
    } while (LoginFaild);
    ShowATMMenue();
}
int main() {
    Login();
    system("pause>0");
    return 0;
}
