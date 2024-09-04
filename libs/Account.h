#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <fstream>
#include "List.h"
#include "TextFileHandler.h"

using namespace std;

/*Punto 1*/
struct Transaction {
    char type;//1 byte, 'D' or 'C'
    long amountInCents; //8 bytes 
};

/*Punto 2*/
struct AccountBin {
    char clientName[20];//20 bytes
    char accountNumber[10];// 10 bytes
    char accountType;// 1 byte, 'A' or 'C'
    int numTransactions;// 4 bytes
    Transaction* transactions; //9 bytes * numTransactions
};


/*Punto 3*/
AccountBin readAcccountFile(const char* path) {

    fstream file;

    file.open(path, ios::in | ios::binary);

    if (file.fail()) {
        return AccountBin();
    }

    AccountBin accountBin;
    
    /*[20 bytes][10 bytes][1 byte][4 bytes][9 bytes][9 bytes][9 bytes]*/

    //Lee los primeros 20 bytes y los guarda en el campo clientName [20 bytes]
    file.read(reinterpret_cast<char *>(&accountBin.clientName), 
        sizeof(accountBin.clientName));
    //Lee los siguientes 10 bytes y los guarda en el campo accountNumber [10 bytes]
    file.read(reinterpret_cast<char *>(&accountBin.accountNumber), 
        sizeof(accountBin.accountNumber));
    //Lee el siguiente byte y lo guarda en el campo accountType [1 byte]
    file.read(reinterpret_cast<char *>(&accountBin.accountType), 
        sizeof(accountBin.accountType));
    //Lee los siguientes 4 bytes y los guarda en el campo numTransactions [4 bytes]
    file.read(reinterpret_cast<char *>(&accountBin.numTransactions), 
        sizeof(accountBin.numTransactions));

    //Crea un arreglo de transacciones con el tamaño numTransactions
    accountBin.transactions = new Transaction[accountBin.numTransactions];
    
    //Lee las siguientes 9 bytes * numTransactions y las guarda en el arreglo transactions
    for (int i = 0; i < accountBin.numTransactions; i++) {
        //Lee 9 bytes y los guarda en la posición i del arreglo transactions [9 bytes]
        file.read(reinterpret_cast<char *>(&accountBin.transactions[i]), 
            sizeof(accountBin.transactions[i]));
    }

    file.close();

    return accountBin;

};


/*Punto 4*/
long getBalanceOfTransaction(List<Transaction>& transactions, 
    char& typeOfTransaction) {
    
    long balance = 0;

    for (int i = 0; i < transactions.size; i++) {
        Transaction transaction = transactions.get(i);
        if (transaction.type == typeOfTransaction) {
            balance += transaction.amountInCents;
            
        } 
    }

    return balance;
}

long getBalanceOfTransaction(Transaction* transactions, 
    int size, 
    char typeOfTransaction) {
    long balance = 0;

    for (int i = 0; i < size; i++) {
        Transaction transaction = transactions[i];
        if (transaction.type == typeOfTransaction) {
            balance += transaction.amountInCents;
        } 
    }

    return balance;
}

/*Punto 5*/
long getBalance(List<Transaction> transactions) {
   
    return getBalanceOfTransaction(transactions, 'D') - getBalanceOfTransaction(transactions, 'C');
}

long getBalance(Transaction* transactions, int size) {
    
    return getBalanceOfTransaction(transactions, size, 'D') - getBalanceOfTransaction(transactions, size, 'C');
}

/*Punto 6*/
void generateReport(AccountBin& accountBin, string& fileName) {
   
    List<string> lines;

    string clientName = accountBin.clientName;
    lines.add("Cliente: " + clientName);

    string accountNumber = accountBin.accountNumber;
    lines.add("Cuenta: " + accountNumber);

    string accountType = accountBin.accountType == 'A' 
        ? "Ahorros" 
        : "Corriente";
    lines.add("Tipo de cuenta: " + accountType);

    long balance = getBalance(accountBin.transactions, accountBin.numTransactions);
    lines.add("Total en cuenta: " + to_string(balance));

    long totalCredits = getBalanceOfTransaction(accountBin.transactions, accountBin.numTransactions, 'C');
    lines.add("Total créditos: " + to_string(totalCredits));

    long totalDebits = getBalanceOfTransaction(accountBin.transactions, accountBin.numTransactions, 'D');
    lines.add("Total débitos: " + to_string(totalDebits));

    int totalTransactions = accountBin.numTransactions;
    lines.add("Número de Transacciones en Archivo: " + to_string(totalTransactions));

    TextFileHandler fileHandler(fileName);
    fileHandler.writeLines(lines);

}

bool generateReportWithoutFileHandler(AccountBin accountBin,string fileName) {
    
    fstream file;

    file.open(fileName, ios::out);

    if (file.fail()) {
        return false;
    }

    string clientName = accountBin.clientName;
    file << "Cliente: " + clientName + "\n";

    string accountNumber = accountBin.accountNumber;
    file << "Cuenta: " + accountNumber + "\n";

    string accountType = accountBin.accountType == 'A' ? "Ahorros" : "Corriente";
    file << "Tipo de cuenta: " + accountType + "\n";

    long balance = getBalance(accountBin.transactions, accountBin.numTransactions);
    file << "Total en cuenta: " + to_string(balance) + "\n";

    long totalCredits = getBalanceOfTransaction(accountBin.transactions, accountBin.numTransactions, 'C');
    file << "Total créditos: " + to_string(totalCredits) + "\n";

    long totalDebits = getBalanceOfTransaction(accountBin.transactions, accountBin.numTransactions, 'D');
    file << "Total débitos: " + to_string(totalDebits) + "\n";

    int totalTransactions = accountBin.numTransactions;
    
    file << "Número de Transacciones en Archivo: " + to_string(totalTransactions);
    file.close();

    return true;
}


struct Account {
    string clientName;
    string accountNumber;
    char accountType;
    List<Transaction> transactions;

};

Account convertAccountBinToAccount(AccountBin accountBin) {
    Account account;
    account.clientName = accountBin.clientName;
    account.accountNumber = accountBin.accountNumber;
    account.accountType = accountBin.accountType;
    
    for (int i = 0; i < accountBin.numTransactions; i++) {
        account.transactions.add(accountBin.transactions[i]);
    }
    return account;
}

void generateReportUsingAccount(AccountBin accountBin) {
    Account account = convertAccountBinToAccount(accountBin);
    List<string> lines;

    string clientName = account.clientName;
    lines.add("Cliente: " + clientName);

    string accountNumber = account.accountNumber;
    lines.add("Cuenta: " + accountNumber);

    string accountType = account.accountType == 'A' ? "Ahorros" : "Corriente";
    lines.add("Tipo de cuenta: " + accountType);

    long balance = getBalance(account.transactions);
    lines.add("Total en cuenta: " + to_string(balance));

    long totalCredits = getBalanceOfTransaction(account.transactions, 'C');
    lines.add("Total créditos: " + to_string(totalCredits));

    long totalDebits = getBalanceOfTransaction(account.transactions, 'D');

    lines.add("Total débitos: " + to_string(totalDebits));

    int totalTransactions = account.transactions.size;
    lines.add("Número de Transacciones en Archivo: " + to_string(totalTransactions));

    TextFileHandler fileHandler("report.txt");
    fileHandler.writeLines(lines);

}

/*Esto no es parte del parcial*/

bool writeAccountFile(AccountBin accountBin,const char* path) {
    fstream file;

    file.open(path, ios::out | ios::binary);

    if (file.fail()) {
        return false;
    }
        
    
    file.write(reinterpret_cast<char *>(&accountBin.clientName), sizeof(accountBin.clientName));
    file.write(reinterpret_cast<char *>(&accountBin.accountNumber), sizeof(accountBin.accountNumber));
    file.write(reinterpret_cast<char *>(&accountBin.accountType), sizeof(accountBin.accountType));
    file.write(reinterpret_cast<char *>(&accountBin.numTransactions), sizeof(accountBin.numTransactions));
    
    for (int i = 0; i < accountBin.numTransactions; i++) {
        file.write(reinterpret_cast<char *>(&accountBin.transactions[i]), sizeof(Transaction));
    }

    file.close();

    return true;
}

#endif