# cnc-plotter

Software for controlling of CNC machines

Based on C++/MFC as a Microsoft Visual Studio 2010 project 

For the moment contains only the loading of PLT files (generated from Corel Draw) and rendering and display module loaded model.

For rendering (drawing) model I used a minimal graphics engine (which I implemented as a separate library DLL) that supports 
basic processing operations: translate and zoom. 
Algorithm used for drawing the vectorial image is Bresenham
