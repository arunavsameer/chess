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
// add stalemate and substitution
// add en passant move using history last line or any other way

class game
{
    string board[8][8];
    ofstream history;
    pair<int, int> current;
    pair<int, int> final;
    pair<int, int> KG = make_pair(0, 3);
    pair<int, int> kg = make_pair(7, 3);
    vector<pair<int, int>> threat;
    string piece;
    bool kg_move, KG_move, R1_move, r1_move, R2_move, r2_move = false;
    int king_x, king_y;

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
        if (moves % 2 == 1)
        {
            king_x = KG.first;
            king_y = KG.second;
        }
        else
        {
            king_x = kg.first;
            king_y = kg.second;
        }

        if (check(king_x, king_y))
        {
            if (!checkmate())
            {
                // cout << "CHECK!" << endl;
            }
            else
            {
                // cout << "CHECKMATE!";
                game_over = true;
                return;
            }
        }

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
                }
            }
        } while (!valid);

        // doing the checks after move
        board[final.first][final.second] = piece;
        board[current.first][current.second] = "  ";

        // for castling and checking check
        if (piece == "KG")
        {
            KG.first = final.first;
            KG.second = final.second;
            KG_move = true;
        }
        else if (piece == "kg")
        {
            kg.first = final.first;
            kg.second = final.second;
            kg_move = true;
        }
        else if (piece == "R1")
        {
            R1_move = true;
        }
        else if (piece == "R2")
        {
            R2_move = true;
        }
        else if (piece == "r1")
        {
            r1_move = true;
        }
        else if (piece == "r2")
        {
            r2_move = true;
        }

        if (moves % 2 == 1)
        {
            king_x = KG.first;
            king_y = KG.second;
        }
        else
        {
            king_x = kg.first;
            king_y = kg.second;
        }

        if (check(king_x, king_y))
        {

            cout << "CHECK!" << endl;
            board[current.first][current.second] = piece;
            board[final.first][final.second] = "  ";
            if (piece == "KG")
            {
                KG.first = current.first;
                KG.second = current.second;
            }
            else if (piece == "kg")
            {
                kg.first = current.first;
                kg.second = current.second;
            }

            return;
        }

        history.open("history.txt", std::ios_base::app);
        if (history.is_open())
        {
            history << input << endl;
        }
        history.close();

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
            final.second = abs(line[2] - 72);

            board[final.first][final.second] = board[current.first][current.second];
            board[current.first][current.second] = "  ";
            moves++;
        }
        file.close();
    }

    bool check(int x, int y)
    {
        // cout << "checking" << endl;
        if (moves % 2 == 1)
        {
            // king
            for (int i = max(0, king_x - 1); i <= min(7, king_x + 1); i++)
            {
                for (int j = max(0, king_y - 1); j <= min(7, king_y + 1); j++)
                {
                    if (board[i][j] == "kg")
                    {
                        return true;
                    }
                }
            }

            // knight
            if (x >= 1 && y >= 2)
            {
                if ((board[x - 1][y - 2])[0] == 'k')
                {
                    return true;
                }
            }
            if (x >= 2 && y >= 1)
            {
                if ((board[x - 2][y - 1])[0] == 'k')
                {
                    return true;
                }
            }
            if (x >= 1 && y <= 5)
            {
                if ((board[x - 1][y + 2])[0] == 'k')
                {
                    return true;
                }
            }
            if (x <= 6 && y >= 2)
            {
                if ((board[x + 1][y - 2])[0] == 'k')
                {
                    return true;
                }
            }
            if (x <= 6 && y <= 5)
            {
                if ((board[x + 1][y + 2])[0] == 'k')
                {
                    return true;
                }
            }
            if (x <= 5 && y <= 6)
            {
                if ((board[x + 2][y + 1])[0] == 'k')
                {
                    return true;
                }
            }
            if (x >= 2 && y <= 6)
            {
                if ((board[x - 2][y + 1])[0] == 'k')
                {
                    return true;
                }
            }
            if (x <= 5 && y >= 1)
            {
                if ((board[x + 2][y - 1])[0] == 'k')
                {
                    return true;
                }
            }
            // cout << "knight" << endl;

            // pawn
            if (x <= 6 && y <= 6)
            {
                if ((board[x + 1][y + 1])[0] == 'p')
                {
                    return true;
                }
            }
            if (x <= 6 && y >= 1)
            {
                if ((board[x + 1][y - 1])[0] == 'p')
                {
                    return true;
                }
            }
            // cout << "pawn" << endl;
            //  vertical line
            for (int i = x + 1; i < 8; i++)
            {
                if ((board[i][y])[0] == 'q' || (board[i][y])[0] == 'r')
                {
                    return true;
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }
            for (int i = x - 1; i >= 0; i--)
            {
                if ((board[i][y])[0] == 'q' || (board[i][y])[0] == 'r')
                {
                    return true;
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }
            // cout << "vertical" << endl;
            //  horizontal line
            for (int j = y + 1; j < 8; j++)
            {
                if ((board[x][j])[0] == 'q' || (board[x][j])[0] == 'r')
                {
                    return true;
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }
            for (int j = y - 1; j >= 0; j--)
            {
                if ((board[x][j])[0] == 'q' || (board[x][j])[0] == 'r')
                {
                    return true;
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }
            // cout << "horizontal" << endl;
            //  diagonals
            int i = x + 1;
            int j = y + 1;
            while (i != 8 && j != 8)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {
                    // cout << x << " " << y << " " << i << " " << j << endl;
                    // cout << board[i][j];
                    //  cout << " found" << endl;

                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j++;
            }
            // cout << "diag" << endl;
            i = x + 1;
            j = y - 1;
            while (i != 8 && j != -1)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {

                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j--;
            }

            // cout << "diag" << endl;
            i = x - 1;
            j = y + 1;
            while (i != -1 && j != 8)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {

                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j++;
            }
            // cout << "diag" << endl;
            i = x - 1;
            j = y - 1;
            while (i != -1 && j != -1)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {

                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j--;
            }
            // cout << "diag" << endl;
        }
        else
        {
            // king
            for (int i = max(0, king_x - 1); i <= min(7, king_x + 1); i++)
            {
                for (int j = max(0, king_y - 1); j <= min(7, king_y + 1); j++)
                {
                    if (board[i][j] == "KG")
                    {
                        return true;
                    }
                }
            }

            // knight
            if (x >= 1 && y >= 2)
            {
                if ((board[x - 1][y - 2])[0] == 'K')
                {
                    return true;
                }
            }
            if (x >= 2 && y >= 1)
            {
                if ((board[x - 2][y - 1])[0] == 'K')
                {
                    return true;
                }
            }
            if (x >= 1 && y <= 5)
            {
                if ((board[x - 1][y + 2])[0] == 'K')
                {
                    return true;
                }
            }
            if (x <= 6 && y >= 2)
            {
                if ((board[x + 1][y - 2])[0] == 'K')
                {

                    return true;
                }
            }
            if (x <= 6 && y <= 5)
            {
                if ((board[x + 1][y + 2])[0] == 'K')
                {

                    return true;
                }
            }
            if (x <= 5 && y <= 6)
            {
                if ((board[x + 2][y + 1])[0] == 'K')
                {

                    return true;
                }
            }
            if (x >= 2 && y <= 6)
            {
                if ((board[x - 2][y + 1])[0] == 'K')
                {

                    return true;
                }
            }
            if (x <= 5 && y >= 1)
            {
                if ((board[x + 2][y - 1])[0] == 'K')
                {
                    return true;
                }
            }

            // pawn
            if (x >= 1 && y <= 6)
            {
                if ((board[x - 1][y + 1])[0] == 'p')
                {
                    threat.push_back(make_pair(x + 1, y + 1));
                }
            }
            if (x >= 1 && y >= 1)
            {
                if ((board[x - 1][y - 1])[0] == 'p')
                {
                    threat.push_back(make_pair(x + 1, y - 1));
                }
            }

            // vertical line
            for (int i = x + 1; i < 8; i++)
            {
                if ((board[i][y])[0] == 'Q' || (board[i][y])[0] == 'R')
                {
                    return true;
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }
            for (int i = x - 1; i >= 0; i--)
            {
                if ((board[i][y])[0] == 'Q' || (board[i][y])[0] == 'R')
                {
                    return true;
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }

            // horizontal line
            for (int j = y + 1; j < 8; j++)
            {
                if ((board[x][j])[0] == 'Q' || (board[x][j])[0] == 'R')
                {
                    return true;
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }
            for (int j = y - 1; j >= 0; j--)
            {
                if ((board[x][j])[0] == 'Q' || (board[x][j])[0] == 'R')
                {
                    return true;
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }

            // diagonals
            int i = x + 1;
            int j = y + 1;
            while (i != 8 && j != 8)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {
                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j++;
            }

            i = x + 1;
            j = y - 1;
            while (i != 8 && j != -1)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {

                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j--;
            }

            i = x - 1;
            j = y + 1;
            while (i != -1 && j != 8)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {

                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j++;
            }

            i = x - 1;
            j = y - 1;
            while (i != -1 && j != -1)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {

                    return true;
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j--;
            }
        }
        return false;
    }

    void check_threats(int x, int y, int colour)
    {
        // cout << "checking" << endl;
        if (colour % 2 == 1)
        {
            // king
            for (int i = max(0, king_x - 1); i <= min(7, king_x + 1); i++)
            {
                for (int j = max(0, king_y - 1); j <= min(7, king_y + 1); j++)
                {
                    if (board[i][j] == "kg")
                    {
                        threat.push_back(make_pair(i, j));
                    }
                }
            }

            // knight
            if (x >= 1 && y >= 2)
            {
                if ((board[x - 1][y - 2])[0] == 'k')
                {
                    threat.push_back(make_pair(x - 1, y - 2));
                }
            }
            if (x >= 2 && y >= 1)
            {
                if ((board[x - 2][y - 1])[0] == 'k')
                {
                    threat.push_back(make_pair(x - 2, y - 1));
                }
            }
            if (x >= 1 && y <= 5)
            {
                if ((board[x - 1][y + 2])[0] == 'k')
                {
                    threat.push_back(make_pair(x - 1, y + 2));
                }
            }
            if (x <= 6 && y >= 2)
            {
                if ((board[x + 1][y - 2])[0] == 'k')
                {
                    threat.push_back(make_pair(x + 1, y - 2));
                }
            }
            if (x <= 6 && y <= 5)
            {
                if ((board[x + 1][y + 2])[0] == 'k')
                {
                    threat.push_back(make_pair(x + 1, y + 2));
                }
            }
            if (x <= 5 && y <= 6)
            {
                if ((board[x + 2][y + 1])[0] == 'k')
                {
                    threat.push_back(make_pair(x + 2, y + 1));
                }
            }
            if (x >= 2 && y <= 6)
            {
                if ((board[x - 2][y + 1])[0] == 'k')
                {
                    threat.push_back(make_pair(x - 2, y + 1));
                }
            }
            if (x <= 5 && y >= 1)
            {
                if ((board[x + 2][y - 1])[0] == 'k')
                {
                    threat.push_back(make_pair(x + 2, y - 1));
                }
            }
            // cout << "knight" << endl;

            // pawn
            if (x <= 6 && y <= 6)
            {
                if ((board[x + 1][y + 1])[0] == 'p')
                {
                    threat.push_back(make_pair(x + 1, y + 1));
                }
            }
            if (x <= 6 && y >= 1)
            {
                if ((board[x + 1][y - 1])[0] == 'p')
                {
                    threat.push_back(make_pair(x + 1, y - 1));
                }
            }
            // cout << "pawn" << endl;
            //  vertical line
            for (int i = x + 1; i < 8; i++)
            {
                if ((board[i][y])[0] == 'q' || (board[i][y])[0] == 'r')
                {
                    threat.push_back(make_pair(i, y));
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }
            for (int i = x - 1; i >= 0; i--)
            {
                if ((board[i][y])[0] == 'q' || (board[i][y])[0] == 'r')
                {
                    threat.push_back(make_pair(i, y));
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }
            // cout << "vertical" << endl;
            //  horizontal line
            for (int j = y + 1; j < 8; j++)
            {
                if ((board[x][j])[0] == 'q' || (board[x][j])[0] == 'r')
                {
                    threat.push_back(make_pair(x, j));
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }
            for (int j = y - 1; j >= 0; j--)
            {
                if ((board[x][j])[0] == 'q' || (board[x][j])[0] == 'r')
                {
                    threat.push_back(make_pair(x, j));
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }
            // cout << "horizontal" << endl;
            //  diagonals
            int i = x + 1;
            int j = y + 1;
            while (i != 8 && j != 8)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {
                    // cout << x << " " << y << " " << i << " " << j << endl;
                    // cout << board[i][j];
                    //  cout << " found" << endl;
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j++;
            }
            // cout << "diag" << endl;
            i = x + 1;
            j = y - 1;
            while (i != 8 && j != -1)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j--;
            }

            // cout << "diag" << endl;
            i = x - 1;
            j = y + 1;
            while (i != -1 && j != 8)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j++;
            }
            // cout << "diag" << endl;
            i = x - 1;
            j = y - 1;
            while (i != -1 && j != -1)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j--;
            }
            // cout << "diag" << endl;
        }
        else
        {
            // king
            for (int i = max(0, king_x - 1); i <= min(7, king_x + 1); i++)
            {
                for (int j = max(0, king_y - 1); j <= min(7, king_y + 1); j++)
                {
                    if (board[i][j] == "KG")
                    {
                    }
                }
            }

            // knight
            if (x >= 1 && y >= 2)
            {
                if ((board[x - 1][y - 2])[0] == 'K')
                {
                    threat.push_back(make_pair(x - 1, y - 2));
                }
            }
            if (x >= 2 && y >= 1)
            {
                if ((board[x - 2][y - 1])[0] == 'K')
                {
                    threat.push_back(make_pair(x - 2, y - 1));
                }
            }
            if (x >= 1 && y <= 5)
            {
                if ((board[x - 1][y + 2])[0] == 'K')
                {
                    threat.push_back(make_pair(x - 1, y + 2));
                }
            }
            if (x <= 6 && y >= 2)
            {
                if ((board[x + 1][y - 2])[0] == 'K')
                {
                    threat.push_back(make_pair(x + 1, y - 2));
                }
            }
            if (x <= 6 && y <= 5)
            {
                if ((board[x + 1][y + 2])[0] == 'K')
                {
                    threat.push_back(make_pair(x + 1, y + 2));
                }
            }
            if (x <= 5 && y <= 6)
            {
                if ((board[x + 2][y + 1])[0] == 'K')
                {
                    threat.push_back(make_pair(x + 2, y + 1));
                }
            }
            if (x >= 2 && y <= 6)
            {
                if ((board[x - 2][y + 1])[0] == 'K')
                {
                    threat.push_back(make_pair(x - 2, y + 1));
                }
            }
            if (x <= 5 && y >= 1)
            {
                if ((board[x + 2][y - 1])[0] == 'K')
                {
                    threat.push_back(make_pair(x + 2, y - 1));
                }
            }

            // pawn
            if (x >= 1 && y <= 6)
            {
                if ((board[x - 1][y + 1])[0] == 'P')
                {
                    threat.push_back(make_pair(x - 1, y + 1));
                }
            }
            if (x >= 1 && y >= 1)
            {
                if ((board[x - 1][y - 1])[0] == 'P')
                {
                    threat.push_back(make_pair(x - 1, y - 1));
                }
            }

            // vertical line
            for (int i = x + 1; i < 8; i++)
            {
                if ((board[i][y])[0] == 'Q' || (board[i][y])[0] == 'R')
                {
                    threat.push_back(make_pair(i, y));
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }
            for (int i = x - 1; i >= 0; i--)
            {
                if ((board[i][y])[0] == 'Q' || (board[i][y])[0] == 'R')
                {
                    threat.push_back(make_pair(i, y));
                }
                else if (board[i][y] != "  ")
                {
                    break;
                }
            }

            // horizontal line
            for (int j = y + 1; j < 8; j++)
            {
                if ((board[x][j])[0] == 'Q' || (board[x][j])[0] == 'R')
                {
                    threat.push_back(make_pair(x, j));
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }
            for (int j = y - 1; j >= 0; j--)
            {
                if ((board[x][j])[0] == 'Q' || (board[x][j])[0] == 'R')
                {
                    threat.push_back(make_pair(x, j));
                }
                else if (board[x][j] != "  ")
                {
                    break;
                }
            }

            // diagonals
            int i = x + 1;
            int j = y + 1;
            while (i != 8 && j != 8)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j++;
            }

            i = x + 1;
            j = y - 1;
            while (i != 8 && j != -1)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i++;
                j--;
            }

            i = x - 1;
            j = y + 1;
            while (i != -1 && j != 8)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j++;
            }

            i = x - 1;
            j = y - 1;
            while (i != -1 && j != -1)
            {
                if ((board[i][j])[0] == 'B' || (board[i][j])[0] == 'Q')
                {
                    threat.push_back(make_pair(i, j));
                }
                else if (board[i][j] != "  ")
                {
                    break;
                }
                i--;
                j--;
            }
        }
    }

    bool checkmate()
    {
        cout << "checking for checkmate" << endl;
        // see if king can move to safety
        for (int i = max(0, king_x - 1); i <= min(7, king_x + 1); i++)
        {
            for (int j = max(0, king_y - 1); j <= min(7, king_y + 1); j++)
            {
                if (moves % 2 == 1)
                {
                    if (!check(i, j) && (board[i][j] == "  " || (board[i][j])[0] > 95))
                    {
                        return false;
                    }
                }
                else
                {
                    if (!check(i, j) && (board[i][j] == "  " || (board[i][j])[0] < 95))
                    {
                        return false;
                    }
                }
            }
        }
        cout << "king can't move" << endl;

        // create a list of threats of king
        threat.clear();
        check_threats(king_x, king_y, moves);

        // after this if there is more than one threat left, it is checkmate as both can't be blocked or attacked at the same time
        if (threat.size() > 1)
        {
            return true;
        }
        cout << "only one threat" << endl;

        int threat_x = threat[0].first;
        int threat_y = threat[0].second;
        // cout << threat_x << " " << threat_y << endl;

        // create list of threats of the threat
        threat.clear();
        check_threats(threat_x, threat_y, moves + 1);

        // eliminate the threat
        string saver, threater;
        threater = board[threat_x][threat_y];
        for (int i = 0; i < threat.size(); i++)
        {
            // cout << threat[i].first << " " << threat[i].second << endl;
            saver = board[threat[i].first][threat[i].second];
            if (saver == "KG" || saver == "kg")
            {
                king_x = threat_x;
                king_y = threat_y;
            }
            // cout << saver << " " <<threater <<endl;
            board[threat[i].first][threat[i].second] = "  ";
            board[threat_x][threat_y] = saver;
            if (!check(king_x, king_y))
            {
                board[threat[i].first][threat[i].second] = saver;
                board[threat_x][threat_y] = threater;
                return false;
            }
            board[threat[i].first][threat[i].second] = saver;
            board[threat_x][threat_y] = threater;
            if (saver == "KG" || saver == "kg")
            {
                king_x = threat[i].first;
                king_y = threat[i].second;
            }
        }

        cout << "can't eliminate threat" << endl;

        // block the threat
        // rook
        int dir_to_threat_x;
        int dir_to_threat_y;

        if (king_x > threat_x)
        {
            dir_to_threat_x = -1;
        }
        else if (king_x < threat_x)
        {
            dir_to_threat_x = 1;
        }
        else
        {
            dir_to_threat_x = 0;
        }

        if (king_y > threat_y)
        {
            dir_to_threat_y = -1;
        }
        else if (king_y < threat_y)
        {
            dir_to_threat_y = 1;
        }
        else
        {
            dir_to_threat_y = 0;
        }

        int block_x = king_x + dir_to_threat_x;
        int block_y = king_y + dir_to_threat_y;
        string blocker;

        while (!(block_x == threat_x || block_y == threat_y))
        {
            // pawn block
            if (moves % 2 == 1)
            {
                if ((board[block_x - 1][block_y])[0] == 'P')
                {
                    return false;
                }
            }
            else
            {
                if ((board[block_x + 1][block_y])[0] == 'p')
                {
                    return false;
                }
            }
            // others
            threat.clear();
            check_threats(block_x, block_y, moves + 1);
            for (int i = 0; i < threat.size(); i++)
            {
                blocker = board[threat[i].first][threat[i].second];
                if (blocker[0] == 'p' || blocker[0] == 'P')
                {
                    // removing pawn attack as block
                    continue;
                }
                blocker = board[threat[i].first][threat[i].second];

                // cout << blocker <<endl;
                board[threat[i].first][threat[i].second] = "  ";
                board[block_x][block_y] = blocker;
                if (!check(king_x, king_y))
                {
                    board[threat[i].first][threat[i].second] = blocker;
                    board[block_x][block_y] = "  ";
                    return false;
                }
                board[threat[i].first][threat[i].second] = blocker;
                board[block_x][block_y] = "  ";
            }
            block_x += dir_to_threat_x;
            block_y += dir_to_threat_y;
        }
        return true;
    }

    bool algorithm()
    {
        // pawn
        if (piece[0] == 'p')
        {
            if (current.first == 6)
            {
                if (final.first == 5 || final.first == 4)
                {
                    if (current.second == final.second)
                    {
                        if (board[final.first][final.second] == "  ")
                        {
                            return true;
                        }
                    }
                    else if (board[final.first][final.second] != "  " && (board[final.first][final.second])[0] < 97)
                    {
                        if (abs(current.second - final.second) == 1)
                        {
                            return true;
                        }
                    }
                }
            }
            else
            {
                if (current.first - final.first == 1)
                {
                    if (current.second == final.second)
                    {
                        if (board[final.first][final.second] == "  ")
                        {
                            return true;
                        }
                    }
                    else if (board[final.first][final.second] != "  " && (board[final.first][final.second])[0] < 97)
                    {
                        if (abs(current.second - final.second) == 1)
                        {
                            return true;
                        }
                    }
                }
            }
        }

        if (piece[0] == 'P')
        {
            if (current.first == 1)
            {
                if (final.first == 2 || final.first == 3)
                {
                    if (current.second == final.second)
                    {
                        if (board[final.first][final.second] == "  ")
                        {
                            return true;
                        }
                    }
                    else if (board[final.first][final.second] != "  " && (board[final.first][final.second])[0] > 97)
                    {
                        if (abs(current.second - final.second) == 1)
                        {
                            return true;
                        }
                    }
                }
            }
            else
            {
                if (final.first - current.first == 1)
                {
                    if (current.second == final.second)
                    {
                        if (board[final.first][final.second] == "  ")
                        {
                            return true;
                        }
                    }
                    else if (board[final.first][final.second] != "  " && (board[final.first][final.second])[0] > 97)
                    {
                        if (abs(current.second - final.second) == 1)
                        {
                            return true;
                        }
                    }
                }
            }
        }

        // rook
        if (piece[0] == 'r' || piece[0] == 'R')
        {
            bool obstacle = false;
            bool straight = false;
            if (current.first == final.first)
            {
                straight = true;
                for (int i = min(current.second, final.second) + 1; i < max(current.second, final.second); i++)
                {
                    if (board[current.first][i] != "  ")
                    {
                        obstacle = true;
                        break;
                    }
                }
            }
            else if (current.second == final.second)
            {
                straight = true;
                for (int i = min(current.first, final.first) + 1; i < max(current.first, final.first); i++)
                {
                    if (board[i][current.second] != "  ")
                    {
                        return false;
                    }
                }
            }
            if (straight)
            {
                if (board[final.first][final.second] != "  ")
                {
                    if (piece[0] == 'r')
                    {
                        if ((board[final.first][final.second])[0] < 97)
                        {
                            return true;
                        }
                    }
                    else if (piece[0] == 'R')
                    {
                        if ((board[final.first][final.second])[0] > 97)
                        {
                            return true;
                        }
                    }
                }
                else
                {
                    return true;
                }
            }
        }

        // knight
        if (piece[0] == 'k' || piece[0] == 'K')
        {
            if ((abs(current.first - final.first) == 2 && abs(current.second - final.second) == 1) || (abs(current.first - final.first) == 1 && abs(current.second - final.second) == 2))
            {
                if (board[final.first][final.second] == "  ")
                {
                    return true;
                }
                else if (piece[0] == 'K')
                {
                    if ((board[final.first][final.second])[0] > 97)
                    {
                        return true;
                    }
                }
                else if (piece[0] == 'k')
                {
                    if ((board[final.first][final.second])[0] < 97)
                    {
                        return true;
                    }
                }
            }
        }

        // bishop
        if (piece[0] == 'b' || piece[0] == 'B')
        {
            if (abs(final.first - current.first) == abs(final.second - current.second))
            {
                int dir_x, dir_y;
                if (final.first > current.first)
                {
                    dir_x = 1;
                }
                else
                {
                    dir_x = -1;
                }
                if (final.second > current.second)
                {
                    dir_y = 1;
                }
                else
                {
                    dir_y = -1;
                }
                int i = current.first + dir_x;
                int j = current.second + dir_y;
                while (i != final.first)
                {
                    if (board[i][j] != "  ")
                    {
                        return false;
                    }
                    i += dir_x;
                    j += dir_y;
                }

                if (board[final.first][final.second] == "  ")
                {
                    return true;
                }
                else if (piece[0] == 'b')
                {
                    if ((board[final.first][final.second])[0] < 97)
                    {
                        return true;
                    }
                }
                else if (piece[0] == 'B')
                {
                    if ((board[final.first][final.second])[0] > 97)
                    {
                        return true;
                    }
                }
            }
        }

        // king
        if (piece == "KG" || piece == "kg")
        {
            // castling

            if (piece == "KG" && !KG_move)
            {
                bool obstacle = false;
                int dir;
                if (final.second > current.second)
                {
                    dir = 1;
                }
                else
                {
                    dir = -1;
                }

                if (board[final.first][final.second + (2 * dir)] == "R1" && !R1_move)
                {

                    if (check(king_x, king_y) || check(king_x, king_y + 1) || check(king_x, king_y + 2) || check(king_x, king_y + 3))
                    {
                        return false;
                    }
                    for (int i = current.second + dir; i != final.second + (2 * dir); i += dir)
                    {
                        if (board[current.first][i] != "  ")
                        {
                            return false;
                        }
                    }
                    board[current.first][final.second - dir] = "R1";
                    board[current.first][final.second + (2 * dir)] = "  ";
                    return true;
                }
                else if (board[final.first][final.second + dir] == "R2" && !R2_move)
                {
                    if (check(king_x, king_y) || check(king_x, king_y - 1) || check(king_x, king_y - 2))
                    {
                        return false;
                    }
                    int i = current.first + dir;
                    while (i != final.second)
                    {
                        if (board[current.first][i] != "  ")
                        {
                            return false;
                        }
                        i += dir;
                    }

                    board[current.first][final.second - dir] = "R2";
                    board[current.first][final.second + dir] = "  ";
                    return true;
                }
            }
            else if (piece == "kg" && !kg_move)
            {

                bool obstacle = false;
                int dir;
                if (final.second > current.second)
                {
                    dir = 1;
                }
                else
                {
                    dir = -1;
                }
                if (board[final.first][final.second + (2 * dir)] == "r1" && !r1_move)
                {
                    if (check(king_x, king_y) || check(king_x, king_y + 1) || check(king_x, king_y + 2) || check(king_x, king_y + 3))
                    {
                        return false;
                    }
                    for (int i = current.second + dir; i != final.second + (2 * dir); i += dir)
                    {
                        if (board[current.first][i] != "  ")
                        {
                            return false;
                        }
                    }

                    board[current.first][final.second - dir] = "r1";
                    board[current.first][final.second + (2 * dir)] = "  ";
                    return true;
                }
                else if (board[final.first][final.second] == "r2" && !r2_move)
                {
                    if (check(king_x, king_y) || check(king_x, king_y - 1) || check(king_x, king_y - 2))
                    {
                        return false;
                    }
                    for (int i = current.second + dir; i != final.second; i += dir)
                    {
                        if (board[current.first][i] != "  ")
                        {
                            return false;
                        }
                    }

                    board[current.first][final.second - dir] = "r2";
                    board[current.first][final.second + dir] = "  ";
                    return true;
                }
            }
            // king normal move
            if (abs(final.first - current.first) == 1 || abs(final.second - current.second) == 1)
            {
                if ((board[final.first][final.second]) == "  ")
                {
                    return true;
                }
                else if (piece == "KG")
                {
                    if ((board[final.first][final.second])[0] > 97)
                    {
                        return true;
                    }
                }
                else if (piece == "kg")
                {
                    if ((board[final.first][final.second])[0] < 97)
                    {
                        return true;
                    }
                }
            }
        }

        // queen
        if (piece == "qn" || piece == "QN")
        {
            bool obstacle = false;
            if ((abs(final.first - current.first) == abs(final.second - current.second)) || (current.first == final.first) || (current.second == final.second))
            {
                int dir_x;
                int dir_y;
                if (current.first == final.first)
                {
                    dir_x = 0;
                    if (final.second > current.second)
                    {
                        dir_y = 1;
                    }
                    else
                    {
                        dir_y = -1;
                    }
                }
                else if (current.second == final.second)
                {
                    dir_y = 0;
                    if (final.first > current.first)
                    {
                        dir_x = 1;
                    }
                    else
                    {
                        dir_x = -1;
                    }
                }
                else if (abs(final.first - current.first) == abs(final.second - current.second))
                {
                    if (final.first > current.first)
                    {
                        dir_x = 1;
                    }
                    else
                    {
                        dir_x = -1;
                    }
                    if (final.second > current.second)
                    {
                        dir_y = 1;
                    }
                    else
                    {
                        dir_y = -1;
                    }
                }

                int i = current.first + dir_x;
                int j = current.second + dir_y;

                while (i != final.first && j != final.second)
                {
                    if (board[i][j] != "  ")
                    {
                        obstacle = true;
                        break;
                    }
                    i += dir_x;
                    j += dir_y;
                }
                if (!obstacle)
                {
                    if (board[final.first][final.second] == "  ")
                    {
                        return true;
                    }
                    else if (piece == "qn")
                    {
                        if ((board[final.first][final.second])[0] < 97)
                        {
                            return true;
                        }
                    }
                    else if (piece == "QN")
                    {
                        if ((board[final.first][final.second])[0] > 97)
                        {
                            return true;
                        }
                    }
                }
            }
        }

        return false;
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
                    cout << " | ";
                    if (board[i][j] != "  ")
                    {
                        cout << board[i][j];
                    }
                    else
                    {
                        if ((i + j) % 2 == 0)
                        {
                            cout << "::";
                        }
                        else
                        {
                            cout << "  ";
                        }
                    }
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
                    cout << " | ";
                    if (board[i][j] != "  ")
                    {
                        cout << board[i][j];
                    }
                    else
                    {
                        if ((i + j) % 2 == 0)
                        {
                            cout << "::";
                        }
                        else
                        {
                            cout << "  ";
                        }
                    }
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
        // system("cls");
        g.print_board();
    }
}
