# EscapeRoomSuitcase

## Project vision:

In this project we design a suitcase for an escaperoom-game, which contains several task that need to be solved one after another.
The suitcase represents a bomb and when all task are solved the bomb will be diffused. There will be three main tasks that need to be solved.
First to level out three force bars with force sensors. Second to solve a mealy automat visualized with LEDs. And finally to play a simple melody with a theremin like device.

## Description and realization

All the sensors for the different tasks will be connected to an ESP32. That will display hints over a LCD display.

### Task 1 Force level

The first task uses three Square Force-Sensitive Resistors. The Output value of the Force sensor is parsed to a value between 1 and 10 and displayed on a LED board with 10 steps. The Goal is to level out the three LED boards by applying preassure to the Force Resistor. If all three levels are held in a yet to be defined range, for a certain amount of time the first task is solved.

### Task 2 Mealy automat

Two switches are conencted to a bit shift register. One switch changes the input from 0 to 1 the other shifts the current value by one bit inserting the 0 or 1 currently applied at the other switch. The goal is to get a certain value in the shift register. The value is displayed by a row of LEDs where a lit LED symbolizes an one and a non lit LED a zero.

### Task 3 Theremin

The goal is to play a simple melody. A tone is played by holding your hand a certain distance from an ultra sonic sensor. The melody supposed to be played will be given and the display will give visual feedback if the right tone is played.

### Components

- ESP32-PICO

- SquareForce-Sensitive Resistor

- 8-Bit Shift Register

- Sensopart FT 10-RLA laser distance sensor

## Time plan

| Time   | Task  |
|--------|-------|
| KW 21  | get familiar with sensors and connect them to esp 32|
| KW 22  | create a rough software prototype for each task|
| KW 23  | try out communication between two esp 32| 
| KW 24  | Add Help Button and Hints |
| KW 25  | Connect all the tasks |
| KW 26  | Finish Software and Hardware|
| KW 27  | Build Hardware into Suitcase| 
| KW 28  |
| KW 29  | Final Presentation |

## BOM:
https://octopart.com/bom-tool/dn4D4DHd/wdb3715TmVMupruC

## Evaluation:
- minimum Goal: Have all three tasks working in the Suitcase
- optimal Goal: Have all tasks working and the ouput of the task is a hint or an input for the next task
