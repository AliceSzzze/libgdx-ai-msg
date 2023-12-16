# CUGL MSG Implementation

The C++ implementation of gdx-ai-msg can be found in the C++ folder.

## Usage Example

A usage example, which demonstrates adding/removing listeners and visualizes
the R-Tree, can be found in HelloApp.cpp. An arbitray object is chosen to
continuously send messages to all objects of a certain tag around a certain
range. An object on the screen is red if it received a message that frame.
In the simulation, objects will be removed as listeners, but they will not be
removed from the visualization.

To run the usage example, move the files to a new 4152 demo CUGL project, and
replace HelloApp.cpp with this repo's HelloApp.cpp.
