//
// Created by kuril on 1.6.22.
//

#ifndef BOMBERMAN_GAME_H
#define BOMBERMAN_GAME_H

#include <ncurses.h>
#include <string>
#include <vector>
#include "CEntity.h"
#include "CPlayer.h"
#include "map.h"
#include "CStats.h"

void startGame(int mode, string mapfile, vector<char> & controls0, vector<char> & controls1, vector<string> & bonuses, string p1Name, string p2Name);

bool validateFile(string mapfile, vector<vector<CEntity *>> & map, vector<pair<string, int>> & highScore);

bool quitDialogue();

void findCoords(int & x, int & y, vector<vector<CEntity *>> & map);

void endDialogue(bool highScoreSet, vector<pair<string, int>> & highScore);

bool setHighScore(list<pair<CHuman *, CStats *>> & players, list<pair<CHuman *, CStats *>> & deadPlayers, vector<pair<string, int>> & highScore);

#endif //BOMBERMAN_GAME_H
