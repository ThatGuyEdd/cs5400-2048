/*
Edward Tsang
Missouri University of Science and Technology
2048 Puzzle Assignment
CS5400: Introduction to Artificial Intelligence
Dr. Morales
*/

#include "2048.h"

int main()
{
    auto start = high_resolution_clock::now();
    
    int goal; //Goal state
    int tile; //Variable for tiles
    string spawnTiles; //String for series of spawn tiles
    vector<vector<int>> board; //Game board
    vector<int> genTiles; //Vector of spawning tiles

    //Receiving input from files and filling in related vectors
    cin >> goal;
    cin >> columns >> rows;
    cin.ignore();
    getline(cin, spawnTiles);

    stringstream ss;
    ss << spawnTiles;
    int spawnTileVal;
    char tempStr[4];

    while(!ss.eof())
    {
        ss >> tempStr;
        spawnTileVal = atoi(tempStr);
        genTiles.push_back(spawnTileVal);
    }

    for(int r = 0; r < rows; r++)
    {
        vector<int> tempVec;
        for(int c = 0; c < columns; c++)
        {
            cin >> tile; 
            tempVec.push_back(tile);
        }
        board.push_back(tempVec);
    }
    
    //Temporary placeholder board state for solving the game
    vector<vector<int>> tempState;
    tempState = solve(board, genTiles, goal);

    //Keeping track of runtime
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << endl;

    //Prints final solution moveset
    if(!noSolution)
    {
        moves.erase(moves.begin());
        cout << moves.length() << endl;
        cout << moves << endl;
        printBoard(tempState);
    }

    return 0;
}