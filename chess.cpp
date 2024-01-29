#include <bits/stdc++.h>
#include <fstream>
using namespace std;

string B1 = "R1K1B1KGQNB2K2R2";
string B2 = "P1P2P3P4P5P6P7P8";
string W1 = "r1k1b1qnkgb2k2r2";
string W2 = "p1p2p3p4p5p6p7p8";
string column = "ABCDEFGH";
string spacer = "|----+----+----+----+----+----+----+----|";
string spacer_ends = "-----------------------------------------";

// TO-DO
// add algorithm for each type of piece and moves like castling
// read history and recreate the game's last move
// try to make the black and white box pattern, or somehow immitate it

class game
{
    string board[8][8];
    ofstream history;

public:
    bool game_over = false;

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

    void input(int n)
    {
        int delta;
        if (n % 2 == 0)
        {
            delta = 97;
        }
        else
        {
            delta = 65;
        }

        string input;
        bool valid = false;
        bool occur = false;
        pair<int, int> current;
        pair<int, int> final;
        string piece;

        do
        {
            cin >> input;
            piece += input[0];
            piece += input[1];
            if (input[0] >= delta)
            {
                for (int i = 0; i < 8; i++)
                {
                    for (int j = 0; j < 8; j++)
                    {
                        if (board[i][j] == piece)
                        {
                            occur = true;
                            current.first = i;
                            current.second = j;
                            break;
                        }
                    }
                }
            }

            final.first = input[3] - 49; // input[3] will be as char and not int data type
            final.second = input[2] - 65;

            if (occur)
            {
                if (algorithm())
                {
                    valid = true;
                    history.open("history.txt", std::ios_base::app);
                    if (history.is_open())
                    {
                        history << input << endl;
                    }
                    history.close();
                }
            }
        } while (!valid);

        board[final.first][final.second] = board[current.first][current.second];
        board[current.first][current.second] = "  ";
    }

    bool algorithm()
    {
        return true; // test
    }

    void print_board(int n)
    {
        if (n % 2 == 0)
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
        else
        {
            cout << " ";
            for (int i = 7; i >= 0; i--)
            {
                cout << "    " << column[i];
            }
            cout << endl;
            string spacer_local;
            for (int i = 7; i >= 0; i--)
            {
                if (i == 7)
                {
                    spacer_local = spacer_ends;
                }
                else
                {
                    spacer_local = spacer;
                }
                cout << "  " << spacer_local << endl
                     << i + 1;
                for (int j = 7; j >= 0; j--)
                {
                    cout << " | " << board[i][j];
                }
                cout << " |" << endl;
            }
            cout << "  " << spacer_ends << endl;
        }
    }
};

int main()
{
    game g;
    int moves = 0;
    g.reset_board();
    while (!g.game_over)
    {
        g.print_board(moves);
        g.input(moves);
        moves++;
    }
}
