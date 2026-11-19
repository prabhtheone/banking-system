# Banking System in C

[![C CI](https://github.com/prabhtheone/banking-system/actions/workflows/ci.yml/badge.svg)](https://github.com/prabhtheone/banking-system/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A terminal-based banking simulation written in C. It is a small project for practising structs, input validation, file I/O, and breaking a program into functions.

> **Educational use only.** This program stores PINs in plaintext and saves data in local binary files. Do not use real credentials, personal financial information, or real money.

## Features

- Create accounts with sequential IDs such as `2026/1` and `2026/2`.
- Log in with an account number and four-digit PIN.
- Check balances, deposit, withdraw, and transfer between accounts.
- View transaction history and a dashboard with recent activity.
- Change a PIN and close an account once its balance is zero.
- Save account and transaction records locally between runs.
- Use a small ANSI terminal interface; set `NO_COLOR=1` to disable colour.

## Build and run

Requirements: a C11 compiler (GCC or Clang) and Make.

```sh
make
./banking_system
```

You can also compile without Make:

```sh
gcc -Wall -Wextra -Wpedantic -std=c11 -O2 -o banking_system banking_system.c
```

## Tests

```sh
make test     # build and run the regression tests
make verify   # run tests and AddressSanitizer/UndefinedBehaviorSanitizer
make clean    # remove the executable and local test data
```

The test script runs the program in a temporary directory. It covers account creation, input validation, deposits, withdrawals, PIN changes, menu end-of-input, transfers, and transaction history. The sanitizer target requires compiler and runtime support for AddressSanitizer and UndefinedBehaviorSanitizer.

## Files

- `banking_system.c` — menus, validation, account actions, and file operations.
- `ui.h` — small terminal formatting helpers.
- `tests/smoke_test.sh` — automated command-line regression tests.
- `Makefile` — build, test, sanitizer, and cleanup targets.
- `.github/workflows/ci.yml` — runs the verification commands on pushes and pull requests.

The application creates `accounts.dat` and `transactions.dat` in its current working directory. These files contain local demo data and are ignored by Git. Do not commit them.

## Account IDs

New IDs use the configured year and the next sequence number. Deleted records remain in the account file, so their sequence numbers are not reused. The year is currently set by `ACCOUNT_YEAR` in the source code.

## Limitations

This is not a secure or production banking application:

- PINs are stored as plaintext.
- Account and transaction files are not encrypted.
- Balances use `double`, which is not suitable for precise financial calculations.
- Binary records depend on the compiler's structure layout.
- Transfers use ordinary file writes and best-effort rollback; they are not atomic.
- The program has no database, concurrent-access protection, or external banking integration.
- The account directory displays names and balances.

See [SECURITY.md](SECURITY.md) before testing or contributing. Use fictional names, PINs, and balances only. For transparency about development assistance, see [AI_DISCLOSURE.md](AI_DISCLOSURE.md).

## Learning notes

[CODE_EXPLAINED.md](CODE_EXPLAINED.md) gives a short walkthrough of the data structures, input handling, persistence, and main banking operations. See [CONTRIBUTING.md](CONTRIBUTING.md) for the development workflow.
