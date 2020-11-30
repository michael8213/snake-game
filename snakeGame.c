/*
michael kan tor
318433232
*/
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define TURNS 5
#define UPPERLOWERLIM 25
#define LEFTRIGHTLIM 75


typedef struct snakeNode
{
	int x;
	int y;
	struct snakeNode* next;
}SNKAENODE;


void printHead(SNKAENODE* head);//this function prints @ at x,y location of node

void space(SNKAENODE* head);//this function prints " " at x,y location

void freeLast(SNKAENODE* head);//this function frees the last node of the snake

void gameOver(SNKAENODE* head);//this function handels game over situation, prints message.

void freeAll(SNKAENODE* head);//this function frees the snake nodes

void init(void);//this function initialises the program

void gotoxy(int x, int y);//this function puts the cursor on location x,y on the board

void sleep(float secs);//this function delies action

void printBoardLim();//this function prints the borders of the game board

SNKAENODE* newSnakeNode(int x, int y, SNKAENODE* next);//this function creats a new node of the snakes body

SNKAENODE* insertToHead(SNKAENODE* head, SNKAENODE* newHead);//this function inserts node to the head of the snake

SNKAENODE* moveSnake(SNKAENODE* head, char ch, int stepsCount, bool* ContinueGame);//this function moves the snake
//according to input (a,s,d,w) = (left,down,right,up)

bool countinueGame(SNKAENODE* head);//this function returns true if non of the game ending conditions are met,
//and false if they are met

bool selfCollide(SNKAENODE* head);//this function checks if the snakes collides with itself



int main()
{
	init();//initialize
	printBoardLim();

	SNKAENODE* head = newSnakeNode(12, 37, NULL);//initialis snake at center of board
	int stepsCount = 1;
	float delay = 1.00;
	bool continueGame = true;
	char ch = 'd';//move right

	while (!_kbhit() && continueGame)//game not over and no input
	{
		printHead(head);//print snake
		sleep(delay);//delay action
		head = moveSnake(head, ch, stepsCount, &continueGame);//move snake in apropriate direction
		stepsCount++;//add to steps count 1

		if ((stepsCount % TURNS == 0) && (delay > 0.1))//every 5 turns decrease delay by 0.03 seconds until delay is 0.1 seconds
			delay -= (float)0.03;

		if (_kbhit())//if there is input
		{
			ch = _getch();//change direction according to setup
		};

	} 

	gameOver(head);//when loop is over, game is over, print game over message
}

void gameOver(SNKAENODE* head)
{
	space(head);//erase snake
	freeAll(head);//free space
	gotoxy(UPPERLOWERLIM / 2, LEFTRIGHTLIM / 3);//print game over message
	printf("	GAME OVER!!!");
	gotoxy(UPPERLOWERLIM + 1, LEFTRIGHTLIM + 1);
}

void freeAll(SNKAENODE* head)
{
	SNKAENODE* next;

	while (head != NULL)
	{
		next = head->next;
		free(head);
		head = next;
	}
}


void init(void) {
	system("cls");
}

void gotoxy(int x, int y) {
	printf("\x1b[%d;%df", x, y);
}

void sleep(float secs) {
	clock_t clocks_start = clock();
	while (clock() - clocks_start < secs * CLOCKS_PER_SEC);
}

void printBoardLim()
{
	for (int y = 1, x = 2; y <= LEFTRIGHTLIM || x < UPPERLOWERLIM; y++, x++)//print limits, left to right is 75, up to down is 25
	{
		gotoxy(1, y);
		printf("#");
		gotoxy(UPPERLOWERLIM, y);
		printf("#");

		if (x < UPPERLOWERLIM + 1) {
			gotoxy(x, 1);
			printf("#");
			gotoxy(x, LEFTRIGHTLIM);
			printf("#");
		}
	}
	printf("\n");
}

void printHead(SNKAENODE* head)
{
	gotoxy(head->x, head->y);//print @ at x,y location of node
	printf("@");
}

void space(SNKAENODE* head)
{
	SNKAENODE* current = head->next;
	while (current != NULL)
	{
		gotoxy(current->x, current->y);
		printf(" ");
		current = current->next;
	}
}

SNKAENODE* newSnakeNode(int x, int y, SNKAENODE* next)
{
	SNKAENODE* node = (SNKAENODE*)malloc(sizeof(SNKAENODE));//craete new space for node

	node->x = x;//insert values
	node->y = y;
	node->next = next;

	return node;
}

SNKAENODE* insertToHead(SNKAENODE* head, SNKAENODE* newHead)
{
	newHead->next = head;//insert new node to head
	return newHead;
}

SNKAENODE* moveSnake(SNKAENODE* head, char ch, int stepsCount, bool* ContinueGame)
{
	switch (ch)
	{
	case 'w'://if 'w' up
		head = insertToHead(head, newSnakeNode((head->x) - 1, head->y, head));
		break;
	case 's'://if 's' down
		head = insertToHead(head, newSnakeNode((head->x) + 1, head->y, head));
		break;
	case 'a'://if 'a' left
		head = insertToHead(head, newSnakeNode(head->x, (head->y) - 1, head));
		break;
	case 'd'://if 'd' right
		head = insertToHead(head, newSnakeNode(head->x, (head->y) + 1, head));
		break;
	}

	*ContinueGame = countinueGame(head);

	if (stepsCount % TURNS != 0)
	{
		freeLast(head);
	}

	return head;
}

void freeLast(SNKAENODE* head)
{
	SNKAENODE* current = head->next;
	SNKAENODE* previous = head;

	while (current->next != NULL)
	{
		previous = current;
		current = current->next;
	}

	gotoxy(current->x, current->y);
	printf(" ");
	free(current);
	previous->next = NULL;
}

bool countinueGame(SNKAENODE* head)
{
	if (head->y == 1 || head->y == LEFTRIGHTLIM || head->x == 1 || head->x == UPPERLOWERLIM)//checks collision with limits
		return false;
	if (selfCollide(head))//checks coliision with snake body
		return false;
	return true;
}

bool selfCollide(SNKAENODE* head)
{
	SNKAENODE* current = head;

	while (current->next != NULL)
	{
		if ((head->x == current->next->x) && (head->y == current->next->y))//checks if head x,y values equals to snake body nodes x,y values
		{
			return true;//if so return true, meaning the snake colided with itself
		}

		current = current->next;
	}
	return false;
}