//
// Created by kuril on 26.5.22.
//

#include "helpers.h"
#include "CEntity.h"
#include "CPlayer.h"
using namespace std;

int random(int lbound, int ubound) {
    /* Random number generator by user "Cornstalks" from https://stackoverflow.com/a/13445752 */
    srand(time(0));
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> range(lbound, ubound);
    return range(rng);
}