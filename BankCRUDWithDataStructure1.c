/* CRUD operations of bank accounts with linked list*/

#include "miscFunctionsOfAccounts.c"

#define DATA_FILE "accounts1.dat"
#define DELETED_DATA_FILE "deletedAccounts.dat"
#define LENGTH 20

typedef struct {
	char accountNumber[LENGTH];
	char accountName[LENGTH];
	float balance;
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
void printUpdateMenu();
void saveListIntoFile();
void loadAccountsIntoList();
void pointCurrentToEnd();
void readAccountDetails(Bank*);
void appendAccountToList(Bank);
struct node* createNode(struct node*, Bank*);
void saveDeletedIntoFile(struct node*);
void searchAccount(struct node**, int*, char*);
void updateNode(struct node*, char*);
void deleteNode(struct node*, char*);
void printRecord(struct node*);

int main() 
{    
	loadAccountsIntoList();
	showMenu();
    	return 0;
}

void createAccount()
{
	Bank tempAccount;
	readAccountDetails(&tempAccount);
	appendAccountToList(tempAccount);	
	saveListIntoFile();
	printf("Account Saved Successfully!\n");
	pause();
}

void updateAccount()
{
	struct node *node = NULL;
	int foundAccount;
	char accountNumberToCheck[LENGTH];
	searchAccount(&node, &foundAccount, accountNumberToCheck);
	if(foundAccount == 0)
	{
		printNotFoundMessage(accountNumberToCheck);
		pause();
		return;
	}
	else
	{
		updateNode(node, accountNumberToCheck);
	}
	pause();
}

void deleteAccount()
{
	struct node *node = NULL;
	int foundAccount;
	char accountNumberToCheck[LENGTH];
	searchAccount(&node, &foundAccount, accountNumberToCheck);
	if (foundAccount == 0)
	{
		printNotFoundMessage(accountNumberToCheck);
		pause();
		return;
	}
	else
	{
		deleteNode(node, accountNumberToCheck);
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
	printHeading("Bank Accounts");
	printf("\n");
	while (current != NULL)	
	{
		printRecord(current);
		current = current->next;		
	}
	printf("End of Accounts!\n");
	pause();
}

void printRecord(struct node *node)
{
	printf("Account Number: %s\n", node->account.accountNumber);
	printf("Account Name: %s\n", node->account.accountName);
	printf("Account Balance: %.2f\n------------------------\n", node->account.balance);
}

void loadAccountsIntoList()
{
	current = NULL;
	FILE *fpData = fopen(DATA_FILE, "rb");
	if (!fpData)
	{
		printNotFoundMessage(DATA_FILE);
		return;
	}
	Bank tempAccount;
	while (fread(&tempAccount, sizeof(Bank), 1, fpData))
	{
		struct node *newNode;
		newNode = createNode(newNode, &tempAccount);
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
	printAccountNameMessage();
	readText(tempAccount->accountName, sizeof(tempAccount->accountName));
	printAccountBalanceMessage();
	readNumber(&tempAccount->balance);
}

void appendAccountToList(Bank tempAccount)
{
	struct node *newNode;
	newNode = createNode(newNode, &tempAccount);
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

struct node* createNode(struct node *newNode, Bank *tempAccount)
{
	newNode = malloc(sizeof(struct node));
	newNode->account = *tempAccount;
	newNode->next = NULL;
	return newNode;
}
void saveListIntoFile()
{	
	FILE *fpData = fopen(DATA_FILE, "wb");
	if (!fpData)
	{
		printNotFoundMessage(DATA_FILE);
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

void saveDeletedIntoFile(struct node *node)
{
	FILE *fpData = fopen(DELETED_DATA_FILE, "ab");
	if (!fpData)
	{
		printNotFoundMessage(DATA_FILE);
		return;
	}
	
	fwrite(&node->account, sizeof(Bank), 1, fpData);
	fclose(fpData);
}

void searchAccount(struct node **previous, int *foundAccount, char *accountNumberToCheck)
{
	printAccountNumberMessage();
	readText(accountNumberToCheck, LENGTH);
	*foundAccount = 0;
	*previous = NULL;
	current = start;
	if (current == NULL)
	{
		printf("No Records!\n"); return;
	}
	while (current != NULL)
	{
		if (strcmp(current->account.accountNumber, accountNumberToCheck) == 0)
		{
			*previous = current;
			*foundAccount = 1;
			break;
		}
		current = current->next;
	}
}

void updateNode(struct node *nodeToBeUpdated, char *accountNumberToCheck)
{
	printf("Match Found!\n");					
	int choice;
	printUpdateMenu();
	readChoice(&choice);
	switch(choice)
	{
		case 1:	
			printAccountNameMessage();
			readText(nodeToBeUpdated->account.accountName, sizeof(nodeToBeUpdated->account.accountName));
			break;
		case 2:
			printAccountBalanceMessage();
			readNumber(&nodeToBeUpdated->account.balance);
			break;
		case 3:
			printAccountNameMessage();
			readText(nodeToBeUpdated->account.accountName, sizeof(nodeToBeUpdated->account.accountName));
			printAccountBalanceMessage();
			readNumber(&nodeToBeUpdated->account.balance);
			break;
		default:
			printf("Invalid choice!\n");
			return;
	}
	saveListIntoFile();
	printf("%s Updated Successfully!\n", accountNumberToCheck);
}

void deleteNode(struct node *nodeToBeDeleted, char *accountNumberToCheck)
{
	current = start;
	struct node *temp = NULL;	
	struct node **indirect = &start;
	while (*indirect != nodeToBeDeleted)
	{
		indirect = &(*indirect)->next;
	}
	saveDeletedIntoFile(nodeToBeDeleted);
	*indirect = nodeToBeDeleted->next;
	saveListIntoFile();
	printf("%s Deleted Successfully!\n", accountNumberToCheck);
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
	printHeading("BANK Details");
	printf("\n1.Add Account\n2.Update Account\n3.Delete Account\n4.Show All Accounts\n5.Exit\n-------------\nEnter Your Choice: ");
}

void printUpdateMenu()
{
	printHeading("Update Menu");
	printf("\n1.Account Name\n2.Account Balance\n3.Account Name and Balance\nEnter Your Choice: ");	
}

void showMenu()
{
	int choice;
   	do {
		clear();
		printMenu();
		readChoice(&choice);
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