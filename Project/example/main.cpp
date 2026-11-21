
// Author: Pierce Brooks

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

class Player : public sf::CircleShape
{
public:
    Player(float radius) :
        sf::CircleShape()
    {
        setRadius(radius);
    }
};

class Enemy : public sf::RectangleShape
{
public:
    Enemy(const sf::Vector2f& bounds) :
        sf::RectangleShape()
    {
        setSize(bounds);
        setOrigin(bounds*0.5f);
    }
};

class Bullet : public sf::CircleShape
{
private:
    sf::Vector2f velocity;
public:
    Bullet(float radius, const sf::Vector2f& velocity) :
        sf::CircleShape(),
        velocity(velocity)
    {
        setRadius(radius);
        setOrigin(sf::Vector2f(radius*0.5f, radius*0.5f));
    }
    const sf::Vector2f& getVelocity() const
    {
        return velocity;
    }
};

int main()
{
    int score = 0;
    int step = 0;
    int stride = 600;
    std::vector<Enemy*> enemies;
    std::vector<Bullet*> bullets;
    Player* player = new Player(25.0f);
    sf::RenderWindow* window = new sf::RenderWindow();
    window->create(sf::VideoMode(sf::Vector2u(1280, 720)), "Game");
    window->setFramerateLimit(60);
    player->setPosition(sf::Vector2f(window->getSize())*0.5f);
    player->setFillColor(sf::Color::Green);
    while (window->isOpen())
    {
        while (auto event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window->close();
                break;
            }
        }
        if (!window->isOpen())
        {
            break;
        }
        window->clear(sf::Color::Black);
        for (int i = 0; i != enemies.size(); ++i)
        {
            sf::Vector2f velocity = player->getPosition()-enemies[i]->getPosition();
            velocity *= 5.0f/sqrtf((velocity.x*velocity.x)+(velocity.y*velocity.y));
            enemies[i]->move(velocity);
            if (enemies[i]->getGlobalBounds().findIntersection(player->getGlobalBounds()))
            {
                window->close();
                break;
            }
            window->draw(*enemies[i]);
            for (int j = 0; j != bullets.size(); ++j)
            {
                if (bullets[j]->getGlobalBounds().findIntersection(enemies[i]->getGlobalBounds()))
                {
                    ++score;
                    std::cout << score << std::endl;
                    delete bullets[j];
                    delete enemies[i];
                    bullets.erase(bullets.begin()+j);
                    enemies.erase(enemies.begin()+i);
                    --i;
                    break;
                }
            }
        }
        if (!window->isOpen())
        {
            break;
        }
        for (int i = 0; i != bullets.size(); ++i)
        {
            bullets[i]->move(bullets[i]->getVelocity());
            window->draw(*bullets[i]);
            if (!sf::FloatRect(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(window->getSize().x, window->getSize().y)).contains(bullets[i]->getPosition()))
            {
                delete bullets[i];
                bullets.erase(bullets.begin()+i);
                --i;
            }
        }
        window->draw(*player);
        window->display();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            player->move(sf::Vector2f(-player->getRadius()*0.5f, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            player->move(sf::Vector2f(0.0f, -player->getRadius()*0.5f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            player->move(sf::Vector2f(player->getRadius()*0.5f, 0.0f));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            player->move(sf::Vector2f(0.0f, player->getRadius()*0.5f));
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            if (step%5 == 0)
            {
                sf::Vector2f velocity = sf::Vector2f(sf::Mouse::getPosition(*window))-player->getPosition();
                velocity *= player->getRadius()/sqrtf((velocity.x*velocity.x)+(velocity.y*velocity.y));
                bullets.push_back(new Bullet(5.0f, velocity));
                bullets.back()->setPosition(player->getPosition());
                bullets.back()->move(sf::Vector2f(player->getRadius()*0.5f, player->getRadius()*0.5f));
                bullets.back()->setFillColor(sf::Color::Cyan);
            }
        }
        if (step%30 == 0)
        {
            float position = (((float)(step%stride))/((float)stride))*(22.0f/7.0f)*2.0f;
            if (step%60 == 0)
            {
                position = -position;
            }
            enemies.push_back(new Enemy(sf::Vector2f(1.0f, 1.0f)*50.0f));
            enemies.back()->setPosition(sf::Vector2f(cosf(position)*((float)window->getSize().x), sinf(position)*((float)window->getSize().y)));
            enemies.back()->move(sf::Vector2f(window->getSize())*0.5f);
            enemies.back()->setFillColor(sf::Color::Red);
        }
        if (step%stride == 0)
        {
            step = 0;
        }
        ++step;
    }
    delete window;
    for (int i = 0; i != bullets.size(); ++i)
    {
        delete bullets[i];
    }
    bullets.clear();
    for (int i = 0; i != enemies.size(); ++i)
    {
        delete enemies[i];
    }
    enemies.clear();
    delete player;
    return 0;
}
