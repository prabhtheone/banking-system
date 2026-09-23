# Changelog

All notable changes to this project are documented here.

## [Unreleased]

### Added

- Unified `make verify` workflow for build, regression tests, and sanitizer checks.
- Consistent validation for `YYYY/N` account IDs during authentication and transfers.
- Regression coverage for malformed account IDs.
- Shared `.editorconfig` settings for contributors.
- Sequential year-based account IDs starting at `2026/1` and continuing as `2026/2`, `2026/3`, and so on.
- Updated login, transfers, transaction history, account directory, tests, and documentation to use the new `YYYY/N` account ID format.
- Expanded security documentation with known limitations and safe-testing guidance.
- Added README troubleshooting guidance for compiler, Make, runtime-file, and sanitizer issues.

### Planned

- Continue improving security and test coverage.
- Evaluate integer-based money handling.
- Plan stronger PIN protection for a future version.

## [0.1.0] - 2026-09-17

### Added

- Console-based account creation and PIN authentication.
- Deposits, withdrawals, balance checks, and account-to-account transfers.
- Timestamped transaction history.
- Persistent binary-file storage.
- Defensive line-based input validation.
- File-write error handling and transfer rollback attempts.
- Automated smoke test with `make test`.
- GitHub Actions build, test, and strict compiler checks.
- Contribution guide, security notes, and issue templates.

### Changed

- PIN input now requires exactly four digits.
- Account deletion now requires a zero balance.
- Invalid numeric input is handled without leaving stale input in `stdin`.
