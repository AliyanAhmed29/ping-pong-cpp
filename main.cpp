
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;         //sfml's namespace

//global variables
string name1, name2;





int twoPlayerEasy()
{
    srand(static_cast<unsigned int>(time(NULL)));

    // framing the playing field and stuff
    //****************************************************************
    
    Vector2f racketSize(25, 100);
    Vector2f midLineSize(10, 600);
    const int gameWid = 800;        //x axis
    const int gameHght = 600;       //y axis
    float ballRadi = 10.f;
    const float pi = 3.14159f;
    //****************************************************************



    // Creating the window                           32 bits per pixel
    //****************************************************************
    RenderWindow window(VideoMode(gameWid, gameHght, 32), "FAREEHA'S Pong", Style::Titlebar | Style::Close);
    window.setVerticalSyncEnabled(true);
    //****************************************************************



    // sound for collision
    //****************************************************************
    SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    Sound ballSound(ballSoundBuffer);
    //****************************************************************


    //the middle line on the ground
    //****************************************************************
    RectangleShape middleLine;
    middleLine.setSize(midLineSize - Vector2f(3, 3));
    middleLine.setOutlineThickness(2);
    middleLine.setOutlineColor(Color::Black);
    middleLine.setFillColor(Color::White);
    middleLine.setOrigin(midLineSize/1.f);
    middleLine.setPosition(400, 600);
    //****************************************************************



    // left racket  measurements and positoning
    //****************************************************************
    RectangleShape leftRacket;
    leftRacket.setSize(racketSize - Vector2f(3, 3));
    leftRacket.setOutlineThickness(2);
    leftRacket.setOutlineColor(Color::White);
    leftRacket.setFillColor(Color::Blue);
    leftRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // right racket measurements and positoning
    //****************************************************************
    RectangleShape rightRacket;
    rightRacket.setSize(racketSize - Vector2f(3, 3));
    rightRacket.setOutlineThickness(3);
    rightRacket.setOutlineColor(Color::White);
    rightRacket.setFillColor(Color::Red);
    rightRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // ball measurements and positioning
    //****************************************************************
    CircleShape ball;
    ball.setRadius(ballRadi - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(Color::Black);
    ball.setFillColor(Color::Cyan);
    ball.setOrigin(ballRadi / 2, ballRadi / 2);
    //****************************************************************


    // the text font being loaded from the file
    //****************************************************************
    Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
    //****************************************************************


    // msg details etc
    //****************************************************************
    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setPosition(250.f, 250.f);
    msg.setFillColor(Color::White);
    msg.setString("Welcome to the pong game!\n\nPress ENTER to start the game\n\nplayer 1 is red\nplayer 2 is blue");
    //****************************************************************


    //Rackets and ball properties
    //****************************************************************
    Clock AITimer;                      //only for the computer to play
    const Time AITime = seconds(0.1f);
    const float racketSpeed = 400.f;
    const float ballSpeed = 400.f;
    float ballAngle = 0.f;          // gets calculated and changed later
    //****************************************************************



    //names of both of the players
     //****************************************************************
    Text p1name;
    p1name.setFont(font);
    p1name.setCharacterSize(20);
    p1name.setPosition(250.f, 220.f);
    p1name.setFillColor(Color::White);
    p1name.setString(name1);


    Text p2name;
    p2name.setFont(font);
    p2name.setCharacterSize(20);
    p2name.setPosition(250.f, 220.f);
    p2name.setFillColor(Color::White);
    p2name.setString(name2);       

    //****************************************************************



  
    //to maintain the scores of the two players
   //****************************************************************
    Text p1Score;
    p1Score.setFont(font);
    p1Score.setCharacterSize(50);
    p1Score.setPosition(600.f, 0.f);
    p1Score.setFillColor(Color::White); 


    Text p2Score;
    p2Score.setFont(font);
    p2Score.setCharacterSize(50);
    p2Score.setPosition(200.f, 0.f);
    p2Score.setFillColor(Color::White);
  
        
    //****************************************************************



    Clock clock;

    //****************************************************************
    bool isPlaying = false;         //is the game being played???
    int leftScore = 0;              //player1 score variable
    int rightScore = 0;             //player2 score variable
    int isScoreTen = false;
    //****************************************************************

  
        
    //outer while loop
    while (window.isOpen()) 
    {
     
        // my event
        Event e;


        //inner while loop
        while (window.pollEvent(e)) 
        {
            // Window closed or escape key pressed: exit
            if ((e.type == Event::Closed) || ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Enter key pressed: play
            if ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Enter))
            {
                if (!isPlaying)
                {
                    //start the game
                    isPlaying = true;
                    clock.restart();

                    // set the position of the rackets and ball
                    leftRacket.setPosition(10 + racketSize.x / 2, gameHght / 2);
                    rightRacket.setPosition(gameWid - 10 - racketSize.x / 2, gameHght / 2);
                    ball.setPosition(gameWid / 2, gameHght / 2);

                    // Reset the ball angle
                    do
                    {
                        // adjusting the ball's angle (pure maths thingy)
                        ballAngle = (rand() % 360) * 2 * pi / 360;
                    } while (abs(cos(ballAngle)) < 0.7f);
                }
            }
        }

        if (isPlaying )   
        {
            float dTime = clock.restart().asSeconds();      //deltaTime

            // Movement of the left player's racket
            if (Keyboard::isKeyPressed(Keyboard::W) && (leftRacket.getPosition().y - racketSize.y / 2 > 5.f))
            {
                leftRacket.move(0.f, -racketSpeed * dTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::S) && (leftRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f))
            {
                leftRacket.move(0.f, racketSpeed * dTime);
            }


            // Movement of the right player's racket
            //****************************************************************
            if (Keyboard::isKeyPressed(Keyboard::Up) && (rightRacket.getPosition().y - racketSize.y / 2 > 5.f))
            {
                rightRacket.move(0.f, -racketSpeed * dTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && (rightRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f))
            {
                rightRacket.move(0.f, racketSpeed * dTime);
            }

            //****************************************************************

           

            // ball movement
            float factor = ballSpeed * dTime;
            ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);

            // Check collisions (ball-frameEdges)
            if (ball.getPosition().x - ballRadi < 0.f)
            {
                
                rightScore++;
                ballAngle = -ballAngle;
                ball.setPosition(700,300);
                if (rightScore == 10)
                {
                    isPlaying = false;
                }
              
            }
            if (ball.getPosition().x + ballRadi > gameWid)
            {
               
                leftScore++;
                ballAngle = -ballAngle;
                ball.setPosition(100, 300);
                if (leftScore == 10)
                {
                    isPlaying = false;
                }
              
            }
            if (ball.getPosition().y - ballRadi < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadi + 0.1f);
            }
            if (ball.getPosition().y + ballRadi > gameHght)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHght - ballRadi - 0.1f);
            }


            // Check collisions (ball-rackets)
            // Left racket
            if (ball.getPosition().x - ballRadi < leftRacket.getPosition().x + racketSize.x / 2 &&
                ball.getPosition().x - ballRadi > leftRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= leftRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= leftRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > leftRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(leftRacket.getPosition().x + ballRadi + racketSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right racket
            if (ball.getPosition().x + ballRadi > rightRacket.getPosition().x - racketSize.x / 2 &&
                ball.getPosition().x + ballRadi < rightRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= rightRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= rightRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > rightRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightRacket.getPosition().x - ballRadi - racketSize.x / 2 - 0.1f, ball.getPosition().y);
            }

        }
        
        



        // background of the window
        window.clear(Color::Black);

        if (isPlaying)
        {
            // Draw the paddles, ball and scores of players after each shot
            window.draw(middleLine);
            window.draw(leftRacket);
            window.draw(rightRacket);
            window.draw(ball);
            
            string str1 = to_string(rightScore);        //typecasting (int to string) just to print in text form of sfml
            p1Score.setString(str1);

            string str2 = to_string(leftScore);
            p2Score.setString(str2);


            window.draw(p1Score);
            window.draw(p2Score);
           

        }
        //see if player1 has scored "10" the display the winning msg
        else if(rightScore==10)
        {
            window.draw(p1name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }
        //see if player2 has scored "10" the display the winning msg
        else if (leftScore == 10)
        {
            window.draw(p2name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }

        // Display all of the drawn(on the background buffer) things on the screen(on the front buffer)
        window.display();
    }
  


    return EXIT_SUCCESS;    //end of game
}
int twoPlayerHard()
{
    srand(static_cast<unsigned int>(time(NULL)));

    // framing the playing field and stuff
    //****************************************************************

    Vector2f racketSize(25, 100);
    Vector2f midLineSize(10, 600);
    const int gameWid = 800;
    const int gameHght = 600;
    float ballRadi = 10.f;
    const float pi = 3.14159f;
    //****************************************************************



    // Creating the window                           32 bits per pixel
    //****************************************************************
    RenderWindow window(VideoMode(gameWid, gameHght, 32), "FERRY'S Pong", Style::Titlebar | Style::Close);
    window.setVerticalSyncEnabled(true);
    //****************************************************************



    // sounf for collision
    //****************************************************************
    SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    Sound ballSound(ballSoundBuffer);
    //****************************************************************


    //the middle line on the ground
    //****************************************************************
    RectangleShape middleLine;
    middleLine.setSize(midLineSize - Vector2f(3, 3));
    middleLine.setOutlineThickness(2);
    middleLine.setOutlineColor(Color::Black);
    middleLine.setFillColor(Color::White);
    middleLine.setOrigin(midLineSize / 1.f);
    middleLine.setPosition(400, 600);
    //****************************************************************



    // left racket  measurements and positoning
    //****************************************************************
    RectangleShape leftRacket;
    leftRacket.setSize(racketSize - Vector2f(3, 3));
    leftRacket.setOutlineThickness(2);
    leftRacket.setOutlineColor(Color::White);
    leftRacket.setFillColor(Color::Blue);
    leftRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // right racket measurements and positoning
    //****************************************************************
    RectangleShape rightRacket;
    rightRacket.setSize(racketSize - Vector2f(3, 3));
    rightRacket.setOutlineThickness(3);
    rightRacket.setOutlineColor(Color::White);
    rightRacket.setFillColor(Color::Red);
    rightRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // ball measurements and positioning
    //****************************************************************
    CircleShape ball;
    ball.setRadius(ballRadi - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(Color::Black);
    ball.setFillColor(Color::White);
    ball.setOrigin(ballRadi / 2, ballRadi / 2);
    //****************************************************************


    // the text font being loaded from the file
    //****************************************************************
    Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
    //****************************************************************


    // msg details etc
    //****************************************************************
    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setPosition(250.f, 250.f);
    msg.setFillColor(Color::White);
    msg.setString("Welcome to FERRY'S pong game!\n\nPress ENTER to start the game\n\nplayer 1 is red\nplayer 2 is blue");
    //****************************************************************


    //Rackets properties
    //****************************************************************
    Clock AITimer;
    const Time AITime = seconds(0.1f);
    const float racketSpeed = 400.f;
    const float ballSpeed = 600.f;
    float ballAngle = 0.f;          // gets calculated and changed later
    //****************************************************************



    //names of both of the players
     //****************************************************************
    Text p1name;
    p1name.setFont(font);
    p1name.setCharacterSize(20);
    p1name.setPosition(250.f, 220.f);
    p1name.setFillColor(Color::White);
    p1name.setString(name1);


    Text p2name;
    p2name.setFont(font);
    p2name.setCharacterSize(20);
    p2name.setPosition(250.f, 220.f);
    p2name.setFillColor(Color::White);
    p2name.setString(name2);

    //****************************************************************




    //to maintain the scores of the two players
   //****************************************************************
    Text p1Score;
    p1Score.setFont(font);
    p1Score.setCharacterSize(50);
    p1Score.setPosition(600.f, 0.f);
    p1Score.setFillColor(Color::Yellow);


    Text p2Score;
    p2Score.setFont(font);
    p2Score.setCharacterSize(50);
    p2Score.setPosition(200.f, 0.f);
    p2Score.setFillColor(Color::Yellow);


    //****************************************************************



    Clock clock;

    //****************************************************************
    bool isPlaying = false;         //is the game being played???
    int leftScore = 0;              //player1 score variable
    int rightScore = 0;             //player2 score variable
    int isScoreTen = false;
    //****************************************************************



    //outer while loop
    while (window.isOpen())
    {

        // my event
        Event e;


        //inner while loop
        while (window.pollEvent(e))
        {
            // Window closed or escape key pressed: exit
            if ((e.type == Event::Closed) || ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Enter key pressed: play
            if ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Enter))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddles and ball
                    leftRacket.setPosition(10 + racketSize.x / 2, gameHght / 2);
                    rightRacket.setPosition(gameWid - 10 - racketSize.x / 2, gameHght / 2);
                    ball.setPosition(gameWid / 2, gameHght / 2);

                    // Reset the ball angle
                    do
                    {
                        // adjusting the ball's angle (pure maths thingy)
                        ballAngle = (rand() % 360) * 2 * pi / 360;
                    } while (abs(cos(ballAngle)) < 0.7f);
                }
            }
        }

        if (isPlaying)
        {
            float dTime = clock.restart().asSeconds();

            // Movement of the left player's racket
            if (Keyboard::isKeyPressed(Keyboard::W) && (leftRacket.getPosition().y - racketSize.y / 2 > 5.f))
            {
                leftRacket.move(0.f, -racketSpeed * dTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::S) && (leftRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f))
            {
                leftRacket.move(0.f, racketSpeed * dTime);
            }


            // Movement of the right player's racket
            //****************************************************************
            if (Keyboard::isKeyPressed(Keyboard::Up) && (rightRacket.getPosition().y - racketSize.y / 2 > 5.f))
            {
                rightRacket.move(0.f, -racketSpeed * dTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && (rightRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f))
            {
                rightRacket.move(0.f, racketSpeed * dTime);
            }

            //****************************************************************



            // ball movement
            float factor = ballSpeed * dTime;
            ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);

            // Check collisions (ball-frameEdges)
            if (ball.getPosition().x - ballRadi < 0.f)
            {

                rightScore++;
                ballAngle = -ballAngle;
                ball.setPosition(700, 300);
                if (rightScore == 10)
                {
                    isPlaying = false;
                }

            }
            if (ball.getPosition().x + ballRadi > gameWid)
            {

                leftScore++;
                ballAngle = -ballAngle;
                ball.setPosition(100, 300);
                if (leftScore == 10)
                {
                    isPlaying = false;
                }

            }
            if (ball.getPosition().y - ballRadi < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadi + 0.1f);
            }
            if (ball.getPosition().y + ballRadi > gameHght)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHght - ballRadi - 0.1f);
            }


            // Check collisions (ball-rackets)
            // Left racket
            if (ball.getPosition().x - ballRadi < leftRacket.getPosition().x + racketSize.x / 2 &&
                ball.getPosition().x - ballRadi > leftRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= leftRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= leftRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > leftRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(leftRacket.getPosition().x + ballRadi + racketSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right racket
            if (ball.getPosition().x + ballRadi > rightRacket.getPosition().x - racketSize.x / 2 &&
                ball.getPosition().x + ballRadi < rightRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= rightRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= rightRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > rightRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightRacket.getPosition().x - ballRadi - racketSize.x / 2 - 0.1f, ball.getPosition().y);
            }

        }





        // background of the window
        window.clear(Color::Black);

        if (isPlaying)
        {
            // Draw the paddles, ball and scores of players after each shot
            window.draw(middleLine);
            window.draw(leftRacket);
            window.draw(rightRacket);
            window.draw(ball);

            string str1 = to_string(rightScore);        //typecasting (int to string) just to print in text form of sfml
            p1Score.setString(str1);

            string str2 = to_string(leftScore);
            p2Score.setString(str2);


            window.draw(p1Score);
            window.draw(p2Score);


        }
        //see if player1 has scored "10" the display the winning msg
        else if (rightScore == 10)
        {
            window.draw(p1name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }
        //see if player2 has scored "10" the display the winning msg
        else if (leftScore == 10)
        {
            window.draw(p2name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }

        // Display all of the drawn(on the background buffer) things on the screen(on the front buffer)
        window.display();
    }



    return EXIT_SUCCESS;    //end of game
}
int computerPlayerEasy()
{
    srand(static_cast<unsigned int>(time(NULL)));

    // framing the playing field and stuff
    //****************************************************************

    Vector2f racketSize(25, 100);
    Vector2f midLineSize(10, 600);
    const int gameWid = 800;
    const int gameHght = 600;
    float ballRadi = 10.f;
    const float pi = 3.14159f;
    //****************************************************************



    // Creating the window                           32 bits per pixel
    //****************************************************************
    RenderWindow window(VideoMode(gameWid, gameHght, 32), "FAREEHA'S Pong", Style::Titlebar | Style::Close);
    window.setVerticalSyncEnabled(true);
    //****************************************************************



    // sounf for collision
    //****************************************************************
    SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    Sound ballSound(ballSoundBuffer);
    //****************************************************************


    //the middle line on the ground
    //****************************************************************
    RectangleShape middleLine;
    middleLine.setSize(midLineSize - Vector2f(3, 3));
    middleLine.setOutlineThickness(2);
    middleLine.setOutlineColor(Color::Black);
    middleLine.setFillColor(Color::White);
    middleLine.setOrigin(midLineSize / 1.f);
    middleLine.setPosition(400, 600);
    //****************************************************************



    // left racket  measurements and positoning
    //****************************************************************
    RectangleShape leftRacket;
    leftRacket.setSize(racketSize - Vector2f(3, 3));
    leftRacket.setOutlineThickness(2);
    leftRacket.setOutlineColor(Color::Black);
    leftRacket.setFillColor(Color::Blue);
    leftRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // right racket measurements and positoning
    //****************************************************************
    RectangleShape rightRacket;
    rightRacket.setSize(racketSize - Vector2f(3, 3));
    rightRacket.setOutlineThickness(3);
    rightRacket.setOutlineColor(Color::Black);
    rightRacket.setFillColor(Color::Red);
    rightRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // ball measurements and positioning
    //****************************************************************
    CircleShape ball;
    ball.setRadius(ballRadi - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(Color::Black);
    ball.setFillColor(Color::Magenta);
    ball.setOrigin(ballRadi / 2, ballRadi / 2);
    //****************************************************************


    // the text font being loaded from the file
    //****************************************************************
    Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
    //****************************************************************


    // msg details etc
    //****************************************************************
    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setPosition(250.f, 250.f);
    msg.setFillColor(Color::Blue);
    msg.setString("Welcome to the pong game!\n\nPress ENTER to start the game\n\nplayer 1 is red\nplayer 2 is blue");
    //****************************************************************


    //Rackets properties
    //****************************************************************
    Clock AITimer;
    const Time AITime = seconds(0.1f);
    const float racketSpeed = 400.f;
    float rightPaddleSpeed = 0.f;
    const float ballSpeed = 400.f;
    float ballAngle = 0.f;          // gets calculated and changed later
    //****************************************************************



    //names of both of the players
     //****************************************************************
    Text p1name;
    p1name.setFont(font);
    p1name.setCharacterSize(20);
    p1name.setPosition(250.f, 220.f);
    p1name.setFillColor(Color::Blue);
    p1name.setString(name1);


    Text p2name;
    p2name.setFont(font);
    p2name.setCharacterSize(20);
    p2name.setPosition(250.f, 220.f);
    p2name.setFillColor(Color::Blue);
    p2name.setString(name2);

    //****************************************************************




    //to maintain the scores of the two players
   //****************************************************************
    Text p1Score;
    p1Score.setFont(font);
    p1Score.setCharacterSize(50);
    p1Score.setPosition(600.f, 0.f);
    p1Score.setFillColor(Color::Black);


    Text p2Score;
    p2Score.setFont(font);
    p2Score.setCharacterSize(50);
    p2Score.setPosition(200.f, 0.f);
    p2Score.setFillColor(Color::Black);


    //****************************************************************



    Clock clock;

    //****************************************************************
    bool isPlaying = false;         //is the game being played???
    int leftScore = 0;              //player1 score variable
    int rightScore = 0;             //player2 score variable
    int isScoreTen = false;
    //****************************************************************



    //outer while loop
    while (window.isOpen())
    {

        // my event
        Event e;


        //inner while loop
        while (window.pollEvent(e))
        {
            // Window closed or escape key pressed: exit
            if ((e.type == Event::Closed) || ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Enter key pressed: play
            if ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Enter))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddles and ball
                    leftRacket.setPosition(10 + racketSize.x / 2, gameHght / 2);
                    rightRacket.setPosition(gameWid - 10 - racketSize.x / 2, gameHght / 2);
                    ball.setPosition(gameWid / 2, gameHght / 2);

                    // Reset the ball angle
                    do
                    {
                        // adjusting the ball's angle (pure maths thingy)
                        ballAngle = (rand() % 360) * 2 * pi / 360;
                    } while (abs(cos(ballAngle)) < 0.7f);
                }
            }
        }

        if (isPlaying)
        {
            float dTime = clock.restart().asSeconds();

            // Movement of the left player's racket
            if (Keyboard::isKeyPressed(Keyboard::Up) && (leftRacket.getPosition().y - racketSize.y / 2 > 5.f))
            {
                leftRacket.move(0.f, -racketSpeed * dTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && (leftRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f))
            {
                leftRacket.move(0.f, racketSpeed * dTime);
            }


            // Movement of the right player's racket
            //****************************************************************
            //****************************************************************
            // Movement of the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightRacket.getPosition().y - racketSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f)))
            {
                rightRacket.move(0.f, rightPaddleSpeed * dTime);
            }

            //  computer's paddle direction relative to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadi > rightRacket.getPosition().y + racketSize.y / 2)
                    rightPaddleSpeed = racketSpeed;
                else if (ball.getPosition().y - ballRadi < rightRacket.getPosition().y - racketSize.y / 2)
                    rightPaddleSpeed = -racketSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }
            //****************************************************************
            //****************************************************************



            // ball movement
            float factor = ballSpeed * dTime;
            ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);

            // Check collisions (ball-frameEdges)
            if (ball.getPosition().x - ballRadi < 0.f)
            {

                rightScore++;
                ballAngle = -ballAngle;
                ball.setPosition(700, 300);
                if (rightScore == 10)
                {
                    isPlaying = false;
                }

            }
            if (ball.getPosition().x + ballRadi > gameWid)
            {

                leftScore++;
                ballAngle = -ballAngle;
                ball.setPosition(100, 300);
                if (leftScore == 10)
                {
                    isPlaying = false;
                }

            }
            if (ball.getPosition().y - ballRadi < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadi + 0.1f);
            }
            if (ball.getPosition().y + ballRadi > gameHght)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHght - ballRadi - 0.1f);
            }


            // Check collisions (ball-rackets)
            // Left racket
            if (ball.getPosition().x - ballRadi < leftRacket.getPosition().x + racketSize.x / 2 &&
                ball.getPosition().x - ballRadi > leftRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= leftRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= leftRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > leftRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(leftRacket.getPosition().x + ballRadi + racketSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right racket (COMPUTER)
            //****************************************************************
            //****************************************************************
            if (ball.getPosition().x + ballRadi > rightRacket.getPosition().x - racketSize.x / 2 &&
                ball.getPosition().x + ballRadi < rightRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= rightRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= rightRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > rightRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightRacket.getPosition().x - ballRadi - racketSize.x / 2 - 0.1f, ball.getPosition().y);
            }

            //****************************************************************
            //****************************************************************

        }





        // background of the window
        window.clear(Color::Cyan);

        if (isPlaying)
        {
            // Draw the paddles, ball and scores of players after each shot
            window.draw(middleLine);
            window.draw(leftRacket);
            window.draw(rightRacket);
            window.draw(ball);

            string str1 = to_string(rightScore);        //typecasting (int to string) just to print in text form of sfml
            p1Score.setString(str1);

            string str2 = to_string(leftScore);
            p2Score.setString(str2);


            window.draw(p1Score);
            window.draw(p2Score);


        }
        //see if player1 has scored "10" the display the winning msg
        else if (rightScore == 10)
        {
            window.draw(p1name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }
        //see if player2 has scored "10" the display the winning msg
        else if (leftScore == 10)
        {
            window.draw(p2name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }

        // Display all of the drawn(on the background buffer) things on the screen(on the front buffer)
        window.display();
    }



    return EXIT_SUCCESS;    //end of game
}
int computerPlayerHard()
{
    srand(static_cast<unsigned int>(time(NULL)));

    // framing the playing field and stuff
    //****************************************************************

    Vector2f racketSize(25, 100);
    Vector2f midLineSize(10, 600);
    const int gameWid = 800;
    const int gameHght = 600;
    float ballRadi = 10.f;
    const float pi = 3.14159f;
    //****************************************************************



    // Creating the window                           32 bits per pixel
    //****************************************************************
    RenderWindow window(VideoMode(gameWid, gameHght, 32), "FERRY'S Pong", Style::Titlebar | Style::Close);
    window.setVerticalSyncEnabled(true);
    //****************************************************************



    // sounf for collision
    //****************************************************************
    SoundBuffer ballSoundBuffer;
    if (!ballSoundBuffer.loadFromFile("resources/ball.wav"))
        return EXIT_FAILURE;
    Sound ballSound(ballSoundBuffer);
    //****************************************************************


    //the middle line on the ground
    //****************************************************************
    RectangleShape middleLine;
    middleLine.setSize(midLineSize - Vector2f(3, 3));
    middleLine.setOutlineThickness(2);
    middleLine.setOutlineColor(Color::Black);
    middleLine.setFillColor(Color::White);
    middleLine.setOrigin(midLineSize / 1.f);
    middleLine.setPosition(400, 600);
    //****************************************************************



    // left racket  measurements and positoning
    //****************************************************************
    RectangleShape leftRacket;
    leftRacket.setSize(racketSize - Vector2f(3, 3));
    leftRacket.setOutlineThickness(2);
    leftRacket.setOutlineColor(Color::Black);
    leftRacket.setFillColor(Color::Blue);
    leftRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // right racket measurements and positoning
    //****************************************************************
    RectangleShape rightRacket;
    rightRacket.setSize(racketSize - Vector2f(3, 3));
    rightRacket.setOutlineThickness(3);
    rightRacket.setOutlineColor(Color::Black);
    rightRacket.setFillColor(Color::Red);
    rightRacket.setOrigin(racketSize / 2.f);
    //****************************************************************



    // ball measurements and positioning
    //****************************************************************
    CircleShape ball;
    ball.setRadius(ballRadi - 3);
    ball.setOutlineThickness(3);
    ball.setOutlineColor(Color::Black);
    ball.setFillColor(Color::Magenta);
    ball.setOrigin(ballRadi / 2, ballRadi / 2);
    //****************************************************************


    // the text font being loaded from the file
    //****************************************************************
    Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;
    //****************************************************************


    // msg details etc
    //****************************************************************
    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setPosition(250.f, 250.f);
    msg.setFillColor(Color::Blue);
    msg.setString("Welcome to FERRY'S pong game!\n\nPress ENTER to start the game\n\nplayer 1 is red\nplayer 2 is blue");
    //****************************************************************


    //Rackets properties
    //****************************************************************
    Clock AITimer;
    const Time AITime = seconds(0.1f);
    const float racketSpeed = 400.f;
    float rightPaddleSpeed = 0.f;
    const float ballSpeed = 600.f;
    float ballAngle = 0.f;          // gets calculated and changed later
    //****************************************************************



    //names of both of the players
     //****************************************************************
    Text p1name;
    p1name.setFont(font);
    p1name.setCharacterSize(20);
    p1name.setPosition(250.f, 220.f);
    p1name.setFillColor(Color::Blue);
    p1name.setString(name1);


    Text p2name;
    p2name.setFont(font);
    p2name.setCharacterSize(20);
    p2name.setPosition(250.f, 220.f);
    p2name.setFillColor(Color::Blue);
    p2name.setString(name2);

    //****************************************************************




    //to maintain the scores of the two players
   //****************************************************************
    Text p1Score;
    p1Score.setFont(font);
    p1Score.setCharacterSize(50);
    p1Score.setPosition(600.f, 0.f);
    p1Score.setFillColor(Color::Black);


    Text p2Score;
    p2Score.setFont(font);
    p2Score.setCharacterSize(50);
    p2Score.setPosition(200.f, 0.f);
    p2Score.setFillColor(Color::Black);


    //****************************************************************



    Clock clock;

    //****************************************************************
    bool isPlaying = false;         //is the game being played???
    int leftScore = 0;              //player1 score variable
    int rightScore = 0;             //player2 score variable
    int isScoreTen = false;
    //****************************************************************



    //outer while loop
    while (window.isOpen())
    {

        // my event
        Event e;


        //inner while loop
        while (window.pollEvent(e))
        {
            // Window closed or escape key pressed: exit
            if ((e.type == Event::Closed) || ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Enter key pressed: play
            if ((e.type == Event::KeyPressed) && (e.key.code == Keyboard::Enter))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();

                    // Reset the position of the paddles and ball
                    leftRacket.setPosition(10 + racketSize.x / 2, gameHght / 2);
                    rightRacket.setPosition(gameWid - 10 - racketSize.x / 2, gameHght / 2);
                    ball.setPosition(gameWid / 2, gameHght / 2);

                    // Reset the ball angle
                    do
                    {
                        // adjusting the ball's angle (pure maths thingy)
                        ballAngle = (rand() % 360) * 2 * pi / 360;
                    } while (abs(cos(ballAngle)) < 0.7f);
                }
            }
        }

        if (isPlaying)
        {
            float dTime = clock.restart().asSeconds();

            // Movement of the left player's racket
            if (Keyboard::isKeyPressed(Keyboard::Up) && (leftRacket.getPosition().y - racketSize.y / 2 > 5.f))
            {
                leftRacket.move(0.f, -racketSpeed * dTime);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && (leftRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f))
            {
                leftRacket.move(0.f, racketSpeed * dTime);
            }


            // Movement of the right player's racket
            //****************************************************************
            //****************************************************************
            // Move the computer's paddle
            if (((rightPaddleSpeed < 0.f) && (rightRacket.getPosition().y - racketSize.y / 2 > 5.f)) ||
                ((rightPaddleSpeed > 0.f) && (rightRacket.getPosition().y + racketSize.y / 2 < gameHght - 5.f)))
            {
                rightRacket.move(0.f, rightPaddleSpeed * dTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadi > rightRacket.getPosition().y + racketSize.y / 2)
                    rightPaddleSpeed = racketSpeed;
                else if (ball.getPosition().y - ballRadi < rightRacket.getPosition().y - racketSize.y / 2)
                    rightPaddleSpeed = -racketSpeed;
                else
                    rightPaddleSpeed = 0.f;
            }
            //****************************************************************
            //****************************************************************



            // ball movement
            float factor = ballSpeed * dTime;
            ball.move(cos(ballAngle) * factor, sin(ballAngle) * factor);

            // Check collisions (ball-frameEdges)
            if (ball.getPosition().x - ballRadi < 0.f)
            {

                rightScore++;
                ballAngle = -ballAngle;
                ball.setPosition(700, 300);
                if (rightScore == 10)
                {
                    isPlaying = false;
                }

            }
            if (ball.getPosition().x + ballRadi > gameWid)
            {

                leftScore++;
                ballAngle = -ballAngle;
                ball.setPosition(100, 300);
                if (leftScore == 10)
                {
                    isPlaying = false;
                }

            }
            if (ball.getPosition().y - ballRadi < 0.f)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadi + 0.1f);
            }
            if (ball.getPosition().y + ballRadi > gameHght)
            {
                ballSound.play();
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHght - ballRadi - 0.1f);
            }


            // Check collisions (ball-rackets)
            // Left racket
            if (ball.getPosition().x - ballRadi < leftRacket.getPosition().x + racketSize.x / 2 &&
                ball.getPosition().x - ballRadi > leftRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= leftRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= leftRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > leftRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(leftRacket.getPosition().x + ballRadi + racketSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right racket (COMPUTER)
            //****************************************************************
            //****************************************************************
            if (ball.getPosition().x + ballRadi > rightRacket.getPosition().x - racketSize.x / 2 &&
                ball.getPosition().x + ballRadi < rightRacket.getPosition().x &&
                ball.getPosition().y + ballRadi >= rightRacket.getPosition().y - racketSize.y / 2 &&
                ball.getPosition().y - ballRadi <= rightRacket.getPosition().y + racketSize.y / 2)
            {
                if (ball.getPosition().y > rightRacket.getPosition().y)
                    ballAngle = pi - ballAngle + (rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (rand() % 20) * pi / 180;

                ballSound.play();
                ball.setPosition(rightRacket.getPosition().x - ballRadi - racketSize.x / 2 - 0.1f, ball.getPosition().y);
            }

            //****************************************************************
            //****************************************************************

        }





        // background of the window
        window.clear(Color::Cyan);

        if (isPlaying)
        {
            // Draw the paddles, ball and scores of players after each shot
            window.draw(middleLine);
            window.draw(leftRacket);
            window.draw(rightRacket);
            window.draw(ball);

            string str1 = to_string(rightScore);        //typecasting (int to string) just to print in text form of sfml
            p1Score.setString(str1);

            string str2 = to_string(leftScore);
            p2Score.setString(str2);


            window.draw(p1Score);
            window.draw(p2Score);


        }
        //see if player1 has scored "10" the display the winning msg
        else if (rightScore == 10)
        {
            window.draw(p1name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }
        //see if player2 has scored "10" the display the winning msg
        else if (leftScore == 10)
        {
            window.draw(p2name);
            msg.setString("Won!\n\nPress ENTER to play again or\nESCAPE to exit");
            window.draw(msg);

        }

        // Display all of the drawn(on the background buffer) things on the screen(on the front buffer)
        window.display();
    }



    return EXIT_SUCCESS;    //end of game
}

class MyGame
{
    int type;
public:
    void setType()
    {
        cout << "How do you want to play???\n\n";
        cout << "1) PLAYER to PLAYER (easy version)\n";
        cout << "2) PLAYER to PLAYER (HARD version)\n";
        cout << "3) PLAYER to COMPUTER (easy version)\n";
        cout << "4) PLAYER to COMPUTER (HARD version)\n";
        cout << endl << endl << endl;

        cout << "ENTER 1,2,3 or 4 accordingly: ";
        cin >> type;
        
    }

    int getType()
    {
        return type;
    }
};


int main()
{
   
    MyGame game1;

    game1.setType();

    int version = -1;
    version= game1.getType();

    

    if (version < 1 || version>4)
    {
        cout << "INVALID CHOICE OF VERSION!";
        exit(0);
    }

    if (version == 1 || version == 2)
    {
       cout << "enter name of player 1: ";
       cin >> name1;
       cout << "enter name of player 2: ";
       cin >> name2;

       if (version == 1)
           twoPlayerEasy();
       else
           twoPlayerHard();


    }

    else
    {
        name1 = "Computer";
        cout << "enter name of player : ";
        cin >> name2;

        if (version == 3)
            computerPlayerEasy();
        else
            computerPlayerHard();
            
        

    }

    

    
    
  

    system("pause>0");
    return 0;
}



