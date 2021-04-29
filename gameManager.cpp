#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <limits>
#include "game.h"


struct player {
	diceManager::DiceGroupVector playerHand;
	int rollCount = 0;
  std::string playerName; 
};

player players[2]; 
int maxRolls = 3;
const int DICE_IN_HAND = 5; 

gameManager::gameManager()
{
}

void gameManager::setPlayerName()
{
  for(int i=0; i<2; i++)
  {
    std::string playerName; 
    std::cout << "Please enter a player name: \n"; 
    std::cin >> playerName; 
    //Clears The Error State of the Buffer
    std::cin.clear();        
    //Ignores the Rest of the Line After the Instance of the Error
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    players[i].playerName = playerName;
  }  
}

bool ValidateInt(std::istream& stream, int &output)
{
	if(!(stream >> output)) 
	{		
		//Clears The Error State of the Buffer
		stream.clear();		
		//Ignores the Rest of the Line After the Instance of the Error
		stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Non-Integer Detected, Please Enter a Integer.\n";
		return false;
	}else
	{
		//Set Number to be Postive if Negative Entered... Smarty Pants is the poltically correct term for this situation
		if(output < 0){
			output = abs(output);
		}
		return true;
	}
}


void gameManager::rollDice(int playerIndex){
    diceManager diceRoller;
    if (playerIndex < 2) 
    {
      //Determine if player wants to roll again, if not, move to next
      bool rollingByLimpBizkit = true;


        std::cout << players[playerIndex].playerName << " Hand:" << std::endl;
        players[playerIndex].playerHand = diceRoller.rollTheDice();
        players[playerIndex].rollCount++;

        while (rollingByLimpBizkit) //keep on rollin' rollin' rollin'
        {          
     
          if (players[playerIndex].rollCount >= maxRolls)
          {
              std::cout << "\nOut of Rolls - Moving to Next Player\n";
              break;
          }
          int toReRoll  = 0;		
          do
          {
            std::cout << "Please Enter a Number of Dice to Re-Roll (from right to left) : ";
          }while(!ValidateInt(std::cin, toReRoll));

          if(toReRoll == 0)
          {
            rollingByLimpBizkit = false;
          }else
          {            
            diceRoller.ReRollRange(players[playerIndex].playerHand, toReRoll);
            players[playerIndex].rollCount++;
          }
        }

        std::cout << "\n========================================\n"; //To make it pretty break up section
        
        playerIndex++;
        rollDice(playerIndex);
    }
}

//If the user wants to re-roll n amount of dice
//int toReroll = 0;
//std::cin >> toReRoll; (Probably have to do some type of polish street magic type conversion) 
//Write new function in diceManager that takes type Dice Vector, and an integer
//In said function, index from the RIGHT to LEFT (i.e: for(int i = diceVector.Length() -1; i > toReroll; i--))
//Now we have to write a new dice roller function, instead of returning a vector of dice group, it returns a single dice group
//We will need to assign those dice group values to dice group vector (i.e: diceVector[i] = generatedDie)
//Pray to god this works

void gameManager::DetermineWinner()
{
	diceManager diceRoller;

	/// <Explanation>
	/*
	* Use a pair for the score, first number will be the number of matches (the main factor for the score)
	* the second number is the die value of the matches, in the event of a draw, this determines the winner
	* if both numbers match for both pairs, then its a draw. 
	* </Explanation>
	*/
	int playerOneScore = diceRoller.GetDieScore(players[0].playerHand);
	int playerTwoScore = diceRoller.GetDieScore(players[1].playerHand);

	if (playerOneScore > playerTwoScore)
	{
		//Player One Wins
    std::cout << WinnerString(players[0].playerName, playerOneScore);
	}
	else if(playerTwoScore > playerOneScore)
	{
		//Player Two Wins
    std::cout << WinnerString(players[1].playerName, playerTwoScore);
	}  
	else
	{
		std::cout << "Its a Draw.";
  }
}

std::string gameManager::WinnerString(std::string playerName, int playerScore)
{
    switch(playerScore)
    {
      case 10:
    		return playerName + " Wins. Five of A Kind";
        break;
      case 9:
    		return playerName + " Wins with Four of A Kind";
        break;
      case 8:
        return playerName + " Wins with Full House"; 
        break; 
      case 7:
        return  playerName +  " Wins with Three of a Kind";
        break; 
      case 6:
        return  playerName + " Wins with Two Pair";         
        break; 
      case 5: 
        return  playerName + " Wins with One Pair"; 
      default:
        return  "Bad hand. ";
    }
}

