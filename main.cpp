#include "func.cpp"
#include <math.h>
#include <map>
#include <sstream>
#define FPS_COUNT



int main()
{
    setlocale(LC_ALL, "Russian");
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "MyChess");
    std::stringstream TitleString;
    window.setFramerateLimit(500);

    std::map<char, sf::Texture> dict;
    SetDict(dict);
    char board[8][8] = 
    {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    int moves[8][8] = 
    {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    };

    SetBoard(board, figures, dict);

    short turn = 0;

    for(int i = 0; i < figures.size(); i++)
        ShowMoves(figures[i], moves, board, true, turn);
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
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << std::endl;
                    for(int i = 0; i < 8; i++)
                    {
                        for(int j = 0; j < 8; j++)
                        {
                            std::cout << moves[i][j] << ' ';
                        }
                        std::cout << std::endl;
                    }
                }
                if(event.key.code == sf::Keyboard::Q)
                {
                    std::cout << std::endl << GlobalCheck[0] << " " << GlobalCheck[1] << std::endl;
                }
                if(event.key.code == sf::Keyboard::W)
                {
                    std::cout << std::endl << "AttackerPos: " << AttackerPos.x << ' ' << AttackerPos.y << std::endl;
                }
                if(event.key.code == sf::Keyboard::Space)
                { 
                    std::cout << std::endl; 
                    for(int i = 0; i < 8; i++)
                    {
                        for(int j = 0; j < 8; j++)
                        {
                            std::cout << board[i][j] << ' ';
                        }
                        std::cout << std::endl;
                    }
                }
            }
            if(event.type == sf::Event::MouseButtonPressed)
            {
                bool doBreak = false;
                if(current != -1)
                {
                    if(!doBreak)
                    {
                        for(int i = 0; i < figures[current].moves.size(); i++)
                        {
                            sf::FloatRect col(figures[current].moves[i].x*128, figures[current].moves[i].y*128, 128.f, 128.f);
                            if(col.contains(MousePos.x, MousePos.y) && !doBreak && (turn == (figures[current].IsBlack ? 1 : 0)))
                            {
                                for(int j = 0; j < figures.size(); j++)
                                    if(figures[current].moves[i].x == figures[j].x && figures[current].moves[i].y == figures[j].y)
                                    {
                                        std::cout << "Срубил!" << std::endl;
                                        AttackerPos = {-1, -1};
                                        figures[current].SetSpritePost(figures[j].sprite.getPosition());
                                        board[figures[current].y][figures[current].x] = ' ';
                                        BoardSquares[figures[current].x][figures[current].y].setFillColor((figures[current].x + figures[current].y) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                                        figures[current].SetX(figures[j].x);
                                        figures[current].SetY(figures[j].y);
                                        board[figures[current].y][figures[current].x] = figures[current].type;
                                        figures[j].sprite.move(10000, 10000);
                                        figures[j].SetX(-1);
                                        figures[j].SetX(-1);
                                        doBreak = true;
                                        break;
                                    }
                                if(!doBreak)
                                {
                                    AttackerPos = {-1, -1};
                                    figures[current].SetSpritePost(figures[current].moves[i].circle.getPosition());
                                    board[figures[current].y][figures[current].x] = ' ';
                                    BoardSquares[figures[current].x][figures[current].y].setFillColor((figures[current].x + figures[current].y) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                                    figures[current].SetX(figures[current].moves[i].x);
                                    figures[current].SetY(figures[current].moves[i].y);
                                    board[figures[current].y][figures[current].x] = figures[current].type;
                                    doBreak = true;
                                }
                                for(short i = 0; i < 8; i++)
                                    for(short j = 0; j < 8; j++)
                                        moves[i][j] = 0;
                                for(int i = 0; i < figures.size(); i++)
                                    ShowMoves(figures[i], moves, board, true, turn);
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
                        if(figures[i].sprite.getGlobalBounds().contains(MousePos.x, MousePos.y) && (turn == (figures[i].IsBlack ? 1 : 0)))
                        {
                            if(current != -1)
                            {
                                BoardSquares[figures[current].x][figures[current].y].setFillColor((figures[current].x + figures[current].y) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                            }
                            BoardSquares[figures[i].x][figures[i].y].setFillColor(sf::Color(BoardSquares[figures[i].x][figures[i].y].getFillColor().r-60, BoardSquares[figures[i].x][figures[i].y].getFillColor().g-60, BoardSquares[figures[i].x][figures[i].y].getFillColor().b-60));
                            current = i;
                            ShowMoves(figures[i], moves, board, false, turn, true);
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
                    window.draw(figures[current].moves[i].circle);
            window.display();
        }
#ifdef FPS_COUNT
        TitleString << "FPS: " << 1.f / DeltaTime << " ";
        window.setTitle(TitleString.str());
#endif
    }

    return 0;

}