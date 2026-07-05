<div align="center">
  
# The Physarum Engine
**Bio-Inspired Slime Mold Simulation | ACA, IIT Kanpur**

[![C++17](https://img.shields.io/badge/C++-17-blue.svg?style=for-the-badge&logo=c%2B%2B)](https://isocpp.org/)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green.svg?style=for-the-badge&logo=opengl)](https://www.opengl.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10+-red.svg?style=for-the-badge&logo=cmake)](https://cmake.org/)

*A high-performance C++/OpenGL agent-based simulation for pathfinding and logistics routing.*

---

</div>

## Overview
**Physarum polycephalum** (the "many-headed slime") is an organism capable of solving complex routing and graph theory problems without a central nervous system. 

The **Physarum Engine** is a high-performance C++/OpenGL simulation that translates this biological intelligence into a computational model to solve NP-hard graph optimization networks. By implementing a "flood, decay, and reinforce" behavior on a discrete grid, the engine evaluates emergent, agent-generated path topologies for real-world applications.

## Engineering Metrics & Impact
- **Scale:** Simulated **10,000 concurrent agents** on a **1024x1024 grid** using chemotaxis to generate emergent networks.
- **Performance:** Engineered a **zero-allocation double-buffered diffusion engine** to handle real-time chemoattractant gradient generation.
- **Hardware Acceleration:** Rendered the simulation via a highly optimized **OpenGL compute shader pipeline**.
- **Application:** Demonstrated stochastic bio-inspired algorithms solving combinatorial NP-hard graph optimization problems, including maze navigation, **PCB trace routing**, and dynamic transportation network design.

## Weekly Progression Pipeline

The engine was developed systematically over an 8-week structured curriculum:

### Week 1: Core Systems & Memory Management `[`[View Code](./CMakeLists.txt)`]`
- Initialized the C++ CMake build environment and configured library linkages (OpenGL, GLFW).
- Designed foundational utility structures and established strict, efficient memory management protocols for high-performance execution.

### Week 2: Naive Swarm Mechanics `[`[View Code](./extern/glad/include/Engine.hpp)`]`
- Architected the `Agent` struct encapsulating position, velocity, and trajectory vectors.
- Implemented the primary render loop to smoothly simulate and render 10,000 independent agents across the discrete grid domain.

### Week 3: Stigmergy & Chemotaxis `[`[View Code](./include/Agent.hpp)`]`
- Engineered the core biological feedback loop. Agents deposit pheromone trails into a globally shared environment grid.
- Implemented discrete, multi-sensor raycasting enabling agents to sample pheromone gradients and adjust their steering dynamically.

### Week 4: Diffusion & Double Buffering `[`[View Code](./main.cpp)`]`
- Integrated a zero-allocation double-buffered state machine to eliminate simulation tearing and race conditions.
- Applied a 3x3 convolution blur matrix and temporal decay factors to the memory grid, simulating the natural diffusion and evaporation of chemoattractants over time.

### Week 5: Environmental Mapping `[`[View Code](./include/ImageMapper.hpp)`]`
- Integrated the `stb_image.h` library to ingest complex black-and-white `.png` topologies (e.g., city layouts, mazes).
- Implemented physics collision boundaries and boundary-avoidance steering logic.

### Week 6: Reinforcement & Multi-Channel Routing `[`[View Code](./include/Agent.hpp)`]`
- Introduced goal nodes (Food) and multi-channel RGB pheromone states.
- Applied Temporal Difference (TD) learning logic where agents flip states upon reaching goals, leaving massive reinforcement trails to optimize multi-source routing.

### Week 7: PCB Maps & Restricted Pathing
- Transitioned testing environments to complex Printed Circuit Board (PCB) routing maps.
- Constrained agent turning angles to strict, practical increments (e.g., 45 and 90 degrees) to simulate real-world hardware routing constraints.

### Week 8: Optimization & Evaluation
- Finalized parameter tuning for rapid stabilization of the shortest-path solutions on blind test configurations.
- Polished the OpenGL visualization layer for the final technical demonstration.

## Demonstration

Watch the Intelligent Swarm navigate physical obstacles, discover nodes, and establish mathematically optimized networks:

**👉 [Watch the Showcase Video Here](https://drive.google.com/file/d/1MbVc28mqKWlpI2RFb8xDIh0Dt_WBjjKU/view?usp=sharing) 👈**

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
Provide a PNG map as an argument to define the environment obstacles.
```bash
cd build
./PhysarumEngine ../obstacle.png
```

## Author
- **Mansoju Vivekananda** (Roll No: 240628)

## Project Mentors
This project was built under the guidance of the ACA, IIT Kanpur mentorship team:
- **Aman Srivastava**
- **Garvit Bhutani**
- **Naina Bhalla**
- **Piyush Verma**

## Show Your Support
If you found this simulation architecture insightful, please consider giving this repository a **Star**!
