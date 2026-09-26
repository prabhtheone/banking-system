<div align="center">

# 🏦 C Banking System
### *Complete Beginner Code & Architecture Guide*

[![Language: C](https://img.shields.io/badge/Language-C-blue.svg)](#)
[![Storage: Binary Files](https://img.shields.io/badge/Storage-Binary%20Files-green.svg)](#)
[![License: Educational](https://img.shields.io/badge/Purpose-Educational-orange.svg)](#)

---

</div>

## 📌 Core Summary

This project is a **command-line banking application written in C**, built to demonstrate fundamental C programming concepts, file handling, and safe transaction processing.

### **How the System Operates**
* 🌐 **Main Menu:** Lets users create an account, log in using a unique ID and 4-digit PIN, list active accounts, or exit.
* 🔐 **Account Menu:** Once authenticated, users can check balances, deposit, withdraw, transfer money, view transaction history, or soft-delete their account.
* 💾 **Persistent Storage:** Account and transaction records are saved permanently using binary files (`accounts.dat` and `transactions.dat`).

---

## ⚡ Execution Pipeline

Every core banking action follows a strict **5-step safety model**:

---
---

## 📚 Standard Header Libraries Used

| Header File | Key Responsibilities |
| :--- | :--- |
| **`<stdio.h>`** | Manages file I/O operations (`fopen`, `fread`, `fwrite`, `fclose`) and user interaction (`printf`, `fgets`). |
| **`<stdlib.h>`** | Provides memory handling and numeric conversions (`strtol`, `strtod`). |
| **`<string.h>`** | Handles string operations (`strlen`, `strcmp`, `strncpy`, `memset`). |
| **`<ctype.h>`** | Validates character types (`isdigit`, `isspace`, `iscntrl`). |
| **`<errno.h>`** | Detects conversion errors, numeric overflow, and underflow during input reading. |
| **`<math.h>`** | Performs safety checks (`isfinite()`) to reject invalid mathematical values like `NaN` or Infinity. |
| **`<time.h>`** | Generates system timestamps (`time`, `localtime`, `strftime`) for logging transactions. |
| **`<limits.h>` / `<float.h>`** | Defines bounds for integer and floating-point limits (`INT_MAX`, `DBL_MAX`). |

---

## 🗂️ Data Structures & File Storage

Data is split across two main binary structure formats:

### 1️⃣ `Account` Structure ➔ `accounts.dat`
```c
struct Account {
    char accountNumber[16]; // Unique ID format: YYYY/N (e.g., 2026/1)
    char name[64];          // Customer name
    char pin[5];            // 4-digit security PIN
    double balance;         // Current balance
    int active;             // Status flag: 1 = Active, 0 = Soft-Deleted
};

struct Transaction {
    char accountNumber[16]; // Associated account ID
    int type;              // DEPOSIT, WITHDRAW, TRANSFER_OUT, TRANSFER_IN, etc.
    double amount;         // Money involved in transaction
    double balanceAfter;   // Account balance after operation completes
    char timestamp[32];    // Date and time string
    char description[128]; // Detailed log message
};

🔒 Logic & Defensive Features
Safe Input Reading: Functions like readLine() and readAmount() clean input buffers, enforce boundary limits, and eliminate trailing newline characters (\n).

Sequential ID Generation: getNextAccountNumber() scans existing records to assign the next available account ID (e.g., 2026/8 after 2026/7).

State Rollback: If writing an updated balance to accounts.dat fails during a transaction, the in-memory state automatically reverts to prevent database corruption.

Soft Deletion: Deleting an account updates the active flag to 0 without physically erasing the row, maintaining full historical transaction records.

[!NOTE]
Educational Purpose Only: This application stores PINs in plaintext and uses standard double-precision floats (double) for currency to demonstrate basic logic. Commercial banking platforms require cryptographic hashing (e.g., bcrypt), decimal/fixed-point financial data types, and server-side concurrency locks.
