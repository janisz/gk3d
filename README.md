GK3D [![Build Status](https://travis-ci.org/janisz/gk3d.svg?branch=master)](https://travis-ci.org/janisz/gk3d)
=======

### Installation

1. Install OpenGL SDK

        sudo apt-get install cmake freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libxmu-dev libxi-dev
  
2. Generate makefile `cmake .`
3. Make project `make`
4. Run `./gk3d.out <path to *.obj fils> <path to *.obj fils>` e.g.:

        ./gk3d.out "resources/models/bench.obj" "resources/models/female.obj"


### Usage

| Key                | Action             |
|--------------------|--------            |
|`SPACEBAR`          | toggle FPS control |
|`W`                 | forward            |
|`A`                 | left               |
|`S`                 | backward           |
|`D`                 | right              |
| mouse              | look around        |
| left mouse button  | up                 |
| right mouse button | down               |
|`ESC`               |  quit              |
