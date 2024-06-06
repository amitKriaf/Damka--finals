#pragma once
#define _CRT_SECURE_NO_WARNINGS

#define BOARD_SIZE 8
#define SMALLEST_NUM '1'
#define BIGGEST_NUM '8'
#define SMALLEST_ROW 'A'
#define BIGGEST_ROW 'H'
#define PLAYER_B 'B'
#define PLAYER_T 'T'
#define EMPTY ' '
#define MAX_TOOLS 12
#define ERROR 'X'

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



unsigned short maxCapture;
int countMovesB, countMovesT;
char maxCapPlayer;

typedef struct checkersPos
{
	char row, col;
}checkersPos;

typedef unsigned char Board[BOARD_SIZE][BOARD_SIZE];

typedef unsigned char Player;

typedef struct SingleSourceMovesListCell
{
	checkersPos* position;
	unsigned short captures;
	struct SingleSourceMovesListCell* next;
}SingleSourceMovesListCell;

typedef struct SingleSourceMovesList
{
	SingleSourceMovesListCell* head;
	SingleSourceMovesListCell* tail;
}SingleSourceMovesList;

typedef struct multipleSourceMovesListCell {
	SingleSourceMovesList* single_source_moves_list;
	struct multipleSourceMovesListCell* next;
}MultipleSourceMovesListCell;

typedef struct multipleSourceMovesList {
	MultipleSourceMovesListCell* head;
	MultipleSourceMovesListCell* tail;
}MultipleSourceMovesList;



typedef struct SingleSourceMovesTreeNode
{
	Board board;
	checkersPos* pos;
	unsigned short total_capture_so_far;
	struct SingleSourceMovesTreeNode* next_move[2];
}SingleSourceMovesTreeNode;

typedef struct SingleSourceMovesTree
{
	SingleSourceMovesTreeNode* source;
}SingleSourceMovesTree;




SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree);
SingleSourceMovesList FindSingleSourceOptimalMoveHelper(SingleSourceMovesTreeNode* moves_root, int* size, Player player);
void makeEmptyList(SingleSourceMovesList* lst);
bool isEmptyList(SingleSourceMovesList* lst);
SingleSourceMovesListCell* createNewListNode(char row, char col, unsigned short cap, SingleSourceMovesListCell* next);
void freeList(SingleSourceMovesList* lst);
void insertDataToStartList(SingleSourceMovesList* lst, char row, char col, unsigned short capture);
void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head);
MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player);

void makeEmptyListMultiple(MultipleSourceMovesList* lst);
bool isEmptyListMultiple(MultipleSourceMovesList* lst);
void freeMultopleList(MultipleSourceMovesList* lst);
void insertDataToStartMultipleList(MultipleSourceMovesList* lst, SingleSourceMovesList* single_list);
void insertNodeToStartMultipleList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* head);
MultipleSourceMovesListCell* createNewListMultipleNode(SingleSourceMovesList* single_list, MultipleSourceMovesListCell* next);

SingleSourceMovesList* maxTurnList(MultipleSourceMovesList* lst, Player player);
