// consoleDIceGame.cpp : This file contains the 'main' function. Program
// execution begins and ends there.
//
#include "game.h"
#include <iostream>
#include <time.h>

int main() {
	gameManager theGame;
	theGame.setPlayerName();
	theGame.rollDice(0);
	theGame.DetermineWinner();
	return 0;
}

