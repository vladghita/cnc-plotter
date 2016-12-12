# cnc-plotter

Software for controlling of CNC machines in Windows

Based on C++/MFC as a Microsoft Visual Studio 2010 project 

For the moment contains the following features: 
 - Loading of PLT files (generated from Corel Draw) 
 - Rendering the loaded file
 - Manipulating the vectorial object (translate and zoom)
 - Simulating the real process directly on the loaded object

For rendering (drawing)  I used a minimal graphics engine (which I implemented as a separate library DLL)
Algorithm used for drawing the vectorial image is Bresenham

Briefing video: https://www.youtube.com/watch?v=HstfU-UoCEM

