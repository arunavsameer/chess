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
// add checkmate function
// try to make the black and white box pattern, or somehow immitate it

class game
{
    string board[8][8];
    ofstream history;
    pair<int, int> current;
    pair<int, int> final;
    pair<int, int> KG = make_pair(0, 3);
    pair<int, int> kg = make_pair(7, 3);
    string piece;
    bool kg_move, KG_move, R1_move, r1_move, R2_move, r2_move = false;

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
        // for(int i = 0; i < 8; i++){
        //     for(int j = 0; j < 8; j++){
        //         if(board[i][j] == "  "){
        //             if((i + j) % 2 == 0){
        //                 board[i][j] = "||";
        //             }
        //         }
        //     }
        // }
    }

    void input()
    {
        if (check())
        {
            if (!checkmate())
            {
                cout << "CHECK!" << endl;
            }
            else
            {
                cout << "CHECKMATE!";
                game_over = true;
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

        if (check())
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

    bool check()
    {
        // cout << "checking" << endl;
        //  check if the knight can attack the king, then check along straight and diagonal lines of any can attack the king
        if (moves % 2 == 1)
        {
            // knight
            if (KG.first >= 1 && KG.second >= 2)
            {
                if ((board[KG.first - 1][KG.second - 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first >= 2 && KG.second >= 1)
            {
                if ((board[KG.first - 2][KG.second - 1])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first >= 1 && KG.second <= 5)
            {
                if ((board[KG.first - 1][KG.second + 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 6 && KG.second >= 2)
            {
                if ((board[KG.first + 1][KG.second - 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 6 && KG.second <= 5)
            {
                if ((board[KG.first + 1][KG.second + 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 5 && KG.second <= 6)
            {
                if ((board[KG.first + 2][KG.second + 1])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first >= 2 && KG.second <= 1)
            {
                if ((board[KG.first - 2][KG.second + 1])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 5 && KG.second >= 1)
            {
                if ((board[KG.first + 2][KG.second - 1])[0] == 'k')
                {
                    return true;
                }
            }
            // cout << "knight" << endl;

            // pawn
            if ((board[KG.first + 1][KG.second + 1])[0] == 'p')
            {
                return true;
            }
            else if ((board[KG.first + 1][KG.second - 1])[0] == 'p')
            {
                return true;
            }
            // cout << "pawn" << endl;
            //  vertical line
            for (int i = KG.first + 1; i < 8; i++)
            {
                if ((board[i][KG.second])[0] == 'q' || (board[i][KG.second])[0] == 'r')
                {
                    return true;
                }
                else if (board[i][KG.second] != "  ")
                {
                    break;
                }
            }
            for (int i = KG.first - 1; i >= 0; i--)
            {
                if ((board[i][KG.second])[0] == 'q' || (board[i][KG.second])[0] == 'r')
                {
                    return true;
                }
                else if (board[i][KG.second] != "  ")
                {
                    break;
                }
            }
            // cout << "vertical" << endl;
            //  horizontal line
            for (int j = KG.second + 1; j < 8; j++)
            {
                if ((board[KG.first][j])[0] == 'q' || (board[KG.first][j])[0] == 'r')
                {
                    return true;
                }
                else if (board[KG.first][j] != "  ")
                {
                    break;
                }
            }
            for (int j = KG.second - 1; j >= 0; j--)
            {
                if ((board[KG.first][j])[0] == 'q' || (board[KG.first][j])[0] == 'r')
                {
                    return true;
                }
                else if (board[KG.first][j] != "  ")
                {
                    break;
                }
            }
            // cout << "horizontal" << endl;
            //  diagonals
            int i = KG.first + 1;
            int j = KG.second + 1;
            while (i != 8 && j != 8)
            {
                if ((board[i][j])[0] == 'b' || (board[i][j])[0] == 'q')
                {
                    // cout << KG.first << " " << KG.second << " " << i << " " << j << endl;
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
            i = KG.first + 1;
            j = KG.second - 1;
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
            i = KG.first - 1;
            j = KG.second + 1;
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
            i = KG.first - 1;
            j = KG.second - 1;
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
            // knight
            if (KG.first >= 1 && KG.second >= 2)
            {
                if ((board[KG.first - 1][KG.second - 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first >= 2 && KG.second >= 1)
            {
                if ((board[KG.first - 2][KG.second - 1])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first >= 1 && KG.second <= 5)
            {
                if ((board[KG.first - 1][KG.second + 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 6 && KG.second >= 2)
            {
                if ((board[KG.first + 1][KG.second - 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 6 && KG.second <= 5)
            {
                if ((board[KG.first + 1][KG.second + 2])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 5 && KG.second <= 6)
            {
                if ((board[KG.first + 2][KG.second + 1])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first >= 2 && KG.second <= 1)
            {
                if ((board[KG.first - 2][KG.second + 1])[0] == 'k')
                {
                    return true;
                }
            }
            else if (KG.first <= 5 && KG.second >= 1)
            {
                if ((board[KG.first + 2][KG.second - 1])[0] == 'k')
                {
                    return true;
                }
            }

            // pawn
            if ((board[kg.first - 1][kg.second + 1])[0] == 'P')
            {
                return true;
            }
            else if ((board[kg.first - 1][kg.second - 1])[0] == 'P')
            {
                return true;
            }

            // vertical line
            for (int i = kg.first + 1; i < 8; i++)
            {
                if ((board[i][kg.second])[0] == 'Q' || (board[i][kg.second])[0] == 'R')
                {
                    return true;
                }
                else if (board[i][kg.second] != "  ")
                {
                    break;
                }
            }
            for (int i = kg.first - 1; i >= 0; i--)
            {
                if ((board[i][kg.second])[0] == 'Q' || (board[i][kg.second])[0] == 'R')
                {
                    return true;
                }
                else if (board[i][kg.second] != "  ")
                {
                    break;
                }
            }

            // horizontal line
            for (int j = kg.second + 1; j < 8; j++)
            {
                if ((board[kg.first][j])[0] == 'Q' || (board[kg.first][j])[0] == 'R')
                {
                    return true;
                }
                else if (board[kg.first][j] != "  ")
                {
                    break;
                }
            }
            for (int j = kg.second - 1; j >= 0; j--)
            {
                if ((board[kg.first][j])[0] == 'Q' || (board[kg.first][j])[0] == 'R')
                {
                    return true;
                }
                else if (board[kg.first][j] != "  ")
                {
                    break;
                }
            }

            // diagonals
            int i = kg.first + 1;
            int j = kg.second + 1;
            while (i != 8 && j != 8)
            {
                if ((board[i][i])[0] == 'B' || (board[i][i])[0] == 'Q')
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

            i = kg.first + 1;
            j = kg.second - 1;
            while (i != 8 && j != -1)
            {
                if ((board[i][i])[0] == 'B' || (board[i][i])[0] == 'Q')
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

            i = kg.first - 1;
            j = kg.second + 1;
            while (i != -1 && j != 8)
            {
                if ((board[i][i])[0] == 'B' || (board[i][i])[0] == 'Q')
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

            i = kg.first - 1;
            j = kg.second - 1;
            while (i != -1 && j != -1)
            {
                if ((board[i][i])[0] == 'B' || (board[i][i])[0] == 'Q')
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

    bool checkmate()
    {

        return false;
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
                    if (check())
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
                    if (check())
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
                    if (check())
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
                    if (check())
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
        // find something better to make white spaces
        //   for(int i = 0; i < 8; i++){
        //      for(int j = 0; j < 8; j++){
        //          if(board[i][j] == "  "){
        //              if((i + j) % 2 == 0){
        //                  board[i][j] = "||";
        //              }
        //          }
        //      }
        //  }
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
