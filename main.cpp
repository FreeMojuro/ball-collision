#include <raylib.h>
#include <vector>
#include <cmath>

const unsigned int window_width{1200};
const unsigned int window_height{800};


class Ball{
    public:

    float x;
    float y;
    float x_spd;
    float y_spd;
    float radius;

    Ball(float x, float y, float xspd, float yspd, float rad): x{x}, y{y}, x_spd{xspd}, y_spd{yspd}, radius{rad}{}

    void Drop(){
        DrawCircle(x, y, radius, WHITE);
    }

    void Update(){
        if(y + radius > window_height){y = window_height - radius; y_spd*=-1;}
        else if(y - radius < 0){y = radius; y_spd*=-1;}
        if(x + radius > window_width){x = window_width - radius; x_spd*=-1;}
        else if(x - radius < 0){x = radius; x_spd*=-1;}

        x += x_spd;
        y += y_spd;
        


    }


};



int main(){
    InitWindow(window_width, window_height, "Simulation");
    SetTargetFPS(60);

    std::vector<Ball> balls{};
    


    while(!WindowShouldClose()){

        if(IsKeyPressed(KEY_SPACE)){
            Ball new_ball{
                static_cast<float>(window_width) / 2.0f,
                static_cast<float>(window_height) / 2.0f,
                static_cast<float>(GetRandomValue(-10, 10)),
                static_cast<float>(GetRandomValue(-10, 10)),
                20.0f
            };
            balls.push_back(new_ball);
        }

        BeginDrawing();
        for(auto& ball: balls){ball.Update();}

        for(int i{0}; i < balls.size(); ++i){
            for(int j{i + 1}; j < balls.size(); ++j){

                Ball& ball1 = balls[i];
                Ball& ball2 = balls[j];

                float dx{ball2.x - ball1.x};
                float dy{ball2.y - ball1.y};
                float mindist{ball1.radius + ball2.radius};
                float dist{std::sqrt(dx*dx + dy*dy)};

                if(dist < mindist && dist > 0.0f){
                    float overlap = 0.5f * (mindist - dist);
                    float nx = dx / dist;
                    float ny = dy / dist;

                    ball1.x -= nx * overlap;
                    ball1.y -= ny * overlap;
                    ball2.x += nx * overlap;
                    ball2.y += ny * overlap;

                    float kx = ball1.x_spd - ball2.x_spd;
                    float ky = ball1.y_spd - ball2.y_spd;
                    float p = 2.0f * (nx * kx + ny * ky) / 2.0f;

                    ball1.x_spd -= p * nx;
                    ball1.y_spd -= p * ny;
                    ball2.x_spd += p * nx;
                    ball2.y_spd += p * ny;
                }
                
            }
        }
        

        ClearBackground(BLACK);

        for(auto& ball: balls){
            ball.Drop();
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
