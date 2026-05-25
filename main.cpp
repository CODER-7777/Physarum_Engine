#include <iostream>
#include <cstdint>

// Define constant height and width for the simulation space
const int WIDTH = 1024;
const int HEIGHT = 1024;

// This function checks whether the given coordinates (x, y) lie within the simulation bounds
bool validCoordinates(int x, int y)
{
    return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

// 2D to 1D : updates the value for the flattened grid
void setTrail(float *grid, int x, int y, float value)
{
    // Check bounds so you do not access invalid memory locations
    if (validCoordinates(x, y))
    {
        grid[y * WIDTH + x] = value;
    }
}

// 2D to 1D : returns the value of trail at coordinate (x, y) if they are within the bounds, otherwise returns 0.0f.
float getTrail(const float *grid, int x, int y)
{
    if (validCoordinates(x, y))
    {
        return grid[y * WIDTH + x];
    }
    else
    {
        return 0.0f;
    }
}

int main()
{
    std::cout << "Initializing Memory for Physarum Engine..." << std::endl;

    // The State Grid : Grid(i, j) = 0 for Air, 1 for Wall, 2 for Food
    // The Trail Grid : Each grid point stores the floating point value of the pheromone trail.
    uint8_t* stateGrid = new uint8_t[WIDTH * HEIGHT]();
    float* trailGrid = new float[WIDTH * HEIGHT]();

    std::cout << "Memory Allocated Successfully" << std::endl;

    // Sandbox Experiment

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (x == y) {
                setTrail(trailGrid, x, y, 1.0f);
            }
        }
    }

    // Verify the math worked
    std::cout << "Trail at (500, 500): " << getTrail(trailGrid, 500, 500) << std::endl; // Should be 1
    std::cout << "Trail at (500, 501): " << getTrail(trailGrid, 500, 501) << std::endl; // Should be 0

    // Free the Heap memory to prevent leaks
    delete[] stateGrid;
    delete[] trailGrid;
    
    std::cout << "Memory freed. Engine shutdown." << std::endl;
    return 0;
}