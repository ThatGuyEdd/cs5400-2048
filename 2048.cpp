#include "2048.h"

auto start = high_resolution_clock::now();

int main()
{
    int goal; //Goal state
    int tile; //Variable for spawning tiles
    vector<vector<int>> board; //Game board
    vector<int> genTiles; //Vector of spawning tiles

    //Receiving input from files and filling in related vectors
    cin >> goal;
    cin >> columns >> rows;
    for(int i = 0; i < 3; i++)
    {
        cin >> tile;
        genTiles.push_back(tile);
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

/*==================DEBUGGING INPUT==================//
    cout << goal << endl;
    cout << columns << " " << rows << endl;
    for(int v = 0; v < genTiles.size(); v++)
    {
        cout << genTiles[v] << " ";
    }
    cout << endl;
    cout << "=====INITIAL STATE=====" << endl;
    printBoard(board);
//===================================================*/
    
    //Temporary placeholder board state for solving the game
    vector<vector<int>> tempState;
    tempState = solve(board, genTiles, goal);

    //Keeping track of runtime
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << duration.count() << endl;

    //Prints final solution moveset
    cout << moves.length() << endl;
    cout << moves << endl;

    printBoard(tempState);
    
/*=================DEBUGGING OUTPUT==================//
    vector<vector<int>> testB;

    testB = upTest(board, genTiles);

    cout << "=======UP STATE========" << endl;
    printBoard(testB);

    testB = downTest(board, genTiles);

    cout << "======DOWN STATE=======" << endl;
    printBoard(testB);

    testB = leftTest(board, genTiles);

    cout << "======LEFT STATE=======" << endl;
    printBoard(testB);

    testB = rightTest(board, genTiles);

    cout << "======RIGHT STATE======" << endl;
    printBoard(testB);
//===================================================*/
    
    return 0;
}