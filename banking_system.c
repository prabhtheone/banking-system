/*
 * ============================================================
 *  SIMPLE BANKING SYSTEM (Console, C language)
 * ============================================================
 *  Features:
 *    - Create account
 *    - Deposit money
 *    - Withdraw money
 *    - Check balance
 *    - Transfer money between accounts
 *    - View transaction history
 *    - List all accounts
 *    - Delete account
 *    - Persistent storage using binary files (accounts.dat, transactions.dat)
 *
 *  Compile:
 *    gcc banking_system.c -o banking_system
 *
 *  Run:
 *    ./banking_system
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ACCOUNTS_FILE "accounts.dat"
#define TRANSACTIONS_FILE "transactions.dat"
#define MAX_NAME_LEN 50
#define MAX_TXN_DESC 100
#define MIN_BALANCE 0.0
#define PIN_BUF_LEN 16   /* room for up to a 4-digit PIN + newline + null, safely */

/* ---------------------------- Data Structures ---------------------------- */

typedef struct {
    int accountNumber;
    char name[MAX_NAME_LEN];
    char pin[5];          /* simple 4-digit PIN stored as string */
    double balance;
    int active;            /* 1 = active, 0 = deleted */
} Account;

typedef struct {
    int accountNumber;
    char type[20];          /* DEPOSIT, WITHDRAW, TRANSFER_IN, TRANSFER_OUT */
    double amount;
    double balanceAfter;
    char timestamp[26];
    char description[MAX_TXN_DESC];
} Transaction;

/* ---------------------------- Utility Functions --------------------------- */

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Reads a line into buf (up to bufSize-1 chars), strips the trailing
 * newline, and drains any extra characters left on the input line so
 * stdin never gets desynced (this matters even for short fixed-width
 * fields like a 4-digit PIN, where fgets alone can leave '\n' behind). */
void readLine(char *buf, size_t bufSize) {
    if (fgets(buf, (int)bufSize, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        /* line was longer than the buffer; discard the rest */
        clearInputBuffer();
    }
}

void getTimestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

void pause_screen(void) {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
}

/* ---------------------------- File Handling -------------------------------- */

int getNextAccountNumber(void) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    int maxAcc = 1000; /* accounts start at 1001 */
    Account acc;
    if (fp != NULL) {
        while (fread(&acc, sizeof(Account), 1, fp) == 1) {
            if (acc.accountNumber > maxAcc) {
                maxAcc = acc.accountNumber;
            }
        }
        fclose(fp);
    }
    return maxAcc + 1;
}

int findAccount(int accNo, Account *result) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    if (fp == NULL) return 0;

    Account acc;
    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.accountNumber == accNo && acc.active) {
            *result = acc;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int updateAccount(Account updated) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb+");
    if (fp == NULL) return 0;

    Account acc;
    long pos;
    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.accountNumber == updated.accountNumber) {
            pos = ftell(fp) - (long)sizeof(Account);
            fseek(fp, pos, SEEK_SET);
            fwrite(&updated, sizeof(Account), 1, fp);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void logTransaction(int accNo, const char *type, double amount, double balanceAfter, const char *desc) {
    FILE *fp = fopen(TRANSACTIONS_FILE, "ab");
    if (fp == NULL) return;

    Transaction txn;
    txn.accountNumber = accNo;
    strncpy(txn.type, type, sizeof(txn.type) - 1);
    txn.type[sizeof(txn.type) - 1] = '\0';
    txn.amount = amount;
    txn.balanceAfter = balanceAfter;
    getTimestamp(txn.timestamp, sizeof(txn.timestamp));
    strncpy(txn.description, desc, sizeof(txn.description) - 1);
    txn.description[sizeof(txn.description) - 1] = '\0';

    fwrite(&txn, sizeof(Transaction), 1, fp);
    fclose(fp);
}

/* ---------------------------- Core Banking Operations ---------------------- */

void createAccount(void) {
    Account acc;
    acc.accountNumber = getNextAccountNumber();
    acc.active = 1;

    printf("\n=== Create New Account ===\n");
    printf("Enter full name: ");
    readLine(acc.name, MAX_NAME_LEN);

    char pinInput[PIN_BUF_LEN];
    printf("Set a 4-digit PIN: ");
    readLine(pinInput, sizeof(pinInput));
    strncpy(acc.pin, pinInput, sizeof(acc.pin) - 1);
    acc.pin[sizeof(acc.pin) - 1] = '\0';

    printf("Enter initial deposit amount: ");
    if (scanf("%lf", &acc.balance) != 1 || acc.balance < MIN_BALANCE) {
        printf("Invalid amount. Initial balance set to 0.\n");
        acc.balance = 0.0;
        clearInputBuffer();
    }

    FILE *fp = fopen(ACCOUNTS_FILE, "ab");
    if (fp == NULL) {
        printf("Error: could not open accounts file.\n");
        return;
    }
    fwrite(&acc, sizeof(Account), 1, fp);
    fclose(fp);

    if (acc.balance > 0) {
        logTransaction(acc.accountNumber, "DEPOSIT", acc.balance, acc.balance, "Initial deposit");
    }

    printf("\nAccount created successfully!\n");
    printf("Your account number is: %d\n", acc.accountNumber);
    printf("Please save this number, you'll need it to log in.\n");
    pause_screen();
}

int authenticate(Account *acc) {
    int accNo;
    char pin[PIN_BUF_LEN];

    printf("Enter account number: ");
    if (scanf("%d", &accNo) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return 0;
    }
    clearInputBuffer();

    if (!findAccount(accNo, acc)) {
        printf("Account not found.\n");
        return 0;
    }

    printf("Enter PIN: ");
    readLine(pin, sizeof(pin));

    if (strcmp(pin, acc->pin) != 0) {
        printf("Incorrect PIN.\n");
        return 0;
    }
    return 1;
}

void depositMoney(Account *acc) {
    double amount;
    printf("\nEnter amount to deposit: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }

    acc->balance += amount;
    updateAccount(*acc);
    logTransaction(acc->accountNumber, "DEPOSIT", amount, acc->balance, "Cash deposit");

    printf("Deposit successful. New balance: %.2f\n", acc->balance);
}

void withdrawMoney(Account *acc) {
    double amount;
    printf("\nEnter amount to withdraw: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }

    if (amount > acc->balance) {
        printf("Insufficient balance. Current balance: %.2f\n", acc->balance);
        return;
    }

    acc->balance -= amount;
    updateAccount(*acc);
    logTransaction(acc->accountNumber, "WITHDRAW", amount, acc->balance, "Cash withdrawal");

    printf("Withdrawal successful. New balance: %.2f\n", acc->balance);
}

void checkBalance(Account *acc) {
    printf("\n--- Account Summary ---\n");
    printf("Account Number : %d\n", acc->accountNumber);
    printf("Name           : %s\n", acc->name);
    printf("Balance        : %.2f\n", acc->balance);
}

void transferMoney(Account *sender) {
    int targetAcc;
    double amount;
    Account receiver;

    printf("\nEnter recipient account number: ");
    if (scanf("%d", &targetAcc) != 1) {
        clearInputBuffer();
        printf("Invalid input.\n");
        return;
    }

    if (targetAcc == sender->accountNumber) {
        printf("You cannot transfer to your own account.\n");
        return;
    }

    if (!findAccount(targetAcc, &receiver)) {
        printf("Recipient account not found.\n");
        return;
    }

    printf("Enter amount to transfer: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("Invalid amount.\n");
        clearInputBuffer();
        return;
    }

    if (amount > sender->balance) {
        printf("Insufficient balance.\n");
        return;
    }

    sender->balance -= amount;
    receiver.balance += amount;

    updateAccount(*sender);
    updateAccount(receiver);

    char desc[MAX_TXN_DESC];
    snprintf(desc, sizeof(desc), "Transfer to account %d", targetAcc);
    logTransaction(sender->accountNumber, "TRANSFER_OUT", amount, sender->balance, desc);

    snprintf(desc, sizeof(desc), "Transfer from account %d", sender->accountNumber);
    logTransaction(receiver.accountNumber, "TRANSFER_IN", amount, receiver.balance, desc);

    printf("Transfer successful. New balance: %.2f\n", sender->balance);
}

void viewTransactionHistory(int accNo) {
    FILE *fp = fopen(TRANSACTIONS_FILE, "rb");
    if (fp == NULL) {
        printf("\nNo transaction history found.\n");
        return;
    }

    Transaction txn;
    int found = 0;

    printf("\n--- Transaction History for Account %d ---\n", accNo);
    printf("%-20s %-15s %-12s %-12s %s\n", "Timestamp", "Type", "Amount", "Balance", "Description");
    printf("--------------------------------------------------------------------------\n");

    while (fread(&txn, sizeof(Transaction), 1, fp) == 1) {
        if (txn.accountNumber == accNo) {
            printf("%-20s %-15s %-12.2f %-12.2f %s\n",
                   txn.timestamp, txn.type, txn.amount, txn.balanceAfter, txn.description);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No transactions yet.\n");
    }
}

void listAllAccounts(void) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    if (fp == NULL) {
        printf("\nNo accounts found.\n");
        return;
    }

    Account acc;
    int found = 0;

    printf("\n--- All Accounts ---\n");
    printf("%-15s %-25s %-12s\n", "Account No", "Name", "Balance");
    printf("---------------------------------------------\n");

    while (fread(&acc, sizeof(Account), 1, fp) == 1) {
        if (acc.active) {
            printf("%-15d %-25s %-12.2f\n", acc.accountNumber, acc.name, acc.balance);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No active accounts.\n");
    }
}

void deleteAccount(Account *acc) {
    char confirm[10];
    printf("\nAre you sure you want to delete account %d? This cannot be undone. (yes/no): ", acc->accountNumber);
    scanf("%9s", confirm);
    clearInputBuffer();

    if (strcmp(confirm, "yes") != 0) {
        printf("Deletion cancelled.\n");
        return;
    }

    acc->active = 0;
    updateAccount(*acc);
    printf("Account %d has been deleted.\n", acc->accountNumber);
}

/* ---------------------------- Menus ---------------------------------------- */

void accountMenu(Account acc) {
    int choice;
    int loggedIn = 1;

    while (loggedIn) {
        /* refresh account data each loop in case balance changed */
        findAccount(acc.accountNumber, &acc);

        printf("\n========== Account Menu (Acc #%d) ==========\n", acc.accountNumber);
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer Money\n");
        printf("5. Transaction History\n");
        printf("6. Delete Account\n");
        printf("7. Logout\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) {
                printf("\nInput closed. Exiting.\n");
                return;
            }
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: checkBalance(&acc); break;
            case 2: depositMoney(&acc); break;
            case 3: withdrawMoney(&acc); break;
            case 4: transferMoney(&acc); break;
            case 5: viewTransactionHistory(acc.accountNumber); break;
            case 6:
                deleteAccount(&acc);
                loggedIn = 0;
                break;
            case 7:
                printf("Logging out...\n");
                loggedIn = 0;
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }

        if (loggedIn) pause_screen();
    }
}

void mainMenu(void) {
    int choice;
    int running = 1;

    while (running) {
        printf("\n================ BANKING SYSTEM ================\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. List All Accounts\n");
        printf("4. Exit\n");
        printf("==================================================\n");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            if (feof(stdin)) {
                printf("\nInput closed. Exiting.\n");
                return;
            }
            clearInputBuffer();
            printf("Invalid input.\n");
            continue;
        }
        clearInputBuffer();

        Account acc;
        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                if (authenticate(&acc)) {
                    printf("\nWelcome, %s!\n", acc.name);
                    accountMenu(acc);
                } else {
                    pause_screen();
                }
                break;
            case 3:
                listAllAccounts();
                pause_screen();
                break;
            case 4:
                printf("Thank you for using the Banking System. Goodbye!\n");
                running = 0;
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

int main(void) {
    printf("Welcome to the C Banking System\n");
    mainMenu();
    return 0;
}
