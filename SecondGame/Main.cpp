#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

struct RGB
{
public:

    int red;
    int green;
    int blue;

public:

    RGB() = default;

    RGB(int r, int g, int b) : red(r), green(g), blue(b) {}

    ~RGB() {}

};

class AShape 
{
public:
   
    sf::Shape* shape;
    std::string shapeName;
    int shapeTag;
    RGB rgbVal;
    float radius;
    sf::Vector2f dimensions;
    sf::Vector2f initPosition;
    sf::Vector2f speed;

public:

    AShape() = default;

    AShape(std::string inputData) 
    {
       
        // Sort inputData
        // Create an input string stream from the given string
        std::istringstream dataStream(inputData);
        std::vector<std::string> dataElements;
        std::string element;

        while (dataStream >> element) 
        {
            dataElements.push_back(element);
        }

                   
        // Assign data
        if (!dataElements.empty())
        {
            if (dataElements[0] == ("Circle"))
            {
                radius = std::stof(dataElements[9]);
                shape = new sf::CircleShape(radius);
                shapeTag = 1;
                
            }
            else if (dataElements[0] == ("Rectangle"))
            {
                dimensions = sf::Vector2f(std::stof(dataElements[9]), std::stof(dataElements[10]));
                shape = new sf::RectangleShape(dimensions);
                shapeTag = 2;
               
            }

           
            shapeName = dataElements[1];
            initPosition = sf::Vector2f(std::stof(dataElements[2]), std::stof(dataElements[3]));
            speed = sf::Vector2f(std::stof(dataElements[4]), std::stof(dataElements[5]));

            rgbVal.red = std::stoi(dataElements[6]);
            rgbVal.green = std::stoi(dataElements[7]);
            rgbVal.blue = std::stoi(dataElements[8]);

            shape->setFillColor(sf::Color(rgbVal.red, rgbVal.green, rgbVal.blue));
            shape->setPosition(initPosition);
        }
    }

    ~AShape() 
    {
        if (shape != nullptr)
        {
            delete shape;
        }
        else
        {
            std::cout << "shape is nullptr" << std::endl;
        }      
    }
  
};

int main(int argc, char* argv[])
{
    // Get Config.txt
    std::string filePath = "D:/SFML_GameEngine/Config.txt";
    std::ifstream inputFile(filePath);
    sf::Font inputFont;

    // Import font 
    if (!inputFont.loadFromFile("D:/SFML_GameEngine/Fonts/Dosis/Dosis.ttf"))
    {
        std::cerr << "Error opening the font file." << std::endl;
        return 1; 
    }

    // Check if the data file is open successfully
    if (!inputFile.is_open()) 
    {
        std::cerr << "Error opening the data file: " << filePath << std::endl;
        return 2; 
    }

    // Read and print the contents of the file
    std::string windowLine;
    std::string line;
    int lineNumber = 0;
    std::vector<std::string> lines = {};

    while (std::getline(inputFile, line)) 
    {
        if (lineNumber == 0) 
        {
            windowLine = line;   
        }
        else 
        {
            lines.push_back(line);           
        }

        lineNumber++;
    }

    inputFile.close();

    // Read windowLine and extract values
    std::istringstream inputWindowStream(windowLine);   
    std::vector<std::string> windowElements;    
    std::string windowElement;
    while (inputWindowStream >> windowElement) 
    {
        windowElements.push_back(windowElement);
    }
   
   
    // Set window properties
    int windowX = std::stoi(windowElements[1]);
    int windowY = std::stoi(windowElements[2]);
    sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Aquarium");
    window.setFramerateLimit(60);

    // Set text properties
    sf::Text textName;    
    textName.setFont(inputFont);
    textName.setCharacterSize(24);
    textName.setFillColor(sf::Color::White);
    textName.setOutlineColor(sf::Color::Black);
    textName.setOutlineThickness(2.f);
    textName.setPosition(sf::Vector2f(500.f, 500.f));

    sf::Text quitText;
    quitText.setFont(inputFont);
    quitText.setCharacterSize(15);
    quitText.setFillColor(sf::Color::White);
    quitText.setOutlineColor(sf::Color::Black);
    quitText.setOutlineThickness(1.f);
    quitText.setPosition(sf::Vector2f(0.f, windowY - quitText.getCharacterSize() - 5.f));
    quitText.setString("\" ESC \" to quit");
   
    // Fill vector of shapes
    std::vector<AShape*> shapes = {};   

    for (const auto& line : lines) 
    {
        AShape* shape = new AShape(line);
        shapes.push_back(shape); 
    }


    // Main while loop
    std::cout << "Start Game" << std::endl;

    while (window.isOpen())
    {
        // Key events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }

        // Rendering
        window.clear();

        for (auto& element : shapes)
        {
            // Shape rendering
            window.draw(*(element->shape));
            
            // Shape animation
            float currentX = element->shape->getPosition().x;
            float currentY = element->shape->getPosition().y; 
            sf::Vector2f textCenter = {};

            if(currentX <= 0)
            { 
                element->speed.x = -element->speed.x;
            }
            else if (currentY <= 0)
            {
                element->speed.y = -element->speed.y;
            }

            if (element->shapeTag == 1) // Circle
            {
                textCenter.x = currentX + element->radius;
                textCenter.y = currentY + element->radius;

                if (currentX + (2 * element->radius) >= windowX)
                {
                    element->speed.x = -element->speed.x;
                }
                else if (currentY + (2 * element->radius) >= windowY)
                {
                    element->speed.y = -element->speed.y;
                }
            }
            else if (element->shapeTag == 2) // Rectangle
            {
                textCenter.x = currentX + (element->dimensions.x/2);
                textCenter.y = currentY + (element->dimensions.y/2);

                if (currentX + (element->dimensions.x) >= windowX)
                {
                    element->speed.x = -element->speed.x;
                }
                else if (currentY + (element->dimensions.y) >= windowY)
                {
                    element->speed.y = -element->speed.y;
                }
            }
            
            element->shape->setPosition(currentX + element->speed.x, currentY + element->speed.y);

            // Text animation
            textName.setString(element->shapeName);
            float charSize = textName.getCharacterSize();
            std::size_t stringSize = textName.getString().getSize();         
            
            textName.setPosition(textCenter.x - (5 * stringSize) + element->speed.x, textCenter.y - (charSize*0.75f) + element->speed.y);
            window.draw(textName);
            window.draw(quitText);
        }
    
        window.display();
    }

    // Release memory
    for (AShape* shape : shapes)
    {
        if (shape != nullptr)
        {
            delete shape;
            std::cout << "Release memory" << std::endl;
        }
        
    }

    return 0;
}