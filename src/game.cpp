//
// Created by kuril on 1.6.22.
//
#include "game.h"

void findCoords(int & x, int & y, vector<vector<CEntity *>> & map) {
    //finds a set of random coords that are valid for movement.
    x = random(0, map.size() - 1);
    y = random(0, map[0].size() - 1);

    //coords are valid if they are a nullptr. else, keep generating new coords until you find one
    while (map[x][y] != nullptr) {
        x = random(0, map.size() - 1);
        y = random(0, map[0].size() - 1);
    }
}

bool validateFile(string mapfile, vector<vector<CEntity *>> & map, vector<pair<string, int>> & highScore) {
    //returns true or false if the mapfile is of valid format and provides a dialogue

    string text;

    //loadMap returns false if mapfile is invalid. open dialogue informing user of error
    if (!loadMap(mapfile, map, highScore)) {
        text = "FAILED TO LOAD MAP FROM FILE <" + mapfile + ">.";

        //mapfile name is too long, so it is shortened to <...>
        if ((int)text.size() > COLS - 1) {
            text = "FAILED TO LOAD MAP FROM FILE <...>.";
        }
        //display text and wait for user to press key and skip dialogue
        mvprintw(LINES / 2, (COLS / 2) - (text.size() / 2), text.c_str());
        refresh();
        getchar();
        clear();
        refresh();
        return false;
    }
    return true;
}

bool quitDialogue() {
    //dialogue with user to confirm if they want to quit the game and prompts them to press q again to confirm
    string text;
    clear();
    text = "ARE YOU SURE? PRESS Q AGAIN TO QUIT.";
    mvprintw(LINES / 2, (COLS / 2) - (text.size() / 2), text.c_str());
    refresh();

    char confirm = getchar();
    if (confirm != 'q' && confirm != 'Q') {
        return false;
    }
    else {
        return true;
    }
}

bool setHighScore(list<pair<CHuman *, CStats *>> & players, list<pair<CHuman *, CStats *>> & deadPlayers, vector<pair<string, int>> & highScore) {
    //determines if a new highscore was achieved and if the highscores should be displayed
    bool highScoreSet = false;

    //if there are no highscores, then the score must be a highscore
    if (highScore.empty()) {
        highScoreSet = true;
    }

    //compare scores of all alive players against highscores linearly
    for (auto i : players) {
        size_t pos = highScore.size();

        //if highscores is empty, simply push back the name-score pair
        if (pos == 0) {
            highScore.push_back(i.first->returnScore());
        }
        else {
            //linearly find correct position for score
            while (i.second->getScore() > highScore[pos - 1].second && pos > 0) {
                //gradually move position up with each comparison
                pos--;
            }
            if (pos < highScore.size()) {
                //position in leaderboard is high enough, a highscore was set.
                highScoreSet = true;
                highScore.insert(highScore.begin() + pos, i.first->returnScore());
            }
        }

        //memory deallocation as you go.
        delete i.first;
        delete i.second;
    }

    //compare scores of all dead players against highscores linearly
    for (auto i : deadPlayers) {
        size_t pos = highScore.size();

        //if highscores is empty, simply push back the name-score pair
        if (pos == 0) {
            highScore.push_back(i.first->returnScore());
        }
        else {
            //linearly find correct position for score
            while (i.second->getScore() > highScore[pos - 1].second && pos > 0) {
                pos--;
            }
            if (pos < highScore.size()) {
                //position in leaderboard is high enough, a highscore was set.
                highScoreSet = true;
                highScore.insert(highScore.begin() + pos, i.first->returnScore());
            }
        }

        //memory deallocation as you go.
        delete i.first;
        delete i.second;
    }

    //resize highscore since its always top 5.
    if (highScore.size() > 5) {
        highScore.resize(5);
    }

    //return boolean to determine whether to print scoreboard or not.
    return highScoreSet;
}

void endDialogue(bool highScoreSet, vector<pair<string, int>> & highScore) {
    //displays game over message, and situationally, scoreboard.
    string text;
    clear();

    text = "GAME OVER. PRESS ANY KEY TO CONTINUE.";
    mvprintw(LINES / 2 - 1, (COLS / 2) - (text.size() / 2), text.c_str());

    if (highScoreSet) {
        text = "NEW HIGH SCORE!";
        //print NEW HIGH SCORE and make it blink
        attron(A_BLINK);
        mvprintw(LINES / 2, (COLS / 2) - (text.size() / 2), text.c_str());
        attroff(A_BLINK);

        //print scoreboard
        for (size_t i = 0; i < highScore.size(); i++) {
            mvprintw((LINES / 2) + i + 2, (COLS / 2) - (text.size() / 2), "%d. %s: %d", i + 1, highScore[i].first.c_str(),
                     highScore[i].second);
        }
    }

    refresh();
    getchar();
}

void startGame(int mode, string mapfile, vector<char> & controls0, vector<char> & controls1, vector<string> & bonuses, string p1Name, string p2Name) {
    //main game loop.
    //map is a matrix of CEntity pointers size n*n
    vector<vector<CEntity *>> map;
    vector<pair<string, int>> highScore;

    //vector of bombs and explosions to help with memory deallocation
    list<CBomb *> bombs;
    list<CExplosion *> exps;

    //list of all bots to help with controlling them and memory deallocation
    list<CBot *> bots;

    //list of all players, separated by dead and alive
    list<pair<CHuman *, CStats *>> players;
    list<pair<CHuman *, CStats *>> deadPlayers;

    if (!validateFile(mapfile, map, highScore)) {
        deallocMap(map, exps, bots);
        return;
    }

    bool quit = false;
    drawMap(map); //draw map onto screen

    int x = 0, y = 0;
    //find valid coordinates for player 1 spawn
    findCoords(x, y, map);

    //create player 1 entity
    CHuman * p1 = new CHuman(x, y, p1Name, '*', 2, 3, 5, controls0, bonuses);
    //create corresponding stats window for player 1
    CStats * p1stat = new CStats(25, 7, 0, map[0].size(), 2, p1);
    //push the pair of pointers to alive players
    players.push_back(make_pair(p1, p1stat));

    //mode is 1 when it is Co-op and there is a second player
    if (mode == 1) {
        //find valid coordinates for player 2 spawn
        findCoords(x, y, map);
        //create player 2 entity
        CHuman * p2 = new CHuman(x, y, p2Name, '*', 3, 3, 5, controls1, bonuses);
        //create corresponding stats window for player 2
        CStats * p2stat = new CStats(25, 7, 0, (map[0].size()) + 7, 3, p2);
        //push this pointer pair back to alive players
        players.push_back(make_pair(p2, p2stat));
    }

    timeout(1); //so game doesnt wait for user input but continues flowing
    char input = getch();
    auto time = chrono::system_clock::now(); //used in calculating time delays when spawning new bots

    //main game loop
    while (!players.empty() && ! quit) { //play while there is at least one player alive

        //if there are less than 3 bots on the map, spawn more.
        if (bots.size() < 3) {

            //calculate delay so bots aren't spawned too fast:
            auto newtime = chrono::system_clock::now();
            auto timediff = chrono::duration_cast<chrono::seconds>(newtime - time);

            if (timediff >= chrono::seconds(2)) {

                //find valid spawn coordinates for bot
                int xSpawn = 0, ySpawn = 0;
                findCoords(xSpawn, ySpawn, map);

                time = newtime; //reset timer
                //create new bot
                auto temp = new CBot(xSpawn, ySpawn);
                temp->print();
                bots.push_back(temp);
            }
        }

        //update and play every bot in array
        for (auto i = bots.begin(); i != bots.end();) {
            if (!(*i)->update(map)) { //update returns false if bot dies
                refresh();
                delete *i; //deallocate memory
                i = bots.erase(i); //remove from array
            } else {
                //else robot is alive, so it should make a decision
                (*i)->play(map, bombs);
                i++;
            }
        }

        //countdown and explode every bomb on the map
        for (auto i = bombs.begin(); i != bombs.end();) {

            //countdown returns false if bombs timer is finished
            if (!(*i)->countdown(map, exps)) {

                //there can be an explosion on the place of the bomb so check for pointers on coordinates to prevent
                //losing track of CExplosion pointer and causing memory leaks
                if (dynamic_cast<CBomb *>(map[(*i)->getCoords().first][(*i)->getCoords().second])) {
                    map[(*i)->getCoords().first][(*i)->getCoords().second] = nullptr;
                }

                //deallocate memory
                delete (*i);
                i = bombs.erase(i);
            } else {
                ++i;
            }
        }

        //update every explosion
        for (auto i = exps.begin(); i != exps.end();) {
            if (!(*i)->countdown()) { //explosion has lasted long enough and should be deleted
                map[(*i)->getCoords().first][(*i)->getCoords().second] = nullptr;
                delete (*i);
                i = exps.erase(i);
            } else {
                ++i;
            }
        }

        //update and play every alive player
        for (auto i = players.begin(); i != players.end(); i++) {

            //update returns false if the player lost all lives and cannot continue to play
            if (!(*i).first->update(map)) {

                //push the player to deadPlayers array instead of deleting so their highscore can still be saved to file
                deadPlayers.push_back(*i);
                //update scoreboard to reflect death
                (*i).second->update();
                refresh();
                //erase from players position
                i = players.erase(i);
                i--;
            }
            else {

                (*i).first->print(); //print character to screen
                //play returns false if the user presses q to quit
                if (!(*i).first->play(map, bombs, input)) {
                    //open quit dialogue to confirm
                    if (!quitDialogue()) {
                        //if user changes their mind, re-draw map
                        clear();
                        drawMap(map);

                        //re-draw alive players and their stats
                        for (auto plyr : players) {
                            plyr.first->print();
                            plyr.second->print();
                        }

                        //re-draw dead player's stats only, because they are dead.
                        for (auto plyr : deadPlayers) {
                            plyr.second->print();
                        }

                        refresh();
                    }
                    else {
                        quit = true;
                        break;
                    }
                }
                (*i).second->update(); //update character score
            }
        }
        timeout(1);
        input = getch();
    }

    //clear all entities and bots
    deallocMap(map, exps, bots);

    //find out if a highscore was set or not
    bool highScoreSet = setHighScore(players, deadPlayers, highScore);
    //display game over and potential new highscore
    endDialogue(highScoreSet, highScore);

    //try to save highscores, inform user of error if any
    if (!writeHighScore(mapfile, highScore)) {
        clear();
        string text = "FAILED TO SAVE HIGHSCORES. PRESS ANY KEY TO CONTINUE.";
        mvprintw(LINES / 2, (COLS / 2) - (text.length() / 2), text.c_str());
        refresh();
        getchar();
    }

    clear();
    refresh();
}