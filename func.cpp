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
                fig.SetX(j);
                fig.SetY(i);
                sf::Sprite sprite;
                sprite.setTexture(dict[board[i][j]]);
                sprite.setOrigin(64.f, 64.f);  
                sprite.setPosition(128*j + 64, 128*i + 64);
                fig.GetSprite() = sprite;
                fig.SetType(board[i][j]);
                fig.SetBlack(board[i][j] == tolower(board[i][j]) ? true : false);
                figures.push_back(fig);
            }
        }
    }
    figures.push_back(figures[4]);
    figures.push_back(figures[28]);
    figures.erase(figures.begin() + 4);
    figures.erase(figures.begin() + 27);
}

void SetMovePos(figure &inp, move &temp, int i, int j)
{
    temp.GetCircle().setPosition(128*i+64, 128*j+64);
    temp.SetX(i);
    temp.SetY(j);
    inp.GetMoves().push_back(temp);
}

void MoveFigure(figure &inp, int i, char board[8][8], sf::RectangleShape BoardSquares[8][8])
{
    if(inp.GetType() == 'K' || inp.GetType() == 'k')
        KorRmoved[inp.GetBlack()*3] = true;
    if(inp.GetType() == 'R' || inp.GetType() == 'r')
        KorRmoved[inp.GetBlack()*3 + 1 + inp.GetX()%2] = true;
    short y = inp.GetY();
    short x = inp.GetX();
    AttackerPos = {-1, -1};
    inp.SetSpritePost(inp.GetMoves()[i].GetCircle().getPosition());
    board[y][x] = ' ';
    BoardSquares[x][y].setFillColor((x + y) % 2 == 0 ? CHESS_GREEN_COLOR : CHESS_WHITE_COLOR);
    inp.SetX(inp.GetMoves()[i].GetX());
    inp.SetY(inp.GetMoves()[i].GetY());
    board[inp.GetY()][inp.GetX()] = inp.GetType();
}

bool IsMoveLegit(figure &inp, int moves[8][8], char board[8][8], int Check, int NewX, int NewY);

void HelpMoveRBQ(figure &inp, char board[8][8], int moves[8][8], char cells[], int cellsX[], int cellsY[], int j, bool toremove[], bool IsBlack, move temp, char EnemyKing, int Check, bool NeedToCheck, bool IsSimulation)
{
    short y = inp.GetY();
    short x = inp.GetX();
    if(cellsX[j] < 0 || cellsY[j] < 0 || cellsX[j] > 7 || cellsY[j] > 7)
        return;
    if(IsSimulation)
    {
        if(board[cellsY[j]][cellsX[j]] != ' ')
        {
            if(board[cellsY[j]][cellsX[j]] == EnemyKing)
            {
                GlobalCheck = true;
            }
            toremove[j] = true;
            return;
        }
    }
    if(NeedToCheck && !IsSimulation && cells[j] != ' ' && ((IsBlack && !isupper(cells[j])) || (!IsBlack && isupper(cells[j]))))
    {
        toremove[j] = true;
        return;
    }
    if(!IsSimulation && moves[cellsY[j]][cellsX[j]] != (EnemyKing == 'K' ? 1 : 2))
        moves[cellsY[j]][cellsX[j]] = (moves[cellsY[j]][cellsX[j]] == 0 ? (EnemyKing == 'K' ? 1 : 2) : 3);
    if(board[cellsY[j]][cellsX[j]] == EnemyKing)
    {
        toremove[j] = true;
        AttackerPos.x = x;
        AttackerPos.y = y;
        return;
    }
    if(cells[j] == ' ' || ((IsBlack && isupper(cells[j])) || (!IsBlack && islower(cells[j]))))
    {
        if(NeedToCheck && !IsSimulation && IsMoveLegit(inp, moves, board, Check, cellsX[j], cellsY[j]))
            SetMovePos(inp, temp, cellsX[j], cellsY[j]);
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

bool CheckMove(int compare, int EnemyMove)
{
    return ((compare != EnemyMove) && (compare != 3));
}

void CheckCastle(figure &inp, int moves[8][8], char board[8][8], move temp)
{
    short x = inp.GetX();
    short y = inp.GetY();
    short EnemyMove = (inp.GetBlack() ? 2 : 1);
    if(inp.GetBlack())
    {
        if(!KorRmoved[3] && CheckMove(moves[y][1], EnemyMove) && CheckMove(moves[y][2], EnemyMove) && CheckMove(moves[y][3], EnemyMove) && (board[y][2] + board[y][1] + board[y][3] == (char)96))
        {
            SetMovePos(inp, temp, x-2, y);
        }
        if(!KorRmoved[4] && CheckMove(moves[y][5], EnemyMove) && CheckMove(moves[y][6], EnemyMove) && (board[y][5] + board[y][6] == (char)64))
        {
            SetMovePos(inp, temp, x+2, y);
        }
    }
    else
    {
        if(!KorRmoved[1] && CheckMove(moves[y][1], EnemyMove) && CheckMove(moves[y][2], EnemyMove) && CheckMove(moves[y][3], EnemyMove) && (board[y][2] + board[y][1] + board[y][3] == (char)96))
        {
            SetMovePos(inp, temp, x-2, y);
        }
        if(!KorRmoved[2] && CheckMove(moves[y][5], EnemyMove) && CheckMove(moves[y][6], EnemyMove) && ((board[y][5] + board[y][6]) == (char)64))
        {
            SetMovePos(inp, temp, x+2, y);
        }
    }
}   

void ShowMoves(figure &inp, int moves[8][8], char board[8][8], int Check, bool NeedToCheck = false, bool IsSimulation = false)
{
    short x = inp.GetX();
    if(x == -1)
        return;
    short y = inp.GetY();
    inp.GetMoves().clear();
    sf::CircleShape circle(35.f);
    circle.setFillColor(sf::Color(0, 0, 0, 75));
    circle.setOrigin(35.f, 35.f);
    move temp(-1, -1, circle);
    bool IsBlack = inp.GetBlack();
    char type = toupper(inp.GetType());
    char EnemyKing = (IsBlack ? 'K' : 'k');

    if(type == 'K')
    {
        if(!KorRmoved[inp.GetBlack()*3] && moves[y][x] != (IsBlack ? 2 : 1))
        {
            //CheckCastle(inp, moves, board, temp);
        }
        bool HasMoves = false;
        for(int i = x - 1; i <= x + 1; i++)
            for(int j = y - 1; j <= y + 1; j++)
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
                        SetMovePos(inp, temp, i, j); 
                    }
                }
    }
    if(type == 'P')
    {
        int cellY = (IsBlack ? (y+1) : (y-1));
        int cellX[3] = {x, x-1, x+1};
        for(int i = 0; i < 3; i++)
        {
            if(cellX[i] < 0 || cellY < 0 || cellX[i] > 7 || cellY > 7)
                continue;
            if(IsSimulation)
            {
                if(i != 0 && board[cellY][cellX[i]] == EnemyKing)
                {
                    GlobalCheck = true;
                    return;
                }
                continue;
            }
            bool Go1Square = true;

            if(NeedToCheck && i == 0 && !IsSimulation && !IsMoveLegit(inp, moves, board, Check, cellX[i], cellY))
                Go1Square = false;
            if(i != 0 && moves[cellY][cellX[i]] != (EnemyKing == 'K' ? 1 : 2))
                moves[cellY][cellX[i]] = (moves[cellY][cellX[i]] == 0 ? (EnemyKing == 'K' ? 1 : 2) : 3);
            if(i != 0 && board[cellY][cellX[i]] == EnemyKing)
            {
                AttackerPos.x = x;
                AttackerPos.y = y;
                continue;
            }
            if(board[cellY][cellX[i]] == ' ')
            {
                if(i == 0)
                {
                    if(Go1Square)
                        SetMovePos(inp, temp, cellX[i], cellY);
                    if(((y == 6 && !IsBlack) || (y == 1 && IsBlack)) && board[(cellY + (IsBlack ? 1 : -1))][cellX[i]] == ' ')
                    {
                        if(NeedToCheck && i == 0 && !IsSimulation && !IsMoveLegit(inp, moves, board, Check, cellX[i], (cellY + (IsBlack ? 1 : -1))))
                            continue;
                        SetMovePos(inp, temp, cellX[i], (cellY + (IsBlack ? 1 : -1)));
                    }
                }
            }
            else if(i != 0 && (IsBlack && isupper(board[cellY][cellX[i]]) || (!IsBlack && islower(board[cellY][cellX[i]]))))
                SetMovePos(inp, temp, cellX[i], cellY);
        }
    }            
    if(type == 'N')
    {
        int CellX[8] = {x-2, x-1, x+1, x+2, x+2, x+1, x-1, x-2};
        int CellY[8] = {y-1, y-2, y-2, y-1, y+1, y+2, y+2, y+1};
        for(int i = 0; i < 8; i++)
        {
            if(CellX[i] < 0 || CellX[i] > 7 || CellY[i] < 0 || CellY[i] > 7)
                continue;
            if(IsSimulation)
            {
                if(board[CellY[i]][CellX[i]] == EnemyKing)
                {
                    GlobalCheck = true;
                    return;
                }
                continue;
            }
            if(NeedToCheck && !IsSimulation && !IsMoveLegit(inp, moves, board, Check, CellX[i], CellY[i]))
                continue;
            if(moves[CellY[i]][CellX[i]] != (EnemyKing == 'K' ? 1 : 2))
                moves[CellY[i]][CellX[i]] = (moves[CellY[i]][CellX[i]] == 0 ? (EnemyKing == 'K' ? 1 : 2) : 3);
            if(board[CellY[i]][CellX[i]] == EnemyKing)
            {
                AttackerPos.x = x; 
                AttackerPos.y = y;
                continue;
            }
            else if((board[CellY[i]][CellX[i]] == ' ') || (IsBlack && isupper(board[CellY[i]][CellX[i]])) || (!IsBlack && islower(board[CellY[i]][CellX[i]])))
                SetMovePos(inp, temp, CellX[i], CellY[i]);
        }
    }
    if(type == 'R')
    {
        bool toremove[4] = {false};
        for(int i = 1; i < 8 ; i++)
        {
            char cells[4] = {board[y][x - i], board[y][x + i], board[y - i][x], board[y + i][x]};
            int cellsX[4] = {(x - i), (x + i), x, x};
            int cellsY[4] = {y, y, (y - i), (y + i)};
            for(int j = 0; j < 4; j++)
                if(!toremove[j])
                    HelpMoveRBQ(inp, board, moves, cells, cellsX, cellsY, j, toremove, IsBlack, temp, EnemyKing, Check, NeedToCheck, IsSimulation);
        }
    }
    if(type == 'B')
    {
        bool toremove[4] = {false};
        for(int i = 1; i < 8 ; i++)
        {
            char cells[4] = {board[y - i][x - i], board[y + i][x + i], board[y - i][x + i], board[y + i][x - i]};
            int cellsX[4] = {x - i, x + i, x + i, x - i};
            int cellsY[4] = {y - i, y + i, y - i, y + i};
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
            char cells[8] = {board[y][x - i], board[y][x + i], board[y - i][x], board[y + i][x], board[y - i][x - i], board[y + i][x + i], board[y - i][x + i], board[y + i][x - i]};
            int cellsX[8] = {x - i, x + i, x, x, x - i, x + i, x + i, x - i};
            int cellsY[8] = {y, y, y - i, y + i, y - i, y + i, y - i, y + i};
            for(int j = 0; j < 8; j++)
                if(!toremove[j])
                    HelpMoveRBQ(inp, board, moves, cells, cellsX, cellsY, j, toremove, IsBlack, temp, EnemyKing, Check, NeedToCheck, IsSimulation);
            }
    }
    
}

bool IsMoveLegit(figure &inp, int moves[8][8], char board[8][8], int Check, int NewX, int NewY)
{
    short x = inp.GetX();
    short y = inp.GetY();
    sf::Vector2i OldPos = {x, y};
    std::cout << inp.GetType();
    if(AttackerPos.x == NewX && AttackerPos.y == NewY)
        return true;
    char OldType = board[NewY][NewX];
    board[y][x] = ' ';
    inp.SetX(NewX);
    inp.SetY(NewY);
    board[NewY][NewX] = inp.GetType();
    int Start = (inp.GetBlack() ? 15 : 0);
    GlobalCheck = false;
    for(int i = Start; i < Start + 15; i ++)
        ShowMoves(figures[i], moves, board, (1-Check), true, true);
    if(Start == 15)
        ShowMoves(figures[30], moves, board, (1-Check), true, true);
    else
        ShowMoves(figures[31], moves, board, (1-Check), true, true);
    board[OldPos.y][OldPos.x] = inp.GetType();
    board[NewY][NewX] = OldType;
    inp.SetX(OldPos.x);
    inp.SetY(OldPos.y);
    bool StillCheck = GlobalCheck;
    std::cout << "Move legitimacy check completed." << StillCheck << std::endl;
    GlobalCheck = false;
    return (StillCheck == false);
}

void FiguresToSelect(sf::Sprite arr[4], figure &inp, std::map<char, sf::Texture>& dict, char figuresNames[4])
{    
    for(int i = 0; i < 4; i++)
    {   
        arr[i].setTexture(dict[figuresNames[i]]);
        arr[i].setOrigin(64.f, 64.f);
        arr[i].setPosition((i)*128 + 64, 64);
    }
    return;
}