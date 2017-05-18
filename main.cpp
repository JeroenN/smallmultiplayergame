#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>
//#include <Packet.hpp>


class player
{
    sf::RectangleShape block;

public:
    player(const float height, const float width, const float posX, const float posY)
        :block()
    {
        block.setSize(sf::Vector2f(height, width));
        block.setPosition(sf::Vector2f(posX, posY));
        block.setFillColor(sf::Color(255, 127, 63));
    }
    void setPlayerPosition(float posX, float posY)
    {
        //posY+=speedMovingDown;
        block.setPosition(sf::Vector2f(posX, posY));
    }
    float getHeight()const {
        return block.getSize().y;
    }
    float getWidth()const {
        return block.getSize().x;
    }
    float getPosX() const {
        return block.getPosition().x;
    }
    float getPosY()const {
        return block.getPosition().y;
    }

    void display(sf::RenderWindow &window)
    {
        window.draw(block);
    }
};

std::vector<player> add_new_player(int amount_players) noexcept
{
    std::vector<player> newPlayer;
    for(int i=0; i!= amount_players; ++i)
    {
        newPlayer.push_back(player(30,30,0,0));
    }
    return newPlayer;
}

int main()
{

    //sf::IpAddress computersAddress = sf::IpAddress::getLocalAddress();
    //std::cout<<computersAddress ;
    sf::IpAddress clientIP;
    sf::IpAddress sendIP;
    bool server =false;
    bool client =false;
    sf::UdpSocket socket;
    std::string connectionType ="";
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";
    std::string clientCheckShort ="c";
    std::string clientCheck ="client";
    float posXplayer1=0;
    float posYplayer1=0;
    int currentAmountOfPlayers=2;
    char buffer[2000];

    std::size_t received;
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
                clientIP=rIp;
                std::cout<<clientIP;
                std::cin >> answer;
            }
    }

    if(connectionType==clientCheckShort || connectionType==clientCheck)
    {
        client=true;
        std::string sIP="192.168.10.50";
        std::cout << "Enter server ip: ";
        //std::cin>> sIP;
        sendIP=sIP;
        socket.send(text.c_str(), text.length() +1, sendIP, 2000);
    }
    std::vector<player> player{add_new_player(currentAmountOfPlayers)};

    sf::Vector2f prevPosition;
    sf::Vector2f changingPosition;
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
        prevPosition = sf::Vector2f(player[0].getPosX(), player[0].getPosY());
       // prevPositionY = rect1.getPosition().y;
        if(update)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                    posXplayer1+=3;
                    player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                    posXplayer1-=3;
                    player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                posYplayer1-=3;
                player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                posYplayer1+=3;
                player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
        }

        sf::Packet posPacket;
        if(server==true)
        {
            if(prevPosition != sf::Vector2f(player[0].getPosX(), player[0].getPosY()))
            {
                sf::IpAddress recipient = clientIP;
                unsigned short clientPort = 2001;
                posPacket<<player[0].getPosX() <<player[0].getPosY();
                if (socket.send(posPacket, recipient, clientPort) != sf::Socket::Done)
                {
                    //std::cout<<"whoops... some data wasn't sent";
                }

            }
        }
        if(client==true)
        {
            if(prevPosition != sf::Vector2f(player[0].getPosX(), player[0].getPosY()))
            {
                sf::IpAddress recipient = sendIP;
                unsigned short serverPort = 2000;
                posPacket<<player[0].getPosX() <<player[0].getPosY();
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
        player[1].setPlayerPosition(changingPosition.x, changingPosition.y);
        //std::cout<<changingPosition.x;

        window.clear();

        //window.draw(rect1);
        //window.draw(rect2);
        for(int i=0; i<static_cast<int>(player.size()); ++i)
        player[i].display(window);
        window.display();
    }
    return 0;
}
