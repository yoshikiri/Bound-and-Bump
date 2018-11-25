# Bound-and-Bump
Simple 2D game made with mainly OpenGL and GLFW

# How to Play this Game
  type this command in the directory which includes **vertexshader.vert**, **fragmentshader.frag**, **Image**, and **Music**  
  $ ./a.out  

# Info of each Files
* main.cpp - initialize settings and main loop of this game  
* Window.h - manage window system
* shaderManager.h - manage vertex shader and fragment shader
* State.h - parent class of Title, Play, and Result (they represent game state, or game scene)  
* Title.h - definition of Title class, which is responsible for title scene  
* Play.h Play.cpp - responsible for main part of game play  
* Result.h Reslt.cpp - responsible for result scene  
* Figure.h - definition of Figure class, giving OpenGL style drawing  
* DrawFigure.h - drawer some kind of figure easily (all member functions are static)  
