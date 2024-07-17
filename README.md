# RayBoy

This repository contains a project originally developed for visualizing raycasting in the console. The goal was to port this project to the Arduboy platform. Below, you'll find an overview of the project's motivation, the Arduboy's characteristics, and the challenges and solutions encountered during the porting process.

## Project Motivation
I started with a simple yet engaging [project](https://github.com/admtrv/RayCasting) that visualized raycasting in the console using ASCII characters. The goal was to create a retro-style visualization using just standart console window and C programming language. After successfully achieving this, I decided to take on the challenge of porting this project to [Arduboy](https://www.arduboy.com/), a small, portable gaming platform with limited resources.

## Arduboy Characteristics
**Processor**: ATmega32u4 (same as Arduino Leonardo & Micro)

**Memory**: 32KB Flash, 2.5KB RAM, 1KB EEPROM

**Inputs**: 6 Momentary Tactile Buttons

**Outputs**: 128×64 1Bit OLED, 4 Ch. Piezo Speaker & Blinky LED

**Battery**: 180 mAh Thin-Film Lithium Polymer

**Connectivity**: Micro-USB 2.0 w/ built-in HID profile

## Challenges and Solutions
### Initial Project Considerations
In the initial console-based project, performance was not a major concern. The primary limitation was the console output, which was managed efficiently using the 'curses' library. This allowed me to write a straightforward, albeit not the most optimized, raycasting algorithm. The focus was more on visual correctness and educational value rather than computational efficiency.

### Limited Resources
The Arduboy platform, however, posed significant limitations in terms of processing power and memory compared to a typical desktop environment. This presented a unique challenge as my original raycasting algorithm was not optimized for such constraints. The main bottleneck was not the rendering itself but the computational expense of the original algorithm.

### Algorithmic Changes
To accommodate these limitations, I had to completely redesign the raycasting and rendering algorithms. The primary solution was to implement the Digital Differential Analyzer (DDA) line algorithm. The [DDA algorithm](https://www.geeksforgeeks.org/dda-line-generation-algorithm-computer-graphics/) rasterizes a line between two given points using floating-point calculations, making it suitable for the constrained environment of the Arduboy.

### DDA Line Algorithm
Despite being somewhat outdated, the DDA algorithm is an excellent educational tool for understanding the challenges and solutions in line rasterization. By implementing this algorithm, I was able to overcome the limitations of the Arduboy platform and achieve the desired raycasting visualization.

<p align="center">
  <img src="images/game.gif" alt="gamecycle" width="370" />
</p>

## How to Run the Project
1. Setup Arduboy Development Environment by using this [tutorial](https://community.arduboy.com/t/make-your-own-arduboy-game-part-1-setting-up-your-computer/7924) to completely setup your computer to work with Arduboy.
2. Clone the Repository.
3. Open the raycasting.ino file in the Arduino IDE.
4. Connect your Arduboy to your computer via USB.
5. Select the correct board and port.
6. Click the upload button to compile and upload the code directly to your Arduboy.

## Conclusion
Porting the raycasting visualization to the Arduboy was both a challenging and rewarding endeavor. The process required significant modifications to the original algorithm to fit within the constraints of the Arduboy's limited resources. 

Implementing the DDA line algorithm was a key solution, demonstrating how older, simpler methods can be effective in resource-constrained environments. This project not only showcases the potential of the Arduboy but also serves as an educational tool for understanding basic computer graphics principles.

Feel free to explore the code, suggest improvements, and contribute to this project. Happy coding!

## Acknowledgments
Thanks to the [Arduboy community](https://community.arduboy.com/) for their educational resources!
