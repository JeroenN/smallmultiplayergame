#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>
//#include <Packet.hpp>

using namespace std;

int main()
{
    bool server =false;
    bool client =false;
    sf::UdpSocket socket;
    std::string connectionType ="";
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";
    std::string clientCheckShort ="c";
    std::string clientCheck ="client";
    char buffer[2000];

    std::size_t received;
    std::map<unsigned short, sf::IpAddress> computerID;
    std::string text = "Connected to: ";

    std::cout <<"Enter (server) for server, Enter (client) for client: \n";
    std::cin >> connectionType;


    unsigned short port;

    std::cout << "set port number: ";
    std::cin >> port;

    socket.bind(port);

    if(connectionType==serverCheckShort || connectionType==serverCheck)
    {
            server=true;
            std::cout<<"looking for client... \n";
            std::cout<<"Please type (ready) if everyone is ready: \n";
            std::string answer="a";
            while(answer != "r" && answer !="ready")
            {
                sf::IpAddress rIp;
                unsigned short port;
                socket.receive(buffer,sizeof(buffer), received, rIp, port);
                if(received >0)
                {
                    std::cout<<"connected with client \n";
                    computerID[port] = rIp;
                }
                std::cin >> answer;
            }
    }
    if(connectionType==clientCheckShort || connectionType==clientCheck)
    {
        client=true;
        std::string sIP;
        std::cout << "Enter server ip: ";
        std::cin>> sIP;
        sf::IpAddress sendIP(sIP);
        socket.send(text.c_str(), text.length() +1, sendIP, 2000);
    }

    sf::RectangleShape rect1;
    sf::RectangleShape rect2;
    float posXplayer1=0;
    float posYplayer1=0;
    sf::Vector2f prevPosition;
    sf::Vector2f currentPosition;
    sf::Vector2f changingPosition;

    sf::Vector2f positionPlayer2;
    rect2.setSize(sf::Vector2f(20,20));
    rect2.setFillColor(sf::Color::Blue);
    rect1.setSize(sf::Vector2f(20,20));
    rect1.setFillColor(sf::Color::Red);
    sf::RenderWindow window(
       sf::VideoMode(500, 500),
       "multiplayer",
       sf::Style::Titlebar | sf::Style::Close);


    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);
    socket.setBlocking(false);
    bool  update =false;
    while(window.isOpen())
    {
        sf::Event Event;
        while(window.pollEvent(Event))
        {
           //if(Event.type == sf::Event::Closed || Event.key.code == sf::Keyboard::Escape)
              // window.close();
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
        }
        prevPosition = rect1.getPosition();
       // prevPositionY = rect1.getPosition().y;
        if(update)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                    posXplayer1+=3;
                    rect1.setPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                    posXplayer1-=3;
                    rect1.setPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                posYplayer1-=3;
                rect1.setPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                posYplayer1+=3;
                rect1.setPosition(posXplayer1, posYplayer1);
            }
        }

        sf::Packet posPacket;
        if(server==true)
        {
            if(prevPosition != rect1.getPosition())
            {
                sf::IpAddress recipient = "192.168.178.15";
                unsigned short serverPort = 2001;

                posPacket<<rect1.getPosition().x <<rect1.getPosition().y;
                if (socket.send(posPacket, recipient, serverPort) != sf::Socket::Done)
                {
                    //std::cout<<"whoops... some data wasn't sent";
                }

            }
        }
        if(client==true)
        {
            if(prevPosition != rect1.getPosition())
            {
                sf::IpAddress recipient = "192.168.178.15";
                unsigned short serverPort = 2000;

                posPacket<<rect1.getPosition().x <<rect1.getPosition().y;
                if (socket.send(posPacket, recipient, serverPort) != sf::Socket::Done)
                {
                    //std::cout<<"whoops... some data wasn't sent";
                }

            }
        }
        sf::IpAddress sender;
        unsigned short port;
        if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
        {
            //std::cout<<"whoops... some data wasn't received";
        }
        if(posPacket>>changingPosition.x>>changingPosition.y)
        rect2.setPosition(changingPosition);

        window.clear();
        window.draw(rect1);
        window.draw(rect2);
        window.display();
    }
    return 0;
}
