# Security

## Important

This repository is an educational console banking project, **not production banking software**.

The current implementation stores PINs in plaintext inside the local binary data file and does not provide encryption, account lockout, or concurrent-access protection. Do not use real financial credentials, personal data, or real money with it.

## Known limitations

The following limitations are intentional and documented so contributors know where security work is still needed:

- PINs are stored as plaintext.
- Account and transaction files are not encrypted at rest.
- Login attempts are not rate-limited or locked out.
- Runtime data is stored locally without multi-user access controls.
- Transfers are designed for a simple local application and are not backed by a transactional database.
- The application has no real bank, payment, or external identity integration.

These limitations make the project suitable for learning C, file I/O, validation, testing, and basic application architecture—not for handling real financial information.

## Safe testing

Use synthetic names, test PINs, and fictional balances only. Runtime files such as `accounts.dat` and `transactions.dat` should remain local and should not be committed to the repository.

## Reporting a security issue

If you find a security-sensitive problem in this project, please avoid posting credentials or exploitable details publicly. Open a private security report through GitHub if private reporting is enabled for the repository; otherwise contact the maintainer privately through their GitHub profile.
