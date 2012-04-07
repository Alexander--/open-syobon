Syobon Action
=============
This is an improved version of the SDL version of Syobon Action (also
known as Cat Mario or Neko Mario). The original website is at:
http://www.geocities.jp/z_gundam_tanosii/home/Main.html (if you
understand japanese).

Control and keys:
-----------------
The game plays either with the keyboard or a joystick.

F1 = main menu (press during play)
K = suicide (kill)
space = speed mode
1-9 durning main menu = stage select
0 during main menu = randomized level

Changes:
--------
- added Windows port
- added fullscreen mode (by default)
- added '-window' argument to run the game in a window instead of fullscreen
- fixed sound lag introduced in SDL version
- improved sound quality
- added game over effect on exit
- fixed "off by one" pixel bug when blitting reversed sprites introduced in SDL version
- pressing 'k' will commit suicide, instead of 'o'
- translated japanese text to english
- prints which stage is selected when pressing 1-9 on the main menu
- fixed joystick code which was completely broken on windows
- added more joystick buttons for jump

Contact:
--------
David Gerber <dg@zapek.com>
Please note that I am not the author of Syobon Action, I just improved the
SDL port and made it run on Windows (again).

Libraries used:
---------------
SDL: http://www.libsdl.org/download-1.2.php
SDL_gfx: http://freshmeat.net/projects/sdl_gfx/
SDL_image: http://www.libsdl.org/projects/SDL_image/
SDL_mixer: http://www.libsdl.org/projects/SDL_mixer/
SDL_ttf: http://www.libsdl.org/projects/SDL_ttf/