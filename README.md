# 🏦 Banking System in C

[![C CI](https://github.com/prabhtheone/banking-system/actions/workflows/ci.yml/badge.svg)](https://github.com/prabhtheone/banking-system/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Language: C](https://img.shields.io/badge/Language-C-A8B9CC.svg?logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))

A **console-based banking system written in C** that demonstrates core banking operations, file-based persistence, authentication, transfers, and transaction history — without an external database.

> 🎓 **Learning project:** built to practice C programming, structs, file I/O, functions, input handling, and basic application architecture.

## ✨ Features

- 👤 Create bank accounts
- 🔐 PIN-protected login
- 💰 Deposit and withdraw money
- 🔄 Transfer money between accounts
- 📊 Check account balance
- 🧾 View timestamped transaction history
- 📋 List active accounts
- 🗑️ Soft-delete accounts
- 💾 Persistent storage using binary files
- 🧰 Simple `Makefile` build workflow
- 🤖 GitHub Actions CI for automated builds
- 🖥️ Runs on Linux, macOS, and Windows with GCC/MinGW/WSL

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
gcc -Wall -Wextra -std=c11 -O2 -o banking_system banking_system.c
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

## 🧪 Example

```text
================ BANKING SYSTEM ================
1. Create Account
2. Login
3. List All Accounts
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

After login, users can check their balance, deposit, withdraw, transfer funds, review transactions, or delete the account.

## 🧠 What This Project Demonstrates

This project is useful for students learning C because it combines several concepts in one application:

- `struct`-based data modeling
- Functions and modular program flow
- Binary file read/write operations
- Account lookup and record updates
- Transaction logging
- Basic authentication
- Input-buffer handling
- Persistent local application state
- Make-based compilation
- Continuous integration with GitHub Actions

## 📁 Project Structure

```text
.
├── .github/
│   └── workflows/
│       └── ci.yml             # Automated build checks
├── banking_system.c            # Main C application
├── Makefile                    # Build commands
├── LICENSE                     # MIT License
├── CONTRIBUTING.md             # Contribution guide
├── SECURITY.md                 # Security notes
├── README.md                   # Project documentation
├── .gitignore                  # Runtime/build exclusions
├── accounts.dat                # Generated locally at runtime
└── transactions.dat            # Generated locally at runtime
```

`accounts.dat` and `transactions.dat` are runtime-generated files and are intentionally excluded from Git.

## ⚠️ Security & Limitations

This is an **educational project, not production banking software**.

The current implementation intentionally keeps the design simple. In particular:

- PINs are stored as plaintext in the local binary account file.
- Account and transaction files are not encrypted at rest.
- There is no login lockout or rate limiting.
- PIN validation is intentionally basic.
- File operations are designed for a single local user, not concurrent access.
- There is no real bank/payment integration.

**Never use real banking credentials, personal information, or real money with this project.**

## 🗺️ Roadmap

Potential improvements for future versions:

- [ ] Hash PINs instead of storing them directly
- [ ] Add stronger input validation
- [ ] Add automated unit/integration tests
- [ ] Improve transaction atomicity and error handling
- [ ] Add account statements/export
- [ ] Add better terminal UI and colors
- [ ] Add password/PIN retry limits
- [ ] Improve cross-platform support
- [ ] Add a small database-backed version for comparison

## 🤝 Contributing

Contributions are welcome — especially improvements that make the project safer, easier to understand, or more useful for C learners.

See **[CONTRIBUTING.md](CONTRIBUTING.md)** for development guidelines.

If you build on this project, feel free to open an issue or pull request and share what you improved.

## ⭐ Support the Project

If this project helped you learn C, file handling, or basic application design, consider **starring the repository**. It helps other learners discover the project.

Issues, pull requests, documentation improvements, and constructive feedback are also welcome.

## 📄 License

Released under the **MIT License**. See [LICENSE](LICENSE) for details.

## 👨‍💻 Author

**Prabhjot Singh Gill**

GitHub: [@prabhtheone](https://github.com/prabhtheone)

---

### Keywords

`c` `c-language` `banking-system` `banking` `finance` `console-application` `file-handling` `file-io` `binary-files` `authentication` `transaction-history` `makefile` `github-actions` `beginner-project` `student-project` `learning-c` `systems-programming`
