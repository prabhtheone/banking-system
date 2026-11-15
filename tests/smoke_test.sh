#!/usr/bin/env sh
set -eu

project_dir=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
test_dir=$(mktemp -d)
trap 'rm -rf "$test_dir"' EXIT HUP INT TERM

# Run from a temporary directory so tests never overwrite the user's local
# accounts.dat or transactions.dat files in the repository.
cd "$test_dir"
if [ -n "${BANKING_SYSTEM_BIN:-}" ]; then
    program=$BANKING_SYSTEM_BIN
else
    program="$project_dir/banking_system"
fi

output=$(printf '1\nCI Test User\n1234\n100\n\n4\n' | "$program")

printf '%s\n' "$output" | grep -q "Account created successfully!"
printf '%s\n' "$output" | grep -q "Your account number is: 2026/1"
printf '%s\n' "$output" | grep -q "Thank you for using the Banking System. Goodbye!"

[ -s accounts.dat ]
[ -s transactions.dat ]

regression_output=$(printf '1\n   \n1\nRegression Test User\n1234\n100\n\n2\n2026/1\n1234\n3\n0.001\n\n9\n4\n' | "$program")

printf '%s\n' "$regression_output" | grep -q "Name cannot be empty or whitespace-only"
printf '%s\n' "$regression_output" | grep -q "Invalid amount."

# Confirm a successful deposit and withdrawal persist the expected balance.
money_flow_output=$(printf '2\n2026/1\n1234\n3\n50\n\n4\n20\n\n2\n\n9\n4\n' | "$program")
printf '%s\n' "$money_flow_output" | grep -q "Deposit successful. New balance: 150.00"
printf '%s\n' "$money_flow_output" | grep -q "Withdrawal successful. New balance: 130.00"
printf '%s\n' "$money_flow_output" | grep -q "Balance        : 130.00"

invalid_pin_output=$(printf '1\nControl Test\n12a4\n100\n\n4\n' | "$program")
printf '%s\n' "$invalid_pin_output" | grep -q "Invalid PIN."

self_transfer_output=$(printf '2\n2026/1\n1234\n5\n2026/1\n\n9\n4\n' | "$program")
printf '%s\n' "$self_transfer_output" | grep -q "You cannot transfer to your own account."

printf 'x' > transactions.dat
history_output=$(printf '2\n2026/1\n1234\n6\n\n9\n4\n' | "$program")
printf '%s\n' "$history_output" | grep -q "transaction history contains an incomplete record"

invalid_account_output=$(printf '2\n2026\n2026/0\nbad\n\n4\n' | "$program")
printf '%s\n' "$invalid_account_output" | grep -q "Invalid account number. Expected format YYYY/N."

invalid_sequence_output=$(printf '2\n2026/0\n2026\nbad\n\n4\n' | "$program")
printf '%s\n' "$invalid_sequence_output" | grep -q "Invalid account number. Expected format YYYY/N."


# Verify PIN changes are persisted and the new PIN can authenticate.
pin_change_output=$(printf '2\n2026/1\n1234\n8\n1234\n5678\n5678\n\n9\n4\n' | "$program")
printf '%s\n' "$pin_change_output" | grep -q "Your PIN has been changed successfully"
new_pin_login_output=$(printf '2\n2026/1\n5678\n9\n4\n' | "$program")
printf '%s\n' "$new_pin_login_output" | grep -q "Welcome, CI Test User!"


# Blank input is not a valid menu choice, and end-of-input must exit cleanly.
blank_menu_output=$(printf '   \n4\n' | "$program")
printf '%s\n' "$blank_menu_output" | grep -q "Invalid input."
printf '%s\n' "$blank_menu_output" | grep -q "Goodbye!"

eof_output=$("$program" </dev/null)
printf '%s\n' "$eof_output" | grep -q "Input closed. Exiting."

# EOF at the account menu must also return to the main menu and then exit.
account_eof_output=$(printf '2\n2026/1\n5678\n' | "$program")
printf '%s\n' "$account_eof_output" | grep -q "Input closed. Exiting."

# Exercise a transfer between two accounts and verify both persisted balances
# and the recipient's transaction history.
transfer_dir="$test_dir/transfers"
mkdir "$transfer_dir"
(
    cd "$transfer_dir"
    printf '1\nSender User\n1111\n100\n\n1\nReceiver User\n2222\n50\n\n4\n' |
        NO_COLOR=1 "$program" >/dev/null

    transfer_output=$(printf '2\n2026/1\n1111\n5\n2026/2\n25\n\n9\n4\n' |
        NO_COLOR=1 "$program")
    printf '%s\n' "$transfer_output" | grep -q "Transfer successful. New balance: 75.00"

    sender_output=$(printf '2\n2026/1\n1111\n2\n\n9\n4\n' |
        NO_COLOR=1 "$program")
    printf '%s\n' "$sender_output" | grep -q "Balance        : 75.00"

    receiver_output=$(printf '2\n2026/2\n2222\n2\n\n9\n4\n' |
        NO_COLOR=1 "$program")
    printf '%s\n' "$receiver_output" | grep -q "Balance        : 75.00"

    history_output=$(printf '2\n2026/2\n2222\n6\n\n9\n4\n' |
        NO_COLOR=1 "$program")
    printf '%s\n' "$history_output" | grep -q "TRANSFER_IN"
)

printf '%s\n' "Smoke and regression tests passed."
