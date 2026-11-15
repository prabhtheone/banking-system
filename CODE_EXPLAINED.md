# How the Banking System Works

This guide follows the main parts of the program in `banking_system.c`. Read the source alongside it.

## 1. The data

The program uses two structures:

- `Account` stores an account number, name, PIN, balance, and active flag.
- `Transaction` stores the account number, operation type, amount, resulting balance, timestamp, and description.

The program writes these structures directly to binary files. That keeps the example small, but the file layout depends on the compiler and machine. It is not a portable database format.

## 2. Reading input

Input is read with `fgets` through `readLine()`. Numeric values are converted with `strtol()` and `strtod()`, then checked for conversion errors and trailing characters.

The menu reader distinguishes three outcomes:

- `1`: a valid integer was read.
- `0`: the input was present but invalid.
- `-1`: the input stream is closed (for example, the user sent EOF).

This prevents a menu from looping forever when no more input is available.

## 3. Saving data

The account file is `accounts.dat`; the transaction file is `transactions.dat`.

- `getNextAccountNumber()` scans existing account records and chooses the next sequence.
- `findAccount()` searches for an active account.
- `saveNewAccount()` appends a new record.
- `updateAccount()` replaces an existing account record.
- `logTransaction()` appends a transaction record.

The files are created in the directory from which the program is run. Keep them out of version control.

## 4. Banking operations

Account creation validates the name, PIN, and initial deposit before saving the account.

A deposit or withdrawal changes the in-memory balance, writes the updated account, and then records the operation. If the account update fails, the in-memory balance is restored. A transaction-log failure is reported as a warning because the balance has already been saved.

A transfer updates the sender and recipient records separately. If the recipient update fails, the program attempts to restore the sender. This is best-effort rollback, not an atomic transaction: a crash or disk failure can still leave inconsistent data.

## 5. Menus

`mainMenu()` handles account creation, login, the demo account directory, and exit. After login, `accountMenu()` refreshes the account from disk and offers balance, deposit, withdrawal, transfer, history, deletion, and PIN-change actions.

The `ui.h` header contains only terminal formatting helpers. Set `NO_COLOR=1` to disable ANSI colours.

## 6. Testing

Run:

```sh
make
make test
make verify
```

The shell tests use a temporary working directory, so they do not overwrite account files in your project folder. They exercise the main account flows, invalid input, transfers, PIN changes, and transaction history.

## 7. Important limitations

This is a learning exercise, not real banking software. PINs are stored in plaintext, files are not encrypted, balances use `double`, and the program has no database transactions or concurrent-access protection. Use fictional test data only.
