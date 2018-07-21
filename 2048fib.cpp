#define _CRT_SECURE_NO_WARNINGS
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include<conio.h>
#include<malloc.h>
#include<string.h>

typedef struct Board {
	int **table;
	char *playername;
	int NumberofMoves = 0;
	int n;
	int state = 0;  //0 for unfinished, 1 for game over
};
struct Player{
	char name[100];
	int no_moves;
	int size;
};
void playmove(Board *board, char c);
void chooseMenu();
int stringToNum(char *s){
	int len, num = 0;
	for (len = 0; s[len] != '\0'; len++){}
	int f = 10;
	int asc;
	for (int i = 0; i<len; i++){
		if (s[i] == ' ' || s[i] == '\n')
			continue;
		num = num * 10 + (s[i] - '0');
	}
	return num;
}
int isValidInput(char letter){
	if (letter == 'w' || letter == 'a' || letter == 's' || letter == 'd' || letter == 'W' || letter == 'A' || letter == 'S' || letter == 'D' || letter == 'z' || letter == 'Z')
		return 1;

	else
		return 0;
}
void storeLeaderboard(Board *board){
	FILE *fp = fopen("leaderboard.txt", "a");
	fprintf(fp, "%s %d %d\n", board->playername, board->NumberofMoves, board->n);
	fclose(fp);
}
void drawBoardOnScreen(Board *board){
	printf("\nPlayer: %s\tNo.of Moves: %d\n\n", board->playername, board->NumberofMoves);
	for (int i = 0; i < board->n; i++){
		for (int j = 0; j < board->n; j++){
				printf("  %d  ", board->table[i][j]);
		}
		printf("\n\n");
	}
	printf("\n[W] UP --- [S] Down --- [A] LEFT --- [D] Right\n");


}
void clearScreen(){
	system("cls");
}


void clearAndRedrawScreen(Board * board){
	clearScreen();
	drawBoardOnScreen(board);
}

Board * initialisegame(int n,char* name){
	Board *board = (struct Board*)malloc(sizeof(struct Board));
	int **tab = (int**)malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++){
		tab[i] = (int*)malloc(sizeof(int)*n);
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++)
		{
			tab[i][j] = 0;
		}
	}
	tab[0][n-1] = 1;
	tab[n-1][0] = 1;

	board->table = tab;
	board->n = n;
	board->playername = name;
	board->NumberofMoves = 0;
	board->state = 0;

	return board;
}
int validateRowSize(int n){
	if (n < 2)
		return 0;
	return 1;
}
int takeRowSize(){
	int n;
	x:printf("Enter the game size:");
	scanf("%d", &n);
	
	if (n < 2)
		goto x;
	return n;
}
int  findfibind(int n){
	if (n <= 1)
		return n;

	int a = 0, b = 1, c = 1;
	int res = 1;

	while (c < n)
	{
		c = a + b;

		res++;
		a = b;
		b = c;
	}
	return res;

}
int consec(int x, int y){
	if ((x == 1 && y == 1) || (x == 1 && y == 2) || (x == 2 && y == 1))
		return 1;
	int a = findfibind(x);
	int b = findfibind(y);
	if ((a - b == 1) || (b - a == 1))
		return 1;
	return 0;
}
void goUp(Board *board){
	int start, stop;
	int n = board->n;
	for (int j = 0; j < n; j++){
		for (int i = 0; i <n-1; i++){
			if (board->table[i][j] != 0){
				start = i;
				i++;
				while (i<n-1 &&board->table[i][j] == 0){
					i++;
				}
				stop = i;
				if (consec(board->table[start][j], board->table[stop][j])){
					board->table[start][j] += board->table[stop][j];
					board->table[stop][j] = 0;

				}
				else i--;
			}
		}
	}

	int **res = (int **)malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++){
		res[i] = (int *)malloc(sizeof(int)*n);
	}
	for (int j = 0; j < n; j++){
		int k = 0;
		for (int i = 0; i <n; i++){
			if (board->table[i][j] != 0)
				res[k++][j] = board->table[i][j];
		}
		for (int i = k; i <n; i++)
			res[i][j] = 0;

		for (int i = 0; i < n; i++){
			board->table[i][j] = res[i][j];
		}

	}
	return;
}

void goDown(Board *board){
	int start, stop;
	int n = board->n;
	for (int j = 0; j < n; j++){
		for (int i = n - 1; i > 0; i--){
			if (board->table[i][j] != 0){
				start = i;
				i--;
				while (i>0 && board->table[i][j] == 0){
					i--;
				}
				stop = i;
				if (consec(board->table[start][j], board->table[stop][j])){
					board->table[start][j] += board->table[stop][j];
					board->table[stop][j] = 0;

				}
				else i++;
			}
		}
	}

	int **res = (int **)malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++){
		res[i] = (int *)malloc(sizeof(int)*n);
	}
	for (int j = 0; j < n; j++){
		int k = n - 1;
		for (int i = n - 1; i >= 0; i--){
			if (board->table[i][j] != 0)
				res[k--][j] = board->table[i][j];
		}
		for (int i = 0; i <= k; i++)
			res[i][j] = 0;

		for (int i = 0; i < n; i++){
			board->table[i][j] = res[i][j];
		}
	}
	return;


}
void goLeft(Board *board){
	int start, stop;
	int n = board->n;
	for (int j = 0; j < n; j++){
		for (int i = 0; i <n; i++){
			if (board->table[j][i] != 0){
				start = i;
				i++;
				while (board->table[j][i] == 0){
					i++;
				}
				stop = i;
				if (consec(board->table[j][start], board->table[j][stop])){
					board->table[j][start] += board->table[j][stop];
					board->table[j][stop] = 0;

				}
				else i--;
			}
		}
	}

	int **res = (int **)malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++){
		res[i] = (int *)malloc(sizeof(int)*n);
	}
	for (int j = 0; j < n; j++){
		int k = 0;
		for (int i = 0; i <n; i++){
			if (board->table[j][i] != 0)
				res[j][k++] = board->table[j][i];
		}
		for (int i = k; i <n; i++)
			res[j][i] = 0;

		for (int i = 0; i < n; i++){
			board->table[j][i] = res[j][i];
		}
		
	}
	return;

}
void goRight(Board *board){
	int start, stop;
	int n = board->n;
	for (int j = 0; j < n; j++){
		for (int i = n - 1; i > 0; i--){
			if (board->table[j][i] != 0){
				start = i;
				i--;
				while (board->table[j][i] == 0){
					i--;
				}
				stop = i;
				if (consec(board->table[j][start], board->table[j][stop])){
					board->table[j][start] += board->table[j][stop];
					board->table[j][stop] = 0;

				}
				else i++;
			}
		}
	}
		
		int **res = (int **)malloc(sizeof(int*)*n);
		for (int i = 0; i < n; i++){
			res[i] = (int *)malloc(sizeof(int)*n);
		}
		for (int j = 0; j < n; j++){
			int k = n - 1;
			for (int i = n - 1; i >= 0; i--){
				if (board->table[j][i] != 0)
					res[j][k--] = board->table[j][i];
			}
			for (int i = 0; i <= k; i++)
				res[j][i] = 0;

			for (int i = 0; i < n; i++){
				board->table[j][i] = res[j][i];
			}
		}
		return;
	
}
int hasmove(Board *board){
	int n = board->n;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (board->table[i][j] == 0)
				return 1;
			if (( (j+1<n)&&consec(board->table[i][j], board->table[i][j + 1])) || ( (i+1<n) &&consec(board->table[i][j], board->table[i + 1][j])))
				return 1;
			
		}
	}
	return 0;
}
int checkEnd(Board *board){
	if (hasmove(board)==0)
		return 0;
	return 1;
}
int win(Board *board){
	int n = board->n;
	for (int i = 0; i < n ; i++){
		for (int j = 0; j < n ; j++){
			if (board->table[i][j]==(2*n*n))
				return 1;
		}
	}
	return 0;
}
void incrementMoves(Board *board){
	board->NumberofMoves = board->NumberofMoves + 1;
}
char takeDirection(){
	char *c = (char*)malloc(sizeof(char) * 2);
	printf("Enter the direction of game:");
	scanf("%s", c);
	if (isValidInput(c[0]) == 0){
		printf("Re-enter the direction of game:");
		scanf("%c", c);
		isValidInput(c[0]);
	}
	return c[0];

}
void playgame(Board *board){
	char c = takeDirection();
	playmove(board, c);
}

void saveBoard(Board *board){
	
//	printf("%d %s %d\n", board->n, board->playername, board->NumberofMoves);
	system("pause");

	FILE *fp = fopen("replay.txt", "w");
	fprintf(fp, "%d\n", board->n);
	fprintf(fp, "%s\n",board->playername);
	fprintf(fp, "%d\n", board->NumberofMoves);
	int n = board->n;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			fprintf(fp, "%d\n", board->table[i][j]);
		}
	}
	fclose(fp);
}

void replay(){
	FILE *fp = fopen("replay.txt", "r");
	int n;
	char buff[255];
	int count1 = 0;

	while ((fgets(buff, 255, (FILE*)fp)) != NULL){
		count1++;
//		printf("%s \n", buff);
	}
	if (count1 == 0){
		printf("You havenot saved anything\n");
		system("pause");
		chooseMenu();
		return;
	}
	fp = fopen("replay.txt", "r");
	char s[100];
	fscanf(fp, "%d", &n);

	printf("%d\n", n);
	Board *board = (struct Board*)malloc(sizeof(struct Board));
	int **tab = (int**)malloc(sizeof(int*)*n);
	for (int i = 0; i < n; i++){
		tab[i] = (int*)malloc(sizeof(int)*n);
	}
	char name[100];
	fscanf(fp, "%s", name);
	int no_moves;
	fscanf(fp, "%d", &no_moves);

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++)
		{
			fscanf(fp, "%d", &tab[i][j]);
		}
	}
	
	

	board->table = tab;
	board->n = n;
	board->playername = name;
	board->NumberofMoves = no_moves;
	board->state = 0;
	clearAndRedrawScreen(board);
	playgame(board);
}
void playmove(Board *board ,char c){
	incrementMoves(board);
	if (c == 'w' || c == 'W')
		goUp(board);
	else if (c == 's' || c == 'S')
		goDown(board);
	else if (c == 'a' || c == 'A')
		goLeft(board);
	else if (c == 'd' || c == 'D')
		goRight(board);
	else if (c == 'z' || c == 'Z'){
		board->NumberofMoves = board->NumberofMoves - 1;
		saveBoard(board);
		clearAndRedrawScreen(board);
		playgame(board);
	}
		
	//drawBoardOnScreen(board);
	int n = board->n,f=0;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++)
		{
			if (board->table[i][j] == 0)
			{
				board->table[i][j] = 1;
				f = 1;
				break;
			}
		}
		if (f == 1)
			break;
	}
	
	if (win(board)){
		clearAndRedrawScreen(board);
		printf("\nWon with no.of moves:%d\n", board->NumberofMoves);
		storeLeaderboard(board);
		system("pause");
		chooseMenu();
		return;
	}
	if (checkEnd(board)==0){
		clearAndRedrawScreen(board);
		printf("\nYOU lose with no.of moves:%d\n", board->NumberofMoves);
		storeLeaderboard(board);
		system("pause");

		chooseMenu();
		return;
	}
	
	clearAndRedrawScreen(board);
	playgame(board);
}


void viewLeaderboard(){
	FILE *fp = fopen("leaderboard.txt", "r");
	char buff[255];
	int count1 = 0;
	while ((fgets(buff, 255, (FILE*)fp)) != NULL){
		count1++;
	}
	
	Player *pl = (struct Player*)malloc(sizeof(struct Player)*count1);

	char name[200];
	int no_move;
	int size;
	printf("%d", count1);
	//bubble sort
	fp = fopen("leaderboard.txt", "r");
	for (int i = 0; i < count1; i++){
		fscanf(fp, "%s", pl[i].name);
		fscanf(fp, "%d", &pl[i].no_moves);
		fscanf(fp, "%d", &pl[i].size);
	}

	Player temp;
	for (int i = 0; i < count1-1; i++){
		for (int j = 0; j < count1 - i - 1; j++){
			if (pl[j].no_moves > pl[j+1].no_moves){
				temp = pl[j];
				pl[j] = pl[j + 1];
				pl[j + 1] = temp;
			}
		}
	}
	fp = fopen("leaderboard.txt", "w");
	printf("\nLEADREBOARD\n");
	printf("NAME\tMOVES\tSIZE\n");
	for (int i = 0; i < count1; i++){
		printf("%s\t%d\t%d*%d\n", pl[i].name, pl[i].no_moves, pl[i].size, pl[i].size);
		fprintf(fp,"%s %d %d\n", pl[i].name, pl[i].no_moves, pl[i].size);
	}
	
	system("pause");
	chooseMenu();
}

void chooseMenu(){
	system("cls");
	printf("Enter choice from below\n");
	printf("1.NEWGAME\n2.Leaderboard \n3.replay\n4.exit\n");
	int ch;
	scanf("%d", &ch);
	if (ch == 1){
		int n = takeRowSize();
		char *name = (char*)malloc(sizeof(char) * 10);
		printf("Enter the players name(at max 10 chars):");
		scanf("%s", name);
		Board * board = initialisegame(n, name);
		playgame(board);
	}
	else if (ch == 2){
		viewLeaderboard();
	}
	else if (ch == 3){
		replay();
	}
	else if (ch == 4){
		exit(0);
	}
}
int main(){
	chooseMenu();
	_getch();
}