//
// Created by kuril on 28.5.22.
//
#include "CPlayer.h"

CHuman::CHuman(int x, int y, string name, char icon, int colour, int lives, int bombs, vector<char> & controls, vector<string> & bonuses) : CPlayer(x, y, name, icon, colour, lives) {
    this->bombs = bombs; //counter how many bombs are left for the player
    this->controls = controls; //array of characters which are used to control the character
    this->bonuses = bonuses; //array of bonus strings
    bombRegen = chrono::system_clock::now(); //used in timing bomb regeneration
}

bool CHuman::play(vector<vector<CEntity *>> & map, list<CBomb *> & bmbVec, char input) {
    if (input == controls[0]) { //first element in controls vector is to move up
        move(0, map); //move upwards in map
    }
    else if (input == controls[1]) { //second element in controls vector is to move right
        move(1, map); //move right in map
    }
    else if (input == controls[2]) { //third element in controls vector is to move down
        move(2, map); //move down in map;
    }
    else if (input == controls[3]) { //fourth element in controls vector is to move left
        move(3, map); // move left in map
    }
    else if (input == controls[4]) { //fifth element in controls vector is to plant a bomb
        if (bombs > 0) { //if player still has bombs left
            if (map[coords.first][coords.second] == nullptr) { //if player is standing on an empty slot
                //generate new CBomb entity
                //this pointer shall be passed in so that all explosions caused by this bomb will add score to the player
                CBomb * playerBomb = new CBomb(coords.first, coords.second, bombCountdown, bombRange, this);

                map[coords.first][coords.second] = playerBomb; //place bomb on coordinates
                bmbVec.push_back(playerBomb);
                playerBomb->print();

                //reset bonuses and their corresponding values:
                if (bonus == "Faster countdown" || "Larger range") {
                    bonus = "";
                    bombCountdown = 5;
                    bombRange = 3;
                }

                bombs -= 1;
            }
        }
    }
    else if (input == 'q' || input == 'Q') { //q is universal -- end game
        return false; //return false to end game while loop
    }
    return true; //return true to continue game while loop
}

bool CHuman::update(vector<vector<CEntity *>> &map) {
    //get current time to calculate difference between regeneration times:
    auto newRegen = chrono::system_clock::now();

    //if player has less than 3 bombs, then regenerate a new bomb every 3 seconds
    if (bombs < 3 && chrono::duration_cast<chrono::seconds>(newRegen - bombRegen) >= chrono::seconds(3)) {
        bombRegen = newRegen; //reset bomb regeneration timer
        bombs += 1;
    }

    //if player is standing on an explosion
    if (dynamic_cast<CExplosion *>(map[coords.first][coords.second]) != nullptr) {
        //subtract 100 score from the source of the explosion in case it was friendly fire
        ((CExplosion *)map[coords.first][coords.second])->addScore(-100);

        //loseHealth() will respawn the player if he has enough lives or return false if no lives left
        if (!loseHealth(map)) {
            return false;
        }
    }

    //these bonuses do not go away by themselves so we introduce a check to expire them on their own
    if (bonus == "+ 3 bombs" || bonus == "+ 1 life") {
        auto newtime = chrono::system_clock::now();
        auto timediff = chrono::duration_cast<chrono::seconds>(newtime - time);
        if (timediff >= chrono::seconds (3)) {
            refresh();
            time = newtime;
            bonus = "";
        }
    }

    //if player is standing on a bonus and at least one bonus is activated:
    if (dynamic_cast<CBonus *>(map[coords.first][coords.second]) != nullptr && bonuses.size() > 0) {
        //generate random index of bonus from bonuses vector:
        bonus = bonuses[random(0, bonuses.size() - 1)];
        time = chrono::system_clock::now(); //time is used in expiring bonuses that dont expire on their own

        //check what bonus it is here through string comparison:
        if (bonus == "+ 3 bombs") {
            bombs += 3;
        }
        else if (bonus == "+ 1 life") {
            lives += 1;
        }
        else if (bonus == "Faster countdown") {
            bombCountdown = 3;
        }
        else if (bonus == "Larger range") {
            bombRange = 5;
        }
        delete map[coords.first][coords.second];
        //prevent heap-use-after-free
        map[coords.first][coords.second] = nullptr;
    }
    return true;
}

void CHuman::getStats(int & lives, int & bombs, string & name, string & bonus, int & score) {
    //changes references to return all the vital game statistics of the player. used by CStats
    lives = this->lives;
    bombs = this->bombs;
    name = this->name;
    bonus = this->bonus;
    score = this->score;
}

void CHuman::addScore(int amount) {
    //called by CExplosion. Adds score to the player
    score += amount;
}

pair<string, int> CHuman::returnScore() {
    return make_pair(name, score);
}