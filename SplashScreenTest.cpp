#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
int main()
{
	//window ::size::name::close::resize
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Splash Screen",sf::Style::Close|sf::Style::Resize);

	//rectangle crated the same size as window for image area
	sf::RectangleShape ImgArea(sf::Vector2f(1000.0f, 1000.0f));

	//Texture
	sf::Texture logo;//texture image object

	if (!logo.loadFromFile("logo.jpg"))//get image from file
	{
		//if image failed to load output error
		cout << "error: image not found" << endl;
	}

	ImgArea.setTexture(&logo);//set image texture on image area

	sf::Vector2u textureSize = logo.getSize();//set texture size to logo size

	//Timer or clock
	sf::Clock clock;

	// creating the blank screen as a placement holder for player entree screen
	sf::RectangleShape blankScreen(sf::Vector2f(1000.0f, 1000.0f)); 
	blankScreen.setFillColor(sf::Color::Black);

	while (window.isOpen())
	{
		sf::Event e;
		sf::Time elasped = clock.getElapsedTime(); // start timer when window is open

		while (window.pollEvent(e))
		{
			switch (e.type)
			{
			case sf::Event::Closed: //close window
				window.close();
				break;
			case sf::Event::Resized://resize window 
				cout << "new window width: " << e.size.width <<" new window Hieght: " << e.size.height << endl;
				printf("new window width: %i new window height: %i\n", e.size.width, e.size.height);
				break;
			}
			
		}

		//output elasped time in seconds
		//cout << elasped.asSeconds() << endl;
		
		if (elasped.asSeconds() < 5)//if the timer is less than 5 display splash screen
		{
			window.clear();//clear image
			window.draw(ImgArea);//window draw image
			window.display(); //display
		}
		else //after 3 secs the diplay a blank screen
		{
			window.clear();
			window.draw(blankScreen); //window draw blank screen
			window.display(); //display to window
		}

		//if window is open for more than 7 sec 
		//close window
		/*if (elasped.asSeconds() > 7)
		{
			window.close();
			break;
		}*/

	}
	
	return 0;
}