#include "list.h"
#include "tree.h"
#include "game.h"

void main()
{
	extern unsigned short maxCapture;
	extern int countMovesB, countMovesT;
	extern char maxCapPlayer;
	Board board = { {' ', 'T', ' ','T', ' ', 'T',' ', 'T'},
					{'T', ' ', 'T', ' ','T', ' ', 'T',' '},
					{' ', 'T', ' ','T', ' ', 'T',' ', 'T'},
					{' ', ' ', ' ', ' ',' ', ' ', ' ',' '},
					{' ', ' ', ' ', ' ',' ', ' ', ' ',' '},
					{'B', ' ', 'B', ' ','B', ' ', 'B',' '},
					{' ', 'B', ' ','B', ' ', 'B',' ', 'B'},
					{'B', ' ', 'B', ' ','B', ' ', 'B',' '} };

	Player firstPlayer;
	printf("Please enter first player: (enter 'B' or 'T'): ");
	scanf(" %c", &firstPlayer);
	while (firstPlayer != 'T' && firstPlayer != 'B')
	{
		printf("Invalid input! try again!");
		scanf(" %c", &firstPlayer);
	}
	maxCapture = 0;
	countMovesB = countMovesT = 0;
	maxCapPlayer = firstPlayer;
	PlayGame(board, firstPlayer);
}