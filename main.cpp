#include <iostream>
#include "libs/Account.h"
#include "libs/BinaryFileHandler.h"
#include "libs/List.h"

using namespace std;

int main() {
    
    AccountBin accountBin;
    strcpy(accountBin.clientName, "John Doe");
    strncpy(accountBin.accountNumber, "123456789",10);
    //Account Type can only be 'A' or 'C' 
    accountBin.accountType = 'A';
    accountBin.numTransactions = 3;
    //Transactions can only be 'D' or 'C'
    accountBin.transactions = new Transaction[3];
    accountBin.transactions[0].type = 'C';
    accountBin.transactions[0].amountInCents = 10000;
    accountBin.transactions[1].type = 'D';
    accountBin.transactions[1].amountInCents = 70000;
    
    accountBin.transactions[2].type = 'C';
    accountBin.transactions[2].amountInCents = 10000;
   
    List<AccountBin> list;
    list.add(accountBin);
    string fileName = "account.hbt";

    writeAccountFile(accountBin, fileName.c_str());

    AccountBin accountFromBinaryFile = readAcccountFile(fileName.c_str());

    cout << accountFromBinaryFile.clientName << endl;
    
    for (int i = 0; i < 10; i++) {
        cout << accountFromBinaryFile.accountNumber[i];
    }

    cout << endl;
    cout << accountFromBinaryFile.accountType << endl;
    cout << accountFromBinaryFile.numTransactions << endl;
    
    for (int i = 0; i < accountFromBinaryFile.numTransactions; i++) {
        cout << accountFromBinaryFile.transactions[i].type << endl;
        cout << accountFromBinaryFile.transactions[i].amountInCents << endl;
    }

    generateReport(accountFromBinaryFile, "report.txt");

    return 0;
}