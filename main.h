// laba6.h : включаемый файл для стандартных системных включаемых файлов
// или включаемые файлы для конкретного проекта.

#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <cctype>

#define HIGHLITED_COLOR sf::Color(255, 0, 0)



struct figure
{
    short x = -1;
    short y = -1;
    char type = ' ';
    sf::Sprite sprite;
    std::vector<sf::CircleShape> moves;

};
