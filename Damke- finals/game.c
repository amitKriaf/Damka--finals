#include "game.h"
void newMoveBoard(Board board, Board newBoard, Player player, checkersPos pStart, checkersPos pEnd)
{
	// This func create a new board with one move : eating or just one move.
	copyBoard(board, newBoard);
	if (pEnd.col - pStart.col == '1' || pStart.col - pEnd.col == '1')
	{
		newBoard[pStart.row - 'A'][pStart.col - '0'] = EMPTY;
		newBoard[pEnd.row - 'A'][pStart.col - '0'] = player;
	}
	else
	{
		newBoard[pStart.row - 'A'][pStart.col - '0'] = EMPTY;
		newBoard[pEnd.row - 'A'][pStart.col - '0'] = player;
		if (player == PLAYER_T)
			if (pStart.col > pEnd.col)
				newBoard[pStart.row - 'A' + 1][pStart.col - '0' - 1] = EMPTY;
			else
				newBoard[pStart.row - 'A' + 1][pStart.col - '0' + 1] = EMPTY;
		else
			if (pStart.col > pEnd.col)
				newBoard[pStart.row - 'A' - 1][pStart.col - '0' - 1] = EMPTY;
			else
				newBoard[pStart.row - 'A' - 1][pStart.col - '0' + 1] = EMPTY;

	}
}

bool checkIfInsideBoard(checkersPos* next)
{
	// This func checks if the checker pos is inside the array size.
	return (next->col >= SMALLEST_NUM && next->col <= BIGGEST_NUM &&
		next->row >= SMALLEST_ROW && next->row <= BIGGEST_ROW);
}

Player playerPanel(Board board, char rows, char cols)
{
	// The func cheak if the scr is empty.
	int col = cols - 1 - '0';
	int row = rows - 'A';
	checkersPos temp = { rows, cols };
	if (!checkIfInsideBoard(&temp))
		return ERROR;
	return board[row][col];
}

void copyBoard(Board board, Board  res)
{
	// The func get a board and a pointer to res board and copy the first to the second. 
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++)
		for (j = 0; j < BOARD_SIZE; j++)
			res[i][j] = board[i][j];
}

bool isSkipOk(Board board, char row, char col)
{
	//this func gets a board and a place inside and checks if it is inside the board or if this place is empty and return the answer
	Player pTemp = playerPanel(board, row, col);
	if (pTemp != EMPTY)
		return false;
	return true;
}

void printTable(Board board)
{
	//this func prints the board
	int i, j;
	printf("+-+-+-+-+-+-+-+-+-+\n");
	printf("+ |1|2|3|4|5|6|7|8|\n");
	for (i = 0; i < BOARD_SIZE; i++) {
		printf("+-+-+-+-+-+-+-+-+-+\n");
		printf("|%c|", ('A' + i));
		for (j = 0; j < BOARD_SIZE; j++) {
			printf("%c|", board[i][j]);
		}
		printf("\n");
	}
	printf("+-+-+-+-+-+-+-+-+-+\n");
}

void updateBoard(Board board, SingleSourceMovesList* lst, Player player)
{
	// This func update the board whid the list moves.
	SingleSourceMovesListCell* curr = lst->head;
	int col, row;
	while (curr != NULL)
	{
		row = curr->position->row - 'A';
		col = curr->position->col - '0' - 1;
		board[row][col] = EMPTY;
		curr = curr->next;
	}
	board[row][col] = player;
}

checkersPos createPos(char row, char col)
{
	// This func return a pos with  the row and col.
	checkersPos res = { row, col };
	return res;
}

void turn(Board board, Player player)
{
	// This func doing the player turn.
	extern unsigned short maxCapture;
	extern int countMovesB, countMovesT;
	extern char maxCapPlayer;
	MultipleSourceMovesList* my_turn = FindAllPossiblePlayerMoves(board, player);
	SingleSourceMovesList* the_turn = maxTurnList(my_turn, player);
	SingleSourceMovesListCell* curr = the_turn->head;
	if (maxCapture < the_turn->tail->captures)
	{
		maxCapture = the_turn->tail->captures;
		maxCapPlayer = player;
	}
	printf("%c's turn: \n", player);
	while (curr->next != NULL)
	{
		printf("%c%c -> ", curr->position->row, curr->position->col);
		curr = curr->next;
	}
	if (player == PLAYER_B)
		countMovesB++;
	else
		countMovesT++;
	printf("%c%c\n", curr->position->row, curr->position->col);
	updateBoard(board, the_turn, player);
	printTable(board);
	freeMultopleList(my_turn);
	free(my_turn);
	freeList(the_turn);
	free(the_turn);
}

bool countPlayers(Board board, Player player)
{
	//this func checks if thhere's evem one player in the board
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++)
		{
			if (player == board[i][j])
				return true;
		}
	}
	return false;
}
void PlayGame(Board board, Player starting_player)
{
	//this func gets a board and the starting player and plays the game until winning.
	extern unsigned short maxCapture;
	extern int countMovesB, countMovesT;
	extern char maxCapPlayer;
	bool starts = true;
	int maxCapPlayerOne, maxCapPlayerTwo;
	Player second_player, winner;
	if (starting_player == PLAYER_T)
		second_player = PLAYER_B;
	else
		second_player = PLAYER_T;
	printTable(board);
	winner = checkIfEndOfGame(board);
	while (winner == EMPTY)
	{

		if (starts)
		{
			turn(board, starting_player);
			starts = false;
		}
		else//the second player
		{
			turn(board, second_player);
			starts = true;
		}
		winner = checkIfEndOfGame(board);
	}
	printf("\n%c wins !\n", winner);
	if (winner == PLAYER_B)
		printf("%c performed %d moves.\n", winner, countMovesB);
	else
		printf("%c performed %d moves.\n", winner, countMovesT);
	printf("%c performed the highest number of captures in a single move - %d", maxCapPlayer, maxCapture);

}

Player checkIfEndOfGame(Board board)
{
	//this func checks if someone won, if so it returns its name, else returns space
	int i;
	int numOfTplayers, numOfBplayers;

	if (!countPlayers(board, PLAYER_B))
		return PLAYER_T;
	else if (!countPlayers(board, PLAYER_T))
		return PLAYER_B;
	for (i = 0; i < BOARD_SIZE; i++) {
		if (board[0][i] == PLAYER_B)
			return PLAYER_B;
		else if (board[BOARD_SIZE - 1][i] == PLAYER_T)
			return PLAYER_T;
	}
	return EMPTY;
}