# Physarum Engine: Multi-Agent Reinforcement Learning Simulator

## Overview
The Physarum Engine is a high-performance biological simulation built in C++ and OpenGL. It models the emergent behavior, pathfinding, and network optimization capabilities of the *Physarum polycephalum* (slime mold) using a custom Multi-Agent Reinforcement Learning (MARL) framework driven by Stigmergy.

## Technical Architecture

### Core Technologies
- **C++17:** Core simulation logic, data structures, and memory management.
- **OpenGL 3.3 & GLFW:** Hardware-accelerated graphics rendering pipeline.
- **GLAD:** OpenGL function pointer management.
- **stb_image:** Lightweight texture and dynamic environment parsing.
- **CMake:** Cross-platform build system and dependency management.

### Key Features
1. **Multi-Agent Ecosystem:** Simulates 10,000 independent biological agents in real-time.
2. **Stigmergy & Chemotaxis:** Agents communicate indirectly by modifying their environment (depositing pheromones), utilizing a Temporal Difference (TD) learning approach to optimize networks.
3. **Multi-Channel RGB Memory Grids:**
   - **Red Channel (Hunger):** Deposited by agents seeking resources, mapping exploratory paths.
   - **Green Channel (Fed):** Deposited by agents returning to base, mapping successful resource paths.
   - **Yellow Highways:** The mathematical intersection of overlapping Red and Green pathways, resulting in the most energy-efficient bridges between resource nodes.
4. **Dynamic Environment Mapping:** Utilizes `stb_image.h` to ingest custom `.png` maps, transforming raw pixels into physical boundaries and collision data for the agents via Rejection Sampling.

## Project Structure
- `include/Agent.hpp`: Encapsulates the agent state machine, sensing algorithms, and discrete steering logic.
- `include/ImageMapper.hpp`: Abstracts the parsing of external map files and generation of the Terrain Grid.
- `extern/glad/include/Engine.hpp`: Handles the OpenGL context, vertex/fragment shaders, and texture binding.
- `main.cpp`: The core orchestrator managing the simulation loop, 3-channel diffusion/decay algorithms, and memory grids.

## Demonstration

Watch the Intelligent Swarm navigate physical obstacles, discover food nodes, and establish optimized Yellow networks:

[Watch the Showcase Video Here](https://drive.google.com/file/d/1MbVc28mqKWlpI2RFb8xDIh0Dt_WBjjKU/view?usp=sharing)

## Build Instructions

### Prerequisites
- CMake 3.10+
- A C++17 compatible compiler (GCC/Clang/MSVC)
- OpenGL and GLFW3 libraries installed

### Compilation
```bash
cmake -B build -S .
cmake --build build --config Release
```

### Execution
Provide a black-and-white PNG map as an argument to define the environment obstacles.
```bash
cd build
./PhysarumEngine ../obstacle.png
```
