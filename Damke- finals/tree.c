#include "list.h"
#include "tree.h"
SingleSourceMovesTree* FindSingleSourceMoves(Board board, checkersPos* scr)
{
	// This fun create a tree with all the moves that the scr can do. 
	SingleSourceMovesTree* tree;
	SingleSourceMovesTreeNode* left, * right;
	Player player = playerPanel(board, scr->row, scr->col);
	if (player == EMPTY)
		return NULL;
	tree = (SingleSourceMovesTree*)malloc(sizeof(SingleSourceMovesTree));
	if (tree == NULL)
	{
		printf("Memory allocation falied!!\n");
		exit(1);
	}
	if (player == PLAYER_T)
		tree->source = FindSingleSourceMovesHelperT(board, scr->row, scr->col, player, 0);
	else
		tree->source = FindSingleSourceMovesHelperB(board, scr->row, scr->col, player, 0);
	return tree;
}

SingleSourceMovesTreeNode* FindSingleSourceMovesHelperT(Board board, char row, char col, char player, unsigned short cap)
{
	// The fucn is creating a tree in recursia of the posabilities for movement inside the board for player t.
	Board newBoard;
	checkersPos posStart = { row, col }, posEnd;
	SingleSourceMovesTreeNode* left = NULL, * right = NULL, * root = NULL;
	Player currRight = playerPanel(board, row + 1, col + 1);
	Player currLeft = playerPanel(board, row + 1, col - 1);
	Player currPlayer = playerPanel(board, row, col);
	if (currPlayer == ERROR)
		return NULL;
	if (currLeft == player && currRight == player)
		return createNewTreeNode(board, row, col, cap, NULL, NULL);
	if (currLeft == EMPTY && cap == 0)
		left = createNewTreeNode(board, row + 1, col - 1, cap, NULL, NULL);
	if (currRight == EMPTY && cap == 0)
		right = createNewTreeNode(board, row + 1, col + 1, cap, NULL, NULL);
	if (currLeft == PLAYER_B && isSkipOk(board, row + 2, col - 2))
	{
		posEnd.row = row + 2;
		posEnd.col = col - 2;
		newMoveBoard(board, newBoard, player, posStart, posEnd);
		left = createNewTreeNode(newBoard, row + 1, col - 1, cap + 1, NULL, NULL);
		left->next_move[0] = FindSingleSourceMovesHelperT(newBoard, row + 2, col - 2, player, cap + 1);
		left->next_move[1] = NULL;
	}
	if (currRight == PLAYER_B && isSkipOk(board, row + 2, col + 2))
	{
		posEnd.row = row + 2;
		posEnd.col = col + 2;
		newMoveBoard(board, newBoard, player, posStart, posEnd);
		right = createNewTreeNode(newBoard, row + 1, col + 1, cap + 1, NULL, NULL);
		right->next_move[1] = FindSingleSourceMovesHelperT(newBoard, row + 2, col + 2, player, cap + 1);
		right->next_move[0] = NULL;
	}
	root = createNewTreeNode(board, row, col, cap, left, right);
	return root;
}

SingleSourceMovesTreeNode* FindSingleSourceMovesHelperB(Board board, char row, char col, char player, unsigned short cap)
{
	// The fucn is creating a tree in recursia of the posabilities for movement inside the board for player B.
	Board newBoard;
	checkersPos posStart = { row, col }, posEnd;
	SingleSourceMovesTreeNode* left = NULL, * right = NULL, * root = NULL;
	Player currRight = playerPanel(board, row - 1, col + 1);
	Player currLeft = playerPanel(board, row - 1, col - 1);
	Player currPlayer = playerPanel(board, row, col);
	if (currPlayer == ERROR)
		return NULL;
	if (currLeft == player && currRight == player)
		return createNewTreeNode(board, row, col, cap, NULL, NULL);
	if (currLeft == EMPTY && cap == 0)
		left = createNewTreeNode(board, row - 1, col - 1, cap, NULL, NULL);
	if (currRight == EMPTY && cap == 0)
		right = createNewTreeNode(board, row - 1, col + 1, cap, NULL, NULL);
	if (currLeft == PLAYER_T && isSkipOk(board, row - 2, col - 2))
	{
		posEnd.row = row - 2;
		posEnd.col = col - 2;
		newMoveBoard(board, newBoard, player, posStart, posEnd);
		left = createNewTreeNode(newBoard, row - 1, col - 1, cap + 1, NULL, NULL);
		left->next_move[0] = FindSingleSourceMovesHelperB(newBoard, row - 2, col - 2, player, cap + 1);
		left->next_move[1] = NULL;
	}
	if (currRight == PLAYER_T && isSkipOk(board, row - 2, col + 2))
	{
		posEnd.row = row - 2;
		posEnd.col = col + 2;
		newMoveBoard(board, newBoard, player, posStart, posEnd);
		right = createNewTreeNode(newBoard, row - 1, col + 1, cap + 1, NULL, NULL);
		right->next_move[1] = FindSingleSourceMovesHelperB(newBoard, row - 2, col + 2, player, cap + 1);
		right->next_move[0] = NULL;
	}
	root = createNewTreeNode(board, row, col, cap, left, right);
	return root;
}

SingleSourceMovesTreeNode* createNewTreeNode(Board board, char row, char col, unsigned short total_capture_so_far,
	SingleSourceMovesTreeNode* left, SingleSourceMovesTreeNode* right)
{
	// The func create a new tree node with the details.
	SingleSourceMovesTreeNode* res;
	checkersPos* pos;
	res = (SingleSourceMovesTreeNode*)malloc(sizeof(SingleSourceMovesTreeNode));
	pos = (checkersPos*)malloc(sizeof(checkersPos));
	if (res == NULL || pos == NULL)
	{
		printf("Memory allocation falied!!\n");
		exit(1);
	}
	pos->col = col;
	pos->row = row;
	copyBoard(board, res->board);
	res->pos = pos;
	res->total_capture_so_far = total_capture_so_far;
	res->next_move[0] = left;
	res->next_move[1] = right;
	return res;
}
void freeTree(SingleSourceMovesTree* tr)
{
	// The func free the tree.
	freeTreeHelper(tr->source);
	free(tr);
}

void freeTreeHelper(SingleSourceMovesTreeNode* root)
{
	// This func free the tree nodes.
	if (root == NULL)
		return;
	else
	{
		freeTreeHelper(root->next_move[0]);
		freeTreeHelper(root->next_move[1]);
		free(root->pos);
		free(root);
	}
}