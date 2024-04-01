//
// Created by kuril on 8.5.22.
//

#include "CEntity.h"
#include "CPlayer.h"

CObject::CObject(int x, int y, char icn, int clr, bool destroyable, int lvs) : CEntity(x, y, icn, clr, true) {
    this->destroyable = destroyable;
    lives = lvs;
}

void CObject::destroy(vector<vector<CEntity *>> & map, CPlayer * src) {
    //called by CBomb when it tries to generate an explosion on the place of this obstacle
    if (destroyable) { //obstacle checks if it can be destroyed
        if (src) {
            src->addScore(100); //adds score to player for successfully destroying obstacle
        }
        lives -= 1; //in case there is a need for obstacles that take several explosions to explode
        if (lives <= 0) { //if object was destroyed
            //spawn bonus on its location
            map[coords.first][coords.second] = new CBonus(coords.first, coords.second, 'a');
            map[coords.first][coords.second]->print();
            delete this;
        }
    }
}

bool CObject::isDestroyable() {
    return destroyable;
}