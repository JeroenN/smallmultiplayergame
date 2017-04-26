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
            std::cout<<"Please type (ready) if everyone is ready: \n";
            std::string answer="a";
            while(answer != "r" && answer !="ready")
            {
                sf::IpAddress rIp;
                unsigned short port;
                socket.receive(buffer,sizeof(buffer), received, rIp, port);
                if(received >0)
                {
                    std::cout<<rIp<<" connected \n";
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
    float posXplayer2=0;
    float posYplayer2=0;
    float prevPositionX;
    float prevPositionY;
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
        prevPositionX = rect1.getPosition().x;
        prevPositionY = rect1.getPosition().y;
        if(update)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                    posXplayer1+=3;
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                    posXplayer1-=3;
                    rect1.setPosition(posXplayer1, posYplayer1);
            }
        }
        sf::Packet posPacketX;
        sf::Packet posPacketY;
        sf::IpAddress recipient = "192.168.178.15";
        unsigned short port2 = 2000;
        unsigned short port1 = 2001;
        if(prevPositionX != rect1.getPosition().x)
        {
            //std::cout<<"y u no working \n";
            if(server==true)
            {
                posPacketX << rect1.getPosition().x; //<< rect1.getPosition().y;
                socket.send(posPacketX, recipient, port1);
            }
            if(client==true)
            {
                posPacketX << rect1.getPosition().x; //<< rect1.getPosition().y;
                socket.send(posPacketX, recipient, port2);
            }
        }
        if(prevPositionY != rect1.getPosition().y)
        {
            //std::cout<<"y u no working \n";
            if(server==true)
            {
                posPacketY << rect1.getPosition().y; //<< rect1.getPosition().y;
                socket.send(posPacketY, recipient, port1);
            }
            if(client==true)
            {
                posPacketY << rect1.getPosition().y; //<< rect1.getPosition().y;
                socket.send(posPacketY, recipient, port2);
            }
        }
        if(server==true)
        {
            socket.receive(posPacketY, recipient, port2);
            socket.receive(posPacketX, recipient, port2);

        }
        if(client==true)
        {
            socket.receive(posPacketY, recipient, port1);
            socket.receive(posPacketX, recipient, port1);

        }
        if( posPacketY >> posYplayer2 || posPacketX >> posXplayer2)
        {
            rect2.setPosition(posXplayer2, posYplayer2);
        }
        /*else if(posPacketY >> posYplayer2)
        {
            rect2.setPosition(posXplayer2, posYplayer2);
        }*/

        window.clear();
        window.draw(rect1);
        window.draw(rect2);
        window.display();
    }
    return 0;
}
