//
// Created by kuril on 15.5.22.
//

#ifndef BOMBERMAN_MAP_H
#define BOMBERMAN_MAP_H
#include <vector>
#include "CEntity.h"
#include "CPlayer.h"
#include <string>
#include <fstream>

void drawMap(vector<vector<CEntity *>> & matrix);

void deallocMap(vector<vector<CEntity *>> & matrix, list<CExplosion *> exps, list<CBot *> & bots);

bool loadMap(string fname, vector<vector<CEntity *>> & matrix, vector<pair<string, int>> & highScores);

bool writeHighScore(string filename, vector<pair<string, int>> & highScores);


#endif //BOMBERMAN_MAP_H
