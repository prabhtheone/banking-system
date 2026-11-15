# Contributing

Thanks for taking the time to improve this C project.

## Set up

1. Fork and clone the repository.
2. Make sure a C11 compiler and Make are installed.
3. Build and run the program:

   ```sh
   make
   ./banking_system
   ```

## Before opening a pull request

Run the verification target:

```sh
make verify
```

It builds the program, runs the regression tests, and runs the sanitizer build. Sanitizer support depends on the compiler and runtime available on your system.

Keep each change focused. Add or update a regression test when changing behaviour, and describe what changed and which commands you ran.

Do not commit generated files such as `banking_system`, `accounts.dat`, or `transactions.dat`. Use fictional test data only.

## Commits

Use short messages that describe the change. Keep unrelated changes in separate commits so they can be reviewed or reverted independently.

Examples:

```text
fix: reject blank menu input
test: cover transfer history
docs: clarify local data files
```

For security limitations and safe testing, read [SECURITY.md](SECURITY.md).
