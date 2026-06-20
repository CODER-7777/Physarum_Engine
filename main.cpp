#include<iostream>
#include<vector>
#include<cmath>
#include<random>
#include"Engine.hpp"

const int WIDTH=1024;
const int HEIGHT=1024;
const int NUM_AGENTS=10000;
const float PI = 3.14159265359f;

using namespace std;

struct Agent {
    float x, y;
    float angle;
    
    // Tuning Parameters
    float speed = 2.0f;
    float turnAngle = PI / 10.0f;
    float sensorAngleOffset = PI / 3.0f;
    float sensorDistance = 20.0f;

    float getPheromone(const float* readGrid, int sensorX, int sensorY) {
        if (sensorX < 0 || sensorX >= WIDTH || sensorY < 0 || sensorY >= HEIGHT) {
            return -10.0f; // Repulsion from walls
        }
        return readGrid[sensorY * WIDTH + sensorX];
    }

    void senseAndSteer(const float* readGrid, mt19937& gen, uniform_real_distribution<float>& disDir) {
        // 1. Calculate angles
        float sensorAngleLeft = angle + sensorAngleOffset;
        float sensorAngleFwd = angle;
        float sensorAngleRight = angle - sensorAngleOffset;

        // 2. Cast sensors
        int leftSensorX = static_cast<int>(x + sensorDistance * cos(sensorAngleLeft));
        int leftSensorY = static_cast<int>(y + sensorDistance * sin(sensorAngleLeft));
        
        int fwdSensorX = static_cast<int>(x + sensorDistance * cos(sensorAngleFwd));
        int fwdSensorY = static_cast<int>(y + sensorDistance * sin(sensorAngleFwd));

        int rightSensorX = static_cast<int>(x + sensorDistance * cos(sensorAngleRight));
        int rightSensorY = static_cast<int>(y + sensorDistance * sin(sensorAngleRight));

        // 3. Read Grid
        float weightL = getPheromone(readGrid, leftSensorX, leftSensorY);
        float weightF = getPheromone(readGrid, fwdSensorX, fwdSensorY);
        float weightR = getPheromone(readGrid, rightSensorX, rightSensorY);

        // 4. Steer! (Jeff Jones Algorithm)
        if (weightF > weightL && weightF > weightR) {
            // Keep going straight
        } else if (weightF < weightL && weightF < weightR) {
            // Randomly turn left or right
            if (disDir(gen) > 0.5f) {
                angle += turnAngle;
            } else {
                angle -= turnAngle;
            }
        } else if (weightL > weightR) {
            angle += turnAngle;
        } else if (weightR > weightL) {
            angle -= turnAngle;
        } else {
            // They are equal (e.g. head on to a wall), pick random to break symmetry
            if (disDir(gen) > 0.5f) {
                angle += turnAngle;
            } else {
                angle -= turnAngle;
            }
        }
        agents[i].vx=tangentX*speed;
        agents[i].vy=tangentY*speed;
    }
};

int main(){
    RenderEngine engine(WIDTH,HEIGHT);
    if(!engine.init()){
        cerr<<"Failed to initialize engine!"<<endl;
        return -1;
    }
    
    // Phase 1: Allocate TWO grids
    float* readGrid = new float[WIDTH * HEIGHT]();
    float* writeGrid = new float[WIDTH * HEIGHT]();
    
    Agent* agents = new Agent[NUM_AGENTS];
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> disPos(0.0f, static_cast<float>(WIDTH));
    uniform_real_distribution<float> disDir(0.0f, 2.0f * PI);

    // Initialize Agents Randomly
    for(int i = 0; i < NUM_AGENTS; i++){
        agents[i].x = disPos(gen);
        agents[i].y = disPos(gen);
        agents[i].angle = disDir(gen);
    }
    
    while(!engine.shouldClose()){
        
        // Step A: Diffuse (Blur) and Decay the Environment
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                float sum = 0.0f;
                int count = 0;
                
                // 3x3 Blur loop
                for (int offsetY = -1; offsetY <= 1; offsetY++) {
                    for (int offsetX = -1; offsetX <= 1; offsetX++) {
                        int sampleX = x + offsetX;
                        int sampleY = y + offsetY;
                        
                        if (sampleX >= 0 && sampleX < WIDTH && sampleY >= 0 && sampleY < HEIGHT) {
                            sum += readGrid[sampleY * WIDTH + sampleX];
                            count++;
                        }
                    }
                }
                
                float averageBlur = sum / count;
                
                // Apply Decay
                float decayFactor = 0.99f;
                float finalValue = averageBlur * decayFactor;
                
                writeGrid[y * WIDTH + x] = finalValue;
            }
        }

        // Step B: Update Agents
        for (int i = 0; i < NUM_AGENTS; ++i) {
            // 1. Smell the environment and rotate
            agents[i].senseAndSteer(readGrid, gen, disDir);
            
            // 2. Step forward
            agents[i].x += cos(agents[i].angle) * agents[i].speed;
            agents[i].y += sin(agents[i].angle) * agents[i].speed;
            
            // 3. Boundary Checks (Safety net)
            if(agents[i].x < 0) { agents[i].x = 0; agents[i].angle += PI; }
            if(agents[i].x >= WIDTH) { agents[i].x = WIDTH - 1; agents[i].angle += PI; }
            if(agents[i].y < 0) { agents[i].y = 0; agents[i].angle += PI; }
            if(agents[i].y >= HEIGHT) { agents[i].y = HEIGHT - 1; agents[i].angle += PI; }

            // 4. Draw to the Write Grid
            int gridX = static_cast<int>(agents[i].x);
            int gridY = static_cast<int>(agents[i].y);
            if (gridX >= 0 && gridX < WIDTH && gridY >= 0 && gridY < HEIGHT) {
                writeGrid[gridY * WIDTH + gridX] = 1.0f; // Deposit pheromone
            }
        }

        // Step C: Render the writeGrid
        engine.renderFrame(writeGrid);

        // Step D: Swap the buffers!
        float* temp = readGrid;
        readGrid = writeGrid;
        writeGrid = temp;
    }
    
    delete[] readGrid;
    delete[] writeGrid;
    delete[] agents;
    engine.shutdown();
    return 0;
}