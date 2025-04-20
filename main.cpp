#include "func.cpp"
#include <math.h>
#include <map>
#include <sstream>
//#define FPS_COUNT



int main()
{
    setlocale(LC_ALL, "Russian");
    sf::RenderWindow MainWindow;
    MainWindow.create(sf::VideoMode(1024, 1024), "MyChess");

    sf::RenderWindow Selector;
    std::stringstream TitleString;
    MainWindow.setVerticalSyncEnabled(true);
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
        ShowMoves(figures[i], moves, board, turn);
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
    sf::Sprite* figuresToSelect;
    short current = -1;
    bool ShowChangeWindow = false;
#ifdef FPS_COUNT
    sf::Clock clock;
#endif

    while(MainWindow.isOpen())
    {
#ifdef FPS_COUNT
        float DeltaTime = clock.restart().asSeconds();
#endif                
        MousePos = sf::Mouse::getPosition(MainWindow);
        while (MainWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                MainWindow.close();
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Enter)
                {
                    for(int i = 0; i < 8; i++)
                    {
                        for(int j = 0; j < 8; j++)
                        {
                            std::cout << moves[i][j] << ' ';
                        }
                        std::cout << std::endl;
                    }
                    std::cout << GlobalCheck[0] << " " << GlobalCheck[1] << std::endl;
                    std::cout << "AttackerPos: " << AttackerPos.x << ' ' << AttackerPos.y << std::endl;
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
                if(Selector.isOpen())
                    continue;
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
                                        figures[j].sprite.setPosition(10000, 10000);
                                        figures[j].SetX(-1);
                                        figures[j].SetY(-1);
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
                                if((figures[current].type == 'P' && figures[current].y == 0) || (figures[current].type == 'p' && figures[current].y == 7))
                                {
                                    sf::RenderWindow Selector(sf::VideoMode(512, 128), "Select a figure!");
                                    sf::Sprite ToSelect[4] {sf::Sprite()};
                                    char figuresNames[4] = {'R', 'B', 'N', 'Q'};
                                    if(figures[current].IsBlack)
                                        for(int i = 0; i < 4; i++)
                                            figuresNames[i] = tolower(figuresNames[i]);
                                    FiguresToSelect(ToSelect, figures[current], dict, figuresNames);
                                    std::cout << "SelectWindow created!" << std::endl;
                                    sf::Vector2i NewMousePos;
                                    while(Selector.isOpen())
                                    {
                                        NewMousePos = sf::Mouse::getPosition(Selector);
                                        std::cout << NewMousePos.x << " " << NewMousePos.y << std::endl;
                                        while(Selector.pollEvent(event))
                                        {
                                            if(event.type == sf::Event::Closed)
                                            {
                                                figures[current].sprite.setTexture(dict[figuresNames[3]]);
                                                figures[current].type = figuresNames[3];
                                                Selector.close();
                                            }
                                            if(event.type == sf::Event::MouseButtonPressed)
                                            {
                                                std::cout << "!@32131";
                                                for(int i = 0; i < 4; i++)
                                                {
                                                    if(ToSelect[i].getGlobalBounds().contains(NewMousePos.x, NewMousePos.y))
                                                    {
                                                        std::cout << "123213";
                                                        figures[current].sprite.setTexture(dict[figuresNames[i]]);
                                                        figures[current].type = figuresNames[i];
                                                        Selector.close();
                                                    }
                                                }
                                            }
                                        }
                                        Selector.clear(sf::Color::White);
                                        for(int i = 0; i < 4; i++)
                                            Selector.draw(ToSelect[i]);
                                        Selector.display();
                                    }
                                }
                                for(short i = 0; i < 8; i++)
                                    for(short j = 0; j < 8; j++)
                                        moves[i][j] = 0;
                                for(int i = 0; i < figures.size(); i++)
                                    ShowMoves(figures[i], moves, board, turn);
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
                            ShowMoves(figures[i], moves, board, turn, true);
                            break;
                        }
                    }
                }
            }
        }

        {
            MainWindow.clear();
            
            for(int i = 0; i < 8; i++)
                for(int j = 0; j < 8; j++)
                    MainWindow.draw(BoardSquares[i][j]);
            for(int i = 0; i < figures.size(); i++)
                if(figures[i].x != -1)
                    MainWindow.draw(figures[i].sprite);
            if(current != -1)
                for(int i = 0; i < figures[current].moves.size(); i++)
                    MainWindow.draw(figures[current].moves[i].circle);
            MainWindow.display();
        }
#ifdef FPS_COUNT
        TitleString << "FPS: " << 1.f / DeltaTime << " ";
        MainWindow.setTitle(TitleString.str());
#endif
    }

    return 0;

}