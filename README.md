<p align="center">
  <img alt="A Steering Behavior combination" src="https://user-images.githubusercontent.com/14791312/233503776-0bbb1db4-b799-4c3f-9cc7-fc248dab85cb.JPG">
</p>

# AI Techniques

Artificial intelligence techniques, oriented to video games, made with C/C++ and SDL2 for PC at the university. AI is a discipline that tries to make computers capable of performing cognitive tasks that humans and animals are capable of doing.

## Table of Contents

- [Projects](#projects)
  - [Steering Behaviors](#steering-behaviors)
  - [Pathfinding](#pathfinding)
  - [Decisions](#decisions)
- [Getting Started](#getting-started)
  - [Pre-requisites](#pre-requisites)
- [Installation](#installation)
  - [Windows](#windows)
    - [Run the executables](#run-the-executables)
- [Authors](#authors)
- [License](#license)


# Projects

## Steering Behaviors

These are a set of algorithms that allow autonomous agents to navigate their environment in a realistic way. And the basic idea is to use local information to calculate and apply forces (acceleration) on characters to modify their motion. An agent is anything that senses its environment through a set of actuators.

https://user-images.githubusercontent.com/14791312/233453258-7ca16837-0963-4e46-b21a-e418b7803184.mp4

In the simulations shown you find the implementation of some Steering Behavior in the following order:
1. Basics: `Seek/Flee`, `Arrive`, `Pursue/Evade` and `Wander`.
2. Advanced: `Flocking`.
3. Combination of the above mentioned among others, such as: `Path Following`, `Perimeter Avoidance`, `Collision Avoidance`, `Obstacle Avoidance`, etc.

## Pathfinding

The goal of pathfinding algorithms is to find a realistic (intelligent) path between two locations in the game world. These techniques are needed for long-term planning instead of waiting until the last moment to discover a path problem, such as a wall (obstacle), when it is too late.

https://user-images.githubusercontent.com/14791312/233453287-67155c3c-0b8a-4e29-8d92-d0fa7f737280.mp4

Simulations implemented:
1. `Breadth First Search` (BFS), `Dijkstra`, `Greedy Best-First-Search` (GBFS) and `A*` pathfinding algorithms for an agent (soldier) to find the optimal path to a random location in the maze.
2. Application of the `A*` algorithm for an agent to find the optimal path that passes through a set of N random locations within the maze.
3. A strategy, using a `kind of D*` (dynamic version of A*), that takes into account that there are adversarial agents (enemies) in the game. The agent has to reach a set of N random locations with enemies to avoid.

## Decisions

Decision making is the ability of an agent to decide what to do in any given moment. The agent processes a set of input information that it uses to decide the action it wants to take. And the input to the decision making process is the knowledge that the agent has and the output is a request for action.

https://user-images.githubusercontent.com/14791312/233453694-2e3d5444-325b-454a-a17e-4bf3d4292ef1.mp4

Simulations implemented:
1. `Finite State Machine` (FSM) with Function Pointer Style following the next behavior:

    <p align="center">
      <img alt="Decisions FSM" src="https://user-images.githubusercontent.com/14791312/233454018-a089ead0-bc45-42df-820c-1e931b613822.JPG">
    </p>

2. `Goal Oriented Action Planning` with A* as the research algorithm:

    > **Note** Not shown in the previous video

    <details>
    <summary>Some simulations:</summary>

    <div align="center">

    ![DecisionsGOAP](https://user-images.githubusercontent.com/14791312/233611850-e557bb77-8b1d-4fda-861d-2640882d5494.gif)

    </div>

    </details>

    * A soldier has to scout (search for enemies), approach an enemy, aim, shoot, reload, explode bombs, escape, etc.
    * World status with the following information (all are `true/false`): `Agent_Alive`, `Agent_with_Weapon`, `Weapon_Reloaded`, `Agent_with_Bomb`, `Enemy_Visible`, `Enemy_Aligned`, `Enemy_Nearby`and `Enemy_Alive`.

<div align="right">
  
[ [ ↑ to top ↑ ] ](#ai-techniques)
  
</div>


# Getting Started

This project has been made with Windows. But you should be able to use it on Linux and Mac.

## Pre-requisites

Before you start, make sure you have an IDE/Compiler such as [Visual Studio](https://visualstudio.microsoft.com/downloads), [Code::Blocks](https://www.codeblocks.org/downloads) or [XCode](https://developer.apple.com/xcode).

<div align="right">
  
[ [ ↑ to top ↑ ] ](#ai-techniques)
  
</div>


# Installation

> **Note** If you are on Windows, you can skip the first step

1. [Hello SDL](https://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php).
2. Download the last version of [SDL2](https://github.com/libsdl-org/SDL/releases), [SDL2_image](https://github.com/libsdl-org/SDL_image/releases) and [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf/releases) corresponding to your operating system (if you are on Windows, see the next).

## Windows

1. Download the `VC.zip` for Visual Studio or the `mingw.tar.gz` for Code::Blocks and MinGW.
2. Put the corresponding libraries in the next folder path (you have to create folders): "Libraries/SDL2-version", "../SDL2_image-version" and "/../SDL2_ttf-version".
3. Remove "-version" in the folders. Then you wil have the following names: "SDL2", "SDL2_image", "SDL2_ttf".
4. Paste in "[ProjectName]/x64/Debug" the next `.dll`:
    * `SDL2.dll` (Libraries/SDL2/lib/x64)
    * `SDL2_image.dll` (../SDL2_image/lib/x64)
    * `SDL2_ttf.dll` (../SDL2_ttf/lib/x64)
5. Compile and run the solution in `Debug-x64`.

### Run the executables
1. Paste in "[ProjectName]/x64/Debug" the next files: "res" folder ([ProjectName])
2. Run the executable in "[ProjectName]/x64/Debug".

<div align="right">
  
[ [ ↑ to top ↑ ] ](#ai-techniques)
  
</div>


# Authors

| [<img src="https://user-images.githubusercontent.com/14791312/233219860-32856bfe-bfa3-4a68-b0c4-f4d7f6ab0730.png" width=115><br><sub>Samuel Balcells</sub>](https://github.com/sFaith3) | [<img src="https://user-images.githubusercontent.com/14791312/233445723-8c5ebf50-23e3-4a74-a486-672371ed2a64.jpg" width=115><br><sub>Arnau Macià</sub>](https://github.com/arnaumr) | [<img src="https://user-images.githubusercontent.com/14791312/233445809-c78094da-439c-4a8e-90dc-7fff1878ae18.jpg" width=115><br><sub>Sergi Sánchez</sub>](https://github.com/gyoza14)
| :---: | :---: | :---: |


# License

[MIT License](./LICENSE)

<div align="right">
  
[ [ ↑ to top ↑ ] ](#ai-techniques)
  
</div>
