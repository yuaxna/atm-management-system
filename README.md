## Menu Options Explained

After login, the following user menu options are available:

1. **Create Account**: Create a new bank account. You will be prompted for country, phone, initial balance, and account type (current/savings/fixed01/fixed02/fixed03).
2. **List Accounts**: View a list of all your accounts, including their details.
3. **Update Account Info**: Update the country or phone number for one of your accounts. You must enter the account ID and the field to update.
4. **Check Account Details**: View all details for a specific account by entering its account ID. If the account is savings or fixed, the system will show the interest you will acquire.
5. **Make Transaction**: Deposit or withdraw money from an account. Transactions are not allowed for fixed accounts. You must enter the account ID, transaction type, and amount.
6. **Remove Account**: Delete one of your accounts by entering its account ID. This action cannot be undone.
7. **Logout**: Log out and return to the main menu.

# ATM Management System

A simple ATM management system written in C. This project demonstrates file handling, user authentication, and basic banking operations.

## Features
- User registration and login (unique usernames)
- Create new accounts
- List owned accounts
- Update account information (country, phone)
- Check details of existing accounts (with interest calculation)
- Make transactions (deposit/withdraw, with restrictions)
- Remove existing accounts
- Transfer account ownership (to be implemented)

## File Structure
```
.
├── data
│   ├── records.txt   # Account records
│   └── users.txt     # User credentials
├── src
│   ├── main.c
│   ├── auth.c
│   ├── system.c
│   └── header.h
├── Makefile
└── README.md
```

## How to Build and Run
1. Install GCC (MinGW) on Windows.
2. Open a terminal in the project folder.
3. Compile:
   ```
   gcc -I./src -Wall src/main.c src/auth.c src/system.c -o atm-management-system.exe
   ```
4. Run:
   ```
   ./atm-management-system.exe
   ```

## Data Format
- `users.txt`: `id name password`
- `records.txt`: `id user_id user_name account_id date country phone balance type`

## License
MIT
