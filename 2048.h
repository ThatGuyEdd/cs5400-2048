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
#include <time.h>

using namespace std;
using namespace std::chrono;

//Convenience's sake global variables
int columns, rows; //Board dimensions
int rotation = 0; //Used to set rotation of spawning tiles
string moves; //Placeholder for moves
bool noSolution = false; //Check if no solution

//Specifies heap sorting using average value of all tiles
struct Compare
{
    bool operator()(const tuple<vector<vector<int>>, string, float> &s1, const tuple<vector<vector<int>>, string, float> &s2)
    {
        return get<2>(s1) > get<2>(s2);
    }
};

//Prints game board
void printBoard(vector<vector<int>> b)
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
            cout << b[r][c] << "\t";
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

//Custom heuristic
vector<tuple<vector<vector<int>>, string, float>> heuristic(vector<tuple<vector<vector<int>>, string, float>> gf)
{
    vector<tuple<vector<vector<int>>, string, float>> tempGameFrontier = gf;

    //Heapify the frontier vector 
    make_heap(tempGameFrontier.begin(), tempGameFrontier.end(), Compare());

    return tempGameFrontier;
}

//Solves board using GrBeFGS
vector<vector<int>> solve(vector<vector<int>> b, vector<int> t, int g)
{
    //Variables for frontier, game states, and visited states
    vector<tuple<vector<vector<int>>, string, float>> gameFrontier;
    tuple<vector<vector<int>>, string, float> tempGameState;
    tuple<vector<vector<int>>, string, float> checkGameState;
    unordered_map<string, vector<vector<int>>> visitedStates;
    bool visitedFound; //Used to determine if a game state has been seen

    //To be used later for weight calculation
    float weight;
    float moveLength;
    int nonZeroes;
    string tempMove;

    //Initial game state
    visitedStates.insert(make_pair(" ", b));
    tempGameState = make_tuple(b, " ", 0);
    gameFrontier.push_back(tempGameState);

    //Checks for initial goal state
    if(isGoal(get<0>(gameFrontier.back()), g))
    {   
        moves = get<1>(gameFrontier.back());
        return get<0>(gameFrontier.back());
    }

    while(!(gameFrontier.empty()))
    {
        //Begins with front of frontier
        tempGameState = gameFrontier.front();
        gameFrontier.erase(gameFrontier.begin());

        //Checks all directions
        for(int dir = 0; dir < 4; dir++)
        {
            get<0>(checkGameState) = moveState(get<0>(tempGameState), t, dir);
            get<1>(checkGameState) = get<1>(tempGameState);
            visitedFound = false;

            //Illegal move checking
            if(get<0>(tempGameState) != get<0>(checkGameState))
            {
                if(dir == 0)
                {
                    tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("U");
                    get<1>(checkGameState) = tempMove;
                }
                else if(dir == 1)
                {
                    tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("D");
                    get<1>(checkGameState) = tempMove;
                }
                else if(dir == 2)
                {
                    tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("L");
                    get<1>(checkGameState)= tempMove;
                }
                else if(dir == 3)
                {
                    tempMove = get<1>(tempGameState);
                    tempMove = get<1>(checkGameState).append("R");
                    get<1>(checkGameState) = tempMove;
                }

                //Rotation of spawn tiles based on move length
                rotation = (get<1>(checkGameState).length() + 1) % t.size();
                get<0>(checkGameState) = spawnTile(get<0>(checkGameState), t);

                //Finding if a state has already been visited in the hash table
                unordered_map<string, vector<vector<int>>>::iterator itr;
                for(itr = visitedStates.begin(); itr != visitedStates.end(); itr++)
                {
                    if(itr->second == get<0>(checkGameState))
                    {
                        visitedFound = true;
                        break;
                    }
                }

                if(!visitedFound)
                {
                    weight = 0;
                    nonZeroes = 0;

                    //Calculating weight of a game state to be sorted in heap
                    for(int r = 0; r < rows; r++)
                    {
                        for(int c = 0; c < columns; c++)
                        {
                            weight = weight + get<0>(checkGameState)[r][c];
                            if(get<0>(checkGameState)[r][c] != 0)
                                nonZeroes++;
                        }
                    }
                    weight = nonZeroes / weight;
                    get<2>(checkGameState) = weight;
                
                    //If is a new state, push it onto frontier and add it to visited table
                    visitedStates.insert(make_pair(get<1>(checkGameState), get<0>(checkGameState)));
                    gameFrontier.push_back(checkGameState); 
                }
            }            

            if(gameFrontier.empty())
                break;
            
            //Checks for goal state
            if(isGoal(get<0>(gameFrontier.back()), g))
            {   
                moves = get<1>(gameFrontier.back());
                return get<0>(gameFrontier.back());
            }
            
            //Sorts the frontier based on weight
            gameFrontier = heuristic(gameFrontier);
        }
    }

    noSolution = true;
    return b;
}