#include <SDL.h>
#include <vector>
#include <string>
#include <cstdlib>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

class Circle {
public:
    float x, y, radius;
    float velocityx, velocityY;
    SDL_Color color;

    MovingCircle(float x, float y, float radius, const SDL_Color& color)
    : x(x), y(y), radius(radius), color(color){
        velocityX = (rand()%100)-50;
        velocityY = (rand()%100)-50;
    }

    
    void update(float dt){
        x += velocityX*dt;
        y += velocityY*dt;

        if(x-radius < 0 || x+radius > WINDOW_WIDTH){
            velocityX = -velocityX;
        }
        if (y-radius < 0 || y+radius > WINDOW_HEIGHT){
            velocityY = -velocityY;
        }
    }

    // Renderiza el circulo en la pantalla 
    void render(SDL_Renderer* renderer) {
        // Itera sobre todos los pixeles del circulo
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                    SDL_RenderDrawPoint(renderer, x + dx, y + dy);
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<MovingCircle> circles;
    for (int i = 0; i < 100; i++) {
        SDL_Color randomColor = { Uint8(rand() % 256), Uint8(rand() % 256), Uint8(rand() % 256), 255 };
        circles.push_back(MovingCircle(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, 10 + rand() % 20, randomColor));
    }

    // Calculo FPS
    uint32_t frameCount = 0;
    uint32_t startTime = SDL_GetTicks();
    uint32_t currentTime = startTime;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& circle : circles) {
            circle.update(0.016f);  // Asumiendo 60 fps
            circle.render(renderer);
        }

        SDL_RenderPresent(renderer);

        frameCount++;

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
