/*
AIR HOCKEY GAME
=====================
1. Class ball
functions
- draw
- update
- resetball
2. class paddle
functions
- limitmovement (protected)
- draw
- update
3. cpu paddle : inherited from paddle
functions
- update

y axis / height
up(--)
|
|
|
down(++)
_________ x axis / width
left(--)....right(++)
=====================
*/
#include <iostream>
#include <raylib.h> 
// library for all raylib functions


using namespace std;

int player_score = 0; // score for player
int cpu_score = 0; // score for cpu

class Ball {
public:
    float x, y; // x and y axis for ball
    int speed_x, speed_y; // x and y axis speed increase for movement
    int radius; // radius of ball

    void Draw() {
        DrawCircle(x, y, radius, /*WHITE*/ BLACK); // to make ball
    }

    void Update() {
        x += speed_x; // increase speed in x axis
        y += speed_y; // increase speed in y axis

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1; // if ball y axis + radius[push y axis at top of circle] > height of screen or ball-radius[pushes the y axis to tip of circle] > height of screen
        }  // the ball y direction is reversed (*-1)
        if (x + radius >= GetScreenWidth()) {
            cpu_score++; // if ball x axis goes towards right > widthofscreen, cpu score
            Resetball();
        }
        if (x - radius <= 0) {
            player_score++; // if ball y axis goes left < 0 , player ++
            Resetball();
        }
    }

    void Resetball() {
        x = GetScreenWidth() / 2; // middle of screen height
        y = GetScreenHeight() / 2; // middle of screen width

        int speed_choices[2] = {-1, 1}; // array of values -1 and 1
        speed_x = speed_choices[GetRandomValue(0, 1)] * 7; // generate rand val either 0th or 1st postion of array, then multiply with 7
        speed_y = speed_choices[GetRandomValue(0, 1)] * 7; // generates rand val for y axis
    }
};

class Paddle {
protected:
    void limitmovement() {
        if (y <= 0) { // if paddle y axis goes below 0, reset it to zero instanly
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height; // if paddle + (height/2) > screen height , then keep it below
        }
    }

public:
    float x, y;
    float width, height; // height and width of paddle
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, /*WHITE*/BLACK); // create paddle
    }

    void Update() {
        if (IsKeyDown(KEY_UP)) {
            cout << "Up key detected. Moving paddle up." << endl;
            y -= speed; // computer logic opposite of traditional math, therfore y(--) means up and y(++) means down
        }
        if (IsKeyDown(KEY_DOWN)) { // is key down func check if arrow is pressed returns true
            cout << "Down key detected. Moving paddle down." << endl;
            y += speed;
        }

        cout << "Player paddle Y position before limitmovement: " << y << endl;
        limitmovement(); // func called after update to make sure paddle not out of bounds
        cout << "Player paddle Y position after limitmovement: " << y << endl;
    }
};

class cpupaddle : public Paddle { // inheritance from class paddle
public:
    void Update(int ball_y) {
        if (y + height / 2 > ball_y) {  // if (y axis of ball + (h/2)[push y to tip of paddle] > y axis of ball, then y(--) meaning y go up )
            y -= speed;
        }
        if (y + height / 2 <= ball_y) {
            y += speed; // this func ensures that, when y axis of ball is above or below the yaxis of paddle, the paddle moves accordingly
        }
        limitmovement();
    }
};

int main() {
    cout << "Starting the game !" << endl;
    const int scr_width = 1200;
    const int scr_height = 800;
    InitWindow(scr_width, scr_height, "Air Hockey Game");
    SetTargetFPS(60); // 60 frames per second, the loop is running 

    Ball ball; // create obj of class
    Paddle player;
    cpupaddle cpu; // class cpupaddle obj gets functions from paddle using inheritance 
    
    // creating ball wrt screen height and width
    ball.radius = 20;
    ball.x = scr_width / 2;
    ball.y = scr_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    // positioning the player paddle on right side(pov its left side for me) 
    player.width = 25;
    player.height = 120;
    player.x = scr_width - player.width - 10; // 10 is the extra gap 
    player.y = scr_height / 2 - player.height / 2; 
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;  // Place CPU paddle on the left side(pov its right side for me)
    cpu.y = scr_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    while (!WindowShouldClose()) { // till i press close , loop runs 

        /*
        Game loop logic:
        1. BeginDraw func - Starts a new frame
        2. Update the postions of ball,player paddle, cpu paddle
        3. After update, check for collison
        4. clear background before showing elements
        5. Draw elements
        6. draw text for scoreboard
        7. end drawing [Displays the output]
        8. loop
        */


        BeginDrawing(); // start drawing a new frame 

        // Update game objects
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Check for collisions
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})) {
            ball.speed_x *= -1; // if circle collide with rectangle inbuilt function, x axis speed is inverted(*1)
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) { // bool CheckCollisionCircleRec(Vector2 center, float radius, Rectangle rec);
            ball.speed_x *= -1; // if ball hit cpu paddle , the above one is ball hit player paddle
        }

        // Render game objects
        ClearBackground(/*BLACK*/WHITE); // refresh the screen after each update to erase the old movements

        DrawLine(scr_width / 2, 0, scr_width / 2, scr_height, /*WHITE*/BLACK); // MIDDLE line, void DrawLine(int startPosX, int startPosY, int endPosX, int endPosY, Color color);
        ball.Draw(); // create ball
        cpu.Draw(); // craete cpu paddle
        player.Draw(); // create player paddle

        // Draw scores
        DrawText(TextFormat("%i", cpu_score), scr_width / 4 - 20, 20, 80, /*WHITE*/BLACK); // void DrawText(const char *text, int posX, int posY, int fontSize, Color color);

        DrawText(TextFormat("%i", player_score), 3 * scr_width / 4 - 20, 20, 80, /*WHITE*/BLACK);

        EndDrawing(); // begin then end drawing to help loop again
    }

    CloseWindow();
    return 0;
}




