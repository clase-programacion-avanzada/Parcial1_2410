#ifndef ACCOUNT_H
#define ACCOUNT_H

// =============================================================================
// Includes estándar - No se usan librerías personalizadas (List, TextFileHandler, etc.)
// Solo se usan las librerías estándar de C++ para manejo de archivos, cadenas y memoria
// =============================================================================
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

// =============================================================================
// BOILERPLATE: Estructuras de datos base para el parcial
// Estas estructuras representan los tipos de datos que se usan en el archivo .hbt
// =============================================================================

// Punto 1: Estructura que representa una transacción bancaria
// Según la especificación del archivo .hbt:
//   - type: 1 byte que indica el tipo de transacción ('C' para crédito, 'D' para débito)
//   - amountInCents: 8 bytes (long) que representan el monto en centavos
struct Transaction {
    char type;            // 1 byte: 'D' (débito) o 'C' (crédito)
    long amountInCents;   // 8 bytes: monto de la transacción en centavos
};

// Punto 2: Estructura que representa la información de una cuenta bancaria
// Según la especificación del archivo .hbt:
//   - clientName: 20 bytes para el nombre del cliente (rellenado con espacios si es más corto)
//   - accountNumber: 10 bytes para el número de cuenta (cadena de 10 dígitos)
//   - accountType: 1 byte ('A' = ahorros, 'C' = corriente)
//   - numTransactions: 1 byte indicando cuántas transacciones hay
//   - transactions: arreglo dinámico de transacciones (cada una de 9 bytes en el archivo)
struct AccountBin {
    char clientName[20];         // 20 bytes: nombre del cliente
    char accountNumber[10];      // 10 bytes: número de cuenta
    char accountType;            // 1 byte: tipo de cuenta ('A' o 'C')
    uint8_t numTransactions;     // 1 byte: cantidad de transacciones
    Transaction* transactions;   // Arreglo dinámico: 9 bytes por transacción en archivo
};

// =============================================================================
// SOLUCIÓN DEL PARCIAL: Funciones que implementan los puntos 3 a 6
// =============================================================================

// Punto 3: Función que lee un archivo binario .hbt y retorna una estructura AccountBin
// con toda la información del archivo.
// Parámetros:
//   - path: ruta al archivo .hbt (cadena de caracteres estilo C)
// Retorna: una estructura AccountBin con los datos leídos del archivo
AccountBin readAcccountFile(const char* path) {

    fstream file;

    file.open(path, ios::in | ios::binary);

    // Si el archivo no se puede abrir, retorna una estructura vacía
    if (file.fail()) {
        return AccountBin();
    }

    AccountBin accountBin;
    
    // Formato binario del archivo .hbt:
    // [20 bytes: nombre][10 bytes: cuenta][1 byte: tipo][1 byte: numTransacciones][9 bytes x N: transacciones]

    // Lee los primeros 20 bytes y los guarda en el campo clientName
    file.read(reinterpret_cast<char *>(&accountBin.clientName), 
        sizeof(accountBin.clientName));
    // Lee los siguientes 10 bytes y los guarda en el campo accountNumber
    file.read(reinterpret_cast<char *>(&accountBin.accountNumber), 
        sizeof(accountBin.accountNumber));
    // Lee el siguiente byte y lo guarda en el campo accountType
    file.read(reinterpret_cast<char *>(&accountBin.accountType), 
        sizeof(accountBin.accountType));
    // Lee el siguiente byte y lo guarda en el campo numTransactions
    file.read(reinterpret_cast<char *>(&accountBin.numTransactions), 
        sizeof(accountBin.numTransactions));

    // Crea un arreglo dinámico de transacciones con el tamaño indicado por numTransactions
    int size = (int) accountBin.numTransactions;
    accountBin.transactions = new Transaction[size];
    
    // Lee cada transacción (9 bytes cada una) y la guarda en el arreglo dinámico
    for (int i = 0; i < size; i++) {
        file.read(reinterpret_cast<char *>(&accountBin.transactions[i]), 
            sizeof(accountBin.transactions[i]));
    }

    file.close();

    return accountBin;
}


// Punto 4: Función que calcula la sumatoria de los montos de un tipo de transacción
// Recibe un arreglo dinámico de transacciones, el tamaño del arreglo y el tipo
// de transacción a sumar ('C' para créditos o 'D' para débitos).
// Parámetros:
//   - transactions: apuntador al arreglo dinámico de transacciones
//   - size: tamaño del arreglo
//   - typeOfTransaction: tipo de transacción a sumar ('C' o 'D')
// Retorna: la sumatoria de los montos del tipo de transacción indicado
long getBalanceOfTransaction(Transaction* transactions, 
    int size, 
    char typeOfTransaction) {
    
    long balance = 0;

    // Recorre el arreglo de transacciones y suma los montos del tipo indicado
    for (int i = 0; i < size; i++) {
        Transaction transaction = transactions[i];
        if (transaction.type == typeOfTransaction) {
            balance += transaction.amountInCents;
        } 
    }

    return balance;
}


// Punto 5: Función que calcula el saldo de la cuenta
// El saldo se calcula restando los débitos de los créditos, asumiendo saldo inicial de 0.
// Parámetros:
//   - transactions: apuntador al arreglo dinámico de transacciones
//   - size: tamaño del arreglo
// Retorna: el saldo de la cuenta (créditos - débitos) en centavos
long getBalance(Transaction* transactions, int size) {
    // Saldo = Créditos - Débitos (saldo inicial es 0)
    return getBalanceOfTransaction(transactions, size, 'C') 
         - getBalanceOfTransaction(transactions, size, 'D');
}


// Punto 6: Función que genera un reporte de la cuenta en un archivo de texto
// Escribe directamente al archivo usando fstream (sin dependencias externas).
// Parámetros:
//   - accountBin: referencia a la estructura AccountBin con los datos de la cuenta
//   - fileName: referencia a la ruta del archivo de texto donde se escribirá el reporte
// Retorna: true si el archivo se escribió correctamente, false si hubo error
bool generateReport(AccountBin& accountBin, string& fileName) {
    
    fstream file;

    file.open(fileName, ios::out);

    // Si el archivo no se puede abrir, retorna false
    if (file.fail()) {
        return false;
    }

    // Escribe el nombre del cliente
    string clientName = accountBin.clientName;
    file << "Cliente: " + clientName + "\n";

    // Escribe el número de cuenta
    string accountNumber = accountBin.accountNumber;
    file << "Cuenta: " + accountNumber + "\n";

    // Escribe el tipo de cuenta ('A' = Ahorros, 'C' = Corriente)
    string accountType = accountBin.accountType == 'A' ? "Ahorros" : "Corriente";
    file << "Tipo de cuenta: " + accountType + "\n";

    int totalTransactions = (int) accountBin.numTransactions;

    // Calcula y escribe el saldo total de la cuenta
    long balance = getBalance(accountBin.transactions, totalTransactions);
    file << "Total en cuenta: " + to_string(balance) + " pesos\n";

    // Calcula y escribe el total de créditos
    long totalCredits = getBalanceOfTransaction(accountBin.transactions, totalTransactions, 'C');
    file << "Total créditos: " + to_string(totalCredits) + " pesos\n";

    // Calcula y escribe el total de débitos
    long totalDebits = getBalanceOfTransaction(accountBin.transactions, totalTransactions, 'D');
    file << "Total débitos: " + to_string(totalDebits) + " pesos\n";
    
    // Escribe el número de transacciones procesadas
    file << "Número de Transacciones en Archivo: " + to_string(totalTransactions);
    file.close();

    return true;
}


// =============================================================================
// FUNCIÓN AUXILIAR (no es parte del parcial)
// Se usa en main.cpp para crear el archivo .hbt de prueba
// =============================================================================
bool writeAccountFile(AccountBin accountBin, const char* path) {
    fstream file;

    file.open(path, ios::out | ios::binary);

    if (file.fail()) {
        return false;
    }

    file.write(reinterpret_cast<char *>(&accountBin.clientName), sizeof(accountBin.clientName));
    file.write(reinterpret_cast<char *>(&accountBin.accountNumber), sizeof(accountBin.accountNumber));
    file.write(reinterpret_cast<char *>(&accountBin.accountType), sizeof(accountBin.accountType));
    file.write(reinterpret_cast<char *>(&accountBin.numTransactions), sizeof(accountBin.numTransactions));
    
    for (int i = 0; i < (int) accountBin.numTransactions; i++) {
        file.write(reinterpret_cast<char *>(&accountBin.transactions[i]), sizeof(Transaction));
    }

    file.close();

    return true;
}

#endif