﻿// laba6.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <cctype>

#define HIGHLITED_COLOR sf::Color(255, 0, 0)

sf::Vector2i AttackerPos = {-1, -1};
bool GlobalCheck[2] = {false};

struct move
{
    short x = -1;
    short y = -1;
    sf::CircleShape circle;

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
};

struct figure
{
    short x = -1;
    short y = -1;
    char type = ' ';
    bool IsBlack;
    sf::Sprite sprite;
    std::vector<move> moves;

    void SetX(short NewX)
    {
        x = NewX;
    }
    
    void SetY(short NewY)
    {
        y = NewY;
    }

    void SetSpritePost(sf::Vector2f NewPos)
    {
        sprite.setPosition(NewPos);
    }
};

std::vector<figure> figures;

