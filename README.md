# 🏦 Banking System in C

[![C CI](https://github.com/prabhtheone/banking-system/actions/workflows/ci.yml/badge.svg)](https://github.com/prabhtheone/banking-system/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Language: C](https://img.shields.io/badge/Language-C-A8B9CC.svg?logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))

A **console-based banking system written in C** that demonstrates account management, authentication, file-based persistence, transfers, and transaction history — without an external database.

> 🎓 **Learning project:** built to practice C programming, structs, file I/O, functions, input validation, error handling, and basic application architecture.

> ⚠️ **Important:** this is an educational simulation, not production banking software. Do not use real credentials, personal financial data, or real money.

## ✨ Features

- 👤 Create bank accounts
- 🔐 PIN-protected login with 4-digit validation
- 💰 Deposit and withdraw money
- 🔄 Transfer money between accounts
- 📊 Check account balance
- 🧾 View timestamped transaction history
- 📋 List active demo accounts
- 🗑️ Delete accounts only after the balance reaches zero
- 💾 Persistent storage using binary files
- 🛡️ Defensive input validation and file-write checks
- 🧰 Simple `Makefile` build and test workflow
- 🤖 GitHub Actions CI for automated build and smoke testing
- 🖥️ Runs on Linux, macOS, and Windows with a compatible C11 toolchain

## 🚀 Quick Start

### Requirements

- GCC or another C11-compatible compiler
- Make (recommended)

### Build

```bash
make
```

Or compile directly:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 -O2 -o banking_system banking_system.c
```

### Run

Linux/macOS/WSL:

```bash
./banking_system
```

Windows (MinGW):

```bash
banking_system.exe
```

## 🧪 Test

Run the automated smoke test locally:

```bash
make test
```

The test creates a temporary demo account, verifies successful startup/account creation/exit, checks that runtime data is written, and exercises invalid-input and self-transfer regressions. Generated files are cleaned automatically.

For an additional local memory-safety check, run:

```bash
make sanitize
```

Every push and pull request also runs the build, smoke test, and strict compiler check through GitHub Actions.

## 🖥️ Example

```text
================ BANKING SYSTEM ================
1. Create Account
2. Login
3. List All Accounts (demo)
4. Exit
==================================================
Choose an option: 1

=== Create New Account ===
Enter full name: Alex Kumar
Set a 4-digit PIN: 1234
Enter initial deposit amount: 5000

Account created successfully!
Your account number is: 1001
```

After login, users can check their balance, deposit, withdraw, transfer funds, review transactions, or delete an account after its balance reaches zero.

## 🧠 What This Project Demonstrates

This project is useful for students learning C because it combines several concepts in one application:

- `struct`-based data modeling
- Functions and modular program flow
- Binary file read/write operations
- Account lookup and record updates
- Transaction logging
- Basic authentication
- Robust line-based input parsing
- Persistent local application state
- Make-based compilation and testing
- Continuous integration with GitHub Actions

## 📁 Project Structure

```text
.
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   ├── bug_report.md
│   │   └── feature_request.md
│   └── workflows/
│       └── ci.yml
├── tests/
│   └── smoke_test.sh
├── banking_system.c
├── Makefile
├── LICENSE
├── CONTRIBUTING.md
├── SECURITY.md
├── README.md
└── .gitignore
```

`accounts.dat` and `transactions.dat` are generated locally at runtime and are intentionally excluded from Git.

## ⚠️ Security & Limitations

This is an **educational project, not production banking software**.

The current implementation intentionally keeps the design simple. In particular:

- PINs are stored as plaintext in the local binary account file.
- Account and transaction files are not encrypted at rest.
- There is no login lockout or rate limiting.
- The demo account directory displays account data and should not be treated as a real bank interface.
- File operations are designed for a simple local application, not concurrent users.
- There is no real bank/payment integration.

See [SECURITY.md](SECURITY.md) for the project's security notes and responsible-reporting guidance.

## 🗺️ Roadmap

Potential next improvements:

- [ ] Hash PINs with a vetted password-hashing library
- [ ] Replace floating-point money with integer cents
- [ ] Add comprehensive automated unit/integration tests
- [ ] Make transfers fully atomic with stronger storage guarantees
- [ ] Add account statements/export
- [ ] Add richer terminal UI and colors
- [ ] Add login retry limits and lockout
- [ ] Improve cross-platform tooling
- [ ] Add a database-backed version for comparison

## 🤝 Contributing

Contributions are welcome — especially improvements that make the project safer, easier to understand, or more useful for C learners.

See **[CONTRIBUTING.md](CONTRIBUTING.md)** for development guidelines. Bug reports and feature ideas can also use the repository's issue templates.

## ⭐ Support the Project

If this project helped you learn C, file handling, or application design, consider **starring the repository**. A genuine star helps other learners discover the project.

Issues, pull requests, documentation improvements, and constructive feedback are also welcome.

## 📄 License

Released under the **MIT License**. See [LICENSE](LICENSE) for details.

## 👨‍💻 Author

**Prabhjot Singh Gill**

GitHub: [@prabhtheone](https://github.com/prabhtheone)

---

### Keywords

`c` `c-programming` `c11` `banking-system` `banking` `finance` `console-application` `file-handling` `file-io` `binary-files` `authentication` `transaction-history` `makefile` `github-actions` `smoke-test` `beginner-project` `student-project` `learning-c` `systems-programming`
