//_____________________________________________________Description_______________________________________________________
//Snake classic game- eat 10 $ in order to win. if you hit the wall or the snake you lose.
//ID 316444777
//Full Name- IDO FASSBERG
//_______________________________________________________________________________________________________________________
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

//______________________________________________________Constants________________________________________________________
#define UP 72  // Up Arrow 
#define DOWN 80 // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27  // Escape Key
#define MAX 20 //max lengh of the snake
#define FOOD 1 //represent the food location
#define HEAD 0 //cordinates of the snake head
//_______________________________________________________Structs_________________________________________________________
typedef struct Snake {
	int x; //cordinate x (cols)
	int y; //cordinate y (rows)
}Snake;

typedef struct {
	int x; //cordinate x (cols)
	int y; //cordinate y (rows)
}FoodLocation;
//________________________________________________Functions Declerations_________________________________________________
void gotoxy(int y, int x); // This function moves the cursor to the line y and column x.
int getKey(); // gets keyCodes for special keys such as arrows and others, returns the Key code such as 72 for arrow up
void printBoard(); // print the lines #
void firstPostion(Snake arr[], int snake_len); //start the snake from the middle of the board
void printMainMenu();
void printGameOver(int counter); //win or lose is printed
void GamePlay(Snake arr[], int snake_len, FoodLocation food[],int speed);
void changeLocation(Snake arr[], int snake_len); //to make the snake "move"
char firstMove();
void plantFood(FoodLocation food[], Snake arr[], int snake_len); //randomaly plant food in the board
void rightReset(Snake arr[], int snake_len);
bool eatFood(Snake arr[], FoodLocation food[]); //make the snake bigger
void deleteTail(Snake arr[], int snake_len);
bool loseGame(Snake arr[], int snake_len);
bool start();
//________________________________________________________Main____________________________________________________________

void main()
{
	srand((int)time(NULL)); //using the time to plant food randomly
	Snake arr[MAX];
	FoodLocation food[FOOD];
	int snake_len = 10;
	int speed = 1000;
	bool isPlaying = true, exit = false;
	while (isPlaying) {
		exit = start(); //show the main menu and if user select 0, then exit the program
		if (exit) {
			isPlaying = false;
			continue;
		}
		printBoard();
		GamePlay(arr, snake_len, food, speed);
	}
}
//____________________________________________Functions Implemenations___________________________________________________

void gotoxy(int y, int x) { 
	printf("\x1b[%d;%df", x + 1, y + 1);
}
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}
void printBoard() {

	int i;
	int len = 76, width = 25;
	for (i = 0; i <= len; i++) { // print the upper line
		printf("#");
	}
	for (i = 1; i <= width; i++) {

		gotoxy(0, i);
		printf("#"); //print the left border
		gotoxy(len, i);
		printf("#"); //print the right border
	}
	gotoxy(0, width + 1);
	for (i = 0; i <= len; i++) {
		printf("#"); //print the low line
	}
} 
void firstPostion(Snake arr[], int snake_len) {

	for (int i = 0; i < snake_len; i++) {
		arr[i].x = 30 + i;
		arr[i].y = 10;
		gotoxy(arr[i].x, arr[i].y);
		printf("@");
	}
}
void GamePlay(Snake arr[], int snake_len, FoodLocation food[],int speed) {
	char c, temp;
	bool GameOver = false;
	int counter = 0; //how many $ the snake eat

	plantFood(food, arr, snake_len);
	firstPostion(arr, snake_len);
	c = firstMove();
	if (c == RIGHT)
		rightReset(arr, snake_len);
	if (c == ESC) {
		system("cls");
	}
	while (c != ESC && !GameOver)
	{
		Sleep(speed);  // sleeps for a second before continuing to the next command

		if (_kbhit()) { // if any key was hit
			temp = getKey();
			if (temp == UP || temp == DOWN || temp == RIGHT || temp == LEFT || temp == ESC)
				c = temp;  // change direction
		}

		switch (c) 	// make a move in current direction if no change
		{
		case RIGHT:
			deleteTail(arr, snake_len);
			changeLocation(arr, snake_len);
			arr[HEAD].x = arr[1].x + 1;
			break;
		case DOWN:
			deleteTail(arr, snake_len);
			changeLocation(arr, snake_len);
			arr[HEAD].y = arr[1].y + 1;
			break;
		case LEFT:
			deleteTail(arr, snake_len);
			changeLocation(arr, snake_len);
			arr[HEAD].x = arr[1].x - 1;
			break;
		case UP:
			deleteTail(arr, snake_len);
			changeLocation(arr, snake_len);
			arr[HEAD].y = arr[1].y - 1;
			break;
		}
		gotoxy(arr[HEAD].x, arr[HEAD].y);
		printf("@");
		if (eatFood(arr, food)){
			plantFood(food, arr, snake_len);
			snake_len++;
			counter++;
		}
		if (loseGame(arr, snake_len) || counter == 10)
			GameOver = true;

		gotoxy(0, 30);
	}
	printGameOver(counter);
}
void printMainMenu() {
	printf("***************************************\n");
	printf("* Pick Your Level, to exit press 0\n");
	printf("* 0 - Exit\n");
	printf("* 1 - Basic Level\n");
	printf("* 2 - Intermidiate Level\n");
	printf("* 3 - Advanced Level\n");
	printf("***************************************\n");
}
void printGameOver(int counter) {
	system("cls");
	printf("#######################\n");
	if (counter == 10)
		printf("      YOU WIN!\n");
	else
		printf("      GAME OVER\n");
	printf("#######################\n");
}
void changeLocation(Snake arr[], int snake_len) {
	int i;
	for (i = snake_len - 1; i > 0; i--) {
		arr[i].x = arr[i - 1].x;
		arr[i].y = arr[i - 1].y;
	}
}
bool loseGame(Snake arr[], int snake_len) {
	bool lose = false;
	if (arr[HEAD].x == 0 || arr[HEAD].x == 75 || arr[HEAD].y == 0 || arr[HEAD].y == 26) //if snake hit border
		lose = true;
	for (int i = 1; i < snake_len - 1; i++) { //if snake hit himself
		if (arr[HEAD].x == arr[i].x && arr[HEAD].y == arr[i].y)
			lose = true;
	}
	return lose;
}
char firstMove() {
	char c;
	bool choose = false;
	while (choose == false) {
		_kbhit(); //user decide witch diraction go for start
		c = getKey();
		if (c == UP || c == DOWN || c == RIGHT || c == LEFT || c == ESC)
			choose = true;
	}
	return c;

}
void plantFood(FoodLocation food[], Snake arr[], int snake_len) {
	int tempX, tempY;
	bool X_ok = false, Y_ok = false;
	bool isChoosing = true;
	while (isChoosing) { //make sure it doesn't plant the food on the snake itself
		isChoosing = false;
		tempX = 1 + rand() % 75;
		tempY = 1 + rand() % 25;
		for (int i = 0; i < snake_len - 1; i++) {
			if (tempX == arr[i].x) {
				for (int j = 0; j < snake_len - 1; j++) {
					if (tempY == arr[j].y) {
						isChoosing = true;
					}
				}
			}
			else
				continue;
		}
	}
	food[0].x = tempX;
	food[0].y = tempY;
	gotoxy(food[0].x, food[0].y);
	printf("$");
}
void rightReset(Snake arr[], int snake_len) {
	for (int i = snake_len - 1; i >= 0; i--) {
		arr[i].x = 39 - i;
		arr[i].y = 10;
		gotoxy(arr[i].x, arr[i].y);
		printf("@");
	}
}
bool eatFood(Snake arr[], FoodLocation food[]) {
	bool eat = false;
	if (arr[HEAD].x == food[0].x && arr[HEAD].y == food[0].y)
		eat = true;
	return eat;
}
void deleteTail(Snake arr[], int snake_len) {
	gotoxy(arr[snake_len - 1].x, arr[snake_len - 1].y);
	printf(" ");
}
bool start() {
	bool isChoosing = true, exit = false;
	int choice;
	while (isChoosing) {
		printMainMenu();
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			isChoosing = false;
			break;
		case 0:
			exit = true;
			isChoosing = false;
		default:
			continue;
		}
	}
	system("cls");
	return exit;
}
//_______________________________________________________________________________________________________________________