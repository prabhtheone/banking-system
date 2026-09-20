#!/usr/bin/env sh
set -eu

cleanup() {
    rm -f accounts.dat transactions.dat
}
trap cleanup EXIT INT TERM

output=$(printf '1\nCI Test User\n1234\n100\n\n4\n' | ./banking_system)

printf '%s\n' "$output" | grep -q "Account created successfully!"
printf '%s\n' "$output" | grep -q "Your account number is: 1001"
printf '%s\n' "$output" | grep -q "Thank you for using the Banking System. Goodbye!"

[ -s accounts.dat ]
[ -s transactions.dat ]

regression_output=$(printf '1\n   \n1\nRegression Test User\n1234\n100\n\n2\n1001\n1234\n2\n0.001\n\n7\n4\n' | ./banking_system)

printf '%s\n' "$regression_output" | grep -q "Name cannot be empty or whitespace-only"
printf '%s\n' "$regression_output" | grep -q "Invalid amount."

invalid_pin_output=$(printf '1\nControl Test\n12a4\n100\n\n4\n' | ./banking_system)
printf '%s\n' "$invalid_pin_output" | grep -q "Invalid PIN."

self_transfer_output=$(printf '2\n1001\n1234\n4\n1001\n\n7\n4\n' | ./banking_system)
printf '%s\n' "$self_transfer_output" | grep -q "You cannot transfer to your own account."

printf 'x' > transactions.dat
history_output=$(printf '2\n1001\n1234\n5\n\n7\n4\n' | ./banking_system)
printf '%s\n' "$history_output" | grep -q "transaction history contains an incomplete record"

printf '%s\n' "Smoke and regression tests passed."
