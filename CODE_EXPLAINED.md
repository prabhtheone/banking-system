# C Banking System — Complete Code Explanation

This guide explains the actual implementation in `banking_system.c`, the purpose of each important function, how data moves through the program, and how to build and test it. It is written for a beginner learning C.

> **Educational demo only:** this program stores PINs in plaintext in local binary files. It is not suitable for real money, real banking credentials, or production use.

## 1. Project files

| File | Purpose |
|---|---|
| `banking_system.c` | Main C source: menus, validation, account operations, storage and transaction history |
| `Makefile` | Build, test, sanitizer and cleanup commands |
| `tests/smoke_test.sh` | Automated command-line regression tests |
| `.github/workflows/ci.yml` | Runs automated checks on GitHub |
| `README.md` | Project setup, features and usage |
| `CONTRIBUTING.md` | Contribution and verification instructions |
| `SECURITY.md` | Security limitations and safe-testing guidance |
| `CHANGELOG.md` | Project change history |
| `accounts.dat` | Runtime binary account records; generated locally and should not be committed |
| `transactions.dat` | Runtime binary transaction records; generated locally and should not be committed |

## 2. C headers and constants

The `#include` lines bring standard-library declarations into the program:

- `stdio.h`: terminal I/O and file operations such as `printf`, `fopen`, `fread`, `fwrite`, and `fclose`.
- `stdlib.h`: utility functions, including `strtol` and `strtod` for converting input strings to numbers.
- `string.h`: string and memory functions such as `strlen`, `strcmp`, `strncpy`, and `memset`.
- `ctype.h`: character checks such as `isdigit`, `isspace`, and `iscntrl`.
- `errno.h`: lets conversion code detect numeric conversion errors.
- `limits.h` and `float.h`: integer and floating-point limits, including `INT_MAX` and `DBL_MAX`.
- `math.h`: `isfinite`, used to reject NaN and infinity as money values.
- `time.h`: timestamps for transaction records.

Important macros near the top of the source centralize configuration:

- `ACCOUNTS_FILE` and `TRANSACTIONS_FILE` name the runtime files.
- `MAX_NAME_LEN`, `MAX_TXN_DESC`, and `MAX_INPUT_LEN` bound input and text buffers.
- `PIN_LEN` is four digits; `PIN_BUF_LEN` gives the input buffer extra room to detect invalid PINs.
- `ACCOUNT_YEAR` is currently `2026`; `ACCOUNT_ID_LEN` sizes account-number storage.
- `MIN_TRANSACTION_AMOUNT` and `MAX_TRANSACTION_AMOUNT` define accepted transaction amounts.

## 3. Data structures

### `Account`

An `Account` structure stores one account:

- `accountNumber`: text ID such as `2026/1`.
- `name`: account holder's name.
- `pin`: four-digit PIN stored as text, plus the string terminator.
- `balance`: current balance as a `double`.
- `active`: `1` means active and `0` means soft-deleted.

### `Transaction`

A `Transaction` structure records one event:

- `accountNumber`: the account associated with the event.
- `type`: a label such as `DEPOSIT`, `WITHDRAW`, `TRANSFER_OUT`, or `TRANSFER_IN`.
- `amount`: amount involved.
- `balanceAfter`: balance recorded after the operation.
- `timestamp`: formatted local date and time.
- `description`: a short human-readable explanation.

The program writes these structures as binary records. That is straightforward for a learning project, but the file format depends on the C structure layout and is not a portable database format.

## 4. Input and validation helpers

### `clearInputBuffer()`
Reads and discards characters until newline or end-of-file. It is used when an input line is longer than its destination buffer.

### `readLine(char *buf, size_t bufSize)`
Reads one line with `fgets`, removes the trailing newline when present, and avoids writing beyond the provided buffer. If reading fails, it sets the first character to the string terminator.

### `isBlank(const char *text)`
Returns true-like value `1` for a null, empty, or whitespace-only string; otherwise returns `0`.

### `isValidName(const char *name)`
Rejects empty/whitespace-only names and names containing control characters.

### `readInt(const char *prompt, int *value)`
Prints a prompt, reads text, converts it with `strtol`, and rejects conversion errors, trailing non-whitespace text, and values outside the `int` range. The output parameter is only assigned when validation succeeds.

### `readAmount(const char *prompt, double *amount)`
Converts input using `strtod`. It rejects conversion errors, trailing text, NaN/infinity, values below `0.01`, and values above the configured maximum. It writes to `*amount` only after validation passes.

### `isValidPin(const char *pin)`
Requires exactly four characters, all digits. The PIN is text so leading zeroes are preserved.

### `isValidAccountNumber(const char *accountId)`
Checks that the ID parses as a year and sequence separated by a slash, the year equals `ACCOUNT_YEAR`, and the sequence is greater than zero. For example, `2026/1` is valid for this configured year, while `2026/0` is invalid.

### `pauseScreen()`
Waits for the user to press Enter before continuing.

## 5. Account ID generation and storage functions

### `getNextAccountNumber(char *accountId, size_t accountIdSize)`
Reads existing account records, finds the largest sequence number for the configured year, and formats the next ID into the supplied buffer. Deleted accounts remain in the file, so their sequence numbers are not reused. If the sequence has reached `INT_MAX`, it reports failure.

### `findAccount(const char *accNo, Account *result)`
Scans `accounts.dat` and copies the matching account into `*result` only if the account is active. It returns `1` when found and `0` otherwise.

### `updateAccount(Account updated)`
Opens the account file for reading and writing, finds the matching record, seeks back to that record's starting position, and overwrites it. The return value indicates whether the update and close succeeded.

### `saveNewAccount(const Account *acc)`
Appends a new account record to `accounts.dat`. It returns failure if the file cannot be opened or the write/close fails.

### `logTransaction(...)`
Creates a zero-initialized `Transaction`, copies in the account ID, event type and description, sets the amount and resulting balance, and creates a timestamp. It appends the record to `transactions.dat` and returns success/failure.

## 6. Main banking actions

### `createAccount()`
1. Clears a new `Account` structure with `memset`.
2. Generates the next account number and marks the account active.
3. Reads and validates the name and PIN.
4. Reads the initial deposit.
5. Stores the initial balance and appends the account record.
6. Writes the initial deposit to transaction history.
7. Displays the new account number.

If transaction logging fails after the account is saved, the program warns the user; it does not automatically remove the newly created account.

### `authenticate(Account *acc)`
Reads an account ID, validates its format, finds an active account, and compares the entered four-digit PIN with the stored PIN. It returns success only when all checks pass.

### `depositMoney(Account *acc)`
Validates the amount, checks for floating-point overflow, increases the in-memory balance, and writes the updated account. If the account write fails, it reverses the in-memory change. On success it appends a transaction record.

### `withdrawMoney(Account *acc)`
Validates the amount and checks that the account has enough funds. It subtracts the amount and persists the new balance. If the account update fails, it restores the in-memory balance. A successful withdrawal is then logged.

### `checkBalance(const Account *acc)`
Displays the account number, holder name and current balance. The `const` pointer signals that this function should not modify the account.

### `transferMoney(Account *sender)`
1. Reads and validates the recipient's account ID.
2. Rejects a transfer to the same account and checks that the recipient exists.
3. Validates the amount and verifies the sender has sufficient funds.
4. Checks that adding the amount will not overflow the recipient balance.
5. Calculates the sender's reduced balance and recipient's increased balance.
6. Saves the sender first, then the recipient. If the recipient update fails, it attempts to restore the sender's original balance.
7. Logs `TRANSFER_OUT` and `TRANSFER_IN` records.

**Important limitation:** this is not a fully atomic bank transfer. A process or disk failure between writes, or a failed rollback, can leave inconsistent records. The warning and rollback logic are best-effort safeguards for a local educational demo, not database-grade transaction guarantees.

### `viewTransactionHistory(const char *accNo)`
Reads transaction records and prints only those matching the requested account ID. It reports read errors and detects a file whose length ends partway through a binary transaction record.

### `listAllAccounts()`
Prints active accounts from `accounts.dat`. This is intentionally labelled a demo directory because it exposes account details and would not be appropriate as-is in a real banking product.

### `deleteAccount(Account *acc)`
Requires the balance to be exactly zero and asks for explicit confirmation. It performs a soft deletion by setting `active = 0`, so the record remains in the file and the ID is not reused. It also attempts to log the deletion. If the account update fails, it restores the in-memory active flag.

## 7. Menu functions and program entry

### `accountMenu(Account acc)`
Repeatedly refreshes the logged-in account from storage, displays the account menu, reads a validated menu choice, and calls the relevant action. Deleting the account or choosing logout ends the logged-in session.

### `mainMenu(void)`
Displays the top-level menu and dispatches account creation, login, active-account listing, or exit. Invalid menu choices are reported instead of being treated as valid actions.

### `main(void)`
The program entry point. It prints a welcome message, calls `mainMenu()`, and returns `0` when the application exits normally.

## 8. How the program runs

```text
main()
  └─ mainMenu()
      ├─ createAccount()
      ├─ authenticate()
      │   └─ accountMenu()
      │       ├─ checkBalance()
      │       ├─ depositMoney()
      │       ├─ withdrawMoney()
      │       ├─ transferMoney()
      │       ├─ viewTransactionHistory()
      │       └─ deleteAccount()
      └─ listAllAccounts()

Account records      → accounts.dat
Transaction records → transactions.dat
```

Most input is read as text first and converted only after validation. Account records are the source of the current balance; transaction records are a history log.

## 9. Build and test

Run these commands from the repository directory:

```sh
make          # compile banking_system.c
make test     # run smoke and regression tests
make verify   # run tests and the sanitizer target
make clean    # remove the executable and local runtime data files
```

The smoke test checks account creation, expected account numbering, invalid names and PINs, invalid transaction amounts, self-transfer rejection, malformed transaction history, and malformed account IDs. The sanitizer target depends on support in the installed compiler and runtime.

## 10. Security and design limitations

- PINs are stored as plaintext; there is no cryptographic password hashing.
- Runtime account and transaction files are not encrypted.
- There is no login lockout or rate limiting.
- Balances use binary floating-point `double`, which is not ideal for financial arithmetic.
- File writes are not protected by a database transaction or cross-process locking.
- Transfer rollback is best-effort, not guaranteed atomic.
- The demo account directory displays names and balances.
- Binary files should be treated as local application data, not a secure or portable database.

Never put real credentials or personal financial information into this project. Potential future improvements include integer-cent money representation, stronger PIN storage, more failure-injection tests, and a transactional database backend.
