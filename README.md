# game-of-life -- A Game of Life Application

![title](https://media.giphy.com/media/uiDas4rhuakasdmCKj/giphy.gif)

## Install gtkmm3

### Linux
- sudo apt-get install libgtkmm-3.0-dev

### Mac
- brew install gtkmm3

## Install game-of-life
- git clone https://github.com/lkallio/game-of-life ; cd game-of-life
- make

## Usage
- ./gol [cells per row / col] [iterations per sec]
- if you want to change the window scale, change `WIN_SCALE` in includes/global.h and run make re

## Controls
- left mouse button to resurrect cells
- right mouse button to kill cells
- you can hold down the button to draw continuously
- "Next" to move to the next generation
- "Run" to move to the next generation `iterations_per_sec` amount of times per sec
