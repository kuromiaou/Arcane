#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace sf;



Clock clockSpawn;

int length = 864;
int height = 571;
int ground = 220;

string
textr_player_right = "Images/Sprout Lands - Sprites - Basic pack/Characters/MC/chara_R.png"
, textr_player_left = "Images/Sprout Lands - Sprites - Basic pack/Characters/MC/chara_L.png"
, textr_player_front = "Images/Sprout Lands - Sprites - Basic pack/Characters/MC/chara.png"
, textr_bg = "Images/Sprout Lands - Sprites - Basic pack/Map/Bridge.jpg"
, textr_ennemy_right = "Images/Sprout Lands - Sprites - Basic pack/Characters/MC/cow_right0.png"
, textr_ennemy_left = "Images/Sprout Lands - Sprites - Basic pack/Characters/cow_left0.png";
Texture player_right, player_left, player_front, enemy_right, enemy_left;


const float gravitySpeed = 0.3f;

class Background {
private:
    Texture texture;
    Sprite sprite;
public:
    void loadTexture(string file) {
        texture.loadFromFile(file);
    }
    void setSpriteTexture() {
        sprite.setTexture(texture);
    }
    void drawTo(RenderWindow& window) {
        window.draw(sprite);
    }
    Vector2u getSize() { return texture.getSize(); }

};


class Enemy {
protected:
    string name;
    int hp = 3, hpMax = 3, atkCooldown = 1;
    FloatRect bounds;
    Vector2f direction;
    Vector2f Behaviour;

public:
    Texture Text;
    Texture erer;
    Sprite sprite;
    IntRect rectSourceSprite;
    Enemy(int x, int y, Texture Text, Vector2f _BEHAVIOUR) : Behaviour(_BEHAVIOUR) { sprite.setTexture(Text); sprite.setPosition(x, y); }

    Texture loadTexture(string file) {
        erer.loadFromFile(file);
    }
    void setSpriteTexture(IntRect rectSourceSprite) {
        sprite.setTexture(erer);
        sprite.setTextureRect(rectSourceSprite);
    }
    void deacreaseHp(int i = 1) {
        hp -= i;
    }
    void decreaseAtkCooldown(int i = 1) {
        if (atkCooldown > 0) atkCooldown -= i;
        if (atkCooldown < 0) atkCooldown = 0;
    }
    void drawTo(RenderWindow& window) {
        window.draw(sprite);
    }
    void move(Vector2f distance) {
        sprite.move(distance);
    }
    void setPos(int x, int y) {
        sprite.setPosition(x, y);
    }
    Vector2f getPos() { return sprite.getPosition(); }
    Sprite getSprite() { return sprite; }
    Vector2f getBehaviour() { return Behaviour; }
    void reverseBehaviour() { Behaviour *= -1.f; }
};
class Player {
protected:
    string name;
    int hp, hpMax, atkCooldown = 1;
    Texture texture_right, texture_left, texture_front;
    FloatRect bounds;
    Vector2f direction;

public:
    bool isJumping = false;
    Sprite sprite;
    IntRect rectSourceSprite;
    Player(string _NAME, int _HP = 5, int _HPMAX = 5)
        : name(_NAME), hp(_HP), hpMax(_HPMAX) {
        sprite.setScale(2, 2);
    };
    void loadTextureRight(string file) {
        texture_right.loadFromFile(file);
    }
    void loadTextureLeft(string file) {
        texture_right.loadFromFile(file);
    }
    void loadTextureFront(string file) {
        texture_front.loadFromFile(file);
    }
   
    void move(Vector2f distance) {
        sprite.move(distance);
    }
    void setPos(int x, int y) {
        sprite.setPosition(x, y);
    }
    void deacreaseHp(int i = 1) {
        hp -= i;
    }
    void decreaseAtkCooldown(int i = 1) {
        if (atkCooldown > 0) atkCooldown -= i;
        if (atkCooldown < 0) atkCooldown = 0;
    }
    void drawTo(RenderWindow& window) {
        window.draw(sprite);
    }
    int getY() { return sprite.getPosition().y; }
    Vector2f getPos() { return sprite.getPosition(); }
    int feet() { return sprite.getGlobalBounds().top + sprite.getGlobalBounds().height; }
    bool isColliding(Enemy other) {
        if (sprite.getGlobalBounds().intersects(other.sprite.getGlobalBounds())) return true;
        else return false;
    }
};

class Manager {
private:
    vector<Enemy*> allEnnemies;
public:
    ~Manager() {
        for (auto objet : allEnnemies)
            delete objet;
        allEnnemies.clear();
    }
    Enemy* createEntity(int x, int y, Texture Text, Vector2f Behaviour) { 
        Enemy* obj = new Enemy(x, y, Text, Behaviour);
        allEnnemies.push_back(obj);
        return obj;
    }
    void createEnemy(int x, int y, Texture Text, Vector2f Behaviour) {
        createEntity(x, y, Text, Behaviour);
    }
    vector<Enemy*> getAllEnemy() { return allEnnemies; }
    void deleteEnemy(Enemy* ennemi) {
        auto it = find(allEnnemies.begin(), allEnnemies.end(), ennemi);
        if (it != allEnnemies.end()) {
            delete* it;
            allEnnemies.erase(it);
        }
    }
};

int main() {
    Clock clockPlayer;
    srand(time(0));
    // Création de la fenêtre
    Clock clock;
    RenderWindow window(VideoMode(length, height, 32), "Arcane Survivor");
    Image icon; icon.loadFromFile("Images/Sprout Lands - Sprites - Basic pack/icons/jinx.jpg");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Texture frames[200];
    Font font;
    font.loadFromFile("Font/happycow.ttf");
    Text lose;
    lose.setFont(font);
    lose.setCharacterSize(50);
    lose.setPosition({200,10});
    lose.setString("YOU LOSE.");
    for (int i = 1; i < 200; ++i) {
        if (i <= 9) {
            frames[i].loadFromFile("Images/ending/ezgif-frame-00" + to_string(i) + ".png");
        }
        if (i >= 10 && i <= 99) {
            frames[i].loadFromFile("Images/ending/ezgif-frame-0" + to_string(i) + ".png");
        }
        if (i >= 100 && i <= 200) {
            frames[i].loadFromFile("Images/ending/ezgif-frame-" + to_string(i) + ".png");
        }
        if (i >= 200) {
        }
    }

    Sprite sprite2;
    int frameIndex = 0;
    Clock clock2;    
    Manager manager;
    Background bg;
    Player player("owo");
    player_front.loadFromFile(textr_player_front);
    player_right.loadFromFile(textr_player_right);
    player_left.loadFromFile(textr_player_left);
    bg.loadTexture(textr_bg);
    bg.setSpriteTexture();
    player.setPos(450, 220);
    bool isGrounded = true;

    SoundBuffer come_playSB, jazzSB;
    come_playSB.loadFromFile("Sounds/Music/wav/comeplay-straykids.wav");
    jazzSB.loadFromFile("Sounds/Music/wav/gandalf.wav");
    Sound come_play(come_playSB);
    Sound jazz(jazzSB);
    jazz.setLoop(true);
    come_play.setLoop(true);
    come_play.setVolume(50.f);
    come_play.play();
    RectangleShape rect;
    rect.setSize(Vector2f(100, 5));
    rect.setOutlineColor(Color::Red);
    rect.setOutlineThickness(5);
    rect.setPosition(400, 20);
    CircleShape circle;
    circle.setRadius(10);
    circle.setPosition(rect.getPosition().x + rect.getSize().x / 2 - 5, rect.getPosition().y - rect.getSize().y / 2 - 5);
    bool onVolume = false;    
    Vector2f direction = Vector2f(0, 0);
    float timeUpward = 0.f;
    // Boucle principale    
    while (window.isOpen()) {
        float moveSpeed = 200.f;
        Time timeSinceLastFrame = clock.restart();
        if (clockSpawn.getElapsedTime().asSeconds() > 3.f) {
            float pos = rand() % 2 + 1;
            if (pos > 1)
            {
                cout << "right ";
                manager.createEnemy(10, 210, enemy_left, Vector2f(1.f, 0));
            }
            else if (pos <= 1)
            {
                cout << "left ";
                manager.createEnemy(850, 220, enemy_left, Vector2f(-1.f, 0));
            }
            clockSpawn.restart();
        }
        for (Enemy* ennemi : manager.getAllEnemy()) {
            ennemi->move(ennemi->getBehaviour() * timeSinceLastFrame.asSeconds() * moveSpeed / 2.f);
            if (ennemi->getPos().x >= 864)
                ennemi->reverseBehaviour();
            if (ennemi->getPos().x <= 0)
                ennemi->reverseBehaviour();
            if (player.isColliding(*ennemi)) {
                window.clear();
                window.draw(lose);
                window.display();
                come_play.pause();
                jazz.play();
                while (true) {
                    
                    RenderWindow videoWindow(VideoMode(600, 338), "Easter Egg");
                    while (videoWindow.isOpen()) {
                        Event event;
                        while (videoWindow.pollEvent(event)) {
                            if (event.type == Event::Closed)
                                videoWindow.close();
                        }
                        if (clock2.getElapsedTime().asMilliseconds() > 50) {
                            frameIndex = (frameIndex + 1) % 200;
                            sprite2.setTexture(frames[frameIndex]);
                            clock2.restart();
                            if (frameIndex >= 199) {
                                videoWindow.close();
                            }
                        }

                        videoWindow.clear();
                        videoWindow.draw(sprite2);
                        videoWindow.display();
                    }
                }

            }
            else jazz.play();
        }

        Event event, e;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close(); 

        if (player.getPos().y >= 220) {
            isGrounded = true;
            player.isJumping = false;
        }
        else {
            isGrounded = false;
        }
        if (isGrounded)
        {
            if (Keyboard::isKeyPressed(Keyboard::delocalize(Keyboard::Space))) {
                player.isJumping = true;
                isGrounded = false;
            }
        }

        direction = Vector2f(0, 0);

        if (Keyboard::isKeyPressed(Keyboard::delocalize(Keyboard::Q))) {
            player.sprite.setTexture(player_left);
            player.sprite.setTextureRect(IntRect(0, 0, 10, 16));
            if (clockPlayer.getElapsedTime().asSeconds() > 0.750f) {

                if (player.rectSourceSprite.left == 30)
                    player.rectSourceSprite.left = 0;
                else
                {
                    player.rectSourceSprite.left += 10;
                }

                player.sprite.setTextureRect(player.rectSourceSprite);
                clockPlayer.restart();
            }
            direction += Vector2f(-1.f, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::delocalize(Keyboard::D))) {
            player.sprite.setTexture(player_right);
            player.sprite.setTextureRect(IntRect(0, 0, 10, 16));
            if (clockPlayer.getElapsedTime().asSeconds() > 0.750f) {

                if (player.rectSourceSprite.left == 30)
                    player.rectSourceSprite.left = 0;
                else
                {
                    player.rectSourceSprite.left += 10;
                }

                player.sprite.setTextureRect(player.rectSourceSprite);
                clockPlayer.restart();
            }
            direction += Vector2f(1.f, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::delocalize(Keyboard::S))) {
            player.sprite.setTexture(player_front);
            player.sprite.setTextureRect(IntRect(0, 0, 13, 16));
        }
        if (Keyboard::isKeyPressed(Keyboard::delocalize(Keyboard::LShift)) || Keyboard::isKeyPressed(Keyboard::delocalize(Keyboard::RShift)))
        {
            moveSpeed *= 2;
            cout << "La touche shift a ete pressee!" << endl;
        }
        if (player.isJumping) {
            direction += Vector2f(0.f, -1.2f);
        }
        if (!isGrounded) {
            direction += Vector2f(0.f, gravitySpeed);
        }
        player.move(direction * moveSpeed * timeSinceLastFrame.asSeconds());
        if (direction.y < 0) {
            timeUpward += timeSinceLastFrame.asSeconds();
        }
        if (timeUpward >= 1.1f) {
            player.isJumping = false;
            timeUpward = 0;
        }
        
        Vector2f iddd = player.getPos();
        if (iddd.x < 0)                                                             // x<15 -> x=15,y=y
            player.setPos(0, player.getPos().y);
        if (iddd.x > 844)                                                           // x>864(length of bg=windowshape) -> x=844,y=y
            player.setPos(844, player.getPos().y);
        if (iddd.y > ground)                    // first layer on the left ([15;244],265)
            player.setPos(player.getPos().x, ground);
        if (iddd.y > bg.getSize().y)
            player.setPos(player.getPos().x, ground);
        if (iddd.y < 0)
            player.setPos(player.getPos().x, 0);
                
        window.clear();
        
        enemy_left.loadFromFile(textr_ennemy_left);
        bg.drawTo(window);
        player.drawTo(window);
        for (Enemy* ennemi : manager.getAllEnemy()) {
            ennemi->sprite.setTexture(enemy_left);
            window.draw(ennemi->sprite);
        }

        if (Mouse::isButtonPressed(Mouse::Left) and onVolume) {
            if ((Mouse::getPosition(window).x >= rect.getPosition().x) and (Mouse::getPosition(window).x <= rect.getSize().x + rect.getPosition().x)) {
                circle.setPosition(Mouse::getPosition(window).x, circle.getPosition().y);
                come_play.setVolume(circle.getPosition().x - rect.getPosition().x);
            }
        }
        if (circle.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y) or rect.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y)) {
            onVolume = true;
        }
        else if (!Mouse::isButtonPressed(Mouse::Left)) { onVolume = false; }

        window.draw(rect);
        window.draw(circle);
        window.display();
    }
    return 0;
}
