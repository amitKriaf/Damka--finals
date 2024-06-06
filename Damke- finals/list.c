#include "list.h"
#include "tree.h"
#include "game.h"


SingleSourceMovesList* FindSingleSourceOptimalMove(SingleSourceMovesTree* moves_tree)
{
	// This func retur the list of the longest path.
	int size = 0;
	Player player = playerPanel(moves_tree->source->board, moves_tree->source->pos->row, moves_tree->source->pos->col);
	SingleSourceMovesList* res = (SingleSourceMovesList*)malloc(sizeof(SingleSourceMovesList));
	if (res == NULL)
	{
		printf("Memory allocation falied!!\n");
		exit(1);
	}
	*res = FindSingleSourceOptimalMoveHelper(moves_tree->source, &size, player);
	return res;
}

SingleSourceMovesList FindSingleSourceOptimalMoveHelper(SingleSourceMovesTreeNode* moves_root, int* size, Player player)
{
	// This func create a list of the longest path.
	int Sleft, Sright;
	SingleSourceMovesList res, left, right;
	if (moves_root == NULL)
	{
		*size = -1;
		makeEmptyList(&res);
		return res;
	}
	else
	{
		left = FindSingleSourceOptimalMoveHelper(moves_root->next_move[0], &Sleft, player);
		insertDataToStartList(&left, moves_root->pos->row, moves_root->pos->col, moves_root->total_capture_so_far);
		right = FindSingleSourceOptimalMoveHelper(moves_root->next_move[1], &Sright, player);
		insertDataToStartList(&right, moves_root->pos->row, moves_root->pos->col, moves_root->total_capture_so_far);
		if (Sleft == Sright)
		{
			*size = Sleft + 1;
			if (player == PLAYER_B)
			{
				freeList(&right);
				res = left;
			}
			else
			{
				freeList(&left);
				res = right;
			}
		}
		else if (Sleft > Sright)
		{
			*size = 1 + Sleft;
			freeList(&right);
			res = left;
		}
		else
		{
			*size = 1 + Sright;
			freeList(&left);
			res = right;
		}
		return res;
	}

}

void makeEmptyList(SingleSourceMovesList* lst)
{
	// The func make an empty list.
	lst->head = lst->tail = NULL;
}

bool isEmptyList(SingleSourceMovesList* lst)
{
	// The fun cheak if the lst is empty
	return (lst->head == NULL);
}

SingleSourceMovesListCell* createNewListNode(char row, char col, unsigned short cap, SingleSourceMovesListCell* next)
{
	// This func creates new list node.
	SingleSourceMovesListCell* res;
	checkersPos* pos = (checkersPos*)malloc(sizeof(checkersPos));
	res = (SingleSourceMovesListCell*)malloc(sizeof(SingleSourceMovesListCell));
	if (res == NULL || pos == NULL)
	{
		printf("Memory allocation falied!!\n");
		exit(1);
	}
	pos->row = row;
	pos->col = col;
	res->position = pos;
	res->captures = cap;
	res->next = next;
	return res;
}

void freeList(SingleSourceMovesList* lst)
{
	// The fucntion get a list and free all the data and the list nodes in the list.
	SingleSourceMovesListCell* curr = lst->head;
	SingleSourceMovesListCell* next;
	while (curr != NULL)
	{
		next = curr->next;
		free(curr->position);
		free(curr);
		curr = next;
	}
	lst->head = lst->tail = NULL;
}

void insertDataToStartList(SingleSourceMovesList* lst, char row, char col, unsigned short capture)
{
	// The function get a list and a char.
	// It's creade a list node and put the new list nide to the end of the list.
	SingleSourceMovesListCell* newHead;
	newHead = createNewListNode(row, col, capture, NULL);
	insertNodeToStartList(lst, newHead);
}

void insertNodeToStartList(SingleSourceMovesList* lst, SingleSourceMovesListCell* head)
{
	// The fucntion get a list and a list node and put the list node to the end of the list.
	if (isEmptyList(lst))
	{
		head->next = NULL;
		lst->head = lst->tail = head;
	}
	else {
		head->next = lst->head;
		lst->head = head;
	}
}

MultipleSourceMovesList* FindAllPossiblePlayerMoves(Board board, Player player) {
	//this func return a list of moves that  each tool of player can do.
	MultipleSourceMovesList* res;
	SingleSourceMovesTree* tempTree;
	checkersPos pos;
	SingleSourceMovesList* tempLst;
	int i, j, count = 0;
	res = (MultipleSourceMovesList*)malloc(sizeof(MultipleSourceMovesList));
	makeEmptyListMultiple(res);
	if (res == NULL)
	{
		printf("Memory allocation falied!!\n");
		exit(1);
	}
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j] == player) {
				count++;
				pos = createPos(i + 'A', j + 1 + '0');
				tempTree = FindSingleSourceMoves(board, &pos);
				if (tempTree->source->next_move[0] != NULL || tempTree->source->next_move[1] != NULL)
				{
					tempLst = FindSingleSourceOptimalMove(tempTree);
					insertDataToStartMultipleList(res, tempLst);
				}
				freeTree(tempTree);
			}
			if (count >= MAX_TOOLS)
				break;
		}
	}

	return res;

}

void makeEmptyListMultiple(MultipleSourceMovesList* lst)
{
	// The func make an empty list.
	lst->head = lst->tail = NULL;
}

bool isEmptyListMultiple(MultipleSourceMovesList* lst)
{
	// The fun cheak if the lst is empty
	return (lst->head == NULL);
}

void freeMultopleList(MultipleSourceMovesList* lst)
{
	// The fucntion get a list and free all the data and the list nodes in the list.
	MultipleSourceMovesListCell* curr = lst->head;
	MultipleSourceMovesListCell* next;
	while (curr != NULL)
	{
		next = curr->next;
		freeList(curr->single_source_moves_list);
		free(curr);
		curr = next;
	}
	lst->head = lst->tail = NULL;
}

void insertDataToStartMultipleList(MultipleSourceMovesList* lst, SingleSourceMovesList* single_list)
{
	// The function get a list and a char.
	// It's creade a list node and put the new list nide to the end of the list.
	MultipleSourceMovesListCell* newHead;
	newHead = createNewListMultipleNode(single_list, NULL);
	insertNodeToStartMultipleList(lst, newHead);
}

void insertNodeToStartMultipleList(MultipleSourceMovesList* lst, MultipleSourceMovesListCell* head)
{
	// The fucntion get a list and a list node and put the list node to the end of the list.
	if (isEmptyListMultiple(lst))
	{
		head->next = NULL;
		lst->head = lst->tail = head;
	}
	else {
		head->next = lst->head;
		lst->head = head;
	}
}

MultipleSourceMovesListCell* createNewListMultipleNode(SingleSourceMovesList* single_list, MultipleSourceMovesListCell* next)
{
	// This func creates new list node.
	MultipleSourceMovesListCell* res;
	res = (MultipleSourceMovesListCell*)malloc(sizeof(MultipleSourceMovesListCell));
	if (res == NULL)
	{
		printf("Memory allocation falied!!\n");
		exit(1);
	}
	res->next = next;
	res->single_source_moves_list = single_list;
	return res;
}

SingleSourceMovesList* maxTurnList(MultipleSourceMovesList* lst, Player player)
{
	// This func returns the max capute list.
	MultipleSourceMovesListCell* curr = lst->head;
	unsigned short maxCap = 0;
	SingleSourceMovesList* max_single_list = curr->single_source_moves_list;
	checkersPos tempPos = *(curr->single_source_moves_list->head->position);
	while (curr != NULL)
	{
		if (maxCap < curr->single_source_moves_list->tail->captures) // If the last move have more captures then the max.
		{
			tempPos = *(curr->single_source_moves_list->head->position);
			maxCap = curr->single_source_moves_list->tail->captures;
			max_single_list = curr->single_source_moves_list;
		}
		else if (maxCap == curr->single_source_moves_list->tail->captures) // If the max captures equal to the max.
		{
			if (player == PLAYER_B)
			{
				if (curr->single_source_moves_list->head->position->row == tempPos.row)
				{
					if (curr->single_source_moves_list->head->position->col < tempPos.col)
					{
						tempPos = *(curr->single_source_moves_list->head->position);
						max_single_list = curr->single_source_moves_list;
					}
				}
				else if (curr->single_source_moves_list->head->position->row < tempPos.row)
				{
					tempPos = *(curr->single_source_moves_list->head->position);
					max_single_list = curr->single_source_moves_list;
				}
			}
			else // If player T
			{
				if (curr->single_source_moves_list->head->position->row == tempPos.row)
				{
					if (curr->single_source_moves_list->head->position->col > tempPos.col)
					{
						tempPos = *(curr->single_source_moves_list->head->position);
						max_single_list = curr->single_source_moves_list;
					}
				}
				else if (curr->single_source_moves_list->head->position->row > tempPos.row)
				{
					tempPos = *(curr->single_source_moves_list->head->position);
					max_single_list = curr->single_source_moves_list;
				}
			}
		}
		curr = curr->next;
	}
	return max_single_list;
}

