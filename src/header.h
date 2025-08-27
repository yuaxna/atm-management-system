#ifndef HEADER_H
#define HEADER_H

#define NAME_LEN 50
#define PASS_LEN 50
#define COUNTRY_LEN 30
#define PHONE_LEN 20
#define TYPE_LEN 20

typedef struct {
	int id;
	char name[NAME_LEN];
	char password[PASS_LEN];
} User;

typedef struct {
	int id;
	int user_id;
	char user_name[NAME_LEN];
	int account_id;
	char creation_date[11]; // dd/mm/yyyy
	char country[COUNTRY_LEN];
	char phone[PHONE_LEN];
	double balance;
	char type[TYPE_LEN];
} Account;

void login();
void register_user();
// Add more function prototypes as needed

#endif
