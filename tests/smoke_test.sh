#!/usr/bin/env sh
set -eu

project_dir=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT HUP INT TERM

# Run from a temporary directory so tests never overwrite the user's local
# accounts.dat or transactions.dat files in the repository.
cd "$test_dir"
program="$project_dir/banking_system"

output=$(printf '1\nCI Test User\n1234\n100\n\n4\n' | "$program")

printf '%s\n' "$output" | grep -q "Account created successfully!"
printf '%s\n' "$output" | grep -q "Your account number is: 2026/1"
printf '%s\n' "$output" | grep -q "Thank you for using the Banking System. Goodbye!"

[ -s accounts.dat ]
[ -s transactions.dat ]

regression_output=$(printf '1\n   \n1\nRegression Test User\n1234\n100\n\n2\n2026/1\n1234\n2\n0.001\n\n7\n4\n' | "$program")

printf '%s\n' "$regression_output" | grep -q "Name cannot be empty or whitespace-only"
printf '%s\n' "$regression_output" | grep -q "Invalid amount."

# Confirm a successful deposit and withdrawal persist the expected balance.
money_flow_output=$(printf '2\n2026/1\n1234\n2\n50\n\n3\n20\n\n1\n\n7\n4\n' | "$program")
printf '%s\n' "$money_flow_output" | grep -q "Deposit successful. New balance: 150.00"
printf '%s\n' "$money_flow_output" | grep -q "Withdrawal successful. New balance: 130.00"
printf '%s\n' "$money_flow_output" | grep -q "Balance        : 130.00"

invalid_pin_output=$(printf '1\nControl Test\n12a4\n100\n\n4\n' | "$program")
printf '%s\n' "$invalid_pin_output" | grep -q "Invalid PIN."

self_transfer_output=$(printf '2\n2026/1\n1234\n4\n2026/1\n\n7\n4\n' | "$program")
printf '%s\n' "$self_transfer_output" | grep -q "You cannot transfer to your own account."

printf 'x' > transactions.dat
history_output=$(printf '2\n2026/1\n1234\n5\n\n7\n4\n' | "$program")
printf '%s\n' "$history_output" | grep -q "transaction history contains an incomplete record"

invalid_account_output=$(printf '2\n2026\n4\n' | "$program")
printf '%s\n' "$invalid_account_output" | grep -q "Invalid account number. Expected format YYYY/N."

invalid_sequence_output=$(printf '2\n2026/0\n4\n' | "$program")
printf '%s\n' "$invalid_sequence_output" | grep -q "Invalid account number. Expected format YYYY/N."

printf '%s\n' "Smoke and regression tests passed."
