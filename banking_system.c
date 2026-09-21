/*
 * ============================================================
 *  SIMPLE BANKING SYSTEM (Console, C language)
 * ============================================================
 *  Educational console banking application demonstrating:
 *    - Account creation and authentication
 *    - Deposits, withdrawals and transfers
 *    - Transaction history
 *    - Persistent binary-file storage
 *    - Defensive input validation and error handling
 *
 *  NOTE: This is an educational project, not production banking
 *  software. PINs are stored locally and are not cryptographically
 *  protected.
 * ============================================================
 */

#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ACCOUNTS_FILE "accounts.dat"
#define TRANSACTIONS_FILE "transactions.dat"
#define MAX_NAME_LEN 50
#define MAX_TXN_DESC 100
#define PIN_LEN 4
#define PIN_BUF_LEN 16
#define MAX_INPUT_LEN 128
#define ACCOUNT_YEAR 2026
#define ACCOUNT_ID_LEN 16
#define MIN_TRANSACTION_AMOUNT 0.01
#define MAX_TRANSACTION_AMOUNT 1000000000.0

typedef struct {
    char accountNumber[ACCOUNT_ID_LEN];
    char name[MAX_NAME_LEN];
    char pin[PIN_LEN + 1];
    double balance;
    int active; /* 1 = active, 0 = deleted */
} Account;

typedef struct {
    char accountNumber[ACCOUNT_ID_LEN];
    char type[20];
    double amount;
    double balanceAfter;
    char timestamp[26];
    char description[MAX_TXN_DESC];
} Transaction;

void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void readLine(char *buf, size_t bufSize) {
    if (buf == NULL || bufSize == 0) return;
    if (fgets(buf, (int)bufSize, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        clearInputBuffer();
    }
}

int isBlank(const char *text) {
    if (text == NULL) return 1;
    while (*text != '\0') {
        if (!isspace((unsigned char)*text)) return 0;
        text++;
    }
    return 1;
}

int isValidName(const char *name) {
    if (name == NULL || isBlank(name)) return 0;
    for (const unsigned char *p = (const unsigned char *)name; *p != '\0'; p++) {
        if (iscntrl(*p)) return 0;
    }
    return 1;
}

int readInt(const char *prompt, int *value) {
    char input[MAX_INPUT_LEN];
    char *end;
    long parsed;

    printf("%s", prompt);
    readLine(input, sizeof(input));
    if (input[0] == '\0') return 0;

    errno = 0;
    parsed = strtol(input, &end, 10);
    while (isspace((unsigned char)*end)) end++;
    if (errno != 0 || *end != '\0' || parsed < INT_MIN || parsed > INT_MAX) return 0;

    *value = (int)parsed;
    return 1;
}

int readAmount(const char *prompt, double *amount) {
    char input[MAX_INPUT_LEN];
    char *end;
    double parsed;

    printf("%s", prompt);
    readLine(input, sizeof(input));
    if (input[0] == '\0') return 0;

    errno = 0;
    parsed = strtod(input, &end);
    while (isspace((unsigned char)*end)) end++;
    if (errno != 0 || *end != '\0' || !isfinite(parsed) ||
        parsed < MIN_TRANSACTION_AMOUNT || parsed > MAX_TRANSACTION_AMOUNT) return 0;

    *amount = parsed;
    return 1;
}

int isValidPin(const char *pin) {
    if (pin == NULL || strlen(pin) != PIN_LEN) return 0;
    for (size_t i = 0; i < PIN_LEN; i++) {
        if (!isdigit((unsigned char)pin[i])) return 0;
    }
    return 1;
}

void pauseScreen(void) {
    char input[4];
    printf("\nPress Enter to continue...");
    readLine(input, sizeof(input));
}

int getNextAccountNumber(char *accountId, size_t accountIdSize) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    int maxSequence = 0;
    Account acc;

    if (accountId == NULL || accountIdSize == 0) return 0;

    if (fp != NULL) {
        while (fread(&acc, sizeof(acc), 1, fp) == 1) {
            int year;
            int sequence;
            if (sscanf(acc.accountNumber, "%d/%d", &year, &sequence) == 2 &&
                year == ACCOUNT_YEAR && sequence > maxSequence) {
                maxSequence = sequence;
            }
        }
        fclose(fp);
    }

    if (maxSequence == INT_MAX) return 0;
    snprintf(accountId, accountIdSize, "%d/%d", ACCOUNT_YEAR, maxSequence + 1);
    return 1;
}

int findAccount(const char *accNo, Account *result) {
    FILE *fp;
    Account acc;

    if (result == NULL) return 0;
    fp = fopen(ACCOUNTS_FILE, "rb");
    if (fp == NULL) return 0;

    while (fread(&acc, sizeof(acc), 1, fp) == 1) {
        if (strcmp(acc.accountNumber, accNo) == 0 && acc.active) {
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
    Account acc;

    if (fp == NULL) return 0;
    while (fread(&acc, sizeof(acc), 1, fp) == 1) {
        if (strcmp(acc.accountNumber, updated.accountNumber) == 0) {
            long pos = ftell(fp) - (long)sizeof(acc);
            if (pos < 0 || fseek(fp, pos, SEEK_SET) != 0 ||
                fwrite(&updated, sizeof(updated), 1, fp) != 1) {
                fclose(fp);
                return 0;
            }
            return fclose(fp) == 0;
        }
    }
    fclose(fp);
    return 0;
}

int saveNewAccount(const Account *acc) {
    FILE *fp;
    if (acc == NULL) return 0;
    fp = fopen(ACCOUNTS_FILE, "ab");
    if (fp == NULL) return 0;
    int success = fwrite(acc, sizeof(*acc), 1, fp) == 1;
    if (fclose(fp) != 0) return 0;
    return success;
}

int logTransaction(const char *accNo, const char *type, double amount,
                   double balanceAfter, const char *desc) {
    FILE *fp = fopen(TRANSACTIONS_FILE, "ab");
    Transaction txn;

    if (fp == NULL) return 0;
    memset(&txn, 0, sizeof(txn));
    strncpy(txn.accountNumber, accNo, sizeof(txn.accountNumber) - 1);
    strncpy(txn.type, type, sizeof(txn.type) - 1);
    txn.amount = amount;
    txn.balanceAfter = balanceAfter;

    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    if (local == NULL || strftime(txn.timestamp, sizeof(txn.timestamp),
                                  "%Y-%m-%d %H:%M:%S", local) == 0) {
        strncpy(txn.timestamp, "unknown", sizeof(txn.timestamp) - 1);
    }
    strncpy(txn.description, desc, sizeof(txn.description) - 1);

    int success = fwrite(&txn, sizeof(txn), 1, fp) == 1;
    if (fclose(fp) != 0) return 0;
    return success;
}

void createAccount(void) {
    Account acc;
    char pinInput[PIN_BUF_LEN];
    double initialDeposit;

    memset(&acc, 0, sizeof(acc));
    acc.accountNumber = getNextAccountNumber();
    if (acc.accountNumber < 0) {
        printf("Account creation failed: account number limit reached.\n");
        return;
    }
    acc.active = 1;

    printf("\n=== Create New Account ===\n");
    printf("Enter full name: ");
    readLine(acc.name, sizeof(acc.name));
    if (!isValidName(acc.name)) {
        printf("Name cannot be empty or whitespace-only. Account creation cancelled.\n");
        return;
    }

    printf("Set a 4-digit PIN: ");
    readLine(pinInput, sizeof(pinInput));
    if (!isValidPin(pinInput)) {
        printf("Invalid PIN. PIN must contain exactly 4 digits.\n");
        return;
    }
    strncpy(acc.pin, pinInput, sizeof(acc.pin) - 1);

    if (!readAmount("Enter initial deposit amount: ", &initialDeposit)) {
        printf("Invalid amount. Account creation cancelled.\n");
        return;
    }
    acc.balance = initialDeposit;

    if (!saveNewAccount(&acc)) {
        printf("Error: could not save the account.\n");
        return;
    }
    if (!logTransaction(acc.accountNumber, "DEPOSIT", acc.balance,
                        acc.balance, "Initial deposit")) {
        printf("Warning: account created, but transaction history could not be saved.\n");
    }

    printf("\nAccount created successfully!\n");
    printf("Your account number is: %d\n", acc.accountNumber);
    printf("Please save this number; you'll need it to log in.\n");
    pauseScreen();
}

int authenticate(Account *acc) {
    int accNo;
    char pin[PIN_BUF_LEN];

    if (!readInt("Enter account number: ", &accNo) || accNo <= 1000) {
        printf("Invalid account number.\n");
        return 0;
    }
    if (!findAccount(accNo, acc)) {
        printf("Account not found.\n");
        return 0;
    }

    printf("Enter PIN: ");
    readLine(pin, sizeof(pin));
    if (!isValidPin(pin) || strcmp(pin, acc->pin) != 0) {
        printf("Incorrect PIN.\n");
        return 0;
    }
    return 1;
}

void depositMoney(Account *acc) {
    double amount;
    if (!readAmount("\nEnter amount to deposit: ", &amount)) {
        printf("Invalid amount.\n");
        return;
    }
    if (acc->balance > DBL_MAX - amount) {
        printf("Deposit would exceed the supported balance limit.\n");
        return;
    }

    acc->balance += amount;
    if (!updateAccount(*acc)) {
        acc->balance -= amount;
        printf("Deposit failed: account could not be updated.\n");
        return;
    }
    if (!logTransaction(acc->accountNumber, "DEPOSIT", amount,
                        acc->balance, "Cash deposit")) {
        printf("Warning: deposit completed, but transaction history could not be saved.\n");
    }
    printf("Deposit successful. New balance: %.2f\n", acc->balance);
}

void withdrawMoney(Account *acc) {
    double amount;
    if (!readAmount("\nEnter amount to withdraw: ", &amount)) {
        printf("Invalid amount.\n");
        return;
    }
    if (amount > acc->balance) {
        printf("Insufficient balance. Current balance: %.2f\n", acc->balance);
        return;
    }

    acc->balance -= amount;
    if (!updateAccount(*acc)) {
        acc->balance += amount;
        printf("Withdrawal failed: account could not be updated.\n");
        return;
    }
    if (!logTransaction(acc->accountNumber, "WITHDRAW", amount,
                        acc->balance, "Cash withdrawal")) {
        printf("Warning: withdrawal completed, but transaction history could not be saved.\n");
    }
    printf("Withdrawal successful. New balance: %.2f\n", acc->balance);
}

void checkBalance(const Account *acc) {
    printf("\n--- Account Summary ---\n");
    printf("Account Number : %d\n", acc->accountNumber);
    printf("Name           : %s\n", acc->name);
    printf("Balance        : %.2f\n", acc->balance);
}

void transferMoney(Account *sender) {
    char targetAcc[ACCOUNT_ID_LEN];
    double amount;
    Account receiver;
    double originalSenderBalance = sender->balance;

    if (!readInt("\nEnter recipient account number: ", &targetAcc)) {
        printf("Invalid account number.\n");
        return;
    }
    if (strcmp(targetAcc, sender->accountNumber) == 0) {
        printf("You cannot transfer to your own account.\n");
        return;
    }
    if (!findAccount(targetAcc, &receiver)) {
        printf("Recipient account not found.\n");
        return;
    }
    if (!readAmount("Enter amount to transfer: ", &amount)) {
        printf("Invalid amount.\n");
        return;
    }
    if (amount > sender->balance) {
        printf("Insufficient balance.\n");
        return;
    }
    if (receiver.balance > DBL_MAX - amount) {
        printf("Transfer would exceed the recipient balance limit.\n");
        return;
    }

    sender->balance -= amount;
    receiver.balance += amount;

    if (!updateAccount(*sender)) {
        sender->balance = originalSenderBalance;
        printf("Transfer failed: sender account could not be updated.\n");
        return;
    }
    if (!updateAccount(receiver)) {
        sender->balance = originalSenderBalance;
        updateAccount(*sender);
        printf("Transfer failed: recipient account could not be updated.\n");
        return;
    }

    char desc[MAX_TXN_DESC];
    snprintf(desc, sizeof(desc), "Transfer to account %s", targetAcc);
    if (!logTransaction(sender->accountNumber, "TRANSFER_OUT", amount,
                        sender->balance, desc)) {
        printf("Warning: transfer completed, but sender history could not be saved.\n");
    }
    snprintf(desc, sizeof(desc), "Transfer from account %s", sender->accountNumber);
    if (!logTransaction(receiver.accountNumber, "TRANSFER_IN", amount,
                        receiver.balance, desc)) {
        printf("Warning: transfer completed, but recipient history could not be saved.\n");
    }
    printf("Transfer successful. New balance: %.2f\n", sender->balance);
}

void viewTransactionHistory(int accNo) {
    FILE *fp = fopen(TRANSACTIONS_FILE, "rb");
    Transaction txn;
    int found = 0;

    if (fp == NULL) {
        printf("\nNo transaction history found.\n");
        return;
    }

    printf("\n--- Transaction History for Account %d ---\n", accNo);
    printf("%-20s %-15s %-12s %-12s %s\n",
           "Timestamp", "Type", "Amount", "Balance", "Description");
    printf("--------------------------------------------------------------------------\n");

    while (1) {
        size_t recordsRead = fread(&txn, sizeof(txn), 1, fp);
        if (recordsRead == 1) {
            if (strcmp(txn.accountNumber, accNo) == 0) {
                printf("%-20s %-15s %-12.2f %-12.2f %s\n",
                       txn.timestamp, txn.type, txn.amount,
                       txn.balanceAfter, txn.description);
                found = 1;
            }
            continue;
        }

        if (ferror(fp)) {
            printf("Error: transaction history could not be read completely.\n");
        } else if (!feof(fp) || ftell(fp) % (long)sizeof(txn) != 0) {
            printf("Error: transaction history contains an incomplete record.\n");
        }
        break;
    }
    fclose(fp);
    if (!found) printf("No transactions yet.\n");
}

void listAllAccounts(void) {
    FILE *fp = fopen(ACCOUNTS_FILE, "rb");
    Account acc;
    int found = 0;

    if (fp == NULL) {
        printf("\nNo accounts found.\n");
        return;
    }

    printf("\n--- Demo Account Directory ---\n");
    printf("%-15s %-25s %-12s\n", "Account No", "Name", "Balance");
    printf("---------------------------------------------\n");
    while (fread(&acc, sizeof(acc), 1, fp) == 1) {
        if (acc.active) {
            printf("%-15d %-25s %-12.2f\n",
                   acc.accountNumber, acc.name, acc.balance);
            found = 1;
        }
    }
    fclose(fp);
    if (!found) printf("No active accounts.\n");
}

void deleteAccount(Account *acc) {
    char confirm[10];

    if (acc->balance != 0.0) {
        printf("\nPlease withdraw or transfer the remaining %.2f before deleting the account.\n",
               acc->balance);
        return;
    }

    printf("\nDelete account %s? This action cannot be undone. (yes/no): ",
           acc->accountNumber);
    readLine(confirm, sizeof(confirm));
    if (strcmp(confirm, "yes") != 0) {
        printf("Deletion cancelled.\n");
        return;
    }

    acc->active = 0;
    if (!updateAccount(*acc)) {
        acc->active = 1;
        printf("Account deletion failed.\n");
        return;
    }
    printf("Account %d has been deleted.\n", acc->accountNumber);
}

void accountMenu(Account acc) {
    int choice;
    int loggedIn = 1;

    while (loggedIn) {
        if (!findAccount(acc.accountNumber, &acc)) {
            printf("\nAccount is no longer available. Logging out.\n");
            return;
        }

        printf("\n========== Account Menu (Acc #%d) ==========\n", acc.accountNumber);
        printf("1. Check Balance\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer Money\n");
        printf("5. Transaction History\n");
        printf("6. Delete Account\n");
        printf("7. Logout\n");

        if (!readInt("Choose an option: ", &choice)) {
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: checkBalance(&acc); break;
            case 2: depositMoney(&acc); break;
            case 3: withdrawMoney(&acc); break;
            case 4: transferMoney(&acc); break;
            case 5: viewTransactionHistory(acc.accountNumber); break;
            case 6:
                deleteAccount(&acc);
                if (!acc.active) loggedIn = 0;
                break;
            case 7:
                printf("Logging out...\n");
                loggedIn = 0;
                break;
            default:
                printf("Invalid choice. Try again.\n");
                continue;
        }
        if (loggedIn) pauseScreen();
    }
}

void mainMenu(void) {
    int choice;
    int running = 1;

    while (running) {
        printf("\n================ BANKING SYSTEM ================\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("3. List All Accounts (demo)\n");
        printf("4. Exit\n");
        printf("==================================================\n");

        if (!readInt("Choose an option: ", &choice)) {
            printf("Invalid input.\n");
            continue;
        }

        Account acc;
        switch (choice) {
            case 1: createAccount(); break;
            case 2:
                if (authenticate(&acc)) {
                    printf("\nWelcome, %s!\n", acc.name);
                    accountMenu(acc);
                } else {
                    pauseScreen();
                }
                break;
            case 3:
                listAllAccounts();
                pauseScreen();
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
