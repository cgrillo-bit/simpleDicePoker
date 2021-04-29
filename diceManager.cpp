
#include <iostream>
#include <stdio.h>
#include <random>
#include <vector> 
#include <algorithm>
#include "game.h" 

// forward function declarations
using namespace std;

diceManager::diceManager() {}

typedef vector<int> IntVector;

// Each hand has five dice in it
const int DICE_IN_HAND = 5;

// Each dice has a five line printout for its face
const int DICE_STRING_LINES = 5;
string DICE_STRINGS[6][DICE_STRING_LINES] = {
    {"-----------", "|         |", "|    O    |", "|         |", "-----------"},
    {"-----------", "| O       |", "|         |", "|       O |", "-----------"},
    {"-----------", "| O       |", "|    O    |", "|       O |", "-----------"},
    {"-----------", "| O     O |", "|         |", "| O     O |", "-----------"},
    {"-----------", "| O     O |", "|    O    |", "| O     O |", "-----------"},
    {"-----------", "| O  O  O |", "|         |", "| O  O  O |", "-----------"} };


void printDiceHand(diceManager::DiceGroupVector diceGroupList);

bool diceGroupCompare(diceManager::DiceGroup lhs, diceManager::DiceGroup rhs) //make private 
{

    bool isGreater = false;

    if (rhs.count < lhs.count)
    {
        isGreater = true;
    }
    else if (rhs.count == lhs.count)
    {
        if (rhs.faceValue < lhs.faceValue)
        {
            isGreater = true;
        }
    }
    return isGreater;
}
// print five dice -- a hand
diceManager::DiceGroupVector diceManager::rollTheDice()
{
    // seed our random sequence so we have a good randomness
    random_device diceRandom; 
    mt19937 gen(diceRandom()); 
    uniform_int_distribution<> dis(1,5); 
    // "roll" five dice, store them grouped by face value
    diceManager::DiceGroupVector diceGroupList;
    int value;

    int foundIndex;
    diceManager::DiceGroup groupX;
    for (int i = 0; i < DICE_IN_HAND; i++)
    {
        value = dis(gen); // CG production random generator

        // This isn't efficient, but it is simple, and it demonstrates
        // searching an array of records, so we use it for now.
        foundIndex = -1;
        for (int j = 0; j < diceGroupList.size(); j++)
        {
            if (diceGroupList.at(j).faceValue == value)
            {
                // The facevalue we need is already in the list.
                foundIndex = j;
                break;
            }
        }
        if (foundIndex == -1)
        {
         
            groupX.faceValue = value;
            groupX.count = 1;
            diceGroupList.push_back(groupX);
        }
        else
        {
            diceGroupList.at(foundIndex).count += 1;
        }
    }
    sort(diceGroupList.begin(), diceGroupList.end(), diceGroupCompare);
    // print after sorting
    printDiceHand(diceGroupList);

    return diceGroupList;
}


diceManager::DiceGroupVector diceManager::ReRollRange(diceManager::DiceGroupVector toReRoll, int numReRolls)
{
    //Random number chaos
    random_device diceRandom;
    mt19937 gen(diceRandom());
    uniform_int_distribution<> dis(1, 5);
    //End random number chaos
    int value;

    int haveReRolled = 0;
    int indexAt = toReRoll.size() - 1;
    //Keep Count of How Many Times We "Rolled"
    while (haveReRolled < numReRolls)
    {      
        int numDieAtCurrentIndex = toReRoll[indexAt].count; //Get the number of die we have from where we are currently indexing
        if(numDieAtCurrentIndex <= 0) 
        {
          //If we have no more die at this point in the index, we need to work our way left, subtract one from the index
          //Also ensure we do not go out of bounds by capping at zero
          indexAt = max(0, indexAt - 1);
        }

        value = dis(gen); //Generate Random number for the die
        int foundIndex = -1; //For searching...
        for (int j = 0; j < toReRoll.size(); j++)
        {          
            // The facevalue we need is already in the list, and we check the count to ensure it is "really" there           
            if (toReRoll.at(j).faceValue == value && toReRoll.at(j).count > 0)
            {
                foundIndex = j;
                break;
            }
        }

        //We do not have the die, add a new one, and remove from the current index
        if (foundIndex == -1)
        {
            toReRoll[indexAt].count = max(0, toReRoll[indexAt].count - 1); //Remove the one we found by index
            diceManager::DiceGroup newDie; //Generate new die
            newDie.faceValue = value;
            newDie.count = 1;
            toReRoll.push_back(newDie); //Add it to the vector
        }
        else //The Die Is Currently In Dice Vector, Just Add to the Count of that one
        {
            //Remove The Die At Our Current Index
            toReRoll[indexAt].count = max(0, toReRoll[indexAt].count - 1);
            //Add the new one
            toReRoll.at(foundIndex).count += 1;
        }

        haveReRolled++; //Done Rolling This Hand
    }    

    sort(toReRoll.begin(), toReRoll.end(), diceGroupCompare);
    printDiceHand(toReRoll);
    return toReRoll;
}


void printDiceHand(diceManager::DiceGroupVector diceGroupList)
{
    diceManager::DiceGroup groupX;
    // for each of the rows in a die's face
    for (int printLine = 0; printLine < DICE_STRING_LINES; printLine++)
    {
        // output that row for each face value group in our vector
        // for as many dice are in the count for that group
        for (int groupIndex = 0; groupIndex < diceGroupList.size(); groupIndex++)
        {
            groupX = diceGroupList.at(groupIndex);
          
            for (int dieIndex = 0; dieIndex < groupX.count; dieIndex++)
            {
                cout << DICE_STRINGS[groupX.faceValue][printLine];
                cout << " ";
            }
        }
        cout << endl;
    }
}

diceManager::PlayerHandType diceManager::GetDieScore(diceManager::DiceGroupVector diceVector)
{
  int numOfAKind = 0;
  int totalMatches = 0;
  int highestMatchValue = 0;

  for(int i = 0; i < diceVector.size(); i++)
  {
    DiceGroup dg = diceVector.at(i); // de-reference only to make next line simpler

    if (dg.count > 1) 
    {
        if(dg.count > highestMatchValue)
        {
          highestMatchValue = dg.count;
        }
        totalMatches += 1;
    }

  }

  if(highestMatchValue == 5)
  {
    return Five_of_a_kind;    
  }
  else if(highestMatchValue == 4)
  {
    return Four_of_a_kind;
  }
  else if(highestMatchValue == 3 && totalMatches >= 2)
  {
    return Full_house;
  }
  else if(highestMatchValue == 3)
  {
    return Three_of_a_kind;
  }
  else if(highestMatchValue == 2)
  {
    return Two_pair;
  }
  else if(highestMatchValue == 1)
  {
    return One_pair;
  }
  else
  {
    return Bad_hand;
  }
}

