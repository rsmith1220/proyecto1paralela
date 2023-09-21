#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <omp.h>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Class representing a moving circle
class MovingCircle {
public:
    float x, y, radius;
    float velocityX, velocityY;
    SDL_Color color;

    MovingCircle(float x, float y, float radius, const SDL_Color& color)
        : x(x), y(y), radius(radius), color(color) {
        velocityX = (rand() % 100) - 50; // random between -50 and 50
        velocityY = (rand() % 100) - 50;
    }

    // Update the position of the circle based on its velocity
    void update(float dt) {
        x += velocityX * dt;
        y += velocityY * dt;

        // Bounce off the edges
        if (x - radius < 0 || x + radius > WINDOW_WIDTH) {
            velocityX = -velocityX;
        }
        if (y - radius < 0 || y + radius > WINDOW_HEIGHT) {
            velocityY = -velocityY;
        }
    }

    void render(SDL_Renderer* renderer) {
        // This is a simple way to render circles using SDL's draw points function.
        // For better performance, consider using SDL_gfx or another library.
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
    int numThreads;
    /*if (argc > 1) {
        // Convert the first argument to an integer and set the number of threads
        numThreads = std::stoi(argv[1]);
        omp_set_num_threads(numThreads);
    }*/
    SDL_Init(SDL_INIT_VIDEO);
    int circulos;
    do {
    cout << "Ingrese la cantidad de circulos: ";
    cin >> circulos;

    // Clear the input buffer if the user enters something that's not an integer
    if(cin.fail()) {
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        cout << "Por favor, ingrese un número válido." << endl;
        continue;
    }

    if (circulos <= 0) {
        cout << "El número debe ser positivo y mayor que cero. Inténtalo de nuevo." << endl;
    }

} while (circulos <= 0);

    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<MovingCircle> circles;
    for (int i = 0; i < circulos; i++) {
        SDL_Color randomColor = { Uint8(rand() % 256), Uint8(rand() % 256), Uint8(rand() % 256), 255 };
        circles.push_back(MovingCircle(rand() % WINDOW_WIDTH, rand() % WINDOW_HEIGHT, 10 + rand() % 20, randomColor));
    }

    // For FPS calculation and capping
    const int TARGET_FPS = 144;
    const int TARGET_FRAME_DURATION = 1000 / TARGET_FPS;
    uint32_t frameCount = 0;
    uint32_t startTime = SDL_GetTicks();
    uint32_t currentTime = startTime;

    bool running = true;
    while (running) {
        uint32_t frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        #pragma omp parallel for num_threads(8)
        for (auto& circle : circles) {
            circle.update(0.016f);  // Assuming ~60FPS for simplicity
        }

        // Rendering cannot be done in parallel due to SDL limitations
        for (auto& circle : circles) {
            circle.render(renderer);
        }

        SDL_RenderPresent(renderer);

        frameCount++;

        if (SDL_GetTicks() - currentTime > 1000) {
            char title[100];
            snprintf(title, sizeof(title), "Screensaver - FPS: %d", frameCount);
            SDL_SetWindowTitle(window, title);
            frameCount = 0;
            currentTime += 1000;
        }

        // Frame rate capping to 144 fps
        int frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < TARGET_FRAME_DURATION) {
            SDL_Delay(TARGET_FRAME_DURATION - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
