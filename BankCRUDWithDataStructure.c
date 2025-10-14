/* CRUD operations of bank accounts with linked list*/

#include "miscFunctionsOfAccounts.c"

#define DATA_FILE "accounts.dat"
#define LENGTH 20

typedef struct {
	char accountNumber[20];
	char accountName[20];
	float balance;
	char status;
}Bank;

struct node
{	
	Bank account;
	struct node *next;
};

struct node *start = NULL;
struct node *current = NULL;

void showMenu();
void createAccount();
void updateAccount();
void deleteAccount();
void showAllAccounts();
void printMenu();
void saveIntoFile();
void loadAccountsIntoList();
void pointCurrentToEnd();
void readAccountDetails(Bank*);
void appendAccountToList(Bank);

char status;

int main() {
    
	loadAccountsIntoList();
	showMenu();
    	return 0;
}

void createAccount()
{
	Bank tempAccount;
	readAccountDetails(&tempAccount);
	appendAccountToList(tempAccount);	
	saveIntoFile();
	printf("Account Saved Successfully!\n");
	pause();
}

void updateAccount()
{
	char accountNumberToCheck[LENGTH];
	printAccountNumberMessage();
	readText(accountNumberToCheck, LENGTH);
	int foundAccount = 0;
	current = start;
	while (current != NULL)
	{
		if (strcmp(current->account.accountNumber, accountNumberToCheck) == 0 && current->account.status == 'A')
		{	
			printf("Match Found!\n");
			printAccountNumberMessage();
			readText(current->account.accountName, sizeof(current->account.accountName));
			saveIntoFile();
			foundAccount = 1;
			printf("%s Updated Successfully!\n", accountNumberToCheck);
			break;
		}
		current = current->next;
	
	}
	if(foundAccount == 0)
	{
		printf("%s Not Found! (or) Deleted\n", accountNumberToCheck);
	}
	pause();
}

void deleteAccount()
{
	char accountNumberToCheck[LENGTH];
	printAccountNumberMessage();
	readText(accountNumberToCheck, LENGTH);
	int foundAccount = 0;
	current = start;
	while (current != NULL)
	{
		if (strcmp(current->account.accountNumber, accountNumberToCheck) == 0)
		{	
			current->account.status = 'D';
			saveIntoFile();
			foundAccount = 1;
			printf("%s Deleted Successfully!\n", accountNumberToCheck);
			break;		
		}
		current = current->next;
	}
	if (foundAccount == 0)
	{
		printf("%s Not Found\n", accountNumberToCheck);
	}
	pause();
}

void showAllAccounts()
{	
	current = start;
	if (current == NULL)
	{
		printf("No Records\n");
		pause();
		return;
	}
	printf("\nBank Accounts\n-----------------\n");
	while (current != NULL)	
	{
		if(current->account.status == 'A')
		{
			printf("Account Number: %s\n", current->account.accountNumber);
			printf("Account Name: %s\n", current->account.accountName);
			printf("Account Balance: %.2f\n------------------\n", current->account.balance);
		}
		current = current->next;
	}
	printf("End of Accounts!\n");
	pause();
}

void loadAccountsIntoList()
{
	current = NULL;
	FILE *fpData = fopen(DATA_FILE, "rb");
	if (!fpData)
	{
		printf("%s is Not Found!", DATA_FILE);
		return ;
	}
	Bank tempAccount;
	while (fread(&tempAccount, sizeof(Bank), 1, fpData))
	{
		struct node *newNode;
		newNode = malloc(sizeof(struct node));
		newNode->account = tempAccount;
		newNode->next = NULL;
		if (start == NULL)
		{
			start = newNode;
		}
		else
		{
			current->next = newNode;
		}
		current = newNode;
	}
	fclose(fpData);
}

void readAccountDetails(Bank *tempAccount)
{
	printAccountNumberMessage();
	readText(tempAccount->accountNumber, sizeof(tempAccount->accountNumber));
	printf("Enter Account Name: ");
	readText(tempAccount->accountName, sizeof(tempAccount->accountName));
	printf("Enter Account Balance: ");
	readNumber(&tempAccount->balance);
	tempAccount->status = 'A';
}
void appendAccountToList(Bank tempAccount)
{
	struct node *newNode;
	newNode = malloc(sizeof(struct node));
	newNode->account = tempAccount;
	newNode->next = NULL;
	if (start == NULL)
	{
		start = newNode;
	}
	else
	{
		pointCurrentToEnd();
		current->next = newNode;
	}		
}
void saveIntoFile()
{	
	FILE *fpData = fopen(DATA_FILE, "wb");
	if (!fpData)
	{
		printf("%s Not Found!\n", DATA_FILE);
		return;
	}
	current = start;
	while (current != NULL)
	{
		fwrite(&current->account, sizeof(Bank), 1, fpData);
		current = current->next;
	}
	fclose(fpData);
}

void pointCurrentToEnd()
{
	current = start;
	while(current->next != NULL)
	{
		
		current = current->next;		
	}
}

void printMenu()
{
	char *heading = "BANK Details";
	underLine(heading);
	printf("\n%s\n", heading);
	underLine(heading);
	printf("\n1.Add Account\n2.Update Account\n3.Delete Account\n4.Show All Accounts\n5.Exit\n-------------\nEnter Your Choice: ");
}

void showMenu()
{
	int choice;
   	do {
		clear();
		printMenu();
		scanf("%d", &choice);
		getchar();
		switch(choice)
		{
			case 1: createAccount(); break;
			case 2: updateAccount(); break;
			case 3: deleteAccount(); break;
			case 4: showAllAccounts(); break;
			case 5: exit(0);
			default: printf("Invalid Choice!\n"); pause(); break;
		}
	} while (choice != 5);	
}