#include <iostream>
#include <vector>

class gameManager
{
public:
    gameManager();
    void rollDice(int playerIndex);
    void DetermineWinner();
    void setPlayerName();  
  private:
    std::string WinnerString(std::string playerName, int playerScore);
};

class diceManager {
public:
    diceManager();


    struct DiceGroup
    {
        int faceValue;
        int count;
    };

    enum PlayerHandType
    {
      Five_of_a_kind = 10,
      Four_of_a_kind = 9,
      Full_house= 8,
      Three_of_a_kind = 7,
      Two_pair = 6,
      One_pair = 5,
      Bad_hand = 0
    };

    typedef std::vector < DiceGroup > DiceGroupVector;
    DiceGroupVector rollTheDice();
    DiceGroupVector ReRollRange(DiceGroupVector toReRoll, int numReRolls);

    //Left hand will be matches, right will be Highest value (in event of tie)
    PlayerHandType GetDieScore(DiceGroupVector);
};