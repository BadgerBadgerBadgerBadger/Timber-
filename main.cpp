#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf;

#define SCREEN_WIDTH 1680
#define SCREEN_HEIGHT 1048

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite spriteBranches[NUM_BRANCHES];

enum class Side {LEFT, RIGHT, NONE};
Side branchPositions[NUM_BRANCHES];

int main() {

    VideoMode vm(SCREEN_WIDTH, SCREEN_HEIGHT);
    RenderWindow window(vm, "Timber!");
    std::vector<VideoMode> gf = VideoMode::getFullscreenModes();

    int score = 0;
    Text messageText;
    Text scoreText;

    Font font;
    font.loadFromFile("assets/fonts/KOMIKAP_.ttf");

    messageText.setFont(font);
    scoreText.setFont(font);

    messageText.setString("Press Enter to start!");
    scoreText.setString(("Score = 0"));

    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(50);

    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(
        textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f
    );
    messageText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
    scoreText.setPosition(20, 20);

    Texture texBackground;
    texBackground.loadFromFile("assets/graphics/background.png");

    Sprite spriteBackground;
    spriteBackground.setTexture(texBackground);
    spriteBackground.setPosition(0, 0);

    Texture texTree;
    texTree.loadFromFile("assets/graphics/tree.png");

    Sprite spriteTree;
    spriteTree.setTexture(texTree);
    spriteTree.setPosition(690,0);

    Texture texBee;
    texBee.loadFromFile("assets/graphics/bee.png");

    Sprite spriteBee;
    spriteBee.setTexture(texBee);
    spriteBee.setPosition(SCREEN_WIDTH * 2, 0);

    bool beeActive = false;
    float beeSpeed = 0.0f;

    Texture texCloud;
    texCloud.loadFromFile("assets/graphics/cloud.png");

    Sprite spriteCloud;
    spriteCloud.setTexture(texCloud);
    spriteCloud.setPosition(SCREEN_WIDTH * 2, 0);

    bool cloudActive = false;
    float cloudSpeed = 0.0f;

    Texture texBranch;
    texBranch.loadFromFile("assets/graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++) {

        spriteBranches[i].setTexture(texBranch);
        spriteBranches[i].setPosition(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
        spriteBranches[i].setOrigin(220, 20);
    }

    Texture texPlayer;
    texPlayer.loadFromFile("assets/graphics/player.png");

    Sprite spritePlayer;
    spritePlayer.setTexture(texPlayer);
    spritePlayer.setPosition(1080, 720);

    Side playerSide = Side::LEFT;

    Texture texRIP;
    texRIP.loadFromFile("assets/graphics/rip.png");

    Sprite spriteRIP;
    spriteRIP.setTexture(texRIP);
    spriteRIP.setPosition(SCREEN_WIDTH * 2, 860);

    const float RIP_POSITION_LEFT = 500;
    const float RIP_POSITION_RIGHT = 1080;

    Texture texAxe;
    texAxe.loadFromFile("assets/graphics/axe.png");

    Sprite spriteAxe;
    spriteAxe.setTexture(texAxe);
    spriteAxe.setPosition(SCREEN_WIDTH * 2, 830);

    const float AXE_POSITION_LEFT = 540;
    const float AXE_POSITION_RIGHT = 975;

    Texture texLog;
    texLog.loadFromFile("assets/graphics/log.png");

    Sprite spriteLog;
    spriteLog.setTexture(texLog);
    spriteLog.setPosition(690, 830);

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("assets/sound/chop.wav");

    Sound chopSound;
    chopSound.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("assets/sound/death.wav");

    Sound deathSound;
    deathSound.setBuffer(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("assets/sound/out_of_time.wav");

    Sound ootSound;
    ootSound.setBuffer(ootBuffer);

    bool acceptInput;

    Clock clock;
    bool paused = true;

    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarStartHeight = 80;

    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarStartHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((SCREEN_WIDTH / 2) - timeBarStartWidth / 2, SCREEN_HEIGHT - timeBarStartHeight - 10);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    while (window.isOpen()) {

        Event event{};

        if (window.pollEvent(event)) {

            if (event.type == Event::KeyReleased && !paused) {
                acceptInput = true;
                spriteAxe.setPosition(SCREEN_WIDTH *  2, spriteAxe.getPosition().y);
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape) || event.type == Event::Closed) {
                window.close();
            }

            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                paused = false;

                score = 0;
                timeRemaining = 5;

                for (int i = 0; i < NUM_BRANCHES; i++) {
                    branchPositions[i] = Side::NONE;
                }
            }

            if (!paused) {
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    playerSide = Side::RIGHT;
                    score++;

                    timeRemaining += (2 / score) + 0.15;

                    spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(1080, 720);
                    spritePlayer.setScale(1, 1);

                    updateBranches(score);

                    spriteLog.setPosition(690, 720);
                    logSpeedX = -5000;
                    logActive = true;

                    acceptInput = false;

                    chopSound.play();
                }
                else if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    playerSide = Side::LEFT;
                    score++;

                    timeRemaining += (2 / score) + 0.15;

                    spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                    spritePlayer.setPosition(580, 720);
                    spritePlayer.setScale(-1, 1);

                    updateBranches(score);

                    spriteLog.setPosition(690, 720);
                    logSpeedX = 5000;
                    logActive = true;

                    acceptInput = false;
                    chopSound.play();
                }
            }
        }

        Time dt = clock.restart();

        // Update the scene
        if (!paused) {

            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarStartHeight));

            if (timeRemaining <= 0.0f) {
                paused = true;

                messageText.setString("Out of Time!");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(
                        textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f
                );
                messageText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);

                ootSound.play();
            }

            if (!beeActive) {

                // How fast is the bee
                srand((int)time(0) * 10);
                beeSpeed  = (rand() % 300) +  300;

                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 400) + 400;

                spriteBee.setPosition(SCREEN_WIDTH + 300, height);
                beeActive = true;
            }
            else {

                spriteBee.setPosition(
                        spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                        spriteBee.getPosition().y
                );

                // Has bee reached right-hand edge of screen
                if (spriteBee.getPosition().x < -100) {
                    beeActive = false;
                }
            }

            if (!cloudActive) {

                srand((int)time(0) * 10);
                cloudSpeed = (rand() % 100) + 100;

                srand((int)time(0) * 10);
                float height = (rand() % 200);

                spriteCloud.setPosition(SCREEN_WIDTH + 200, height);
                cloudActive = true;
            }
            else {

                spriteCloud.setPosition(
                        spriteCloud.getPosition().x - (cloudSpeed * dt.asSeconds()),
                        spriteCloud.getPosition().y
                );

                // Has bee reached right-hand edge of screen
                if (spriteCloud.getPosition().x < -300) {
                    cloudActive = false;
                }
            }

            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;

                if (branchPositions[i] == Side::LEFT) {
                    spriteBranches[i].setPosition(470, height);
                    spriteBranches[i].setRotation(180);
                }
                else if (branchPositions[i] == Side::RIGHT) {
                    spriteBranches[i].setPosition(1190, height);
                    spriteBranches[i].setRotation(0);
                }
                else {
                    spriteBranches[i].setPosition(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2);
                }
            }

            if (branchPositions[5] == playerSide) {
                paused = true;
                logActive = false;
                acceptInput = false;

                if (playerSide == Side::LEFT) {
                    spriteRIP.setPosition(RIP_POSITION_LEFT, 760);
                } else if (playerSide == Side::RIGHT) {
                    spriteRIP.setPosition(RIP_POSITION_RIGHT, 760);
                }

                spritePlayer.setPosition(SCREEN_WIDTH * 2, 0);
                spriteAxe.setPosition(SCREEN_WIDTH * 2, 0);

                messageText.setString("SQUISHED");

                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(
                        textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f
                );
                messageText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
                scoreText.setPosition(20, 20);

                deathSound.play();
            }

            if (logActive) {
                spriteLog.setPosition(
                    spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())
                );

                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > SCREEN_WIDTH + 200) {
                    logActive = false;
                    spriteLog.setPosition(690, 720);
                }
            }
        }

        // Draw the scene

        window.clear(Color::Black);

        window.draw(spriteBackground);

        window.draw(spriteCloud);

        window.draw(spriteTree);
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(spriteBranches[i]);
        }

        window.draw(spritePlayer);
        window.draw(spriteRIP);
        window.draw(spriteAxe);
        window.draw(spriteLog);

        window.draw(spriteBee);

        window.draw(scoreText);
        window.draw(timeBar);

        if (paused) {
            window.draw(messageText);
        }

        window.display();
    }

    return 0;
}

void updateBranches(int seed) {

    for (int i = NUM_BRANCHES - 1; i >= 0; i--) {
        branchPositions[i] = branchPositions[i - 1];
    }

    srand((int)time(0) + seed);
    int r = (rand()% 5 );

    switch (r) {
        case 0:
            branchPositions[0] = Side::LEFT;
            break;

        case 1:
            branchPositions[0] = Side::RIGHT;
            break;

        default:
            branchPositions[0] = Side::NONE;
            break;
    }
}
