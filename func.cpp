#include "main.h"

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
                figures.push_back(fig);
            }
        }
    }
}

void HelpMoveRBQ(figure &inp, char board[8][8], char cells[], int cellsX[], int cellsY[], int j, bool toremove[], bool IsBlack, sf::CircleShape circle, char EnemyKing)
{
    if(cellsX[j] < 0 || cellsY[j] < 0 || cellsX[j] > 7 || cellsY[j] > 7 || toremove[j])
        return;
    if(board[cellsY[j]][cellsX[j]] == EnemyKing)
    {
        toremove[j] = true;
        return;
    }
    if(cells[j] == ' ')
    {
        circle.setPosition(128*(cellsX[j])+64, 128*(cellsY[j])+64);
        inp.moves.push_back(circle);
    }
    else if(((IsBlack && isupper(cells[j])) || (!IsBlack && islower(cells[j]))))
    {
        circle.setPosition(128*(cellsX[j])+64, 128*(cellsY[j])+64);
        inp.moves.push_back(circle);
        toremove[j] = true;
        return;
    }
    else
    {
        toremove[j] = true;
        return;
    }
}

void ShowMoves(figure &inp, char board[8][8], bool NeedToChange = false)
{
    if(!NeedToChange && inp.moves.size() > 0)
        return;
    inp.moves.clear();
    sf::CircleShape circle(40.f);
    circle.setFillColor(sf::Color(0, 0, 0, 75));
    circle.setOrigin(40.f, 40.f);
    bool IsBlack = ((inp.type == toupper(inp.type)) ? false : true);
    char type = toupper(inp.type);
    char EnemyKing = (IsBlack ? 'K' : 'k');

    if(type == 'K')
    {
        for(int i = inp.x - 1; i <= inp.x + 1; i++)
            for(int j = inp.y - 1; j <= inp.y + 1; j++)
                if(i >= 0 && i < 8 && j >= 0 && j < 8)
                {
                    bool skipped = false;
                    for(int k = i-1; k <= i + 1; k++)
                        for(int l = j-1; l <= j + 1; l++)
                            if(k >= 0 && k < 8 && l >= 0 && l < 8)
                                if(board[l][k] == EnemyKing)
                                    skipped = true;
                    if(!skipped && ((board[j][i] == ' ') || (IsBlack && isupper(board[j][i])) || (!IsBlack && islower(board[j][i]))))
                    {
                        circle.setPosition(128*i+64, 128*j+64);
                        inp.moves.push_back(circle);
                        
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
                HelpMoveRBQ(inp, board, cells, cellsX, cellsY, j, toremove, IsBlack, circle, EnemyKing);
        }
    }
    if(type == 'N')
    {
        int x[8] = {inp.x-2, inp.x-1, inp.x+1, inp.x+2, inp.x+2, inp.x+1, inp.x-1, inp.x-2};
        int y[8] = {inp.y-1, inp.y-2, inp.y-2, inp.y-1, inp.y+1, inp.y+2, inp.y+2, inp.y+1};
        for(int i = 0; i < 8; i++)
        {
            if(x[i] >= 0 && x[i] < 8 && y[i] >= 0 && y[i] < 8)
            {
                if(board[y[i]][x[i]] == EnemyKing)
                    continue;
                if(board[y[i]][x[i]] == ' ')
                {
                    circle.setPosition(128*x[i]+64, 128*y[i]+64);
                    inp.moves.push_back(circle);
                }
                else if((IsBlack && isupper(board[y[i]][x[i]])) || (!IsBlack && islower(board[y[i]][x[i]])))
                {
                    circle.setPosition(128*x[i]+64, 128*y[i]+64);
                    inp.moves.push_back(circle);
                }
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
                HelpMoveRBQ(inp, board, cells, cellsX, cellsY, j, toremove, IsBlack, circle, EnemyKing);
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
                HelpMoveRBQ(inp, board, cells, cellsX, cellsY, j, toremove, IsBlack, circle, EnemyKing);
        }
    }
    if(type == 'P')
    {
        int cellY = (IsBlack ? (inp.y+1) : (inp.y-1));
        int cellX[3] = {inp.x, inp.x-1, inp.x+1};
        for(int i = 0; i < 3; i++)
        {
            if(board[cellY][cellX[i]] == EnemyKing)
            {
                continue;
            }
            if(board[cellY][cellX[i]] == ' ')
            {
                if(i == 0)
                {
                    circle.setPosition(128*cellX[i]+64, 128*cellY+64);
                    inp.moves.push_back(circle);
                }
            }
            else if(i != 0 && (IsBlack && isupper(board[cellY][cellX[i]]) || (!IsBlack && islower(board[cellY][cellX[i]]))))
            {
                circle.setPosition(128*cellX[i]+64, 128*cellY+64);
                inp.moves.push_back(circle);
            }
        }
    }
}