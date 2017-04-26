#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>
using namespace std;

int main()
{
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::TcpSocket socket;
    char connectionType;

    std::cout <<"Enter (server) for server, Enter (client) for client: \n";
    std::cin >> connectionType;

    if(connectionType=='s')
    {
        std::cout <<"You entered server \n";
        sf::TcpListener listener;
        listener.listen(2000);
        listener.accept(socket);
    }
    else
    {
         std::cout <<"You entered client \n";
        socket.connect(ip, 2000);
    }
    sf::RectangleShape rect1;
    sf::RectangleShape rect2;
    float posX=100;
    float posY=100;
    sf::Vector2f prevPosition;
    sf::Vector2f player2Position;
    rect2.setSize(sf::Vector2f(20,20));
    rect2.setFillColor(sf::Color::Blue);
    rect1.setSize(sf::Vector2f(20,20));
    rect1.setFillColor(sf::Color::Red);

    sf::RenderWindow window(
       sf::VideoMode(500, 500),
       "mt",
       sf::Style::Titlebar | sf::Style::Close);


    window.setFramerateLimit(70);
    window.setKeyRepeatEnabled(true);
    socket.setBlocking(false);
    bool  update =false;
    while(window.isOpen())
    {
        sf::Event Event;
        while(window.pollEvent(Event))
        {
           if(Event.type == sf::Event::Closed || Event.key.code == sf::Keyboard::Escape)
               window.close();
           else if(Event.type == sf::Event::GainedFocus)
                update=true;
           else if(Event.type == sf::Event::LostFocus)
               update = false;
        }
        prevPosition = rect1.getPosition();
        if(update)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                    posX+=3;
                    rect1.setPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                posY-=3;
                rect1.setPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                posY+=3;
                rect1.setPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                    posX-=3;
                    rect1.setPosition(posX, posY);
            }
        }
        sf::Packet posPacket;
        if(prevPosition != rect1.getPosition())
        {
            posPacket << rect1.getPosition().x << rect1.getPosition().y;
            socket.send(posPacket);
        }

        socket.receive(posPacket);
        if(posPacket >> player2Position.x >> player2Position.y)
        {
            rect2.setPosition(player2Position.x, player2Position.y);
        }

        window.clear();
        window.draw(rect1);
        window.draw(rect2);
        window.display();
    }
    return 0;
}
