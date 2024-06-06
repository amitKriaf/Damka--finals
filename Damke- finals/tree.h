#pragma once
#include "list.h"

SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* scr);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(Board board, char row, char col, char player, unsigned short cap);
SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(Board board, char row, char col, char player, unsigned short cap);
SingleSourceMovesTreeNode* createNewTreeNode(Board board, char row, char col, unsigned short total_capture_so_far,
	SingleSourceMovesTreeNode* left, SingleSourceMovesTreeNode* right);
void freeTree(SingleSourceMovesTree* tr);
void freeTreeHelper(SingleSourceMovesTreeNode* root);