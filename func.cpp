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

void ShowMoves(figure &inp, char board[8][8], bool NeedToChange = false)
{
    if(!NeedToChange && inp.moves.size() > 0)
        return;
    inp.moves.clear();
    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color(0, 0, 0, 75));
    circle.setOrigin(50.f, 50.f);
    bool IsBlack = ((inp.type == toupper(inp.type)) ? false : true);
    char type = toupper(inp.type);
    char enemy = (IsBlack ? 'K' : 'k');

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
                                if(board[l][k] == enemy)
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
            {
                if(cellsX[j] < 0 || cellsY[j] < 0 || cellsX[j] > 7 || cellsY[j] > 7 || toremove[j])
                    continue;
                if(board[cellsY[j]][cellsX[j]] == enemy)
                {
                    toremove[j] = true;
                    continue;
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
                    continue;
                }
                else
                {
                    toremove[j] = true;
                    continue;
                }
            }
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
                if(board[y[i]][x[i]] == enemy)
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
}