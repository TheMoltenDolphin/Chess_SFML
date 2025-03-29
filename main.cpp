#include "func.cpp"
#include <math.h>
#include <map>
#include <sstream>
//#define FPS_COUNT



int main()
{

    setlocale(LC_ALL, "Russian");
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "MyChess");
    std::stringstream TitleString;
    window.setFramerateLimit(500);

    std::map<char, sf::Texture> dict;
    SetDict(dict);
    char board[8][8] = {
        {'r', 'n', 'b', 'q', ' ', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', 'B', 'K', ' '},
        {' ', ' ', 'B', ' ', 'k', ' ', ' ', 'R'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', 'B', 'n', ' ', ' ', 'N', 'B', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', ' ', ' ', 'N', ' '}
    };

    std::vector<figure> figures; 
    SetBoard(board, figures, dict);

    sf::RectangleShape BoardSquares[8][8];
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            BoardSquares[i][j].setSize({128.f, 128.f});
            BoardSquares[i][j].setOrigin(64.f, 64.f);
            BoardSquares[i][j].setFillColor((i+j) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
            BoardSquares[i][j].setPosition(128*i + 64, 128*j + 64);
        }
    }

    sf::Event event;
    sf::Vector2i MousePos;
    short current = -1;
    short turn = 0;
#ifdef FPS_COUNT
    sf::Clock clock;
#endif
    while(window.isOpen())
    {
#ifdef FPS_COUNT
        float DeltaTime = clock.restart().asSeconds();
#endif                
        MousePos = sf::Mouse::getPosition(window);
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseButtonPressed)
            {
                bool doBreak = false;
                if(current != -1)
                {
                    std::cout << figures[current].sprite.getPosition().x << figures[current].sprite.getPosition().y;
                    if(!doBreak)
                    {
                        for(int i = 0; i < figures[current].moves.size(); i++)
                        {
                            if(figures[current].moves[i].getGlobalBounds().contains(MousePos.x, MousePos.y) && !doBreak && (turn == (figures[current].type == tolower(figures[current].type) ? 1 : 0)))
                            {
                                for(int j = 0; j < figures.size(); j++)
                                    if(figures[current].moves[i].getPosition() == figures[j].sprite.getPosition())
                                    {
                                        std::cout << "Срубил!";
                                        figures[current].SetSpritePost(figures[j].sprite.getPosition());
                                        board[figures[current].y][figures[current].x] = ' ';
                                        BoardSquares[figures[current].x][figures[current].y].setFillColor((figures[current].x + figures[current].y) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                                        figures[current].SetX(figures[j].x);
                                        figures[current].SetY(figures[j].y);
                                        board[figures[current].y][figures[current].x] = figures[current].type;
                                        figures[j].sprite.move(10000, 10000);
                                        ShowMoves(figures[current], board, true);
                                        doBreak = true;
                                        break;
                                    }
                                if(!doBreak)
                                {
                                    figures[current].SetSpritePost(figures[current].moves[i].getPosition());
                                    board[figures[current].y][figures[current].x] = ' ';
                                    BoardSquares[figures[current].x][figures[current].y].setFillColor((figures[current].x + figures[current].y) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                                    figures[current].SetX(figures[current].moves[i].getPosition().x / 128);
                                    figures[current].SetY(figures[current].moves[i].getPosition().y / 128);
                                    board[figures[current].y][figures[current].x] = figures[current].type;
                                    ShowMoves(figures[current], board, true);
                                    doBreak = true;
                                }
                                current = -1; 
                                turn = (turn == 0 ? 1 : 0);
                                break;
                            }
                        }
                    }
                }
                if(!doBreak)
                {
                    for(int i = 0; i < figures.size(); i++)
                    {
                        if(figures[i].sprite.getGlobalBounds().contains(MousePos.x, MousePos.y) && (turn == (figures[i].type == tolower(figures[i].type) ? 1 : 0)))
                        {
                            std::cout << "figures[current] changed" << std::endl;
                            if(current != -1)
                            {
                                BoardSquares[figures[current].x][figures[current].y].setFillColor((figures[current].x + figures[current].y) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                            }
                            BoardSquares[figures[i].x][figures[i].y].setFillColor(sf::Color(BoardSquares[figures[i].x][figures[i].y].getFillColor().r-60, BoardSquares[figures[i].x][figures[i].y].getFillColor().g-60, BoardSquares[figures[i].x][figures[i].y].getFillColor().b-60));
                            current = i;
                            ShowMoves(figures[current], board, true);
                            break;
                        }
                    }
                }
            }
        }

        {
            window.clear();
            for(int i = 0; i < 8; i++)
                for(int j = 0; j < 8; j++)
                    window.draw(BoardSquares[i][j]);
            for(int i = 0; i < figures.size(); i++)
                window.draw(figures[i].sprite);
            if(current != -1)
                for(int i = 0; i < figures[current].moves.size(); i++)
                    window.draw(figures[current].moves[i]);
            window.display();
        }
#ifdef FPS_COUNT
        TitleString << "FPS: " << 1.f / DeltaTime << " ";
        window.setTitle(TitleString.str());
#endif
    }

    return 0;

}