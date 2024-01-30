#include <bits/stdc++.h>
#include <fstream>
#include <conio.h>
using namespace std;

string B1 = "R2K2B2KGQNB1K1R1";
string B2 = "P8P7P6P5P4P3P2P1";
string W1 = "r2k2b2kgqnb1k1r1";
string W2 = "p8p7p6p5p4p3p2p1";
string column = "ABCDEFGH";
string spacer = "|----+----+----+----+----+----+----+----|";
string spacer_ends = "-----------------------------------------";

// TO-DO
// add algorithm for each type of piece and moves like castling
// try to make the black and white box pattern, or somehow immitate it

class game
{
    string board[8][8];
    ofstream history;
    pair<int, int> current;
    pair<int, int> final;
    string piece;

public:
    bool game_over = false;
    int moves = 1;

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

    void input()
    {
        int delta;
        if (moves % 2 == 0)
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

        do
        {
            if (moves % 2 == 0)
            {
                cout << "BLACK MOVE: ";
            }
            else
            {
                cout << "WHITE MOVE: ";
            }
            cin >> input;
            piece.clear();
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
            final.second = abs(input[2] - 72);

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
        moves++;
    }

    void redo_history()
    {
        ifstream file("history.txt");
        string line;

        while (getline(file, line))
        {
            piece.clear();
            piece += line[0];
            piece += line[1];
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    if (board[i][j] == piece)
                    {
                        current.first = i;
                        current.second = j;
                    }
                }
            }

            final.first = line[3] - 49; // line[3] will be as char and not int data type
            final.second = line[2] - 65;

            board[final.first][final.second] = board[current.first][current.second];
            board[current.first][current.second] = "  ";
            moves++;
        }
        file.close();
    }

    bool algorithm()
    {
        return true; // test
    }

    void print_board()
    {

        if (moves % 2 == 0)
        {
            cout << " ";
            for (int i = 0; i < 8; i++)
            {
                cout << "    " << column[7 - i];
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
                cout << " | " << i + 1 << endl;
            }
            cout << "  " << spacer_ends << endl;
            cout << " ";
            for (int i = 0; i < 8; i++)
            {
                cout << "    " << column[7 - i];
            }
            cout << endl;
        }
        else
        {
            cout << " ";
            for (int i = 0; i < 8; i++)
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
                cout << " | " << i + 1 << endl;
            }
            cout << "  " << spacer_ends << endl;
            cout << " ";
            for (int i = 0; i < 8; i++)
            {
                cout << "    " << column[i];
            }
            cout << endl;
        }
    }
};

int main()
{
    game g;
    g.reset_board();
    g.redo_history();
    g.print_board();
    while (!g.game_over)
    {
        g.input();
        system("cls");
        g.print_board();
    }
}
