#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>

#include "api.h"

#define PIXEL_SIZE 2

static sf::RenderWindow GlobalWindow {};

std::random_device rd;
std::mt19937 gen(rd());

void display()
{
    GlobalWindow.display();
}

int generate()
{
    return gen();
}

void init(unsigned int width, unsigned int height)
{
    GlobalWindow.create(sf::VideoMode(width * PIXEL_SIZE, height * PIXEL_SIZE), "Game Of Life!");
}

void flush()
{
    GlobalWindow.clear();
}

void setPixel(unsigned int x, unsigned int y, int is_alive)
{
    sf::RectangleShape rectangle(sf::Vector2f(PIXEL_SIZE, PIXEL_SIZE));
    rectangle.setPosition(x * PIXEL_SIZE, y * PIXEL_SIZE);
    auto color = is_alive == 1 ? sf::Color::White : sf::Color::Black;
    rectangle.setFillColor(color);
    GlobalWindow.draw(rectangle);
}
