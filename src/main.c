
#include <stdio.h>
#include "header.h"

int main() {
	int choice;
	while (1) {
		printf("\n==== ATM Management System ====\n");
		printf("1. Login\n");
		printf("2. Register\n");
		printf("3. Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		getchar(); // consume newline
		switch (choice) {
			case 1: {
				int user_id;
				char user_name[NAME_LEN];
				if (login(&user_id, user_name)) {
					user_menu(user_id, user_name);
				}
				break;
			}
			case 2:
				register_user();
				break;
			case 3:
				printf("Exiting...\n");
				return 0;
			default:
				printf("Invalid choice. Try again.\n");
		}
	}
	return 0;
}
