#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include "Agent.hpp"
#include "ImageMapper.hpp"
#include "Engine.hpp"

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int NUM_AGENTS = 10000;
const float PI = 3.14159265359f;
const float REPULSION = -10.0f;
const float MAX_SPEED = 2.0f;
const float DECAY_FACTOR = 0.99f;

using namespace std;

// Helper function to draw zones
void addZone(uint8_t *terrainGrid, int centerX, int centerY, int radius, TerrainType type) {
    for (int i = centerX - radius; i <= centerX + radius; i++) {
        for (int j = centerY - radius; j <= centerY + radius; j++) {
            if (validCoords(i, j)) terrainGrid[j * WIDTH + i] = type;
        }
    }
}

inline float getRandomFloat(float min, float max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int main(int argc, char** argv) {
    RenderEngine engine(WIDTH, HEIGHT);
    if (!engine.init()) {
        cerr << "Failed to initialize engine!" << endl;
        return -1;
    }

    uint8_t *terrainGrid = new uint8_t[WIDTH * HEIGHT](); // Initialize the Terrain Grid

    // Only initialize terrain grid with something if an image file was added as a command line argument
    if (argc >= 2)
    {
        if (!loadTerrainMap(argv[1], terrainGrid, WIDTH, HEIGHT)) {
            // If mapping failed, just fill with AIR
            std::fill(terrainGrid, terrainGrid + (WIDTH * HEIGHT), AIR);
        }
    }
    else // otherwise fill it with AIR
    {
        std::fill(terrainGrid, terrainGrid + (WIDTH * HEIGHT), AIR);
    }
    
    int HomeX = WIDTH / 2;
    int HomeY = HEIGHT / 2;
    int HomeRadius = 15;

    // Draw 4 Food Targets in the corners, and 1 Home Base in the center
    addZone(terrainGrid, 50, 50, 15, FOOD);
    addZone(terrainGrid, 50, HEIGHT - 50, 15, FOOD);
    addZone(terrainGrid, WIDTH - 50, 50, 15, FOOD);
    addZone(terrainGrid, WIDTH - 50, HEIGHT - 50, 15, FOOD);
    addZone(terrainGrid, HomeX, HomeY, HomeRadius, HOME);

    // RGB Grids (3 channels)
    float* readGrid = new float[WIDTH * HEIGHT * 3]();
    float* writeGrid = new float[WIDTH * HEIGHT * 3]();
    
    Agent* agents = new Agent[NUM_AGENTS];

    // Spawn Agents INSIDE the Home Base
    for (int i = 0; i < NUM_AGENTS; i++) {
        agents[i].x = getRandomFloat(HomeX - HomeRadius, HomeX + HomeRadius);
        agents[i].y = getRandomFloat(HomeY - HomeRadius, HomeY + HomeRadius);
        agents[i].angle = getRandomFloat(0.0, 2.0 * PI);
        agents[i].speed = getRandomFloat(0.8f * MAX_SPEED, MAX_SPEED);
        agents[i].hasFood = false; // Start hungry!
    }
    
    while (!engine.shouldClose()) {
        
        // Step A: Diffuse and Decay for all 3 Channels
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                // Iterate through Red (0), Green (1), and Blue (2)
                for (int c = 0; c < 3; c++) { 
                    float sum = 0.0f;
                    int count = 0;
                    for (int offsetX = -1; offsetX <= 1; offsetX++) {
                        for (int offsetY = -1; offsetY <= 1; offsetY++) {
                            int X = x + offsetX;
                            int Y = y + offsetY;

                            if (validCoords(X, Y)) {
                                sum += readGrid[(Y * WIDTH + X) * 3 + c];
                                count++;
                            }
                        }
                    }
                    float averageBlur = sum / (float)count;
                    float finalValue = averageBlur * DECAY_FACTOR;
                    finalValue = std::min(finalValue, 5.0f); // Prevent infinite brightness
                    writeGrid[(y * WIDTH + x) * 3 + c] = finalValue;
                }
            }
        }

        // Step B: Update Agents
        for (int i = 0; i < NUM_AGENTS; ++i) {
            
            // Move Forward
            agents[i].x += cos(agents[i].angle) * agents[i].speed;
            agents[i].y += sin(agents[i].angle) * agents[i].speed;
            
            // Boundary Safety
            if(agents[i].x < 0) { agents[i].x = 0; agents[i].angle += PI; }
            if(agents[i].x >= WIDTH) { agents[i].x = WIDTH - 1; agents[i].angle += PI; }
            if(agents[i].y < 0) { agents[i].y = 0; agents[i].angle += PI; }
            if(agents[i].y >= HEIGHT) { agents[i].y = HEIGHT - 1; agents[i].angle += PI; }

            agents[i].senseAndSteer(readGrid, terrainGrid);
            agents[i].detectState(terrainGrid);
            agents[i].deposit(writeGrid);
        }

        // Step C: Render
        engine.renderFrame(writeGrid);

        // Step D: Swap buffers
        float* temp = readGrid;
        readGrid = writeGrid;
        writeGrid = temp;
    }
    
    delete[] terrainGrid;
    delete[] readGrid;
    delete[] writeGrid;
    delete[] agents;
    engine.shutdown();
    return 0;
}