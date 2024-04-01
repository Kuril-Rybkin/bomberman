//
// Created by kuril on 15.5.22.
//

#include "map.h"
#include <iostream>
using namespace std;

//Define used colour pairs here:
#define WALL 1
#define OBSTACLE 5
#define BOMB 6
#define EXPLOSION 7

void drawMap(vector<vector<CEntity *>> & matrix) {
    //call print on every entity in the matrix to display map on screen.
    for (auto i : matrix) {
        for (auto j : i) {
            if (j != nullptr) {
                j->print();
            }
        }
    }
}

void deallocMap(vector<vector<CEntity *>> & matrix, list<CExplosion *> exps, list<CBot *> & bots) {
    //deallocate everything.
    for (auto i : bots) {
        delete i;
    }

    for (auto i : exps) {
        //in case explosion is in both exps and matrix, set pointer in matrix to null to avoid double free
        matrix[i->getCoords().first][i->getCoords().second] = nullptr;
        delete i;
    }

    for (auto i : matrix) {
        //delete all remaining entities on the map such as obstacles and bombs.
        for (auto j : i) {
            delete j;
        }
    }
}

bool loadMap(string fname, vector<vector<CEntity *>> & matrix, vector<pair<string, int>> & highScores) {
    ifstream rfile;
    rfile.open(fname);

    if (!rfile.good()) { //check if file opened correctly
        return false;
    }

    size_t rowcount = 0; //variable for holding the current row of insertion of entity
    size_t colcount = 0; //variable for holding the current column of insertion of entity

    string line; //lines will be read into this variable

    getline(rfile, line); //read first control line
    if (line != ";") { //check if beginning of file is correct
        return false;
    }

    bool pushed = false; //used to determine if first row of matrix was pushed back
    bool readMap = false; //used to determine if map was read at all
    bool freeSpace = false; //used to determine if map has free space to spawn on

    while(getline(rfile, line) && line != ";" && line.length() > 0) { //read rows of the map until you reach ";" which is the end of the map
        readMap = true; //some map was read from file

        for (auto letter : line) {
            if (!pushed) { // for the first row, a new vector must be pushed back always
                matrix.push_back(vector<CEntity *>());
            }
            if (colcount >= matrix.size()) { //check that map is n*n (requirement)
                return false;
            }

            //check if empty space is not on map border. This is to ensure bordered maps to stop player from moving
            //outside map dimensions and causing a segmentation fault.
            if (rowcount != 0 && colcount != 0 && colcount < line.length() && letter == '0') {
                freeSpace = true;
                matrix[colcount].push_back(nullptr);
            }
            else if (letter == '1') {
                matrix[colcount].push_back(new CObject(colcount, rowcount, 'a', WALL, false, 1));
            }
            //again, check that destroyable objects are not on map border to stop player from getting out of bounds
            else if (rowcount != 0 && colcount != 0 && colcount < line.length() && letter == '2') {
                matrix[colcount].push_back(new CObject(colcount, rowcount, '#', OBSTACLE, true, 1));
            }
            else {
                return false;
            }

            colcount++;
        }

        pushed = true; //first row is pushed, no longer needed to push back new vectors
        rowcount++;
        colcount = 0;
    }

    if (line != ";" || !readMap || !freeSpace) { //check that some map was read and for presence of control character and place to spawn
        return false;
    }

    line = ""; //clear line for next validity check

    //read highscores until next control character. these can be empty.
    while (getline(rfile, line) && line != ";") {
        int pos = line.find(' ');
        try {
            highScores.push_back(make_pair(line.substr(0, pos), stoi(line.substr(pos + 1, line.length()))));
        }
        //stoi will catch an invalid integer score. this can be either invalid_argument or out_of_range
        catch (...) {
            return false;
        }
    }

    if (line != ";") { //missing last control character
        return false;
    }

    rfile.close();
    return true;
}

bool writeHighScore(string filename, vector<pair<string, int>> & highScores) {
    fstream rfile;

    rfile.open(filename);

    string totalFile; //totalFile saves the file to a string to overwrite it
    string line;

    //check for control character at the beginning
    if (!getline(rfile, line) || line != ";") {
        return false;
    }
    totalFile += line + '\n'; //adds successfully read line to copy of file

    //skip through the map section of the file
    while (getline(rfile, line) && line != ";") {
        totalFile += line + '\n'; //adds successfully read line of map to copy of file
    }

    //check for second control character
    if (line != ";") {
        return false;
    }
    totalFile += line + '\n'; //adds successfully read line to copy of file
    rfile.close(); //close old file since its not needed anymore

    ofstream fileCopy(filename); //create new file to overwrite old file
    fileCopy << totalFile; //write old file into new file

    //add new highscores
    for (size_t i = 0; i < highScores.size(); i++) {
        fileCopy << highScores[i].first << " " << highScores[i].second << endl;
    }
    //write final control character to file.
    fileCopy << ";" << endl;
    fileCopy.close();

    highScores.clear();
    return true;
}