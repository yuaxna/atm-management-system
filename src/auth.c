#include <stdio.h>
#include <string.h>
#include "header.h"

void login() {
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
		// TODO: Show user menu after login
	} else {
		printf("Login failed. Invalid username or password.\n");
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
