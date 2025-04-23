// laba6.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <cctype>

#define HIGHLITED_COLOR sf::Color(255, 0, 0)
#define CHESS_GREEN_COLOR sf::Color(235, 236, 208)
#define CHESS_WHITE_COLOR sf::Color(115, 149, 82)


sf::Vector2i AttackerPos = {-1, -1};
bool GlobalCheck = false;

class move
{
    private:

        short x;
        short y;
        sf::CircleShape circle;

    public:

        move() : x(-1), y(-1), circle(sf::CircleShape()) {}

        move(short NewX, short NewY, sf::CircleShape NewCircle)
        {
            x = NewX;
            y = NewY;
            circle = NewCircle;
        }

        sf::Vector2i GetPos()
        {
            return sf::Vector2i((int)x, (int)y);
        }

        short GetX()
        {
            return x;
        }

        short GetY()
        {
            return y;
        }

        void SetX(short i)
        {
            x = i;
        }

        void SetY(short i)
        {
            y = i;
        }

        sf::CircleShape& GetCircle()
        {
            return circle;
        }
};

class figure
{
    private:
        short x;
        short y;
        char type;
        bool IsBlack;
        sf::Sprite sprite;
        std::vector<move> moves;

    public:

        figure() : x(-1), y(-1), type(' '), IsBlack(false), sprite(sf::Sprite()), moves(std::vector<move>()) {}

        short GetX()
        {
            return x;
        }

        short GetY()
        {
            return y;
        }

        char GetType()
        {
            return type;
        }

        void SetType(char i)
        {
            type = i;
        }

        void SetX(short NewX)
        {
            x = NewX;
        }
        
        void SetY(short NewY)
        {
            y = NewY;
        }

        bool GetBlack()
        {
            return IsBlack;
        }

        void SetBlack(bool i)
        {
            IsBlack = i;
        }

        sf::Sprite& GetSprite()
        {
            return sprite;
        }

        std::vector<move>& GetMoves()
        {
            return moves;
        }

        void SetSpritePost(sf::Vector2f NewPos)
        {
            sprite.setPosition(NewPos);
        }
};

std::vector<figure> figures;

