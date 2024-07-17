#include<raylib.h>
#include<cstdlib>
#include<ctime>
#include<unistd.h>
#include<cmath>
#include<iostream>

const double pi = 3.14;
const int fps = 60;
const int screenWidth = 1200;
const int screenHeight = 700;
int playerScore = 0;
int cpuScore = 0;

class Ball;
class Paddle;
class PlayerPaddle;
class CPUPaddle;
class Line;

void collision (Ball&, Paddle);

class Ball{

    double x, y;
    double radius;
    double veloX, veloY;
    double theta;
    double rad;
    double ballVelocity;
    Color colour;
    
    public:
    friend class CPUPaddle;
    friend void collision(Ball&, Paddle);
    Ball():x(screenWidth/2), y(screenHeight/2), radius(20),ballVelocity(23), colour(WHITE){
        srand(time(nullptr));
        theta = (rand() % 150) - 75; // (rand() % (Max_value - Min_value + 1) ) + Min_value
    }

    void draw(){
        x += veloX;
        y += veloY;
        DrawCircle(x, y, radius, WHITE);
        //std::cout << x << "\t" << y << "\tth=" << theta <<"\tvx=" <<veloX <<"\tvy="<<veloY << "\t" <<cos(theta)<<"\t"<< sin(theta) <<"\n";
    }

    void reset(){
        srand(time(nullptr));
        theta = (rand() % 150) - 75;
        x = screenWidth/2;
        y = screenHeight/2;
        ClearBackground(BLACK);
        draw();
        sleep(1);
        
    }
    void update(){
        if (y <= radius || y >= screenHeight - radius)
            theta *= (-1);
        if (x <= radius){
            cpuScore++;
            theta = 180 - theta;
            reset();
        }
        else if(x >= screenWidth - radius){
            playerScore++;
            theta = 180 - theta;
            ballVelocity++;
            reset();
        }
            
        rad = theta * (pi / 180);
        veloX = ballVelocity*cos(rad);
        veloY = ballVelocity*sin(rad);
        while (theta > 360)
            theta -= 360;
        while (theta < (-360))
            theta += 360;

    }

};

class Paddle{

    protected:
    double x, y;
    double width, height;
    int speed;
    Color colour;

    public:
    friend void collision(Ball &, Paddle);
    Paddle():y(screenHeight/2 -60), width(25), height(120), speed(18),colour(WHITE){
    }
    void draw(){
        DrawRectangle(x, y, width, height, colour);
    }
    //virtual void update() = 0;
};


class CPUPaddle: public Paddle{
    public:
    CPUPaddle(){x = screenWidth - 30 - width; }
    void update(Ball ball){
        if (ball.y < (y + height/2) && y > 0 )
            y -= speed;
        if (ball.y > (y + height/2) && y < (screenHeight - height))
            y += speed;
    }
};


class PlayerPaddle : public Paddle{

    public:
    PlayerPaddle(){x = 30; };
    void update(){
        if (IsKeyDown(KEY_UP) && y > 0 )
            y -= speed;
        if (IsKeyDown(KEY_DOWN) && y < (screenHeight - height))
            y += speed;
    }
};

class Line{
    int x1, y1;
    int x2, y2;
    Color colour;
    public:
    Line ():x1(screenWidth/2), y1(0), x2(screenWidth/2), y2(screenHeight), colour(WHITE){}
    Line(int a, int b, int c, int d):x1(a),y1(b),x2(c),y2(d), colour(WHITE){}
    void draw(){
        DrawLine(x1, y1, x2, y2, colour);
    }
};


int main(){

    Ball * ball = new Ball;
    PlayerPaddle player;
    CPUPaddle cpu;
    Line middleLine;
    Line topLine(0,1 ,screenWidth,1);
    Line bottomLine(0,screenHeight ,screenWidth,screenHeight);
    int checkcpuScore {},checkplayerScore {};
    InitWindow(screenWidth,screenHeight,"Basic Pong Game");
    SetTargetFPS(fps);
    sleep(1);
    
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        middleLine.draw();
        topLine.draw();
        bottomLine.draw();
        ball->update();
        DrawText(TextFormat("%i",playerScore), screenWidth/4-20, 20, 80,WHITE);
        DrawText(TextFormat("%i",cpuScore), 3*screenWidth/4-20, 20, 80,WHITE);

        ball->draw();
        player.draw();
        player.update();
        cpu.draw();
        cpu.update(*ball);
        collision(*ball, player);
        collision(*ball, cpu);
        EndDrawing();
    }
    delete ball;
    CloseWindow();
    return 0;

} 

void collision(Ball &colball, Paddle colpaddle){
    if(CheckCollisionCircleRec(Vector2{colball.x, colball.y }, colball.radius, Rectangle{colpaddle.x, colpaddle.y, colpaddle.width, colpaddle.height})){
        if(colball.y < (colpaddle.y - colball.radius) || colball.y > (colpaddle.y + colpaddle.height + colball.radius)){
            colball.theta *= -1;
        }
        /*else if(colball.y < colpaddle.y+20 || colball.y >= colpaddle.y + colpaddle.height -20){
        colball.theta -=90;
        //colball.theta = 180 -colball.theta;  
        return;
        }*/
        colball.theta = 180 -colball.theta;    
        return; 
    }
}
