<p align="center">
  <img alt="Dynamic A-star pathfinding project cover" src="https://user-images.githubusercontent.com/14791312/233451852-f62c844e-de22-48db-8678-20100acd66ba.png">
</p>


# AI Techniques

Artificial intelligence techniques made with C/C++ and SDL2 for PC at the university.


# Projects

## Steering Behaviors

https://user-images.githubusercontent.com/14791312/233453258-7ca16837-0963-4e46-b21a-e418b7803184.mp4

- Basics: `Seek/Flee`, `Arrive`, `Pursue/Evade` and `Wander`.
- Advanced: `Flocking`.
- Others and/or combinations: `Path Following`, `Perimeter Avoidance`, `Collision Avoidance`, `Obstacle Avoidance`.

## Pathfinding

https://user-images.githubusercontent.com/14791312/233453287-67155c3c-0b8a-4e29-8d92-d0fa7f737280.mp4

- `Breadth First Search`, `Dijkstra`, `Greedy Best-First-Search` and `A*`.
- `A*`: to get a set of N random locations.
- `Kind of D*`: to get a set of N random locations with enemies to avoid.

## Decisions

https://user-images.githubusercontent.com/14791312/233453694-2e3d5444-325b-454a-a17e-4bf3d4292ef1.mp4

- `Finite State Machines` (Function Pointer Style).

<p align="center">
  <img alt="Decisions 1" src="https://user-images.githubusercontent.com/14791312/233454018-a089ead0-bc45-42df-820c-1e931b613822.JPG">
</p>

- `Goal Oriented Action Planning` (with A* as the research algorithm):
	- A soldier has to scout (search for enemies), approach an enemy, aim, shoot, reload, explode bombs and escape.
	- World status with the following information (all are `true/false`): `Agent_Alive`, `Agent_with_Weapon`, `Weapon_Reloaded`, `Agent_with_Bomb`, `Enemy_Visible`, `Enemy_Aligned`, `Enemy_Nearby`and `Enemy_Alive`.


# Getting Started

This project has been made with Windows. But you should be able to use it on Linux and Mac.

## Pre-requisites

Before you start, make sure you have an IDE/Compiler such as [Visual Studio](https://visualstudio.microsoft.com/downloads), [Code::Blocks](https://www.codeblocks.org/downloads) or [XCode](https://developer.apple.com/xcode).


# Installation

> **Note** If you are on Windows, you can go to the second step

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


# Authors

| [<img src="https://user-images.githubusercontent.com/14791312/233219860-32856bfe-bfa3-4a68-b0c4-f4d7f6ab0730.png" width=115><br><sub>Samuel Balcells</sub>](https://github.com/sFaith3) | [<img src="https://user-images.githubusercontent.com/14791312/233445723-8c5ebf50-23e3-4a74-a486-672371ed2a64.jpg" width=115><br><sub>Arnau Macià</sub>](https://github.com/arnaumr) | [<img src="https://user-images.githubusercontent.com/14791312/233445809-c78094da-439c-4a8e-90dc-7fff1878ae18.jpg" width=115><br><sub>Sergi Sánchez</sub>](https://github.com/gyoza14)
| :---: | :---: | :---: |


# License

[MIT License](./LICENSE)
