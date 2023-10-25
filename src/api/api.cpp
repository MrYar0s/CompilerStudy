#include <SFML/Graphics.hpp>
#include <algorithm>
#include <random>

#include "api.h"

#define PIXEL_SIZE 2

static sf::RenderWindow GlobalWindow {};
static sf::Image GlobalBuffer {};

std::random_device rd;
std::mt19937 gen(rd());

void display()
{
    sf::Texture texture;
    texture.loadFromImage(GlobalBuffer);
    sf::Sprite sprite;
    sprite.setTexture(texture, true);
    GlobalWindow.clear();
    GlobalWindow.draw(sprite);
    GlobalWindow.display();
}

int generate()
{
    return gen();
}

void init(unsigned int width, unsigned int height)
{
    GlobalWindow.create(sf::VideoMode(width * PIXEL_SIZE, height * PIXEL_SIZE), "Game Of Life!");
    GlobalBuffer.create(width * PIXEL_SIZE, height * PIXEL_SIZE, sf::Color());
}

void setPixel(unsigned int x, unsigned int y, int is_alive)
{
    auto color = is_alive == 1 ? sf::Color::Green : sf::Color::Black;
    GlobalBuffer.setPixel(x * PIXEL_SIZE, y * PIXEL_SIZE, color);
}
