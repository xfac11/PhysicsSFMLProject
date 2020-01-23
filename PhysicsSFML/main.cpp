#include"SFML/Graphics.hpp"
#include<iostream>
#include<vector>

struct Body
{
    sf::Shape* shape = nullptr;
    float mass = 1.0f;
    bool dynamic = true;
    sf::Vector2f acceleration = sf::Vector2f(0.0f, 0.0f);
    sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
};
std::string vecToString(const sf::Vector2f& vec)
{
    std::string result;

    result = "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";

    return result;
};
bool CirclevsCircleOptimized(const sf::CircleShape& a, const sf::CircleShape& b)
{
    float r = a.getRadius() + b.getRadius();
    r *= r;
    sf::Vector2f aPos = a.getPosition();
    sf::Vector2f bPos = b.getPosition();

    float ax_bx = aPos.x + bPos.x;
    float ay_by = aPos.y + bPos.y;

    float distance = (ax_bx * ax_bx) + (ay_by * ay_by);
    return r > distance;
}
std::vector<Body> world;
void collisionCheck()
{
    int nrOfBodys = world.size();
    for (int i = 0; i < nrOfBodys; i++)
    {
        sf::CircleShape* body1 = dynamic_cast<sf::CircleShape*>(world.at(i).shape);
        if (body1 == nullptr)
        {
            continue;
        }
        for (int j = 0; j < nrOfBodys; j++)
        {
            sf::CircleShape* body2 = dynamic_cast<sf::CircleShape*>(world.at(j).shape);
            if (body1 == body2 || body2 == nullptr || body1 == nullptr)
            {
                continue;
            }
            if(CirclevsCircleOptimized(*body1, *body2))
            {
                sf::Vector2f normal = body1->getPosition() - body2->getPosition();
                normal *= 1.0f;
                world[j].velocity -= normal / world[j].mass;
                world[i].velocity += normal / world[i].mass;
                std::cout << "collision";
            }
        }
    }
};
void addParticle(sf::Vector2f startPos, float radius, float mass, sf::Vector2f startVelocity = sf::Vector2f(0, 0), sf::Color color = sf::Color::White)
{
    Body newBody;
    newBody.acceleration = sf::Vector2f(0, 9.82);
    newBody.dynamic = true;
    newBody.mass = mass;
    newBody.shape = new sf::CircleShape(radius);
    newBody.velocity = startVelocity;
    newBody.shape->setPosition(startPos);
    newBody.shape->setFillColor(color);
    
    world.push_back(newBody);
};
void addRect(sf::Vector2f pos, sf::Vector2f size, sf::Color color, float mass, bool dynamic)
{
    Body newBody;
    newBody.acceleration = sf::Vector2f(0, 9.82);
    newBody.dynamic = dynamic;
    newBody.mass = mass;
    newBody.shape = new sf::RectangleShape(size);
    newBody.shape->setPosition(pos);
    newBody.shape->setFillColor(color);

    world.push_back(newBody);
};
int main()
{
    sf::RenderWindow window(sf::VideoMode(1080/2, 720/2), "SFML works!");

    for (int i = 0; i < 5; i++)
    {
        if (i == 0)
        {
            addParticle(sf::Vector2f(i * 25, 5.0f), 10.0f, 1.0f,sf::Vector2f(0.01f,0.01f));
        }
        addParticle(sf::Vector2f(i * 25, 5.0f), 10.0f, 1.0f);
    }
    addRect(sf::Vector2f(0, 100.0f), sf::Vector2f(100.0f, 5.0f), sf::Color::Green, 10.0f, false);
    sf::Clock deltaClock;
    sf::Time dt = deltaClock.restart();
    
    while (window.isOpen())
    {
        //update
        float deltaTime = dt.asSeconds();
        deltaTime = 1.0f / 100.0f;
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            deltaTime/= 3;
        }
        collisionCheck();
        for (auto body : world)
        {
            if (body.dynamic == false)
            {
                continue;
            }
            body.acceleration = sf::Vector2f(0.0f, 9.82f);
            body.velocity += body.acceleration * deltaTime;
            body.shape->move(body.velocity * deltaTime);
        }
        //draw
        window.clear();
        for (auto shape : world)
        {
            window.draw(*shape.shape);
        }
        window.display();
        dt = deltaClock.restart();
        //std::cout << vecToString(testBody.velocity) << std::endl;
    }
    for (auto shape : world)
    {
        delete shape.shape;
    }

    return 0;
}