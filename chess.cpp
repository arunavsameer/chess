#include <bits/stdc++.h>
using namespace std;

string B1 = "R1K1B1KGQNB2K2R2";
string B2 = "P1P2P3P4P5P6P7P8";
string W1 = "r1k1b1qnkgb2k2r2";
string W2 = "p1p2p3p4p5p6p7p8";
string column = "ABCDEFGH";
string spacer = "|----+----+----+----+----+----+----+----|";
string spacer_ends = "-----------------------------------------";

// TO-DO
// add movement by input parsing
// add algorithm for each type of piece
// add history.txt and create history for games... also try to somehow read history and recreate the game's last move
// try to make the black and white box pattern, or somehow immitate it
// create print_board for white side as well

class game
{
    string board[8][8];

public:
    void reset_board()
    {
        for (int i = 0; i < 8; i++)
        {
            board[0][i] += B1[(2 * i)];
            board[0][i] += B1[(2 * i) + 1];
        }
        for (int i = 0; i < 8; i++)
        {
            board[1][i] += B2[(2 * i)];
            board[1][i] += B2[(2 * i) + 1];
        }
        for (int i = 2; i < 6; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                board[i][j] = "  ";
            }
        }
        for (int i = 0; i < 8; i++)
        {
            board[6][i] += W2[(2 * i)];
            board[6][i] += W2[(2 * i) + 1];
        }
        for (int i = 0; i < 8; i++)
        {
            board[7][i] += W1[(2 * i)];
            board[7][i] += W1[(2 * i) + 1];
        }
    }

    void input_black()
    {
    }

    void input_white()
    {
    }

    void algorithm()
    {
    }

    void print_board()
    {
        cout << " ";
        for (int i = 0; i < 8; i++)
        {
            cout << "    " << column[i];
        }
        cout << endl;
        string spacer_local;
        for (int i = 0; i < 8; i++)
        {
            if (i == 0)
            {
                spacer_local = spacer_ends;
            }
            else
            {
                spacer_local = spacer;
            }
            cout << "  " << spacer_local << endl
                 << i + 1;
            for (int j = 0; j < 8; j++)
            {
                cout << " | " << board[i][j];
            }
            cout << " |" << endl;
        }
        cout << "  " << spacer_ends << endl;
    }
};

int main()
{
    game g;
    g.reset_board();
    g.print_board();
}