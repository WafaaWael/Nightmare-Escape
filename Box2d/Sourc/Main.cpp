#include <SFML/Graphics.hpp>
#include "box2d.h"
#include <iostream>

const float SCALE = 30.0f;

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML + Box2D Planet Orbit");

    b2World world(b2Vec2(0.0f, 9.8));

    //time calculation
    sf::Clock clock;
    float deltaTime = 0;
    float time = 0;

    bool on_Ground = true;

    sf::RectangleShape blackScreen(sf::Vector2f(1920, 1080));
    blackScreen.setFillColor(sf::Color::Green);

    // Set initial transparency to fully opaque

    //bullets

    sf::CircleShape bullet(0.1f, 0.1f);
    bullet.setFillColor(sf::Color::White);
    sf::Vector2f bullet_Direction;
    float speed = 1;

    b2BodyDef playerDef;
    playerDef.type = b2_dynamicBody;
    playerDef.position.Set(0.0f, 100.0f);  // Initial position

    b2Body* playerBody = world.CreateBody(&playerDef);

    b2PolygonShape playerShape;
    playerShape.SetAsBox(1.0f, 1.0f);  // Assuming the player is a square of size 2x2

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &playerShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    playerBody->CreateFixture(&fixtureDef);



    b2BodyDef groundDef;
    groundDef.position.Set(0.0f, 800.0f); // Adjust the position based on your needs

    b2Body* groundBody = world.CreateBody(&groundDef);

    b2PolygonShape groundShape;
    groundShape.SetAsBox(1450.f, 40.0f); // Adjust the size based on your needs

    b2FixtureDef groundFixtureDef;
    groundFixtureDef.shape = &groundShape;

    groundBody->CreateFixture(&groundFixtureDef);




    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Handle events
        float currentTime = clock.getElapsedTime().asSeconds();
        deltaTime = currentTime - time;
        time = currentTime;

        // Step the Box2D world
        world.Step(1.0f / 60.0f, 8, 3);

        // Update player position
        b2Vec2 playerPosition = playerBody->GetPosition();

        //mouse pos

        sf::Vector2i mousepos = sf::Mouse::getPosition(window);
        bullet_Direction = (sf::Vector2f)mousepos - sf::Vector2f(playerPosition.x , playerPosition.y);

        // Clear window
        window.clear();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            playerBody->SetLinearVelocity(b2Vec2(-5.0f, playerBody->GetLinearVelocity().y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            playerBody->SetLinearVelocity(b2Vec2(5.0f, playerBody->GetLinearVelocity().y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            playerBody->SetLinearVelocity(b2Vec2(playerBody->GetLinearVelocity().x, -20.f));
           // on_Ground = false;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        }


        // Draw your player at the updated position
        sf::CircleShape playerShape(20.0f);  // Assuming the player is a circle with a radius of 20
        playerShape.setPosition(playerPosition.x, playerPosition.y);
        window.draw(playerShape);

        b2Vec2 groundPosition = groundBody->GetPosition();
        sf::RectangleShape groundShape(sf::Vector2f(1500.f, 2.0f)); // Assuming the ground is a rectangle
        groundShape.setPosition(groundPosition.x - 50.0f, groundPosition.y - 1.0f);
        window.draw(groundShape);
        window.draw(blackScreen);
        if (playerShape.getGlobalBounds().intersects(groundShape.getGlobalBounds())) {
            on_Ground = true;
        }

        if (blackScreen.getFillColor().a > 0) {
            blackScreen.setFillColor(sf::Color(0, 0, 0, blackScreen.getFillColor().a * deltaTime));
        }
        window.draw(bullet);
        // Display window
        window.display();
    }
}