#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int playerMove(struct Cell** gameBoard, int x, int y, int rows, int cols, char actOnCell); //The function gets the gameboard and player's move and devides to situations according to the x y.
int isWinner(struct Cell** gameBoard, int rows, int cols);//The function runs over the board and checks how many X are they in it, if 0, prints winning alert.
void bombPlacing(struct Cell** gameBoard, int rows, int cols);//The function gets the gameboard and randomly places mines in it.
void fillWithX(struct Cell** gameBoard, int rows, int cols);//The function fills the gameboard with X for start.
int countOfBombs(struct Cell** gameBoard, int rows, int cols, int x, int y);//the function gets the gameboard and a cell, and checks how many mines are they next to it. also replace the cell to the mine count.
void printGameBoard(struct Cell** gameBoard, int rows, int cols);//The function gets the gameboard and print it with X instead of *.
void printIfLost(struct Cell** gameBoard, int rows, int cols);//The function gets the gameboard and print it after reaviling all the cells.
void revealArea(struct Cell** gameBoard, int rows, int cols, int x, int y);//The function reveals all the empty cells in the area of chosen cell.
int isValidChoice(int x, int y, int cols, int rows);//The function checks if the x, y cordinates are in the array borders

struct Cell
{
	int isMine;
	char visual;
};

void main()
{
	srand(time(NULL));

	int playerChoice = -1, rows = -1, cols = -1, i;
	char actOnCell;

	printf("Hello and welcome to the mine sweeper.\n");

	while (playerChoice == -1)
	{
		printf("Please choose your board size:\n0. Exit :(\n1. 8x8\n2. 12x12\n3. 15x15\n4. Free size\n");
		scanf("%d", &playerChoice);
		if (playerChoice < 0 || playerChoice > 4)
		{
			printf("Invalid choice, please try again\n");
			playerChoice = -1;
		}
	}

	switch (playerChoice)
	{
	case 1:
		rows = 8;
		cols = 8;
		break;
	case 2:
		rows = 12;
		cols = 12;
		break;
	case 3:
		rows = 15;
		cols = 15;
		break;
	case 4:
		while (rows == -1 && cols == -1)
		{
			printf("Enter the size of rows between 1 - 22 and the size of cols between 1 - 22\n");
			scanf("%d %d", &rows, &cols);

			if ((rows < 1 || rows > 22) || (cols < 1 || cols > 22))
			{
				printf("Invalid choice, please try again\n");
				rows = -1;
				cols = -1;
			}
		}
		break;
	}
	struct Cell** gameBoard = malloc(rows * sizeof(char*));
	for (i = 0; i < rows; i++)
		gameBoard[i] = malloc(cols * sizeof(char));

	if (gameBoard == NULL)
	{
		printf("ERROR! Memory allocation faild.");
		return;
	}

	fillWithX(gameBoard, rows, cols);
	bombPlacing(gameBoard, rows, cols);
	printGameBoard(gameBoard, rows, cols);

	while (1)
	{
		int situation;
		int xChoice, yChoice;

		printf("Please enter the row and col of your move\n");

		scanf("%d%d", &xChoice, &yChoice);

		if (gameBoard[xChoice][yChoice].visual != 'X' && gameBoard[xChoice][yChoice].visual != 'F')
		{
			printf("invalid choice, please try again\n");
			continue;
		}

		printf("To open enter O, To flag enter F\n");
		scanf(" %c", &actOnCell);

		if (gameBoard[xChoice][yChoice].visual == 'F' && actOnCell == 'F')
		{
			printf("invalid choice, please try again\n");
			continue;
		}

		if (actOnCell == 'O')
		{
			situation = playerMove(gameBoard, xChoice, yChoice, rows, cols, actOnCell);
			if (countOfBombs(gameBoard, rows, cols, xChoice, yChoice) == 0)
				gameBoard[xChoice][yChoice].visual = 'X';
			else
				gameBoard[xChoice][yChoice].visual = countOfBombs(gameBoard, rows, cols, xChoice, yChoice) + '0';
		}
		else if (actOnCell == 'F')
		{
			gameBoard[xChoice][yChoice].visual = 'F';
			printGameBoard(gameBoard, rows, cols);
			continue;
		}
		else
		{
			printf("Invalid choice. Please try again.\n");
			continue;
		}

		if (situation == -1)
		{
			printGameBoard(gameBoard, rows, cols);
			isWinner(gameBoard, rows, cols);
		}

		else if (situation == 0)
			continue;

		else if (situation == 1)
			break;

		else
		{
			if (gameBoard[xChoice][yChoice].visual != 'F')
				revealArea(gameBoard, rows, cols, xChoice, yChoice);
			printGameBoard(gameBoard, rows, cols);
			isWinner(gameBoard, rows, cols);
		}
	}
	free(gameBoard);
}

void bombPlacing(struct Cell** gameBoard, int rows, int cols)
{
	int xBomb, yBomb;
	int bombNumber;
	bombNumber = (int)sqrt(rows * cols);
	for (int k = 0; k < bombNumber; k++)
	{
		xBomb = rand() % rows;
		yBomb = rand() % cols;
		gameBoard[xBomb][yBomb].isMine = 1;

	}
}

void fillWithX(struct Cell** gameBoard, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			gameBoard[i][j].visual = 'X';
			gameBoard[i][j].isMine = 0;
		}
}

int playerMove(struct Cell** gameBoard, int x, int y, int rows, int cols, char actOnCell)
{
	int rowCount = 0, startX = 0, startY = 0, col;

	if (x == -1)
	{
		for (startX = 0; startX < rows; startX++)
		{
			for (startY = 0; startY < cols; startY++)
			{
				if (gameBoard[startX][startY].visual == 'X')
					break;
			}
			if (gameBoard[startX][startY].visual == 'X')
				break;
		}
		rowCount = startX;
		for (int k = 0; k < y; k++)
		{

			col = (k + startY) % cols;
			if (col == 0 && k != 0)
				rowCount++;
			if (gameBoard[rowCount][col].isMine != 1)
			{
				countOfBombs(gameBoard, rows, cols, rowCount, col);
			}
		}
		return -1;
	}
	if (rows < x || x < -1 || cols < y || y < 0)
	{
		printf("Invalid choice, please try again\n");
		return 0;
	}

	if (actOnCell == 'F')
	{
		gameBoard[x][y].visual = 'F';
		return 2;
	}
	else
	{
		if (gameBoard[x][y].isMine == 1)
		{
			printf("Boom. Game over.\n");
			printIfLost(gameBoard, rows, cols);
			return 1;
		}
	}
	return 2;
}

void printGameBoard(struct Cell** gameBoard, int rows, int cols)
{
	printf("     ");

	for (int k = 0; k < rows; k++)
	{
		if (k <= 9)
			printf("%d%s", k, " | ");
		else
			printf("%d%s", k, "| ");
	}

	printf("\n");

	for (int i = 0; i < rows; i++)
	{
		if (i <= 9)
			printf("%d%s", i, "  | ");
		else
			printf("%d%s", i, " | ");

		for (int j = 0; j < cols; j++)
		{
			printf("%c", gameBoard[i][j].visual);
			printf(" | ");
		}

		printf("\n");
	}
	printf("\n");
}

int countOfBombs(struct Cell** gameBoard, int rows, int cols, int x, int y)
{
	int count = 0;

	if (x - 1 >= 0 && y - 1 >= 0)
	{
		if (gameBoard[x - 1][y - 1].isMine == 1)
			count++;
	}

	if (x - 1 >= 0)
	{
		if (gameBoard[x - 1][y].isMine == 1)
			count++;
	}
	if (x - 1 >= 0 && y + 1 < cols)
	{
		if (gameBoard[x - 1][y + 1].isMine == 1)
			count++;
	}

	if (y - 1 >= 0)
	{
		if (gameBoard[x][y - 1].isMine == 1)
			count++;
	}

	if (y + 1 < cols)
	{
		if (gameBoard[x][y + 1].isMine == 1)
			count++;
	}

	if (x + 1 < rows && y - 1 >= 0)
	{
		if (gameBoard[x + 1][y - 1].isMine == 1)
			count++;
	}

	if (x + 1 < rows)
	{
		if (gameBoard[x + 1][y].isMine == 1)
			count++;
	}

	if (x + 1 < rows && y + 1 < cols)
	{
		if (gameBoard[x + 1][y + 1].isMine == 1)
			count++;
	}
	return count;
}

void printIfLost(struct Cell** gameBoard, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			gameBoard[i][j].visual = countOfBombs(gameBoard, rows, cols, i, j) + '0';
			if (gameBoard[i][j].visual == '0')
				gameBoard[i][j].visual = ' ';
		}
	printf("     ");

	for (int k = 0; k < rows; k++)
	{
		if (k <= 9)
			printf("%d%s", k, " | ");
		else
			printf("%d%s", k, "| ");
	}

	printf("\n");

	for (int i = 0; i < rows; i++)
	{
		if (i <= 9)
			printf("%d%s", i, "  | ");
		else
			printf("%d%s", i, " | ");

		for (int j = 0; j < cols; j++)
		{
			if (gameBoard[i][j].isMine == 1)
				gameBoard[i][j].visual = '*';
			printf("%c", gameBoard[i][j].visual);
			printf(" | ");
		}

		printf("\n");
	}
	printf("\n");
}

int isWinner(struct Cell** gameBoard, int rows, int cols)
{
	int countOfX = 0;
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			if (gameBoard[i][j].visual == 'X')
				countOfX++;
		}
	if (countOfX == 0)
	{
		printf("YOU WON!\n");
		return 1;
	}
	return 2;
}

void revealArea(struct Cell** gameBoard, int rows, int cols, int x, int y)
{
	if (!isValidChoice(x, y, rows, cols))
		return;
	if (gameBoard[x][y].visual != 'X')
		return;
	int bombCount = countOfBombs(gameBoard, rows, cols, x, y);
	if (bombCount > 0)
	{
		if (gameBoard[x][y].isMine)
			return;
		gameBoard[x][y].visual = bombCount + '0';
		return;
	}

	gameBoard[x][y].visual = ' ';
	revealArea(gameBoard, rows, cols, x - 1, y - 1);
	revealArea(gameBoard, rows, cols, x - 1, y);
	revealArea(gameBoard, rows, cols, x - 1, y + 1);
	revealArea(gameBoard, rows, cols, x, y + 1);
	revealArea(gameBoard, rows, cols, x + 1, y + 1);
	revealArea(gameBoard, rows, cols, x + 1, y);
	revealArea(gameBoard, rows, cols, x + 1, y - 1);
	revealArea(gameBoard, rows, cols, x, y - 1);
}

int isValidChoice(int x, int y, int rows, int cols)
{
	if (x < 0 || x >= rows || y < 0 || y >= cols)
		return 0;
	return 1;
}

