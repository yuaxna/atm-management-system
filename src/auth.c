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
		printf("3. Logout\n");
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
			case 3:
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
