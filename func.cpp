#include "main.hpp"

void SetDict(std::map<char, sf::Texture>& dict)
{
    std::string pieces[12] = {"pawn-b", "pawn-w", "rook-b", "rook-w", "knight-b", "knight-w", "bishop-b", "bishop-w", "queen-b", "queen-w", "king-b", "king-w"};
    char piecesChar[12] = {'p', 'P', 'r', 'R', 'n', 'N', 'b', 'B', 'q', 'Q', 'k', 'K'};
    for(int i = 0; i < 12; i++)
    {
        sf::Texture texture;
        texture.loadFromFile("sprites/" + pieces[i] + ".png");
        dict[piecesChar[i]] = texture;
    }
}

void SetBoard(char board[8][8], std::vector<figure>& figures, std::map<char, sf::Texture>& dict)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(board[i][j] != ' ')
            {
                figure fig;
                fig.x = j;
                fig.y = i;
                sf::Sprite sprite;
                sprite.setTexture(dict[board[i][j]]);
                sprite.setOrigin(64.f, 64.f);  
                sprite.setPosition(128*j + 64, 128*i + 64);
                fig.sprite = sprite;
                fig.type = board[i][j];
                fig.IsBlack = (board[i][j] == tolower(board[i][j]) ? true : false);
                figures.push_back(fig);
            }
        }
    }
    std::swap(figures[4], figures[figures.size()-1]);
    std::swap(figures[28], figures[figures.size()-2]);
}

bool IsMoveLegit(figure &inp, int moves[8][8], char board[8][8], int Check, int NewX, int NewY);

void HelpMoveRBQ(figure &inp, char board[8][8], int moves[8][8], char cells[], int cellsX[], int cellsY[], int j, bool toremove[], bool IsBlack, move temp, char EnemyKing, int Check, bool NeedToCheck, bool IsSimulation)
{
    if(cellsX[j] < 0 || cellsY[j] < 0 || cellsX[j] > 7 || cellsY[j] > 7)
        return;
    if(IsSimulation)
    {
        if(board[cellsY[j]][cellsX[j]] != ' ')
        {
            if(board[cellsY[j]][cellsX[j]] == EnemyKing)
            {
                GlobalCheck[Check] = true;
            }
            toremove[j] = true;
            return;
        }
    }
    if(NeedToCheck && !IsSimulation && !((!IsBlack && isupper(cells[j])) || (IsBlack && islower(cells[j]))) && !IsMoveLegit(inp, moves, board, Check, cellsX[j], cellsY[j]))
    {
        toremove[j] = true;
        return;
    }
    if(moves[cellsY[j]][cellsX[j]] != (EnemyKing == 'K' ? 1 : 2))
        moves[cellsY[j]][cellsX[j]] = (moves[cellsY[j]][cellsX[j]] == 0 ? (EnemyKing == 'K' ? 1 : 2) : 3);
    if(board[cellsY[j]][cellsX[j]] == EnemyKing)
    {
        toremove[j] = true;
        AttackerPos.x = inp.x;
        AttackerPos.y = inp.y;
        GlobalCheck[Check] = true;
        return;
    }
    if(cells[j] == ' ' || ((IsBlack && isupper(cells[j])) || (!IsBlack && islower(cells[j]))))
    {
        temp.circle.setPosition(128*(cellsX[j])+64, 128*(cellsY[j])+64);
        temp.x = cellsX[j];
        temp.y = cellsY[j];
        inp.moves.push_back(temp);
        if((IsBlack && isupper(cells[j])) || (!IsBlack && islower(cells[j])))
        {
            toremove[j] = true;
            return;
        }
    }
    else
    {
        toremove[j] = true;
        return;
    }
}


void ShowMoves(figure &inp, int moves[8][8], char board[8][8], bool NeedToChange, int Check, bool NeedToCheck = false, bool IsSimulation = false)
{
    inp.moves.clear();
    sf::CircleShape circle(40.f);
    circle.setFillColor(sf::Color(0, 0, 0, 75));
    circle.setOrigin(40.f, 40.f);
    move temp(-1, -1, circle);
    bool IsBlack = inp.IsBlack;
    bool UnderCheck = GlobalCheck[1 - Check];
    char type = toupper(inp.type);
    char EnemyKing = (IsBlack ? 'K' : 'k');

    if(type == 'K')
    {
        if(UnderCheck)
        {
            if((moves[inp.y][inp.x] == 0) || (moves[inp.y][inp.x] == (IsBlack ? 1 : 2)))
            {   
                GlobalCheck[1 - Check] = false;
            }
        }
        bool HasMoves = false;
        for(int i = inp.x - 1; i <= inp.x + 1; i++)
            for(int j = inp.y - 1; j <= inp.y + 1; j++)
                if(i >= 0 && i < 8 && j >= 0 && j < 8)
                {
                    bool skipped = false;
                    for(int k = i-1; k <= i + 1; k++)
                        for(int l = j-1; l <= j + 1; l++)
                            if(k >= 0 && k < 8 && l >= 0 && l < 8 && board[l][k] == EnemyKing)
                                skipped = true;
                    if((moves[j][i] != (IsBlack ? 2 : 1) && moves[j][i] != 3) && !skipped && ((board[j][i] == ' ') || (IsBlack && isupper(board[j][i])) || (!IsBlack && islower(board[j][i]))))
                    {
                        HasMoves = true;
                        temp.circle.setPosition(128*i+64, 128*j+64);
                        temp.x = i;
                        temp.y = j;
                        inp.moves.push_back(temp);                        
                    }
                }
        if(!HasMoves && UnderCheck)
        {
            int Start = (IsBlack ? 0 : 16);
            bool CheckMate = true;
            for(int i = Start; i < Start + 16; i ++)
            {
                if(i != 4)
                {
                    ShowMoves(figures[i], moves, board, true, Check, NeedToCheck, IsSimulation);
                    if(figures[i].moves.size() != 0)
                    {
                        CheckMate = false;
                        break;
                    }
                }
            }
            if(!IsBlack)
            {
                ShowMoves(figures[4], moves, board, true, Check, NeedToCheck, IsSimulation);
                if(figures[4].moves.size() != 0)
                    CheckMate = false;
            }
            if(CheckMate)
                std::cout << "CheckMate to " << (Check == 1 ? "Black" : "White") << std::endl;
        }
    }
    if(type == 'P')
    {
        int cellY = (IsBlack ? (inp.y+1) : (inp.y-1));
        int cellX[3] = {inp.x, inp.x-1, inp.x+1};
        for(int i = 0; i < 3; i++)
        {
            if(cellX[i] < 0 || cellY < 0 || cellX[i] > 7)
                continue;
            if(IsSimulation)
            {
                if(board[cellY][cellX[i]] == EnemyKing)
                {
                    GlobalCheck[Check] = true;
                    return;
                }
                continue;
            }
            bool Go1Square = true;

            if(NeedToCheck && i == 0 && !IsSimulation && !IsMoveLegit(inp, moves, board, Check, cellX[i], cellY))
                Go1Square = false;
            if(i != 0 && moves[cellY][cellX[i]] != (EnemyKing == 'K' ? 1 : 2))
                moves[cellY][cellX[i]] = (moves[cellY][cellX[i]] == 0 ? (EnemyKing == 'K' ? 1 : 2) : 3);
            if(board[cellY][cellX[i]] == EnemyKing)
            {
                AttackerPos = sf::Vector2i(cellX[i], cellY);
                GlobalCheck[Check] = true;
                continue;
            }
            if(board[cellY][cellX[i]] == ' ')
            {
                if(i == 0)
                {
                    if(Go1Square)
                    {
                        temp.circle.setPosition(128*cellX[i]+64, 128*cellY+64);
                        temp.x = cellX[i];
                        temp.y = cellY;
                        inp.moves.push_back(temp);
                    }
                    if(((inp.y == 6 && !IsBlack) || (inp.y == 1 && IsBlack)) && board[(cellY + (IsBlack ? 1 : -1))][cellX[i]] == ' ')
                    {
                        if(NeedToCheck && i == 0 && !IsSimulation && !IsMoveLegit(inp, moves, board, Check, cellX[i], (cellY + (IsBlack ? 1 : -1))))
                            continue;
                        temp.circle.setPosition(128*cellX[i]+64, 128*(cellY + (IsBlack ? 1 : -1))+64);
                        temp.x = cellX[i];
                        temp.y = (cellY + (IsBlack ? 1 : -1));
                        inp.moves.push_back(temp);
                    }
                }
            }
            else if(i != 0 && (IsBlack && isupper(board[cellY][cellX[i]]) || (!IsBlack && islower(board[cellY][cellX[i]]))))
            {
                temp.circle.setPosition(128*cellX[i]+64, 128*cellY+64);
                temp.x = cellX[i];
                temp.y = cellY;
                inp.moves.push_back(temp);
            }
        }
    }            
    if(type == 'N')
    {
        int x[8] = {inp.x-2, inp.x-1, inp.x+1, inp.x+2, inp.x+2, inp.x+1, inp.x-1, inp.x-2};
        int y[8] = {inp.y-1, inp.y-2, inp.y-2, inp.y-1, inp.y+1, inp.y+2, inp.y+2, inp.y+1};
        for(int i = 0; i < 8; i++)
        {
            if(x[i] < 0 || x[i] > 7 || y[i] < 0 || y[i] > 7)
                continue;
            if(IsSimulation)
            {
                if(board[y[i]][x[i]] == EnemyKing)
                {
                    AttackerPos = sf::Vector2i(x[i], y[i]);
                    std::cout << "ШАХ" << std::endl;
                    GlobalCheck[Check] = true;
                    return;
                }
                continue;
            }
            if(NeedToCheck && !IsSimulation && !IsMoveLegit(inp, moves, board, Check, x[i], y[i]))
                continue;
            if(moves[y[i]][x[i]] != (EnemyKing == 'K' ? 1 : 2))
                moves[y[i]][x[i]] = (moves[y[i]][x[i]] == 0 ? (EnemyKing == 'K' ? 1 : 2) : 3);
            if(board[y[i]][x[i]] == EnemyKing)
            {
                GlobalCheck[Check] = true;
                continue;
            }
            else if(board[y[i]][x[i]] == ' ')
            {
                temp.circle.setPosition(128*x[i]+64, 128*y[i]+64);
                temp.x = x[i];
                temp.y = y[i];
                inp.moves.push_back(temp);
            }
            else if((IsBlack && isupper(board[y[i]][x[i]])) || (!IsBlack && islower(board[y[i]][x[i]])))
            {
                temp.circle.setPosition(128*x[i]+64, 128*y[i]+64);
                temp.x = x[i];
                temp.y = y[i];
                inp.moves.push_back(temp);
            }
        }
    }
    if(type == 'R')
    {
        bool toremove[4] = {false};
        for(int i = 1; i < 8 ; i++)
        {
            char cells[4] = {board[inp.y][inp.x - i], board[inp.y][inp.x + i], board[inp.y - i][inp.x], board[inp.y + i][inp.x]};
            int cellsX[4] = {(inp.x - i), (inp.x + i), inp.x, inp.x};
            int cellsY[4] = {inp.y, inp.y, (inp.y - i), (inp.y + i)};
            for(int j = 0; j < 4; j++)
                if(!toremove[j])
                {
                    HelpMoveRBQ(inp, board, moves, cells, cellsX, cellsY, j, toremove, IsBlack, temp, EnemyKing, Check, NeedToCheck, IsSimulation);
                }
        }
    }
    if(type == 'B')
    {
        bool toremove[4] = {false};
        for(int i = 1; i < 8 ; i++)
        {
            char cells[4] = {board[inp.y - i][inp.x - i], board[inp.y + i][inp.x + i], board[inp.y - i][inp.x + i], board[inp.y + i][inp.x - i]};
            int cellsX[4] = {(inp.x - i), (inp.x + i), (inp.x + i), (inp.x - i)};
            int cellsY[4] = {inp.y - i, inp.y + i, (inp.y - i), (inp.y + i)};
            for(int j = 0; j < 4; j++)
                if(!toremove[j])
                    HelpMoveRBQ(inp, board, moves, cells, cellsX, cellsY, j, toremove, IsBlack, temp, EnemyKing, Check, NeedToCheck, IsSimulation);
            }
    }
    if(type == 'Q')
    {
        bool toremove[8] = {false};
        for(int i = 1; i < 8 ; i++)
        {
            char cells[8] = {board[inp.y][inp.x - i], board[inp.y][inp.x + i], board[inp.y - i][inp.x], board[inp.y + i][inp.x], board[inp.y - i][inp.x - i], board[inp.y + i][inp.x + i], board[inp.y - i][inp.x + i], board[inp.y + i][inp.x - i]};
            int cellsX[8] = {(inp.x - i), (inp.x + i), inp.x, inp.x, (inp.x - i), (inp.x + i), (inp.x + i), (inp.x - i)};
            int cellsY[8] = {inp.y, inp.y, (inp.y - i), (inp.y + i), (inp.y - i), (inp.y + i), (inp.y - i), (inp.y + i)};
            for(int j = 0; j < 8; j++)
                if(!toremove[j])
                    HelpMoveRBQ(inp, board, moves, cells, cellsX, cellsY, j, toremove, IsBlack, temp, EnemyKing, Check, NeedToCheck, IsSimulation);
            }
    }
    
}

bool IsMoveLegit(figure &inp, int moves[8][8], char board[8][8], int Check, int NewX, int NewY)
{
    sf::Vector2i OldPos = {inp.x, inp.y};
    std::cout << inp.type;
    if(AttackerPos.x == NewX && AttackerPos.y == NewY)
        return true;
    char OldType = board[NewY][NewX];
    bool OldCheck = GlobalCheck[1-Check];
    board[inp.y][inp.x] = ' ';
    inp.SetX(NewX);
    inp.SetY(NewY);
    board[NewY][NewX] = inp.type;
    int Start = (inp.IsBlack ? 16 : 0);
    GlobalCheck[0] = false;
    GlobalCheck[1] = false;
    for(int i = Start; i < Start + 16; i ++)
    {
        if(i != 4)
        {
            ShowMoves(figures[i], moves, board, true, (1-Check), true, true);
        }
    }
    if(Start == 0)
        ShowMoves(figures[30], moves, board, true, (1-Check), true, true);
    else
    {
        ShowMoves(figures[4], moves, board, true, (1-Check), true, true);
        ShowMoves(figures[31], moves, board, true, (1-Check), true, true);
    }
    board[OldPos.y][OldPos.x] = inp.type;
    board[NewY][NewX] = OldType;
    inp.SetX(OldPos.x);
    inp.SetY(OldPos.y);
    bool StillCheck = GlobalCheck[1-Check];
    std::cout << "Move legitimacy check completed." << StillCheck << std::endl;
    GlobalCheck[1-Check] = false;
    return (StillCheck == false);
}