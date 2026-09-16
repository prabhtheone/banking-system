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

printf '%s\n' "Smoke test passed."
