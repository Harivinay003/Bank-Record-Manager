#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void pause()
{
	system("pause");	
}

void clear()
{
	system("cls");
}

void readText(char *text, int size)
{
	fgets(text, size, stdin);	
	text[strcspn(text, "\n")] = '\0';	
}

void readNumber(float *number)
{
	scanf("%f", number);
	getchar();
}

void underLine(char *text)
{
	int size = strlen(text);
	for(int counter = 0; counter < size; counter++)
	{
		printf("-");
	}
}

void printNotFoundMessage(char *text)
{
	printf("%s Not Found\n", text);	
}

void printAccountNumberMessage()
{
	printf("Enter Account Number: ");
}

void printAccountNameMessage()
{
	printf("Enter Account Name: ");
}

void printAccountBalanceMessage()
{
	printf("Enter Account Balance: ");
}