#include "supa.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>     
#include <time.h>  
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Add this header for inet_pton
#include <thread>    
#include <chrono>

#include "response.h"
#include "CountDownDisplay.h" 
#include "trafficGenerator.h"

using namespace std;

//Somewhat in progress PlayerEntry screen

//to run g++ -Wall *.cpp -o test.exe -lsfml-graphics -lsfml-window -lsfml-system -lcurl
//then ./test.exe

//INSTRUCTIONS
//First is a player entry screen
//Place mouse cursor over cell you wish to enter info 
//KEYBOARD SELECTING OF CELL DOES NOT WORK YET

const int serverPort = 7500; // Port to send to
const int clientPort = 7501; // Port to listen on
const char* serverIPAddress = "127.0.0.1";

std::string attackerPlayer;
std::string targetPlayer;


class Player {
    public:
        std::string name;
        std::string ID;
        std::string equipNum;
        bool hitBase = 0;
        int points;

    Player(const std::string& n, const std::string& i , const std::string e, const int p) : name(n), ID(i), equipNum(e), points(p) {}
};





struct Cell {
    sf::RectangleShape rect;
    sf::Text text;
    std::string playerName;
    int playerPoints;

    Cell() : playerName(""), playerPoints(0) {}
};

struct actionMessage {
    sf::Text text1;
    sf::Text text2;
    sf::Text text3;
};



int main() {
    Database hi;
    string readBuffer = "";
    hi.displayDatabase(readBuffer);

    hi.decodeJSON(readBuffer, hi.codenames);
    // std::cout << "Extracted codenames:" << std::endl;
    // for (const auto& id : hi.ids){
    //     std::cout << id << std::endl;
    // }
    // std::cout << hi.ids[1] << std::endl;
    string tempID;

    srand (time(NULL));

    
    std::string enterName = "test";
    std::string enterID = "12";
    std::string enterEquipNum = "hi";


    // Create a list of players
    std::vector<Player> playerListRed = {
    };

    std::vector<Player> playerListGreen = {
    };

    int windowSizeX = 1250;
    int windowSizeY = 900;
    sf::RenderWindow window(sf::VideoMode(windowSizeX, windowSizeY), "Player Entry Screen");

    const int rows = 20;
    const int columns = 3;
    // const float cellWidth = 150.f;
    // const float cellHeight = 30.f;

    std::vector<std::vector<Cell>> team1Table(rows, std::vector<Cell>(columns));
    std::vector<std::vector<Cell>> team2Table(rows, std::vector<Cell>(columns));


    Player enterPlayer1(enterName, enterID, enterEquipNum, 0);
    Player enterPlayer2(enterName, enterID, enterEquipNum, 0);

    sf::RectangleShape rectangleRed, rectangleGreen;

    //Controls when the program goes to the play action screen.
    bool playAction = 0;

    //controls when an id(s) are inserted into the database


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {


            if (event.type == sf::Event::Closed) {
                window.close();
            }             
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                for (size_t i = 0; i < rows; ++i) 
                {

                    if((team1Table[i][0].playerName != "" && team1Table[i][1].playerName != "" && team1Table[i][2].playerName != "")||(team2Table[i][0].playerName != "" && team2Table[i][1].playerName != "" && team2Table[i][2].playerName != ""))
                    {
                        // std::cout << team1Table[i][0].playerName << std::endl;       ///ID = team1Table[i][0].playerName 
                        // std::cout << team1Table[i][1].playerName << std::endl;       // Codename = team1Table[i][1].playerName 
                        // std::cout << team1Table[i][2].playerName << std::endl;       //EquipNum = team1Table[i][2].playerName 

                        //Should be able to broadcast the equipnumber from here
                        //When enter is pressed, all rows with their columns filled will activate code in the if statement
                        //Don't know if this will be a problem,
                        std::thread trafficThread(generateTraffic);

                        sleep(5);

                        // Create UDP socket
                        int clientSocket;
                        struct sockaddr_in serverAddr, clientAddr;

                        clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
                        if (clientSocket < 0) {
                            std::cerr << "Error in socket creation" << std::endl;
                            return 1;
                        }

                        serverAddr.sin_family = AF_INET;
                        serverAddr.sin_port = htons(serverPort);
                        serverAddr.sin_addr.s_addr = INADDR_ANY;

                        clientAddr.sin_family = AF_INET;
                        clientAddr.sin_port = htons(clientPort);
                        inet_pton(AF_INET, serverIPAddress, &clientAddr.sin_addr);

                        // Bind client socket
                        if (bind(clientSocket, (struct sockaddr*)&clientAddr, sizeof(clientAddr)) < 0) {
                            std::cerr << "Error in binding" << std::endl;
                            close(clientSocket);
                            return 1;
                        }

                        std::string broadcastNumber1 = team1Table[i][2].playerName;
                        std::string broadcastNumber2 = team2Table[i][2].playerName;

                        // Send equipment number to the server
                        sendto(clientSocket, broadcastNumber1.c_str(), broadcastNumber1.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
                        sendto(clientSocket, broadcastNumber2.c_str(), broadcastNumber2.length(), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

                        close(clientSocket);
                    }
                }

            }
            if (event.type == sf::Event::TextEntered) {
                // Handle text input events
                if (event.text.unicode < 128) {
                    char inputChar = static_cast<char>(event.text.unicode);

                    // Iterate through cells to find the active cell
                    for (size_t i = 0; i < rows; ++i) {
                        for (size_t j = 0; j < columns; ++j) {
                            if (team1Table[i][j].rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                                // // Handle Enter key to commit changes
                                // // // 
                                // if ((inputChar == 13)|| (inputChar == 10)) {
                                //     // team1Table[i][j].text.setString(team1Table[i][j].playerName);
                                //     std::cout <<  (team1Table[i][j].playerName);


                                // } 

                                
                                if (inputChar == 8) 
                                {
                                    // Backspace deletes characters
                                    if (!team1Table[i][j].playerName.empty()) {
                                        team1Table[i][j].playerName.pop_back();
                                    }
                                } 
                                else {
                                    // Append the character to the input string
                                    if (team1Table[i][j].playerName.size() < 15) {  // Limit the player name length
                                        team1Table[i][j].playerName += inputChar;
                                    }
                                }

                            // if (team1Table[i][0].text.getString() != "")
                            // {
                            //     for (int x = 0; x < hi.ids.size(); x++)
                            //     {
                            //         tempID = team1Table[i][0].playerName;
                            //         //std::cout << team1Table[i][1].playerName << std::endl;
                                    
                            //         if((stoi(tempID)) == hi.ids[i])
                            //         {
                            //             team1Table[i][1].playerName = hi.codenames[i];
                            //         }
                            //     } 
                            // }
                            } 




                            if (team2Table[i][j].rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                                // Similar handling for the second team
                                if (inputChar == 13) {
                                    team2Table[i][j].text.setString(team2Table[i][j].playerName);
                                } else if (inputChar == 8) {
                                    if (!team2Table[i][j].playerName.empty()) {
                                        team2Table[i][j].playerName.pop_back();
                                    }
                                } else {
                                    if (team2Table[i][j].playerName.size() < 15) {
                                        team2Table[i][j].playerName += inputChar;
                                    }
                                }

                            }
                        }
                        
                    }
                }
            }
            
        }

        window.clear();

        if (sf::Keyboard:: isKeyPressed(sf::Keyboard::F12))
        {
            //std::cout << "hello" << std::endl;
            for(int i = 0; i < rows; i++)
            {
                for(int j = 0; j < columns; j++)
                {
                    team1Table[i][j].text.setString("");
                    team2Table[i][j].text.setString("");
                    team1Table[i][j].playerName = ("");
                    team2Table[i][j].playerName = ("");
                }
            }
        }

        rectangleRed.setSize(sf::Vector2f(490.f, 850.f));
        rectangleGreen.setSize(sf::Vector2f(490.f, 850.f));

        rectangleRed.setPosition(75.0f, 50.0f);
        rectangleGreen.setPosition((windowSizeX/2)+75.0f, 50.0f);

        rectangleRed.setFillColor(sf::Color::Red);
        rectangleGreen.setFillColor(sf::Color::Green);



        window.draw(rectangleRed);
        window.draw(rectangleGreen);


        int tryStoi = 0;
        bool errorShow = 0;
        float xPosTableIntial = 100.0f;
        float xPosTableMultipier = 163.0f;
        bool redComplete = 1;
        // Draw team 1 entry table
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {

                team1Table[i][j].rect.setSize(sf::Vector2f(120.0f, 30.0f));
                team1Table[i][j].rect.setFillColor(sf::Color::White);  // Set color for team 1
                team1Table[i][j].rect.setPosition(xPosTableIntial + j * xPosTableMultipier, 100.0f + i * 40.0f);
                window.draw(team1Table[i][j].rect);

                sf::Font font;
                if (font.loadFromFile("./arial.ttf")) {
                    team1Table[i][j].text.setFont(font);
                    team1Table[i][j].text.setString(team1Table[i][j].playerName);
                    team1Table[i][j].text.setCharacterSize(12);
                    team1Table[i][j].text.setFillColor(sf::Color::Black);
                    team1Table[i][j].text.setPosition(xPosTableIntial + j * xPosTableMultipier, 100.0f + i * 40.0f);
                    window.draw(team1Table[i][j].text);
                    
                }

  
                // for successful matches, put them in an array/vector, then do something simlar to play action
                // flip a variable and iterate through the array and prompt for the equipment number.



                // tempID = team1Table[i][0].text.getString();
                // std::cout << tempID << std::endl;
            }

                //This function controls the loose communication with the database

                //The user is supposed to input a number (ID) in the left column of either table
                //If that ID matches one within the database, the codename matched to the ID is auto
                // put into the table.

                //If the entered ID/EquipNum isn't a number, it is erased.

                //TODO
                //If that ID is not in the database, the id number and whatever entry put into the second 
                //column are inserted into the database
                //Can't do without trenton

            if (team1Table[i][0].text.getString() != "")
                {
                    for (unsigned long x = 0; x < hi.ids.size(); x++)
                    {
                        tempID = team1Table[i][0].playerName;
                        string tempEquipNum = team1Table[i][2].playerName;

                        //equipNums
                        try {
                            tryStoi = stoi(tempEquipNum);
                        }
                        catch(exception &err)
                        {
                            if (errorShow == 0)
                            {
                                // std::cout << "Invalid ID" << endl;
                                errorShow = 1;
                            }
                            
                            team1Table[i][2].playerName = "";
                            tryStoi = 0;
                        }


                        //IDS
                        try {
                            tryStoi = stoi(tempID);
                        }
                        catch(exception &err)
                        {
                            if (errorShow == 0)
                            {
                                // std::cout << "Invalid ID" << endl;
                                errorShow = 1;
                            }
                            
                            team1Table[i][0].playerName = "";
                            tryStoi = 0;
                        }

                        if((tryStoi) == hi.ids[x])
                        {
                            team1Table[i][1].playerName = hi.codenames[x];
                            // = 1;
                        }
                        else
                        {
                            //new array of codenames to be put in database.
                            if (team1Table[i][1].text.getString() != "")
                            {
                                // to do
                                //insert the id and codename for the new entry in the database
                                // ID = team1Table[i][0].text.getString()
                                // Codename = team1Table[i][1].text.getString()

                            }
                            
                        }
                        tempID = "";
                    } 
                    errorShow = 0;
                    if((team1Table[i][1].playerName == "" || team1Table[i][2].playerName == ""))
                    {
                        redComplete = 0;
                        // std::cout << "Incomplete" << std::endl;
                    }
                    // std::cout << redComplete << std::endl;
                }


        }
            //playerListRed.push_back(enterPlayer1);
        // Draw team 2 entry table
        bool greenComplete = 1;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < columns; ++j) {
                team2Table[i][j].rect.setSize(sf::Vector2f(120.0f, 30.0f));
                team2Table[i][j].rect.setFillColor(sf::Color::White);  // Set color for team 2
                team2Table[i][j].rect.setPosition((windowSizeX/2) + xPosTableIntial + j * xPosTableMultipier, 100.0f + i * 40.0f);
                window.draw(team2Table[i][j].rect);

                sf::Font font;
                if (font.loadFromFile("./arial.ttf")) {
                    team2Table[i][j].text.setFont(font);
                    team2Table[i][j].text.setString(team2Table[i][j].playerName);
                    team2Table[i][j].text.setCharacterSize(12);
                    team2Table[i][j].text.setFillColor(sf::Color::Black);
                    team2Table[i][j].text.setPosition((windowSizeX/2) + xPosTableIntial + j * xPosTableMultipier, 100.0f + i * 40.0f);
                    window.draw(team2Table[i][j].text);
                    
                }
                
             
            }


            if (team2Table[i][0].text.getString() != "")
                {
                    for (unsigned long x = 0; x < hi.ids.size(); x++)
                    {
                        tempID = team2Table[i][0].playerName;
                        // string tempEquipNum = team2Table
                        //std::cout << team1Table[i][2].playerName << std::endl;

                        string tempEquipNum = team2Table[i][2].playerName;

                        //equipNums
                        try {
                            tryStoi = stoi(tempEquipNum);
                        }
                        catch(exception &err)
                        {
                            if (errorShow == 0)
                            {
                                // std::cout << "Invalid ID" << endl;
                                errorShow = 1;
                            }
                            
                            team2Table[i][2].playerName = "";
                            tryStoi = 0;
                        }
                                                
                        
                        try {
                            tryStoi = stoi(tempID);
                        }
                        catch(exception &err)
                        {
                            if (errorShow == 0)
                            {
                                // std::cout << "Invalid equipNum" << endl;
                                errorShow = 1;
                            }
                            
                            team2Table[i][0].playerName = "";
                            // tryStoi = "";
                        }
                        
                        if(tryStoi == hi.ids[x])
                        {
                            team2Table[i][1].playerName = hi.codenames[x];
                            // = 1;
                        }
                        else
                        {
                            //new array of codenames to be put in database.
                            if (team2Table[i][1].text.getString() != "")
                            {
                                // to do
                                //insert the id and codename for the new entry in the database
                                //need to talk w trenton
                            }
                            
                        }
                        tempID = "";
                    } 
                    if(team2Table[i][1].playerName == "" && team2Table[i][2].playerName == "")
                    {
                        greenComplete = 0;
                    }   

                }  

        }


        /*Sets up text descriptors, redTeam and greenTeam are the text above the tables that say 
            Red Team and Green Team

            ID, codename and equipN describe their respective columns for both team tables
            ID is first column, Codename is the second column, 

        */
        Cell redTeam,greenTeam,ID, codename, equipN;
  
        sf::Font font;
        redTeam.rect.setSize(sf::Vector2f(150.0f, 30.0f));
        redTeam.rect.setFillColor(sf::Color::Red);
        redTeam.rect.setPosition(250.0f, 20.0f);
        window.draw(redTeam.rect);
        
        greenTeam.rect.setSize(sf::Vector2f(150.0f, 30.0f));
        greenTeam.rect.setFillColor(sf::Color::Green);
        greenTeam.rect.setPosition((windowSizeX/2) + 250.0f, 20.0f);
        window.draw(greenTeam.rect);
        if (font.loadFromFile("./arial.ttf"))
        {
            redTeam.text.setFont(font);
            redTeam.text.setString("Red Team");
            redTeam.text.setCharacterSize(18);
            redTeam.text.setFillColor(sf::Color::White);
            redTeam.text.setPosition(285.0f, 25.0f);
            window.draw(redTeam.text);
            
            greenTeam.text.setFont(font);
            greenTeam.text.setString("Green Team");
            greenTeam.text.setCharacterSize(18);
            greenTeam.text.setFillColor(sf::Color::White);
            greenTeam.text.setPosition((windowSizeX/2) + 275.0f, 25.0f);
            window.draw(greenTeam.text);

            ID.text.setFont(font);
            ID.text.setString("ID");
            ID.text.setCharacterSize(18);
            ID.text.setFillColor(sf::Color::White);
            ID.text.setPosition(150.0f, 70.0f);
            window.draw(ID.text);

            ID.text.setPosition((windowSizeX/2) + 150.0f, 70.0f);
            window.draw(ID.text);


            codename.text.setFont(font);
            codename.text.setString("Codename");
            codename.text.setCharacterSize(18);
            codename.text.setFillColor(sf::Color::White);
            codename.text.setPosition(278.0f, 70.0f);
            window.draw(codename.text);

            codename.text.setPosition((windowSizeX/2) + 278.0f, 70.0f);
            window.draw(codename.text);


            equipN.text.setFont(font);
            equipN.text.setString("Equip Num");
            equipN.text.setCharacterSize(18);
            equipN.text.setFillColor(sf::Color::White);
            equipN.text.setPosition(443.0f, 70.0f);
            window.draw(equipN.text);

            equipN.text.setPosition(((windowSizeX/2)) + 443.0f, 70.0f);
            window.draw(equipN.text);



            // ID2.text.setFont(font);
            // ID2.text.setString("ID");
            // ID2.text.setCharacterSize(18);
            // ID2.text.setFillColor(sf::Color::White);
            // ID2.text.setPosition((windowSizeX/2) + 150.0f, 70.0f);
            // window.draw(ID2.text);


        }
            

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
        {
            if (redComplete == 1 && greenComplete == 1)
            {
                playAction = 1;
                window.close();
            }
            else
            {
                std::cout << "Tables not complete" << std::endl;
            }

            //void playAction();
            
        }



        window.display();


  
    }

    


    

    // //Randomly choose one for testing, for implementation 
    // int updateShot = 0; //points of shot
    // int updateBase = 2;

    int redPoints = 0;
    int greenPoints = 0;

    // SFML rendering code
    if (playAction == 1)
    {

        for (int i = 0; i < rows; ++i)
        {
            enterID = team1Table[i][0].playerName;
            enterPlayer1.ID = enterID;

            enterName = team1Table[i][1].playerName;
            enterPlayer1.name = enterName;

            // std::cout << team1Table[i][2].playerName << std::endl;

            enterPlayer1.equipNum = team1Table[i][2].playerName;




            enterID = team2Table[i][0].playerName;
            enterPlayer2.ID = enterID;

            enterName = team2Table[i][1].playerName;
            enterPlayer2.name = enterName;

            enterPlayer2.equipNum = team2Table[i][2].playerName;

            playerListRed.push_back(enterPlayer1);
            playerListGreen.push_back(enterPlayer2);

            if (enterPlayer1.ID != "" && enterPlayer1.name != "")
            {
                int id = stoi(enterPlayer1.ID);
                string codename = enterPlayer1.name;

                hi.addRow(id,codename);
            }


            if (enterPlayer2.ID != "" && enterPlayer2.name != "")
            {
                int id = stoi(enterPlayer2.ID);
                string codename = enterPlayer2.name;

                hi.addRow(id,codename);
            }
        }





        bool initialFillCtrl = 0;
        sf::RenderWindow window2(sf::VideoMode(windowSizeX, windowSizeY), "Play Action");
        std::list<actionMessage> actionEvents;
        sf::Clock cooldownTimer;
        const sf::Time cooldownDuration = sf::milliseconds(2000);

        //Countdown
        std::thread trafficThread(generateTraffic);


        CountdownDisplay countdownDisplay(window2);
        countdownDisplay.run();
        
        std::thread responseThread(response, std::ref(countdownDisplay));


        while (window2.isOpen()) {

            sf::Event event;
            while (window2.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window2.close();
                }
            }
            int pointTotalRed = 0;
            int pointTotalGreen = 0;

            int redPlayerCount = 0;
            int greenPlayerCount = 0;

            //set equal to the recieved ID???
            string idRecieved;

            Player selectedPlayer("","","",0);

            int playerSelectedPlace;


            // SFML rendering
            window2.clear(sf::Color::Black);

            sf::Texture redB,greenB;

            if(!redB.loadFromFile("B2Test.jpg"))
            {
                cout << "didn't load b" << endl;
            }
            if(!greenB.loadFromFile("BGreen.jpg"))
            {
                cout << "didn't load b" << endl;
            }

            sf::RectangleShape bArea(sf::Vector2f(20.0f,20.0f));
            bArea.setTexture(&redB);
            
            sf::RectangleShape bAreaGreen(sf::Vector2f(20.0f,20.0f));
            bAreaGreen.setTexture(&greenB);




            // Draw the list of players on the window
            sf::Font font;
            if (font.loadFromFile("./arial.ttf")) {  // Provide  font file
                sf::Text text;
                text.setFont(font);
                text.setCharacterSize(20);
                text.setFillColor(sf::Color::Red);

                float yPosition = 20.0f;

                int index = 0;

                for (const auto& player : playerListRed) {
                    if (player.ID != "")
                    {

                        // text.setString("ID: " + player.ID + "   Codename: " + player.name + "   EquipNum: " + player.equipNum + "     Points: " + std::to_string(player.points));
                        float bSize = 0;
                        if (player.hitBase == 1)
                        {
                            text.setString(player.name + "     " + std::to_string(player.points));
                            bArea.setPosition(100.0f,yPosition + 3);
                            window2.draw(bArea);
                            bSize = 30.0f;
                        }
                        else
                        {
                            text.setString(player.name + "     " + std::to_string(player.points));
                        }
                        
                        
                        text.setPosition(100.0f + bSize, yPosition);
                        window2.draw(text);

                        yPosition += 25.0f;
                        redPlayerCount = redPlayerCount + 1;


                        //ID recieved is matched to a player
                        if (player.ID == idRecieved)
                        {
                            selectedPlayer.name = player.name;
                            selectedPlayer.ID = player.ID;
                            selectedPlayer.equipNum = player.equipNum;
                            selectedPlayer.points = player.points;
                            selectedPlayer.hitBase = player.hitBase;
                            playerSelectedPlace = index;
                        }


                    }
                    index = index + 1;
                    
                }

                // std::cout << to_string(pointTotalRed) << std::endl;


                //printing green team

                yPosition = 20.0f;

                text.setFillColor(sf::Color::Green);
                for (const auto& player : playerListGreen) {
                    if (player.ID != "")
                    {
                        float bSize = 0;
                        if (player.hitBase == 1)
                        {
                            text.setString(player.name + "     " + std::to_string(player.points));
                            bAreaGreen.setPosition((windowSizeX / 2) + 50.0f ,yPosition + 3);
                            window2.draw(bAreaGreen);
                            bSize = 30.0f;
                        }
                        else
                        {
                            text.setString(player.name + "     " + std::to_string(player.points));
                        }
                        
                        
                        text.setPosition((windowSizeX / 2) + 50.0f + bSize, yPosition);
                        window2.draw(text);

                        yPosition += 25.0f;
                        greenPlayerCount = greenPlayerCount + 1;


                        //ID recieved is matched to a player
                        if (player.ID == idRecieved)
                        {
                            selectedPlayer.name = player.name;
                            selectedPlayer.ID = player.ID;
                            selectedPlayer.equipNum = player.equipNum;
                            selectedPlayer.points = player.points;
                            selectedPlayer.hitBase = player.hitBase;
                        }
                    }
                    

                }
            }

            sf::Color recColor(66, 99, 245);

            sf::RectangleShape rectangleAction;
            rectangleAction.setSize(sf::Vector2f(windowSizeX - 200.0, (windowSizeY/2)));

            rectangleAction.setPosition(100.0f, (windowSizeY/2) - 50);

            rectangleAction.setFillColor(recColor);
            window2.draw(rectangleAction);

            

            // sf::Text eventText;
            // eventText.setFont(font);
            // eventText.setCharacterSize(20);
            // eventText.setFillColor(sf::Color::White);


            // cout << initialFillCtrl;
            if (initialFillCtrl != 1)
            {
                // cout << "whiy";
                for (int i = 0; i < 10; i++)
                {
                    // setup Events with 10 empty strings
                    actionMessage message;
                    sf::Text eventText;
                    eventText.setFont(font);
                    eventText.setCharacterSize(20);
                    eventText.setFillColor(sf::Color::White);
                    eventText.setString("");

                    message.text1 = eventText;
                    message.text2 = eventText;
                    message.text3 = eventText;

                    actionEvents.push_front(message);
                    // cout << "hello" << endl;

                }
                initialFillCtrl = 1;
            }



            // cout << redPlayerCount;
            //add new messages to front of list?
            int playerSelectRed;
            int playerSelectGreen;


            //Meat and potatoes of it
            sleep(1);

            if (cooldownTimer.getElapsedTime() >= cooldownDuration) {
                    actionMessage message;


                    // cout << "left";
                    std::string middleMessage = "";
                    std::string firstName = "";
                    std::string secondName = "";
                    
                    sf::Text redText;
                    sf::Text whiteText;
                    sf::Text greenText;


                    redText.setFillColor(sf::Color::Red);
                    whiteText.setFillColor(sf::Color::White);
                    greenText.setFillColor(sf::Color::Green);

                    redText.setFont(font);
                    redText.setCharacterSize(20);

                    whiteText.setFont(font);
                    whiteText.setCharacterSize(20);

                    greenText.setFont(font);
                    greenText.setCharacterSize(20);

                    redText.setString(middleMessage);
                    whiteText.setString(middleMessage);
                    greenText.setString(middleMessage);

                    message.text1 = redText;
                    message.text2 = redText;
                    message.text3 = redText;

                    attackerPlayer = countdownDisplay.getAttackerPlayer();
                    targetPlayer = countdownDisplay.getTargetPlayer();

                    int attackerPlayerNum = stoi(attackerPlayer);
                    int targetPlayerNum = stoi(targetPlayer);


                    //Up above redPlayerCount and greenPlayerCount count the number of entries in the lists that have
                    // an ID entered.

                    //playerSelectRed && playerSelectGreen randomly determine the players to be chosen in the events
                    // based on the counts in the lists.

                    // in a hypothetical list of 3 entries, these variables would pick which of the three to use
                    
                    std::string ack_message = "";
                    // Acknowledge the message
                    if (targetPlayer != "43" && targetPlayer != "53")
                    {
                        ack_message = "Received hit from player " + attackerPlayer + " on player " + targetPlayer;
                        if(attackerPlayerNum % 2 == 0)
                        {
                            playerListGreen[attackerPlayerNum].points += 10;
                            pointTotalGreen += 10;
                        }
                        else
                        {
                            playerListRed[attackerPlayerNum].points += 10;
                            pointTotalRed += 10;
                        }
                        whiteText.setString(ack_message);
                        message.text1 = whiteText;
                    }
                    else
                    {
                        if (targetPlayer == "43")
                        {
                            ack_message = "Received hit from player " + attackerPlayer + " on the green base";
                            playerListRed[attackerPlayerNum].points = playerListRed[attackerPlayerNum].points + 100;

                            whiteText.setString(ack_message);
                            pointTotalRed += 100;
                            message.text1 = whiteText;
                        }
                        else
                        {
                            ack_message = "Received hit from player " + attackerPlayer + " on the red base";
                            playerListGreen[attackerPlayerNum].points += 100;

                            whiteText.setString(ack_message);
                            pointTotalGreen += 100;
                            message.text1 = whiteText;
                        }
                    }

                    //if same team hits same team

                        //red


                        // firstName = playerListRed[playerSelectGreen].name;
                        // middleMessage = " hit ";
                        // secondName = playerListRed[playerSelectRed].name;

                        // playerListGreen[playerSelectGreen].points = playerListGreen[playerSelectGreen].points + 10;

                        // greenText.setString(firstName);
                        // whiteText.setString(middleMessage);
                        // redText.setString(secondName);

                        // message.text1 = greenText;
                        // message.text2 = whiteText;
                        // message.text3 = redText;

                        // pointTotalGreen = pointTotalGreen + 10;


                    actionEvents.push_front(message);

                    actionEvents.pop_back();
                    cooldownTimer.restart();
                    // count = count + 1;            
  
            }
            
            redPoints = redPoints + pointTotalRed;
            greenPoints = greenPoints + pointTotalGreen;

            sf::Text redPoint;
            redPoint.setFillColor(sf::Color::Red);
            redPoint.setFont(font);
            redPoint.setCharacterSize(20);
            redPoint.setString(to_string(redPoints));
            redPoint.setPosition(50.0f, 10.0f);
            window2.draw(redPoint);


            sf::Text greenPoint;
            greenPoint.setFillColor(sf::Color::Green);
            greenPoint.setFont(font);
            greenPoint.setCharacterSize(20);
            greenPoint.setString(to_string(greenPoints));
            greenPoint.setPosition((windowSizeX/2) + 5.0f, 10.0f);
            window2.draw(greenPoint);


            float xEventPos = 120;
            float yEventPos = (windowSizeY/2) - 20;
            for(actionMessage& message : actionEvents)
            {
                //bottom up, pop the last item in the list off

                //draw events
                message.text1.setPosition(xEventPos, yEventPos);

                window2.draw(message.text1);


                yEventPos = yEventPos + 40;
            }

            window2.display();
        }

        trafficThread.join();
        responseThread.join();
        return 0;
    }
    }
// //cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release .

// //mingw32-make clean
// //mingw32-make





/// Enter Id, prompt for the codename in the database, if not in the database, add a new one
//assume ids are unique, codenames can be duplicated

//Linux
//g++ -c PlayerEntry.cpp
//g++ PlayerEntry.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system     //prob font file
//./sfml-app