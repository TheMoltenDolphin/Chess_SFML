#include "func.cpp"
#include <math.h>
#include <map>
#include <sstream>
//#define FPS_COUNT



int main()
{

    
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "MyChess");
    //window.setFramerateLimit(120);

    std::map<char, sf::Texture> dict;
    SetDict(dict);
    char board[8][8] = {
        {'r', 'n', 'b', 'q', ' ', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', 'K', ' '},
        {' ', ' ', ' ', ' ', 'k', ' ', ' ', 'R'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', 'n', ' ', ' ', 'N', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', ' ', 'B', 'N', ' '}
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
    figure& current = figure();
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
                for(int i = 0; i < figures.size(); i++)
                {
                    if(figures[i].sprite.getGlobalBounds().contains(MousePos.x, MousePos.y))
                    {
                        if(current.x != -1)
                        {
                            BoardSquares[current.x][current.y].setFillColor((current.x + current.y) % 2 == 0 ? sf::Color(235, 236, 208) : sf::Color(115, 149, 82));
                        }
                        BoardSquares[figures[i].x][figures[i].y].setFillColor(sf::Color(BoardSquares[figures[i].x][figures[i].y].getFillColor().r-60, BoardSquares[figures[i].x][figures[i].y].getFillColor().g-60, BoardSquares[figures[i].x][figures[i].y].getFillColor().b-60));
                        current = figures[i];
                        ShowMoves(current, board);
                        break;
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
            for(int i = 0; i < current.moves.size(); i++)
                window.draw(current.moves[i]);
            window.display();
        }
#ifdef FPS_COUNT
        std::stringstream TitleString;
        TitleString << "FPS: " << 1.f / DeltaTime << " ";
        window.setTitle(TitleString.str());
#endif
    }

    return 0;
}