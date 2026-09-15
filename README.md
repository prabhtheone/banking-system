# Simple Banking System (C)

A console-based banking system written in C. Supports creating accounts, deposits, withdrawals, transfers between accounts, transaction history, and persistent storage using binary files — no external database required.

## Features

- Create a new account with a name, 4-digit PIN, and optional opening deposit
- PIN-protected login
- Deposit and withdraw funds
- Transfer money between two accounts
- View per-account transaction history (timestamped)
- List all active accounts
- Soft-delete an account
- Data persists across runs via binary files (`accounts.dat`, `transactions.dat`)

## Getting Started

### Requirements

- GCC (or any C compiler with C11 support)
- Works on Linux, macOS, and Windows (via MinGW/WSL)

### Build

```bash
make
```

or manually:

```bash
gcc banking_system.c -o banking_system
```

### Run

```bash
./banking_system
```

## Usage

On launch you'll see the main menu:

```
================ BANKING SYSTEM ================
1. Create Account
2. Login
3. List All Accounts
4. Exit
==================================================
```

Create an account to get an account number, then log in with that number and your PIN to deposit, withdraw, transfer, or view your transaction history.

## Project Structure

```
.
├── banking_system.c     # Main source file
├── Makefile              # Build helper
├── accounts.dat          # Generated at runtime — account records
└── transactions.dat      # Generated at runtime — transaction log
```

## Known Limitations

This was built as a learning project, so a few things are simplified on purpose and worth knowing before treating it as production code:

- **PINs are stored in plain text** inside `accounts.dat`, not hashed. Anyone with access to that file can read every PIN.
- No encryption at rest for account or transaction data.
- No lockout or rate-limiting after repeated failed PIN attempts.
- No input validation on PIN format (any string up to 4 characters is accepted, not just digits).
- Single-user console app — the file I/O isn't safe for concurrent access.

`accounts.dat` and `transactions.dat` are excluded via `.gitignore` so no real account data ever gets pushed to the repo.

## License

Released under the [MIT License](LICENSE).

## Author

Prabhjot Singh Gill
