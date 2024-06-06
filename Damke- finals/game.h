#pragma once
#include "list.h"
void newMoveBoard(Board board, Board newBoard, Player player, checkersPos pStart, checkersPos pEnd);
bool checkIfInsideBoard(checkersPos* next);
Player playerPanel(Board board, char cols, char rows);
bool isSkipOk(Board board, char row, char col);
void printTable(Board board);
void copyBoard(Board board, Board res);
checkersPos createPos(char row, char col);
void turn(Board board, Player player);
void updateBoard(Board board, SingleSourceMovesList* lst, Player player);
void PlayGame(Board board, Player starting_player);
Player checkIfEndOfGame(Board board);
bool countPlayers(Board board, Player player);
