#include <stdio.h>
#include <string.h>
#include <time.h>
#include "header.h"

int login(int *user_id, char *user_name) {
	char name[NAME_LEN], password[PASS_LEN];
	FILE *fp = fopen("../data/users.txt", "r");
	if (!fp) fp = fopen("data/users.txt", "r");
	int id, found = 0;
	char file_name[NAME_LEN], file_pass[PASS_LEN];

	printf("Enter username: ");
	fgets(name, NAME_LEN, stdin);
	name[strcspn(name, "\n")] = 0;
	printf("Enter password: ");
	fgets(password, PASS_LEN, stdin);
	password[strcspn(password, "\n")] = 0;

	if (fp) {
		while (fscanf(fp, "%d %s %s", &id, file_name, file_pass) == 3) {
			if (strcmp(name, file_name) == 0 && strcmp(password, file_pass) == 0) {
				found = 1;
				break;
			}
		}
		fclose(fp);
	}

	if (found) {
		printf("Login successful!\n");
		*user_id = id;
		strcpy(user_name, name);
		return 1;
	} else {
		printf("Login failed. Invalid username or password.\n");
		return 0;
	}
}

void create_account(int user_id, char* user_name) {
	char country[COUNTRY_LEN], phone[PHONE_LEN], type[TYPE_LEN];
	double balance;
	int acc_id = 0, rec_id = 0, max_acc_id = -1, max_rec_id = -1, uid, aid;
	char uname[NAME_LEN], date[11];
	FILE *fp = fopen("../data/records.txt", "r");
	if (!fp) fp = fopen("data/records.txt", "r");
	// Find max account id and record id
	if (fp) {
		while (fscanf(fp, "%d %d %s %d %10s %s %s %lf %s", &rec_id, &uid, uname, &aid, date, country, phone, &balance, type) == 9) {
			if (aid > max_acc_id) max_acc_id = aid;
			if (rec_id > max_rec_id) max_rec_id = rec_id;
		}
		fclose(fp);
	}
	acc_id = max_acc_id + 1;
	rec_id = max_rec_id + 1;

	printf("Enter country: ");
	fgets(country, COUNTRY_LEN, stdin);
	country[strcspn(country, "\n")] = 0;
	printf("Enter phone number: ");
	fgets(phone, PHONE_LEN, stdin);
	phone[strcspn(phone, "\n")] = 0;
	printf("Enter initial balance: ");
	scanf("%lf", &balance); getchar();
	printf("Enter account type (current/savings/fixed01/fixed02/fixed03): ");
	fgets(type, TYPE_LEN, stdin);
	type[strcspn(type, "\n")] = 0;

	// Get current date (dd/mm/yyyy)
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	snprintf(date, sizeof(date), "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

	fp = fopen("../data/records.txt", "a");
	if (!fp) fp = fopen("data/records.txt", "a");
	if (!fp) {
		printf("Error opening records.txt for writing.\n");
		return;
	}
	fprintf(fp, "%d %d %s %d %s %s %s %.2lf %s\n", rec_id, user_id, user_name, acc_id, date, country, phone, balance, type);
	fclose(fp);
	printf("Account created successfully!\n");
}

void user_menu(int user_id, char* user_name) {
	int choice;
	while (1) {
		printf("\n==== User Menu ====\n");
		printf("1. Create Account\n");
		printf("2. List Accounts\n");
		printf("3. Update Account Info\n");
		printf("4. Check Account Details\n");
		printf("5. Make Transaction\n");
	printf("6. Remove Account\n");
	printf("7. Transfer Owner\n");
	printf("8. Logout\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		getchar();
		switch (choice) {
			case 1:
				create_account(user_id, user_name);
				break;
			case 2: {
				FILE *fp = fopen("../data/records.txt", "r");
				if (!fp) fp = fopen("data/records.txt", "r");
				int rec_id, uid, acc_id;
				char uname[NAME_LEN], date[11], country[COUNTRY_LEN], phone[PHONE_LEN], type[TYPE_LEN];
				double balance;
				int found = 0;
				printf("\nYour Accounts:\n");
				printf("ID | AccountID | Date | Country | Phone | Balance | Type\n");
				if (fp) {
					while (fscanf(fp, "%d %d %s %d %10s %s %s %lf %s", &rec_id, &uid, uname, &acc_id, date, country, phone, &balance, type) == 9) {
						if (uid == user_id) {
							printf("%d | %d | %s | %s | %s | %.2lf | %s\n", rec_id, acc_id, date, country, phone, balance, type);
							found = 1;
						}
					}
					fclose(fp);
				}
				if (!found) {
					printf("No accounts found.\n");
				}
				break;
			}
			case 3: {
				int update_acc_id, found = 0;
				char field[10], new_value[COUNTRY_LEN > PHONE_LEN ? COUNTRY_LEN : PHONE_LEN];
				printf("Enter the account ID to update: ");
				scanf("%d", &update_acc_id); getchar();
				printf("Which field do you want to update? (country/phone): ");
				fgets(field, sizeof(field), stdin);
				field[strcspn(field, "\n")] = 0;
				if (strcmp(field, "country") != 0 && strcmp(field, "phone") != 0) {
					printf("Invalid field. Only 'country' or 'phone' can be updated.\n");
					break;
				}
				printf("Enter new value: ");
				fgets(new_value, sizeof(new_value), stdin);
				new_value[strcspn(new_value, "\n")] = 0;

				FILE *fp = fopen("../data/records.txt", "r");
				if (!fp) fp = fopen("data/records.txt", "r");
				FILE *tmp = fopen("../data/records_tmp.txt", "w");
				if (!tmp) tmp = fopen("data/records_tmp.txt", "w");
				int rec_id, uid, acc_id;
				char uname[NAME_LEN], date[11], country[COUNTRY_LEN], phone[PHONE_LEN], type[TYPE_LEN];
				double balance;
				if (fp && tmp) {
					while (fscanf(fp, "%d %d %s %d %10s %s %s %lf %s", &rec_id, &uid, uname, &acc_id, date, country, phone, &balance, type) == 9) {
						if (uid == user_id && acc_id == update_acc_id) {
							found = 1;
							if (strcmp(field, "country") == 0) {
								strncpy(country, new_value, COUNTRY_LEN);
							} else if (strcmp(field, "phone") == 0) {
								strncpy(phone, new_value, PHONE_LEN);
							}
						}
						fprintf(tmp, "%d %d %s %d %s %s %s %.2lf %s\n", rec_id, uid, uname, acc_id, date, country, phone, balance, type);
					}
					fclose(fp);
					fclose(tmp);
					// Replace original file
					remove("../data/records.txt");
					remove("data/records.txt");
					rename("../data/records_tmp.txt", "../data/records.txt");
					rename("data/records_tmp.txt", "data/records.txt");
					if (found) {
						printf("Account updated successfully!\n");
					} else {
						printf("Account not found or you do not have permission to update it.\n");
					}
				} else {
					printf("Error updating account.\n");
				}
				break;
			}
			case 4: {
				int check_acc_id, found = 0;
				printf("Enter the account ID to view: ");
				scanf("%d", &check_acc_id); getchar();
				FILE *fp = fopen("../data/records.txt", "r");
				if (!fp) fp = fopen("data/records.txt", "r");
				int rec_id, uid, acc_id;
				char uname[NAME_LEN], date[11], country[COUNTRY_LEN], phone[PHONE_LEN], type[TYPE_LEN];
				double balance;
				if (fp) {
					while (fscanf(fp, "%d %d %s %d %10s %s %s %lf %s", &rec_id, &uid, uname, &acc_id, date, country, phone, &balance, type) == 9) {
						if (uid == user_id && acc_id == check_acc_id) {
							found = 1;
							printf("\nAccount Details:\n");
							printf("Record ID: %d\nAccount ID: %d\nUser: %s\nDate: %s\nCountry: %s\nPhone: %s\nBalance: %.2lf\nType: %s\n", rec_id, acc_id, uname, date, country, phone, balance, type);
							// Interest calculation
							double interest = 0.0;
							if (strcmp(type, "savings") == 0) {
								interest = balance * 0.07 / 12;
								printf("You will get $%.2lf as interest on day %c%c of every month.\n", interest, date[0], date[1]);
							} else if (strcmp(type, "fixed01") == 0) {
								interest = balance * 0.04 / 12;
								printf("You will get $%.2lf as interest on day %c%c of every month.\n", interest, date[0], date[1]);
							} else if (strcmp(type, "fixed02") == 0) {
								interest = balance * 0.05 / 12;
								printf("You will get $%.2lf as interest on day %c%c of every month.\n", interest, date[0], date[1]);
							} else if (strcmp(type, "fixed03") == 0) {
								interest = balance * 0.08 / 12;
								printf("You will get $%.2lf as interest on day %c%c of every month.\n", interest, date[0], date[1]);
							} else if (strcmp(type, "current") == 0) {
								printf("You will not get interests because the account is of type current.\n");
							}
							break;
						}
					}
					fclose(fp);
				}
				if (!found) {
					printf("Account not found or you do not have permission to view it.\n");
				}
				break;
			}
			case 5: {
				int trans_acc_id, found = 0;
				char trans_type[10];
				double amount;
				printf("Enter the account ID for transaction: ");
				scanf("%d", &trans_acc_id); getchar();
				printf("Enter transaction type (deposit/withdraw): ");
				fgets(trans_type, sizeof(trans_type), stdin);
				trans_type[strcspn(trans_type, "\n")] = 0;
				printf("Enter amount: ");
				scanf("%lf", &amount); getchar();

				FILE *fp = fopen("../data/records.txt", "r");
				if (!fp) fp = fopen("data/records.txt", "r");
				FILE *tmp = fopen("../data/records_tmp.txt", "w");
				if (!tmp) tmp = fopen("data/records_tmp.txt", "w");
				int rec_id, uid, acc_id;
				char uname[NAME_LEN], date[11], country[COUNTRY_LEN], phone[PHONE_LEN], type[TYPE_LEN];
				double balance;
				if (fp && tmp) {
					while (fscanf(fp, "%d %d %s %d %10s %s %s %lf %s", &rec_id, &uid, uname, &acc_id, date, country, phone, &balance, type) == 9) {
						if (uid == user_id && acc_id == trans_acc_id) {
							found = 1;
							if (strcmp(type, "fixed01") == 0 || strcmp(type, "fixed02") == 0 || strcmp(type, "fixed03") == 0) {
								printf("Transactions are not allowed for fixed accounts.\n");
								fprintf(tmp, "%d %d %s %d %s %s %s %.2lf %s\n", rec_id, uid, uname, acc_id, date, country, phone, balance, type);
								continue;
							}
							if (strcmp(trans_type, "deposit") == 0) {
								balance += amount;
								printf("Deposit successful. New balance: %.2lf\n", balance);
							} else if (strcmp(trans_type, "withdraw") == 0) {
								if (balance < amount) {
									printf("Insufficient funds. Transaction cancelled.\n");
								} else {
									balance -= amount;
									printf("Withdrawal successful. New balance: %.2lf\n", balance);
								}
							} else {
								printf("Invalid transaction type.\n");
							}
						}
						fprintf(tmp, "%d %d %s %d %s %s %s %.2lf %s\n", rec_id, uid, uname, acc_id, date, country, phone, balance, type);
					}
					fclose(fp);
					fclose(tmp);
					// Replace original file
					remove("../data/records.txt");
					remove("data/records.txt");
					rename("../data/records_tmp.txt", "../data/records.txt");
					rename("data/records_tmp.txt", "data/records.txt");
					if (!found) {
						printf("Account not found or you do not have permission to transact on it.\n");
					}
				} else {
					printf("Error processing transaction.\n");
				}
				break;
			}
			case 6: {
				int del_acc_id, found = 0;
				printf("Enter the account ID to remove: ");
				scanf("%d", &del_acc_id); getchar();
				FILE *fp = fopen("../data/records.txt", "r");
				if (!fp) fp = fopen("data/records.txt", "r");
				FILE *tmp = fopen("../data/records_tmp.txt", "w");
				if (!tmp) tmp = fopen("data/records_tmp.txt", "w");
				int rec_id, uid, acc_id;
				char uname[NAME_LEN], date[11], country[COUNTRY_LEN], phone[PHONE_LEN], type[TYPE_LEN];
				double balance;
				if (fp && tmp) {
					while (fscanf(fp, "%d %d %s %d %10s %s %s %lf %s", &rec_id, &uid, uname, &acc_id, date, country, phone, &balance, type) == 9) {
						if (uid == user_id && acc_id == del_acc_id) {
							found = 1;
							continue; // skip writing this account
						}
						fprintf(tmp, "%d %d %s %d %s %s %s %.2lf %s\n", rec_id, uid, uname, acc_id, date, country, phone, balance, type);
					}
					fclose(fp);
					fclose(tmp);
					// Replace original file
					remove("../data/records.txt");
					remove("data/records.txt");
					rename("../data/records_tmp.txt", "../data/records.txt");
					rename("data/records_tmp.txt", "data/records.txt");
					if (found) {
						printf("Account removed successfully!\n");
					} else {
						printf("Account not found or you do not have permission to remove it.\n");
					}
				} else {
					printf("Error removing account.\n");
				}
				break;
			}
			case 7: {
				int trans_acc_id, found = 0;
				char recipient[NAME_LEN];
				int recipient_id = -1;
				printf("Enter the account ID to transfer: ");
				scanf("%d", &trans_acc_id); getchar();
				printf("Enter the username to transfer ownership to: ");
				fgets(recipient, NAME_LEN, stdin);
				recipient[strcspn(recipient, "\n")] = 0;

				// Check if recipient exists and get their user ID
				FILE *ufp = fopen("../data/users.txt", "r");
				if (!ufp) ufp = fopen("data/users.txt", "r");
				int uid;
				char uname[NAME_LEN], upass[PASS_LEN];
				if (ufp) {
					while (fscanf(ufp, "%d %s %s", &uid, uname, upass) == 3) {
						if (strcmp(uname, recipient) == 0) {
							recipient_id = uid;
							break;
						}
					}
					fclose(ufp);
				}
				if (recipient_id == -1) {
					printf("Recipient user not found.\n");
					break;
				}

				// Update account ownership in records.txt
				FILE *fp = fopen("../data/records.txt", "r");
				if (!fp) fp = fopen("data/records.txt", "r");
				FILE *tmp = fopen("../data/records_tmp.txt", "w");
				if (!tmp) tmp = fopen("data/records_tmp.txt", "w");
				int rec_id, acc_uid, acc_id;
				char acc_uname[NAME_LEN], date[11], country[COUNTRY_LEN], phone[PHONE_LEN], type[TYPE_LEN];
				double balance;
				if (fp && tmp) {
					while (fscanf(fp, "%d %d %s %d %10s %s %s %lf %s", &rec_id, &acc_uid, acc_uname, &acc_id, date, country, phone, &balance, type) == 9) {
						if (acc_uid == user_id && acc_id == trans_acc_id) {
							found = 1;
							// Transfer ownership
							acc_uid = recipient_id;
							strncpy(acc_uname, recipient, NAME_LEN);
						}
						fprintf(tmp, "%d %d %s %d %s %s %s %.2lf %s\n", rec_id, acc_uid, acc_uname, acc_id, date, country, phone, balance, type);
					}
					fclose(fp);
					fclose(tmp);
					// Replace original file
					remove("../data/records.txt");
					remove("data/records.txt");
					rename("../data/records_tmp.txt", "../data/records.txt");
					rename("data/records_tmp.txt", "data/records.txt");
					if (found) {
						printf("Account ownership transferred successfully!\n");
					} else {
						printf("Account not found or you do not have permission to transfer it.\n");
					}
				} else {
					printf("Error transferring ownership.\n");
				}
				break;
			}
			case 8:
				printf("Logging out...\n");
				return;
			default:
				printf("Invalid choice. Try again.\n");
		}
	}
}

void register_user() {
	char name[NAME_LEN], password[PASS_LEN];
	FILE *fp = fopen("../data/users.txt", "r");
	if (!fp) fp = fopen("data/users.txt", "r"); // fallback for Windows
	int exists = 0, max_id = -1, id;
	char file_name[NAME_LEN], file_pass[PASS_LEN];

	printf("Enter username: ");
	fgets(name, NAME_LEN, stdin);
	name[strcspn(name, "\n")] = 0;
	printf("Enter password: ");
	fgets(password, PASS_LEN, stdin);
	password[strcspn(password, "\n")] = 0;

	if (fp) {
		while (fscanf(fp, "%d %s %s", &id, file_name, file_pass) == 3) {
			if (strcmp(name, file_name) == 0) {
				exists = 1;
			}
			if (id > max_id) max_id = id;
		}
		fclose(fp);
	}

	if (exists) {
		printf("Username already exists. Registration failed.\n");
		return;
	}

	fp = fopen("../data/users.txt", "a");
	if (!fp) fp = fopen("data/users.txt", "a");
	if (!fp) {
		printf("Error opening users.txt for writing.\n");
		return;
	}
	fprintf(fp, "%d %s %s\n", max_id + 1, name, password);
	fclose(fp);
	printf("Registration successful!\n");
}
