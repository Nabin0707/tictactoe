#include <SFML/Graphics.hpp>

bool checkWin(const sf::RectangleShape boxes[9], int& winIndex1, int& winIndex2, int& winIndex3)
{
    // Check rows, columns, and diagonals for a win
    for (int i = 0; i < 3; ++i)
    {
        // Check rows
        if (boxes[i * 3].getTexture() == boxes[i * 3 + 1].getTexture() &&
            boxes[i * 3].getTexture() == boxes[i * 3 + 2].getTexture() &&
            boxes[i * 3].getTexture() != nullptr)
        {
            winIndex1 = i * 3;
            winIndex2 = i * 3 + 1;
            winIndex3 = i * 3 + 2;
            return true;
        }

        // Check columns
        if (boxes[i].getTexture() == boxes[i + 3].getTexture() &&
            boxes[i].getTexture() == boxes[i + 6].getTexture() &&
            boxes[i].getTexture() != nullptr)
        {
            winIndex1 = i;
            winIndex2 = i + 3;
            winIndex3 = i + 6;
            return true;
        }
    }

    // Check diagonals
    if (boxes[0].getTexture() == boxes[4].getTexture() &&
        boxes[0].getTexture() == boxes[8].getTexture() &&
        boxes[0].getTexture() != nullptr)
    {
        winIndex1 = 0;
        winIndex2 = 4;
        winIndex3 = 8;
        return true;
    }

    if (boxes[2].getTexture() == boxes[4].getTexture() &&
        boxes[2].getTexture() == boxes[6].getTexture() &&
        boxes[2].getTexture() != nullptr)
    {
        winIndex1 = 2;
        winIndex2 = 4;
        winIndex3 = 6;
        return true;
    }

    return false;
}

bool checkDraw(const sf::RectangleShape boxes[9])
{
    for (int i = 0; i < 9; ++i)
    {
        if (boxes[i].getTexture() == nullptr)
            return false; // At least one box is empty, not a draw
    }
    return true; // All boxes are filled, it's a draw
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 450), "Tic-Tac-Toe Board");

    // Create the lines for the Tic-Tac-Toe grid
    sf::RectangleShape verticalLine(sf::Vector2f(5.0f, 450.0f));
    sf::RectangleShape horizontalLine(sf::Vector2f(450.0f, 5.0f));

    verticalLine.setFillColor(sf::Color::White); // Set line color to white
    horizontalLine.setFillColor(sf::Color::White); // Set line color to white

    verticalLine.setPosition(150.0f, 0.0f);
    horizontalLine.setPosition(0.0f, 150.0f);
    // Create rectangles for each box on the Tic-Tac-Toe grid
   
    sf::RectangleShape boxes[9];
    for (int i = 0; i < 9; ++i)
    {
        boxes[i].setSize(sf::Vector2f(150.0f, 150.0f));
        boxes[i].setTexture(nullptr);
        boxes[i].setFillColor(sf::Color(50, 50, 50)); // Set a dark background color for the boxes
        boxes[i].setOutlineColor(sf::Color::White); // Set outline color to white
        boxes[i].setOutlineThickness(2.0f); // Set outline thickness
        boxes[i].setPosition((i % 3) * 150.0f, (i / 3) * 150.0f);
    }

    // Load textures for crosses and rectangles
    sf::Texture crossTexture;
    if (!crossTexture.loadFromFile("images/cross.png"))
    {
        return -1;
    }

    sf::Texture rectangleTexture;
    if (!rectangleTexture.loadFromFile("images/circle.png"))
    {
        return -1;
    }

    // Initialize players and turns
    sf::Texture playerTextures[2] = { crossTexture, rectangleTexture };
    int currentPlayer = 0;
    bool gameWon = false;
    bool draw = false;
    int winIndex1 = -1, winIndex2 = -1, winIndex3 = -1;


    sf::RectangleShape replayButton(sf::Vector2f(150.0f, 50.0f));
    replayButton.setFillColor(sf::Color(100, 100, 100)); // Set button color
    replayButton.setOutlineColor(sf::Color::White); // Set outline color to white
    replayButton.setOutlineThickness(2.0f); // Set outline thickness
    replayButton.setPosition(450.0f, 200.0f);

    sf::Font font;
   /* if (!font.loadFromFile("arial.ttf")) // Load a font of your choice
    {
        return -1;
    }*/

    sf::Text replayText("Replay", font, 24);
    replayText.setFillColor(sf::Color::White);
    replayText.setPosition(465.0f, 210.0f);
    bool replayClicked = false;
    // Game loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle mouse click events if the game is not won yet
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Check if replay button was clicked
                if (replayButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
                {
                    // Reset the game state for a new round
                    for (int i = 0; i < 9; ++i)
                    {
                        boxes[i].setTexture(nullptr);
                    }
                    currentPlayer = 0;
                    gameWon = false;
                    draw = false;
                    replayClicked = true;
                }
            }
            if (!gameWon && !draw && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 9; ++i)
                {
                    if (boxes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) &&
                        boxes[i].getTexture() == nullptr)
                    {
                        // Set the texture based on the current player's turn
                        boxes[i].setTexture(&playerTextures[currentPlayer]);

                        // Check if the current player has won
                        if (checkWin(boxes, winIndex1, winIndex2, winIndex3))
                        {
                            gameWon = true;
                            break;
                        }

                        // Check for a draw
                        if (checkDraw(boxes))
                        {
                            draw = true;
                            break;
                        }

                        // Switch to the next player's turn
                        currentPlayer = (currentPlayer + 1) % 2;
                    }
                }
            }
        }

        window.clear(sf::Color::Green);

        // Draw the lines to create the Tic-Tac-Toe grid
        for (int i = 1; i < 3; ++i)
        {
            window.draw(verticalLine);
            verticalLine.move(150.0f, 0.0f);

            window.draw(horizontalLine);
            horizontalLine.move(0.0f, 150.0f);
        }

        // Draw the boxes
        for (int i = 0; i < 9; ++i)
        {
            window.draw(boxes[i]);
        }

        if (gameWon)
        {
            sf::Vector2f startPos = boxes[winIndex1].getPosition() + sf::Vector2f(75.0f, 75.0f);
            sf::Vector2f endPos = boxes[winIndex3].getPosition() + sf::Vector2f(75.0f, 75.0f);

            sf::Vector2f direction = endPos - startPos;
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            sf::Vector2f unitDirection = direction / length;

            const float lineWidth = 20.0f; // Set the desired line width

            sf::VertexArray boldLine(sf::Quads, 4);
            boldLine[0].position = startPos + sf::Vector2f(-unitDirection.y * lineWidth / 2, unitDirection.x * lineWidth / 2);
            boldLine[1].position = endPos + sf::Vector2f(-unitDirection.y * lineWidth / 2, unitDirection.x * lineWidth / 2);
            boldLine[2].position = endPos + sf::Vector2f(unitDirection.y * lineWidth / 2, -unitDirection.x * lineWidth / 2);
            boldLine[3].position = startPos + sf::Vector2f(unitDirection.y * lineWidth / 2, -unitDirection.x * lineWidth / 2);

            for (int i = 0; i < 4; ++i)
            {
                boldLine[i].color = sf::Color::Black;
                boldLine[i].color.a = 255; // Set alpha value to fully opaque
                boldLine[i].color.r = 0;
                boldLine[i].color.g = 0;
                boldLine[i].color.b = 0;
            }

            window.draw(boldLine);
            // ... (same winText as before)
        }

        else if (draw)
        {
            // ... (same drawText as before)
        }
        window.draw(replayButton);
        window.draw(replayText);
        window.display();
    }

    return 0;
}
