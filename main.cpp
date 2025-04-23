#include "func.cpp"
#include <math.h>
#include <map>
#include <sstream>
//#define FPS_COUNT



int main()
{
    setlocale(LC_ALL, "Russian");
    sf::RenderWindow MainWindow;
    sf::RenderWindow Selector;

    MainWindow.create(sf::VideoMode(1024, 1024), "MyChess");
    MainWindow.setVerticalSyncEnabled(true);

    std::stringstream TitleString;
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

    for(short i = 0; i < figures.size(); i++)
        ShowMoves(figures[i], moves, board, turn);
    sf::RectangleShape BoardSquares[8][8];
    for(short i = 0; i < 8; i++)
    {
        for(short j = 0; j < 8; j++)
        {
            BoardSquares[i][j].setSize({128.f, 128.f});
            BoardSquares[i][j].setOrigin(64.f, 64.f);
            BoardSquares[i][j].setFillColor((i+j) % 2 == 0 ? CHESS_GREEN_COLOR : CHESS_WHITE_COLOR);
            BoardSquares[i][j].setPosition(128*i + 64, 128*j + 64);
        }
    }

    sf::Event event;
    sf::Vector2i MousePos;
    short current = -1;
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
                if(event.key.code == sf::Keyboard::Enter)
                {
                    for(short i = 0; i < 8; i++)
                    {
                        for(short j = 0; j < 8; j++)
                            std::cout << moves[i][j] << ' ';
                        std::cout << std::endl;
                    }
                    std::cout << GlobalCheck << std::endl;
                    std::cout << "AttackerPos: " << AttackerPos.x << ' ' << AttackerPos.y << std::endl;
                    for(short i = 0; i < 8; i++)
                    {
                        for(short j = 0; j < 8; j++)
                        {
                            std::cout << board[i][j] << ' ';
                        }
                        std::cout << std::endl;
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
                        for(short i = 0; i < figures[current].GetMoves().size(); i++)
                        {
                            sf::FloatRect col(figures[current].GetMoves()[i].GetX()*128, figures[current].GetMoves()[i].GetY()*128, 128.f, 128.f);
                            if(col.contains(MousePos.x, MousePos.y) && !doBreak && (turn == (figures[current].GetBlack() ? 1 : 0)))
                            {
                                for(short j = 0; j < figures.size(); j++)
                                    if(figures[current].GetMoves()[i].GetX() == figures[j].GetX() && figures[current].GetMoves()[i].GetY() == figures[j].GetY())
                                    {
                                        std::cout << "Срубил!" << std::endl;
                                        MoveFigure(figures[current], i, board, BoardSquares);
                                        figures[j].GetSprite().setPosition(INT_MAX, INT_MAX);
                                        figures[j].SetX(-1);
                                        figures[j].SetY(-1);
                                        doBreak = true;
                                        break;
                                    }
                                if(!doBreak)
                                {
                                    MoveFigure(figures[current], i, board, BoardSquares);
                                    doBreak = true;
                                }
                                if((figures[current].GetType() == 'P' && figures[current].GetY() == 0) || (figures[current].GetType() == 'p' && figures[current].GetY() == 7))
                                {
                                    Selector.create(sf::VideoMode(512, 128), "Select a figure!");
                                    sf::Sprite ToSelect[4] {sf::Sprite()};
                                    char figuresNames[4] = {'R', 'B', 'N', 'Q'};
                                    if(figures[current].GetBlack())
                                        for(short i = 0; i < 4; i++)
                                            figuresNames[i] = tolower(figuresNames[i]);
                                    FiguresToSelect(ToSelect, figures[current], dict, figuresNames);
                                    std::cout << "SelectWindow created!" << std::endl;
                                    sf::Vector2i NewMousePos;
                                    while(Selector.isOpen())
                                    {
                                        NewMousePos = sf::Mouse::getPosition(Selector);
                                        while(Selector.pollEvent(event))
                                        {
                                            if(event.type == sf::Event::Closed)
                                            {
                                                figures[current].GetSprite().setTexture(dict[figuresNames[3]]);
                                                figures[current].SetType(figuresNames[3]);
                                                Selector.close();
                                            }
                                            if(event.type == sf::Event::MouseButtonPressed)
                                                for(short i = 0; i < 4; i++)
                                                    if(ToSelect[i].getGlobalBounds().contains(NewMousePos.x, NewMousePos.y))
                                                    {
                                                        figures[current].GetSprite().setTexture(dict[figuresNames[i]]);
                                                        figures[current].SetType(figuresNames[i]);
                                                        Selector.close();
                                                    }
                                        }
                                        Selector.clear(sf::Color::White);
                                        for(short i = 0; i < 4; i++)
                                            Selector.draw(ToSelect[i]);
                                        Selector.display();
                                    }
                                }
                                for(short i = 0; i < 8; i++)
                                    for(short j = 0; j < 8; j++)
                                        moves[i][j] = 0;
                                for(short i = 0; i < figures.size(); i++)
                                    ShowMoves(figures[i], moves, board, turn);
                                current = -1; 
                                turn = (turn == 0 ? 1 : 0);
                                break;
                            }
                    }
                }
                if(!doBreak)
                    for(short i = 0; i < figures.size(); i++)
                    {
                        if(figures[i].GetSprite().getGlobalBounds().contains(MousePos.x, MousePos.y) && (turn == (figures[i].GetBlack() ? 1 : 0)))
                        {
                            if(current != -1)
                            {
                                BoardSquares[figures[current].GetX()][figures[current].GetY()].setFillColor((figures[current].GetX() + figures[current].GetY()) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                            }
                            BoardSquares[figures[i].GetX()][figures[i].GetY()].setFillColor(sf::Color(BoardSquares[figures[i].GetX()][figures[i].GetY()].getFillColor().r-60, BoardSquares[figures[i].GetX()][figures[i].GetY()].getFillColor().g-60, BoardSquares[figures[i].GetX()][figures[i].GetY()].getFillColor().b-60));
                            current = i;
                            ShowMoves(figures[i], moves, board, turn, true);
                            break;
                        }
                    }
            }
        }

        {
            MainWindow.clear();
            for(short i = 0; i < 8; i++)
                for(short j = 0; j < 8; j++)
                    MainWindow.draw(BoardSquares[i][j]);
            for(short i = 0; i < figures.size(); i++)
                if(figures[i].GetX() != -1)
                    MainWindow.draw(figures[i].GetSprite());
            if(current != -1)
                for(short i = 0; i < figures[current].GetMoves().size(); i++)
                    MainWindow.draw(figures[current].GetMoves()[i].GetCircle());
            MainWindow.display();
        }
#ifdef FPS_COUNT
        TitleString << "FPS: " << 1.f / DeltaTime << " ";
        MainWindow.setTitle(TitleString.str());
#endif
    }
    return 0;
}