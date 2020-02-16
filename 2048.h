/*
Edward Tsang
Missouri University of Science and Technology
Puzzle Assignment #1
CS5400: Introduction to Artificial Intelligence
Dr. Morales
2/16/2020
*/

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;
using namespace std::chrono;

//Convenience's sake global variables
int columns, rows; //Board dimensions
int rotation; //Used to set rotation of spawning tiles
string moves; //Placeholder for moves
bool firstRun = true; //Used in case of a one move solution

//Prints game board
void printBoard(vector<vector<int>> b)
{
    for(int r = 0; r < rows; r++)
    {
        for(int c = 0; c < columns; c++)
        {
            cout << b[r][c] << " ";
        }
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
            {
                return true;
            }
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
    {
        tempState[0][0] = t[rotation];
        rotation++;
    }
    else if(tempState[0][columns - 1] == 0)
    {
        tempState[0][columns - 1] = t[rotation];
        rotation++;
    }
    else if(tempState[rows - 1][columns - 1] == 0)
    {
        tempState[rows - 1][columns - 1] = t[rotation];
        rotation++;
    }
    else if(tempState[rows - 1][0] == 0)
    {
        tempState[rows - 1][0] = t[rotation];
        rotation++;
    }

    //Resets rotation upon reaching final vector element
    if(rotation >= t.size())
    {
        rotation = 0;
    }

    return tempState;
}

//////////////////////////////////////////////////////////////
//====================MOVEMENT FUNCTIONS====================//
//////////////////////////////////////////////////////////////

vector<vector<int>> upState(vector<vector<int>> b, vector<string>& m, vector<int> t)
{
    //Temporary placeholder variables
    vector<vector<int>> tempState = b;
    vector<string> tempMoveset = m;
    string tempMove;
    
    //First run check for one move goal state, otherwise append new moveset
    if(!firstRun)
    {
        string tempMove = tempMoveset[0];
        tempMove = tempMove.append("U");
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }
    else
    {
        tempMove = "U";
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }

    
    /*
    ===Tile Checking===
    FUNCTIONALITY:
    Check the second closest tile to the edge given the direction.
    Check all tiles "below" that tile; if a tile below the checking tile is a match
    or not 0, stopping point for all subsequent tiles will be there.
    Matching tile will be double the old value; old tile position will be reset to 0.
    Proceed to check next "highest" tile (based on direction given).
    */
    for(int c = 0; c < columns; c++)
    {
        int stopPoint = -9999; //Point at which we no longer check for tiles
        for(int r = 1; r < rows; r++)
        {
            for(int x = 1; x <= r; x++)
            {
                if(r - x < stopPoint)
                {
                    break;
                }
                if(tempState[r - x][c] != 0)
                {
                    stopPoint = r;
                }
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

    /*
    ===Dropping Tiles In Direction===
    FUNCTIONALITY:
    Updated board state given from above, however, tiles are "floating" in place,
    not yet dropped in direction.
    This checks for any 0's "under" a tile and drops it accordingly to the first 
    nonzero found.
    */
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
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[dropPoint][c] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    //Spawns new tile given updated board state
    tempState = spawnTile(tempState, t);

    //cout << "=====MOVE " << tempMoveset.back() << "=====" << endl;
    //printBoard(tempState);

    return tempState;
}

vector<vector<int>> downState(vector<vector<int>> b, vector<string>& m, vector<int> t)
{
    vector<vector<int>> tempState = b;    
    vector<string> tempMoveset = m;
    string tempMove;

    if(!firstRun)
    {
        tempMove = tempMoveset[0];
        tempMove = tempMove.append("D");
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }
    else
    {
        tempMove = "D";
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }

    for(int c = 0; c < columns; c++)
    {
        int stopPoint = rows - 1;
        for(int r = rows - 2; r >= 0; r--)
        {
            for(int x = stopPoint; x > r; x--)
            {
                if(tempState[x][c] != 0)
                {
                    stopPoint = r;
                }
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
        for(int r = rows - 2; r > 0; r--)
        {
            int dropPoint = -9999;
            if(tempState[r][c] != 0)
            {
                for(int x = r + 1; x < rows; x++)
                {
                    if(tempState[x][c] == 0)
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[dropPoint][c] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    tempState = spawnTile(tempState, t);

    //cout << "=====MOVE " << tempMoveset.back() << "=====" << endl;
    //printBoard(tempState);

    return tempState;
}

vector<vector<int>> leftState(vector<vector<int>> b, vector<string>& m, vector<int> t)
{
    vector<vector<int>> tempState = b;
    vector<string> tempMoveset = m;
    string tempMove;
    
    if(!firstRun)
    {
        tempMove = tempMoveset[0];
        tempMove = tempMove.append("L");
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }
    else
    {
        tempMove = "L";
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }

    for(int r = 0; r < rows; r++)
    {
        int stopPoint = -9999;
        for(int c = 1; c < columns; c++)
        {
            for(int x = 1; x <= c; x++)
            {
                if(c - x < stopPoint)
                {
                    break;
                }
                if(tempState[r][c - x] != 0)
                {
                    stopPoint = c;
                }
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
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[r][dropPoint] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    tempState = spawnTile(tempState, t);

    //cout << "=====MOVE " << tempMoveset.back() << "=====" << endl;
    //printBoard(tempState);

    return tempState;
}

vector<vector<int>> rightState(vector<vector<int>> b, vector<string>& m, vector<int> t)
{
    vector<vector<int>> tempState = b;
    vector<string> tempMoveset = m;
    string tempMove;
    
    if(!firstRun)
    {
        tempMove = tempMoveset[0];
        tempMove = tempMove.append("R");
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }
    else
    {
        tempMove = "R";
        tempMoveset.push_back(tempMove);
        m = tempMoveset;
    }

    for(int r = 0; r < rows; r++)
    {
        int stopPoint = columns - 1;
        for(int c = columns - 2; c >= 0; c--)
        {
            for(int x = stopPoint; x > c; x--)
            {
                if(tempState[r][x] != 0)
                {
                    stopPoint = c;
                }
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
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[r][dropPoint] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    tempState = spawnTile(tempState, t);

    //cout << "=====MOVE " << tempMoveset.back() << "=====" << endl;
    //printBoard(tempState);

    return tempState;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

//Solves board state given goal state; spawning tile vector passed for generation
vector<vector<int>> solve(vector<vector<int>> b, vector<int> t, int g)
{   
    //Frontiers for board states and movesets
    vector<vector<vector<int>>> boardFrontier;
    vector<string> movesFrontier;

    vector<vector<int>> tempState;

    //Boolean for tree depth change to keep track of when to rotate spawning tiles
    bool depthChange;

    //Initial board state
    boardFrontier.push_back(b);

    //While the frontier has unchecked states, attempt to solve
    while(!(boardFrontier.empty()))
    {
        //Temporary placeholder board state for checking
        tempState = boardFrontier.front();

        //Checks all directional moves and generates board states
        boardFrontier.push_back(upState(tempState, movesFrontier, t));
        if(isGoal(boardFrontier.back(), g))
        {
            moves = movesFrontier.back();
            return boardFrontier.back();
        }        
        boardFrontier.push_back(downState(tempState, movesFrontier, t));
        if(isGoal(boardFrontier.back(), g))
        {
            moves = movesFrontier.back();
            return boardFrontier.back();
        }
        boardFrontier.push_back(leftState(tempState, movesFrontier, t));
        if(isGoal(boardFrontier.back(), g))
        {
            moves = movesFrontier.back();
            return boardFrontier.back();
        }
        boardFrontier.push_back(rightState(tempState, movesFrontier, t));
        if(isGoal(boardFrontier.back(), g))
        {
            moves = movesFrontier.back();
            return boardFrontier.back();
        }

        //Removes first element from the frontiers
        boardFrontier.erase(boardFrontier.begin());
        if(!firstRun)
            movesFrontier.erase(movesFrontier.begin());
        
        //After first run, no longer first run
        firstRun = false;
        
        //Checks if a tree depth change has occurred
        for(int i = 1; i < movesFrontier.size(); i++)
        {
            if(movesFrontier.front().length(), movesFrontier[i].length())
            {
                depthChange = false;
                break;
            }
            depthChange = true;
        }

        //Rotation of spawning tiles
        if(depthChange = true)
        {
            rotation++;
        }
    }

    //In case of error for no solutions
    cout << "=====ERROR: NO SOLUTION=====" << endl;
    return tempState;
}

/*//////////////////////////////////////////////////////////////////////////////
//==========================MOVEMENT DEBUGGING================================//
////////////////////////////////////////////////////////////////////////////////

vector<vector<int>> upTest(vector<vector<int>> b, vector<int> t)
{
    vector<vector<int>> tempState = b;

    for(int c = 0; c < columns; c++)
    {
        int stopPoint = -9999;
        for(int r = 1; r < rows; r++)
        {
            for(int x = 1; x <= r; x++)
            {
                if(r - x < stopPoint)
                {
                    break;
                }
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
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[dropPoint][c] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    tempState = spawnTile(tempState, t);

    return tempState;
}

vector<vector<int>> downTest(vector<vector<int>> b, vector<int> t)
{
    vector<vector<int>> tempState = b;

    for(int c = 0; c < columns; c++)
    {
        int stopPoint = rows - 1;
        for(int r = rows - 2; r >= 0; r--)
        {
            for(int x = stopPoint; x > r; x--)
            {
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
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[dropPoint][c] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    tempState = spawnTile(tempState, t);

    return tempState;
}

vector<vector<int>> leftTest(vector<vector<int>> b, vector<int> t)
{
    vector<vector<int>> tempState = b;

    for(int r = 0; r < rows; r++)
    {
        int stopPoint = -9999;
        for(int c = 1; c < columns; c++)
        {
            for(int x = 1; x <= c; x++)
            {
                if(c - x < stopPoint)
                {
                    break;
                }
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
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[r][dropPoint] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    tempState = spawnTile(tempState, t);

    return tempState;
}

vector<vector<int>> rightTest(vector<vector<int>> b, vector<int> t)
{
    vector<vector<int>> tempState = b;

    for(int r = 0; r < rows; r++)
    {
        int stopPoint = columns - 1;
        for(int c = columns - 2; c >= 0; c--)
        {
            for(int x = stopPoint; x > c; x--)
            {
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
                    {
                        dropPoint = x;
                    }
                    else
                    {
                        break;
                    }
                }
                if(dropPoint != -9999)
                {
                    tempState[r][dropPoint] = tempState[r][c];
                    tempState[r][c] = 0;
                }
            }
        }
    }

    tempState = spawnTile(tempState, t);

    return tempState;
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/