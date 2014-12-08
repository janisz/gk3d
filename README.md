GK3D [![Build Status](https://travis-ci.org/janisz/gk3d.svg?branch=master)](https://travis-ci.org/janisz/gk3d)
=======

### Installation

1. Install OpenGL SDK

        sudo apt-get install cmake freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libxmu-dev libxi-dev
  
2. Generate makefile `cmake .`
3. Make project `make`
4. Run `./gk3d.out`


### Usage

| Key                | Action             |
|--------------------|--------            |
|`SPACEBAR`          | toggle FPS control |
|`F`                 | toggle FOG control |
|`C`                 | toggle clipping control |
|`W`                 | forward or increase fog min distnace  or increase X clip axis|
|`A`                 | left or decrease fog max distance    |
|`S`                 | backward or decrease fog min distance or decrease X clip axis|
|`D`                 | right or increase for max distnace   |
| mouse              | look around        |
| left mouse button  | up                 |
| right mouse button | down               |
|`+`                 | increase density   |
|`-`                 | decrease density   |
|`ESC`               |  quit              |
