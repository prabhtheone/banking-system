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
