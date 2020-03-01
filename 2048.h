#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <iterator>
#include <cmath>
#include <chrono>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

//Convenience's sake global variables
int columns, rows; //Board dimensions
int rotation = 0; //Used to set rotation of spawning tiles
string moves; //Placeholder for moves

bool noSolution = false; //Check is no solution (DFS)
bool goalFound = false; //Check if goal state found (DFS)
vector<vector<int>> goalState; //Returning goal state (DFS)
int limit = 1; //Depth limit for DFS

//Specifies heap sorting using maximum tile value
struct compareMax
{
    bool operator()(const tuple<vector<vector<int>>, string, int> &s1, const tuple<vector<vector<int>>, string, int> &s2)
    {
        return get<2>(s1) < get<2>(s2);
    }
};

//Prints game board
void printBoard(vector<vector<int>> b)
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
            cout << b[r][c] << " ";
        cout << endl;
    }
}

//Checks game board for goal state
bool isGoal(vector<vector<int>> b, int g)
{
    for(int x = 0; x < rows; x++)
    {
        for(int y = 0; y < columns; y++)
        {
            if(b[x][y] == g)
                return true;
        }
    }
    return false;
}

//Spawns tiles on a set rotation
vector<vector<int>> spawnTile(vector<vector<int>> b, vector<int> t)
{
    vector<vector<int>> tempState = b;

    //Checks each corner clockwise starting from top left
    if(tempState[0][0] == 0)
        tempState[0][0] = t[rotation];
    else if(tempState[0][columns - 1] == 0)
        tempState[0][columns - 1] = t[rotation];
    else if(tempState[rows - 1][columns - 1] == 0)
        tempState[rows - 1][columns - 1] = t[rotation];
    else if(tempState[rows - 1][0] == 0)
        tempState[rows - 1][0] = t[rotation];

    return tempState;
}

vector<vector<int>> moveState(vector<vector<int>> b, vector<int> t, int d)
{
    //Temporary placeholder variables
    vector<vector<int>> tempState = b;
    
    //Switch case for movement direction
    switch(d)
    {
        //UP move
        case 0:
            //===Tile Checking===
            //FUNCTIONALITY:
            //Check the second closest tile to the edge given the direction.
            //Check all tiles "below" that tile; if a tile below the checking tile is a match
            //or not 0, stopping point for all subsequent tiles will be there.
            //Matching tile will be double the old value; old tile position will be reset to 0.
            //Proceed to check next "highest" tile (based on direction given).
            for(int c = 0; c < columns; c++)
            {
                int stopPoint = -9999; //Point at which we no longer check for tiles
                for(int r = 1; r < rows; r++)
                {
                    for(int x = 1; x <= r; x++)
                    {
                        if(r - x < stopPoint)
                            break;

                        if(tempState[r - x][c] != 0)
                            stopPoint = r - x;

                        if(tempState[r][c] == tempState[r - x][c] && tempState[r][c] != 0)
                        {
                            tempState[r][c] = 0;
                            tempState[r - x][c] *= 2;
                            r++;
                            stopPoint = r;
                        }
                    }
                }
            }

            //===Dropping Tiles In Direction===
            //FUNCTIONALITY:
            //Updated board state given from above, however, tiles are "floating" in place,
            //not yet dropped in direction.
            //This checks for any 0's "under" a tile and drops it accordingly to the first 
            //nonzero found.
            for(int c = 0; c < columns; c++)
            {
                for(int r = 1; r < rows; r++)
                {
                    int dropPoint = -9999;
                    if(tempState[r][c] != 0)
                    {
                        for(int x = r - 1; x >= 0; x--)
                        {
                            if(tempState[x][c] == 0)
                                dropPoint = x;
                            else
                                break;
                        }
                        if(dropPoint != -9999)
                        {
                            tempState[dropPoint][c] = tempState[r][c];
                            tempState[r][c] = 0;
                        }
                    }
                }
            }
            break;

        //DOWN move
        case 1:
            for(int c = 0; c < columns; c++)
            {
                int stopPoint = rows - 1;
                for(int r = rows - 2; r >= 0; r--)
                {
                    for(int x = stopPoint; x > r; x--)
                    {
                        if(tempState[x][c] != 0)
                            stopPoint = x;

                        if(tempState[r][c] == tempState[x][c] && tempState[r][c] != 0)
                        {
                            tempState[r][c] = 0;
                            tempState[x][c] *= 2;
                            r--;
                            stopPoint = r;
                            break;
                        }
                    }
                }
            }

            for(int c = 0; c < columns; c++)
            {
                for(int r = rows - 2; r >= 0; r--)
                {
                    int dropPoint = -9999;
                    if(tempState[r][c] != 0)
                    {
                        for(int x = r + 1; x < rows; x++)
                        {
                            if(tempState[x][c] == 0)
                                dropPoint = x;
                            else
                                break;
                        }
                        if(dropPoint != -9999)
                        {
                            tempState[dropPoint][c] = tempState[r][c];
                            tempState[r][c] = 0;
                        }
                    }
                }
            }
            break;

        //LEFT move
        case 2:
            for(int r = 0; r < rows; r++)
            {
                int stopPoint = -9999;
                for(int c = 1; c < columns; c++)
                {
                    for(int x = 1; x <= c; x++)
                    {
                        if(c - x < stopPoint)
                            break;

                        if(tempState[r][c - x] != 0)
                            stopPoint = c - x;

                        if(tempState[r][c] == tempState[r][c - x] && tempState[r][c] != 0)
                        {
                            tempState[r][c] = 0;
                            tempState[r][c - x] *= 2;
                            c++;
                            stopPoint = c;
                        }
                    }
                }
            }

            for(int r = 0; r < rows; r++)
            {
                for(int c = 1; c < columns; c++)
                {
                    int dropPoint = -9999;
                    if(tempState[r][c] != 0)
                    {
                        for(int x = c - 1; x >= 0; x--)
                        {
                            if(tempState[r][x] == 0)
                                dropPoint = x;
                            else
                                break;
                        }
                        if(dropPoint != -9999)
                        {
                            tempState[r][dropPoint] = tempState[r][c];
                            tempState[r][c] = 0;
                        }
                    }
                }
            }
            break;

        //RIGHT move
        case 3:
            for(int r = 0; r < rows; r++)
            {
                int stopPoint = columns - 1;
                for(int c = columns - 2; c >= 0; c--)
                {
                    for(int x = stopPoint; x > c; x--)
                    {
                        if(tempState[r][x] != 0)
                            stopPoint = x;

                        if(tempState[r][c] == tempState[r][x] && tempState[r][c] != 0)
                        {
                            tempState[r][c] = 0;
                            tempState[r][x] *= 2;
                            c--;
                            stopPoint = c;
                            break;
                        }
                    }
                }
            }

            for(int r = 0; r < rows; r++)
            {
                for(int c = columns - 2; c >= 0; c--)
                {
                    int dropPoint = -9999;
                    if(tempState[r][c] != 0)
                    {
                        for(int x = c + 1; x < columns; x++)
                        {
                            if(tempState[r][x] == 0)
                                dropPoint = x;
                            else
                                break;
                        }
                        if(dropPoint != -9999)
                        {
                            tempState[r][dropPoint] = tempState[r][c];
                            tempState[r][c] = 0;
                        }
                    }
                }
            }
            break;

        default:
            break;

    }

    return tempState;
}

//////////////////////////////////////////////////////////////////////////////////
//==============================SOLVING ALGORITHMS==============================//
//////////////////////////////////////////////////////////////////////////////////

//Solves board state using BFS
vector<vector<int>> solveBFS(vector<vector<int>> b, vector<int> t, int g)
{   
    //Frontiers for board states and movesets
    vector<vector<vector<int>>> boardFrontier;
    vector<string> movesFrontier{" "};

    //Temporary placeholder board states for checking
    vector<vector<int>> tempState;
    vector<vector<int>> checkState;

    //Temporary placeholder moves for checking
    string tempMoves;

    //Initial board state
    boardFrontier.push_back(b);

    //While the frontier has unchecked states, attempt to solve
    while(!(boardFrontier.empty()))
    {
        tempState = boardFrontier.front(); 
        tempMoves = movesFrontier.front();

        //Removes first element from the frontier
        boardFrontier.erase(boardFrontier.begin());
        movesFrontier.erase(movesFrontier.begin());

        for(int dir = 0; dir < 4; dir++)
        {
            //Checks all directional moves and generates board states
            checkState = moveState(tempState, t, dir);

            //Checks for illegal moves
            if(tempState != checkState)
            {
                if(dir == 0)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("U");
                    movesFrontier.push_back(tempMove);
                }
                else if(dir == 1)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("D");
                    movesFrontier.push_back(tempMove);
                }
                else if(dir == 2)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("L");
                    movesFrontier.push_back(tempMove);
                }
                else if(dir == 3)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("R");
                    movesFrontier.push_back(tempMove);
                }
                //Checks rotation based on depth (move length)
                rotation = (movesFrontier.back().length() + 1) % t.size();
                checkState = spawnTile(checkState, t);
                boardFrontier.push_back(checkState);
            }

            /*
            else if(tempState == checkState)
                cout << "+++++ILLEGAL+++++" << endl;
            cout << "=====MOVE " << movesFrontier.back() << "=====" << endl;
            printBoard(checkState);
            */

            //Checks for goal state
            if(isGoal(boardFrontier.back(), g))
            {
                moves = movesFrontier.back();
                return boardFrontier.back();
            }  
        }
    }

    //In case of error for no solutions
    cout << "+++++ERROR: NO SOLUTION+++++" << endl;
    return tempState;
}

//Solves board state using DFS
void solveDFS(vector<vector<vector<int>>> &bf, vector<string> &m, vector<int> t, int g)
{
    //Temporary placeholder board states for checking
    vector<vector<int>> tempState;  
    vector<vector<int>> checkState; 

    //Temporary placeholder moves for checking
    string tempMoves;

    while(!(bf.empty()))
    {
        tempState = bf.back();
        tempMoves = m.back();

        //Removes first element from the frontier
        bf.pop_back();
        m.pop_back();

        for(int dir = 0; dir < 4; dir++)
        {
            //Checks all directional moves and generates board states
            checkState = moveState(tempState, t, dir);

            //Checks for illegal moves and if reached depth limit
            if(tempState != checkState && (tempMoves.length() < limit))
            {
                if(dir == 0)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("U");
                    //cout << "=====MOVE " << tempMove << "=====" << endl;
                    m.push_back(tempMove);
                }
                else if(dir == 1)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("D");
                    //cout << "=====MOVE " << tempMove << "=====" << endl;                    
                    m.push_back(tempMove);
                }
                else if(dir == 2)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("L");
                    //cout << "=====MOVE " << tempMove << "=====" << endl;                    
                    m.push_back(tempMove);
                }
                else if(dir == 3)
                {
                    string tempMove = tempMoves;
                    tempMove = tempMove.append("R");
                    //cout << "=====MOVE " << tempMove << "=====" << endl;                    
                    m.push_back(tempMove);
                }
                //Checks rotation based on depth (move length)
                rotation = (m.back().length() + 1) % t.size();
                checkState = spawnTile(checkState, t);
                //printBoard(checkState);
                bf.push_back(checkState);
            }

            //Checks for goal state
            if(!(bf.empty()) && (tempMoves.length() < limit))
            {   
                //cout << "=====CHECKING " << m.back() << "=====" << endl;
                //printBoard(checkState);
                if(isGoal(bf.back(), g))
                {
                    moves = m.back();
                    goalState = bf.back();
                    goalFound = true;
                    return;
                }  
            }
        }
    }
    //If space exhausted, no solution
    if(limit > (pow(columns, rows) * 2))
    {
        cout << "+++++ERROR: NO SOLUTION+++++" << endl;
        noSolution = true;
        return;
    }
    //cout << "=====NO SOLUTION IN DEPTH LIMIT " << limit << "=====" << endl;
    return;
}

//Solves board state using ID-DFS
vector<vector<int>> solveID_DFS(vector<vector<int>> b, vector<int> t, int g)
{   
    while(!goalFound)
    {
        //cout << "+++++NOW ATTEMPTING DEPTH LIMIT " << limit << "+++++" << endl;
        vector<vector<vector<int>>> initialFrontier;
        vector<string> movesFrontier{" "};

        initialFrontier.push_back(b);

        solveDFS(initialFrontier, movesFrontier, t, g);
        limit++;

        if(noSolution)
            return b;
    }

    return goalState;
}

//Custom heuristic for GrBeFGS
vector<tuple<vector<vector<int>>, string, int>> heuristic(vector<tuple<vector<vector<int>>, string, int>> gf)
{
    vector<tuple<vector<vector<int>>, string, int>> tempGameFrontier = gf;

    //Heapify the frontier vector 
    make_heap(tempGameFrontier.begin(), tempGameFrontier.end(), compareMax());

    return tempGameFrontier;
}

//Solves board using GrBeFGS
vector<vector<int>> solveGrBeFGS(vector<vector<int>> b, vector<int> t, int g)
{
    //Variables for frontier, game states, and visited states
    vector<tuple<vector<vector<int>>, string, int>> gameFrontier;
    tuple<vector<vector<int>>, string, int> tempGameState;
    tuple<vector<vector<int>>, string, int> checkGameState;
    unordered_map<string, vector<vector<int>>> visitedStates;
    bool visitedFound; //Used to determine if a game state has been seen

    //Initial game state
    visitedStates.insert(make_pair(" ", b));
    tempGameState = make_tuple(b, " ", 0);
    gameFrontier.push_back(tempGameState);

    while(!(gameFrontier.empty()))
    {
        //Begins with front of frontier
        tempGameState = gameFrontier.front();
        gameFrontier.erase(gameFrontier.begin());
        visitedFound = false;

        //Checks all directions
        for(int dir = 0; dir < 4; dir++)
        {
            get<0>(checkGameState) = moveState(get<0>(tempGameState), t, dir);
            get<1>(checkGameState) = get<1>(tempGameState);

            //Illegal move checking
            if(get<0>(tempGameState) != get<0>(checkGameState))
            {
                if(dir == 0)
                {
                    string tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("U");
                    get<1>(checkGameState) = tempMove;
                }
                else if(dir == 1)
                {
                    string tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("D");
                    get<1>(checkGameState) = tempMove;
                }
                else if(dir == 2)
                {
                    string tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("L");
                    get<1>(checkGameState)= tempMove;
                }
                else if(dir == 3)
                {
                    string tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("R");
                    get<1>(checkGameState) = tempMove;
                }

                //Rotation of spawn tiles based on move length
                rotation = (get<1>(checkGameState).length() + 1) % t.size();
                get<0>(checkGameState) = spawnTile(get<0>(checkGameState), t);

                //Calculating weight of a game state to be sorted in heap
                int weight = 0;
                for(int r = 0; r < rows; r++)
                {
                    for(int c = 0; c < columns; c++)
                        weight = weight + get<0>(checkGameState)[r][c];
                }
                weight = weight / (columns * rows);
                get<2>(checkGameState) = weight;

                //Finding if a state has already been visited in the hash table
                unordered_map<string, vector<vector<int>>>::iterator itr;
                for(itr = visitedStates.begin(); itr != visitedStates.end(); itr++)
                {
                    if(itr->second == get<0>(checkGameState))
                        visitedFound = true;
                }
                
                //If is a new state, push it onto frontier and add it to visited table
                if(!visitedFound)
                {
                    visitedStates.insert(make_pair(get<1>(checkGameState), get<0>(checkGameState)));
                    gameFrontier.push_back(checkGameState); 
                }
            }            

            /*
            else if(get<0>(checkGameState) == get<0>(tempGameState))
                cout << "+++++ILLEGAL+++++" << endl;
            cout << "=====MOVE " << get<1>(gameFrontier.back()) << "=====" << endl;
            printBoard(get<0>(checkGameState));
            */

            //Returns an error state if no solution
            if(gameFrontier.empty())
            {
                cout << "+++++ERROR: NO SOLUTION+++++" << endl;
                moves = " ";
                return b;
            }
            
            //Checks for goal state
            if(isGoal(get<0>(gameFrontier.back()), g))
            {
                moves = get<1>(gameFrontier.back());
                return get<0>(gameFrontier.back());
            }
            
            //Sorts the frontier based on weight assigned
            gameFrontier = heuristic(gameFrontier);

            //cout << "MOVE LEN: " << get<1>(gameFrontier.front()).length() << endl;
            //cout << "WEIGHT: " << get<2>(gameFrontier.front()) << endl;
        }
    }

    cout << "+++++ERROR: NO SOLUTION+++++" << endl;
    return b;
}