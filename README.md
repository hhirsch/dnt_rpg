# DNT
A Post-Apocalyptic sci-fi rpg.
> This is very rough. But still too good to let it die.
> I am really looking forward to accepting your Pull Requests and fixing some quirks on my own.

## License                         

DNT  source  code  is  released  under  GPLv3 or any later 
version. The   content   created  by  the   DNTeam  is  also 
released  under  GPLv3  or  later.  Other  content  is either 
licensed under GPLv2 or later or CC-BY or CC-BY-SA licenses.

   See the detailed license attribution files at:
      ./data/models/building/licenses.txt
      ./data/models/junk/licenses.txt
      ./data/models/natural/licenses.txt
      ./data/models/objetos/licenses.txt
      ./data/models/personagens/licenses.txt
      ./data/models/street/licenses.txt
      ./data/models/vehicles/licenses.txt
      ./data/models/warnings/licenses.txt
      ./data/texturas/floor_indoor/licenses.txt
      ./data/texturas/floor_outdoor/licenses.txt

Any  not  explicit attributed file is, by default, licensed 
under GPLv3 or later, and was created by DNTeam.

See  the  DNT's GIT repository* to get source files for all 
contents  on  the game (.blend files).

* git://git.code.sf.net/p/dnt/code


## Requirenments                     

   The  DNT  compilation and execution needs some software and 
hardware. They are:

   Software:
      Cal3D     - http://home.gna.org/cal3d/
      OpenAL    - www.openal.org
      OpenGL    
      SDL       - www.libsdl.org
      SDL_image - www.libsdl.org/projects/SDL_image/
      SDL_ttf   - www.libsdl.org/projects/SDL_ttf/
      LibVorbis - http://www.vorbis.com/

   
   Hardware:
      Video  Card  with  3D  aceleration, OpenGL  compatible.

## Installation

1) Source-Code Distribution

   a) LINUX, BSD, *NIX

   To  "install"  DNT,  decompact  the  tar.bz2,  go  to  the 
directory "dnt", run  "cmake ." , run "make" and, optionally, 
"make install".

   Example:

   farrer@anamabeka:~/$ tar -xvjf dnt.tar.bz2
   farrer@anamabeka:~/$ cd dnt
   farrer@anamabeka:~/dnt/$ cmake .
   farrer@anamabeka:~/dnt/$ make
   farrer@anamabeka:~/dnt/$ sudo make install

   NOTE: the  'make install'  isn't  necessary:  you  can run 
         the game at its local folder.

   NOTE: Any error in the "configure" proccess,  probably  is 
         related with missing development packages for needed
         libraries. Check if you installed them.

   NOTE: If  you  didn't  install  the game, in  order to the 
         language   be   changed,   you'll   need   to  do  a 
         "make install" only  in  the "po" folder. Otherwise,
         the game will be only in english.
 
   b) WINDOWS (We hope you're not using it on Ruindow$).

   To   compile   DNT   on    windows,   you'll   need    the 
Code::Blocks IDE.  Open the project at ./windows folder on it 
and compile.
   Don't blame us. Blame Microsoft!

   c) OSX

   To  compile under  OSX, do the same proccess described for
the  Linux  platform ("a").  Aditionally, you  can create the 
"dnt.app" bundle application by calling:
   
   cmake -DCREATE_BUNDLE=1 .

   instead of just "cmake ."

   NOTE: The CREATE_BUNDLE option won't build the DNT editors,
         just the game itself.


   d) Other

   That's a long time since last time we checked DNT on an OS
different of those described above.
   To  compile  on a different  OS,  try  the  same  proccess 
described for *nix, if you have cmake ported for it.
   Any patches  that  would enable DNT  use on  more  OS  are 
welcome, just submit them as a pull request.


## Execution 

1) Normal Execution (with "make install")

   Just run "dnt" command.  Or, to run the map editor just run 
"dntMapEditor" command.

2) Local Execution (without "make install")

   To   execute  the  program  without  "make install",  go to 
directory ./bin and run the file dnt .

## Input

   Those are the defaults. You can  change  the keymap  in the
options.

### Camera Movement

mouse at left corner  =  turn camera                  (0-360°)
left arrow            =  turn camera left             (0-360°)
mouse at right corner =  turn camera                  (0-360°)
right arrow           =  turn camera right            (0-360°)
up arrow              =  increases zoom               (X-Ym)
down arrow            =  decreases zoom               (X-Ym)
home                  =  maximize zoom
end                   =  minimize zoom
pageup                =  camera up                    (0-89°)
pagedown              =  camera down                  (0-89°)
insert                =  put camera on top
delete                =  put camera on floor
middle mouse button   =  move the camera freely

### Character Movement

mouse right button = move character to position
mouse left button  = do some action
a          =  turn left                       (0-360°)
d          =  turn right                      (0-360°)
w          =  forward                         (0-?)
s          =  backward                        (0-?)
q          =  move left                       (0-?)
e          =  move right                      (0-?)
tab        =  make active next character 
lctrl+tab  =  make active previous character
y          =  protect effect
space      =  enter battle mode
r          =  show range of action
 
## Auxiliar Windows

m      =  Open MiniMap window
i      =  Open Inventory window
c      =  Open Character Window
j      =  Open Journal window


## Contact

Contact us via the forum at http://dnt.dnteam.org .

