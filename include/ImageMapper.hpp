#pragma once
#include <iostream>
#include "Agent.hpp" 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// function which takes the filepath of the image file and modifies terrainGrid accordingly
inline bool loadTerrainMap(const char* filepath, uint8_t* terrainGrid, int expectedWidth, int expectedHeight) {
    int imgWidth, imgHeight, channels;
    
    // Force the image to load as 1 channel (grayscale as of now)
    unsigned char* data = stbi_load(filepath, &imgWidth, &imgHeight, &channels, 1); 
    
    // If the data fails to load or the dimensions don't match, throw an error.
    if (!data || imgWidth != expectedWidth || imgHeight != expectedHeight) {
        std::cerr << "Failed to load map or dimensions mismatch!" << std::endl;
        if(data) stbi_image_free(data);
        return false;
    }

    // Write the data to terrain grid.
    for (int y = 0; y < expectedHeight; ++y) {
        for (int x = 0; x < expectedWidth; ++x) {
            // We use the inverted y coordinate so our image and Terrain Grids match
            // If we were to simply use 'y' you would see an inverted obstacle grid.
            int pixelValue = data[(expectedHeight - 1 - y) * expectedWidth + x];
            
            if (pixelValue < 50) terrainGrid[y * expectedWidth + x] = OBSTACLE;
            else terrainGrid[y * expectedWidth + x] = AIR;
        }
    }
    
    stbi_image_free(data); // IMPORTANT. This closes the opened image after mapping.
    return true;
}
