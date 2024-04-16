#include "supa.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <list>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


//Somewhat in progress PlayerEntry screen

//to run g++ -Wall *.cpp -o test.exe -lsfml-graphics -lsfml-window -lsfml-system -lcurl
//then ./test.exe

//INSTRUCTIONS
//First is a player entry screen
//Place mouse cursor over cell you wish to enter info 
//KEYBOARD SELECTING OF CELL DOES NOT WORK YET


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
            else if (event.type == sf::Event::TextEntered) {
                // Handle text input events
                if (event.text.unicode < 128) {
                    char inputChar = static_cast<char>(event.text.unicode);

                    // Iterate through cells to find the active cell
                    for (size_t i = 0; i < rows; ++i) {
                        for (size_t j = 0; j < columns; ++j) {
                            if (team1Table[i][j].rect.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y)) {
                                // // Handle Enter key to commit changes
                                // // 
                                // if (inputChar == 13) {
                                //     //team1Table[i][j].text.setString(team1Table[i][j].playerName);
                                //     //std::cout <<  (team1Table[i][j].playerName);


                                // } 

                                
                                if (inputChar == 8) 
                                {
                                    // Backspace deletes characters
                                    if (!team1Table[i][j].playerName.empty()) {
                                        team1Table[i][j].playerName.pop_back();
                                    }
                                } else {
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
                                //need to talk w trenton
                            }
                            
                        }
                        tempID = "";
                    } 
                    errorShow = 0;
                }
                // for successful matches, put them in an array/vector, then do something simlar to play action
                // flip a variable and iterate through the array and prompt for the equipment number.



                // tempID = team1Table[i][0].text.getString();
                // std::cout << tempID << std::endl;
            }

        }
            //playerListRed.push_back(enterPlayer1);
        // Draw team 2 entry table
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
            playAction = 1;
            window.close();
            //void playAction();
            
        }


        window.display();


  
    }
    
    for (int i = 0; i < rows; ++i)
    {
        // for (int j = 0; j < columns; ++j)
        // {
        //     if (j == 1)
        //         {
        //             enterID = team1Table[i][j].playerName;
        //             enterPlayer1.ID = enterID;
        //         }
        //     else if (j == 2)
        //         {
        //             enterName = team1Table[i][j].playerName;
        //             enterPlayer1.name = enterName;
        //         }       
        // }
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


        // if (// == 1)
        // {
        //     if (team1Table[i][0].playerName != "")
        //     {
        //         std::cout << "Enter the equipment number for Red Player " << i  << ":" << std::endl;
        //         cin >> enterEquipNum;
        //         enterPlayer1.equipNum = enterEquipNum;
        //         // equipNumInsert(enterPlayer1);
        //     }
        //     if (team2Table[i][0].playerName != "")
        //     {
        //         std::cout << "Enter the equipment number for Green Player " << i  << ":" << std::endl;
        //         cin >> enterEquipNum;
        //         enterPlayer2.equipNum = enterEquipNum;
        //     }
        // }
        playerListRed.push_back(enterPlayer1);
        playerListGreen.push_back(enterPlayer2);
    }

    

    // //Randomly choose one for testing, for implementation 
    // int updateShot = 0; //points of shot
    // int updateBase = 2;

    // SFML rendering code
    if (playAction == 1)
    {
        bool initialFillCtrl = 0;
        sf::RenderWindow window2(sf::VideoMode(windowSizeX, windowSizeY), "Play Action");
        std::list<sf::Text> actionEvents;
        sf::Clock cooldownTimer;
        const sf::Time cooldownDuration = sf::milliseconds(300);




        while (window2.isOpen()) {
            sf::Event event;
            while (window2.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window2.close();
                }
            }

            int redPlayerCount = 0;
            int greenPlayerCount = 0;

            // SFML rendering
            window2.clear(sf::Color::Black);


            // Draw the list of players on the window
            sf::Font font;
            if (font.loadFromFile("./arial.ttf")) {  // Provide  font file
                sf::Text text;
                text.setFont(font);
                text.setCharacterSize(20);
                text.setFillColor(sf::Color::Red);

                float yPosition = 50.0f;

                for (const auto& player : playerListRed) {
                    if (player.ID != "")
                    {

                        // text.setString("ID: " + player.ID + "   Codename: " + player.name + "   EquipNum: " + player.equipNum + "     Points: " + std::to_string(player.points));
                        
                        if (player.hitBase == 1)
                        {
                            text.setString("B " + player.name + "     " + std::to_string(player.points));
                        }
                        else
                        {
                            text.setString(player.name + "     " + std::to_string(player.points));
                        }
                        

                        text.setPosition(50.0f, yPosition);
                        window2.draw(text);

                        yPosition += 30.0f;
                        redPlayerCount = redPlayerCount + 1;
                    }
                    

                }


                //printing green team

                yPosition = 50.0f;

                text.setFillColor(sf::Color::Green);

                for (const auto& player : playerListGreen) {
                    if (player.ID != "")
                    {
                        if (player.hitBase == 1)
                        {
                            text.setString("B " + player.name + "     " + std::to_string(player.points));
                        }
                        else
                        {
                            text.setString(player.name + "     " + std::to_string(player.points));
                        }
                        
                        
                        text.setPosition((windowSizeX / 2) + 50.0f, yPosition);
                        window2.draw(text);

                        yPosition += 30.0f;
                        greenPlayerCount = greenPlayerCount + 1;
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
                    sf::Text eventText;
                    eventText.setFont(font);
                    eventText.setCharacterSize(20);
                    eventText.setFillColor(sf::Color::White);
                    eventText.setString("hello");
                    actionEvents.push_front(eventText);
                    // cout << "hello" << endl;

                }
                initialFillCtrl = 1;
            }



            // cout << redPlayerCount;
            //add new messages to front of list?
            int playerSelectRed;
            int playerSelectGreen;
            if (cooldownTimer.getElapsedTime() >= cooldownDuration) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    // cout << "left";
                    std::string leftMessage;

                    //Random number used to determine what action to take in the context of the traffic generator
                    int random = rand() % 10 + 1;



                    //Up above redPlayerCount and greenPlayerCount count the number of entries in the lists that have
                    // an ID entered.

                    //playerSelectRed && playerSelectGreen randomly determine the players to be chosen in the events
                    // based on the counts in the lists.

                    // in a hypothetical list of 3 entries, these variables would pick which of the three to use

                    //If a list has no entries, the variables are set to 0, meaning empty strings will be displayed.
                    if (redPlayerCount != 0)
                    {
                        playerSelectRed = rand() % redPlayerCount;
                    }
                    else
                    {
                        playerSelectRed = 0;
                    }

                    if (greenPlayerCount != 0)
                    {
                        playerSelectGreen = rand() % greenPlayerCount;
                    }
                    else
                    {
                        playerSelectGreen = 0;
                    }
                    
                    

                    //
                    if (random >= 9)
                    {
                        if (random == 9)
                        {
                            if (playerListRed[playerSelectRed].hitBase != 1)
                            {
                                leftMessage = "Base hit by " + playerListRed[playerSelectRed].name;
                                playerListRed[playerSelectRed].hitBase = 1;
                                playerListRed[playerSelectRed].points = playerListRed[playerSelectRed].points + 100;
                            }
                            else
                            {
                                leftMessage = "Base already hit by " + playerListRed[playerSelectRed].name;
                            }

                        }
                        else if(random == 10)
                        {
                            if (playerListGreen[playerSelectGreen].hitBase != 1)
                            {
                                leftMessage = "Base hit in the gap by " + playerListGreen[playerSelectGreen].name;
                                playerListGreen[playerSelectGreen].hitBase = 1;
                                playerListGreen[playerSelectGreen].points = playerListGreen[playerSelectGreen].points + 100;
                            }
                            else
                            {
                                leftMessage = "Base already hit by " + playerListGreen[playerSelectGreen].name;
                            }

                        }
                        
                    }
                    else if(random < 9 && random > 5)
                    {
                        // if(playerListRed[playerSelectRed].name)
                        leftMessage = playerListRed[playerSelectRed].name + " hit " + playerListGreen[playerSelectGreen].name;
                        playerListRed[playerSelectRed].points = playerListRed[playerSelectRed].points + 10;
                    }
                    else if(random <= 5)
                    {
                        // if(playerListRed[playerSelectRed].name)
                        leftMessage = playerListGreen[playerSelectGreen].name + " hit " + playerListRed[playerSelectRed].name;
                        playerListGreen[playerSelectGreen].points = playerListGreen[playerSelectGreen].points + 10;
                    }
                    // leftMessage = "hallo ";

                    sf::Text leftText;
                    leftText.setFont(font);
                    leftText.setCharacterSize(20);
                    leftText.setFillColor(sf::Color::White);
                    leftText.setString(leftMessage);

                    // eventText.setString("left");

                    actionEvents.push_front(leftText);

                    actionEvents.pop_back();
                    cooldownTimer.restart();
                    // count = count + 1;
                }
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    // cout << "right";

                    sf::Text leftText;
                    leftText.setFont(font);
                    leftText.setCharacterSize(20);
                    leftText.setFillColor(sf::Color::White);
                    leftText.setString("right");

                    // eventText.setString("left");

                    actionEvents.push_front(leftText);

                    // actionEvents.push_front(eventText);

                    actionEvents.pop_back();
                    cooldownTimer.restart();
                }
            }

            float xEventPos = 120;
            float yEventPos = (windowSizeY/2) - 20;
            for(sf::Text& message : actionEvents)
            {
                //bottom up, pop the last item in the list off

                //draw events
                message.setPosition(xEventPos, yEventPos);

                window2.draw(message);


                yEventPos = yEventPos + 40;
            }

            window2.display();
        }

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