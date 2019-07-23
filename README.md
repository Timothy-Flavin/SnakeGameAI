# SnakeGameAI
Using 'Gummy' my machine learning library to play Snake Game as a demo.
'Gummy' will be referenced many times. This is the name for my A.I. the 'O' is the head of the snake
the 'o's are the body. The '@' is the fruit and hitting the walls kills you. 'w''a''s''d' then hit enter
to move if you chose human player. 

# Running the Snake Game
In the Gummy folder there must be present a `gameData.csv` file and `snakeNet.csv` file
The user will be prompted with several things starting out. if on mac compile with the commands
`g++ -c *.cpp`, `g++ *.o`, `./a.out` in the `Gummy` directory
### 'load or save' 
is asking if the user wants to load an already trained neural network (one comes by default) 
or if the user wants to start training from scratch. enter 1 to load and 0 to save a new net
### 'number of iterations'
You will then be asked for the number of iterations you would like to train the net. enter a prefered 
positive integer. (standard of 100,000 or 1,000,000 works well). This number is how many times random data points 
will be pulled from 'gameData.csv' and used for training.
### 'step size'
The user will then be asked to enter the step size. This number dictates how much the net changes afte each iteration.
higher numbers will train faster but less consistently and the net is less likely to get stuck but also less likely to
converge on an optimal result. Lower numbers will train more accurately but it will take longer and is more prone to
being stuck in a local minimum rather than an absolute minimum. '0.05' is standard. if training time is too high
one can increase step size and decrease iterations but this is not recommended for final training. 
### 'put file into...'
This is asking for the user to set which '.csv' file gummy will train from. 'gameData.csv' is included by default 
and is what you should enter if you do not know see `gameData.csv` for details
### 'do you want Gummy to train first'
Asking if you want to train the neural network before playing a set of games or if the user would rather go straight
to playing and recording data. if 'y' gummy will train using the data before. if 'n' gummy will not.
### 'do you want to save'
'y' to overwrite existing snake net. 'n' to keep existing net and move on.
### 'do you want to clear the gameData file'
'y' to start with a blank gameData.csv file. 'n' to keep and add on to existing game Data file.
### 'how many games do you want to play'
Enter the number of games to be played before final prompt to train from recorded data. Data will be recorded after each
fruit eaten and each death.
### 'player type'
'0' for human '1' for Gummy to play '2' for a player which picks it's direction randomly. If you are looking to generate
a lot of play data, random is by far the fastest with neural net comming in second.
### 'controls'
if human is the chosen player type, enter the direction w, a, s, or d and hit enter to make the snake move. The snake does
not move automatically because the human player is only meant to train the snake, not to be a challenging snake game on it's
own. 

# importand variables the programmer can play with
### 'WIDTH and HEIGHT'
thise change the width and height of the arena. The snake should be able to play in any reasonable width and height after being
trained 

### 'snakeVisionX and Y'
these change how far the snake can see in the x and y directions. if this variable is changed the user must chose the 'save'
option, when asked to save or load, in order to properly recreate a neural net with an updated number of inputs. larger vision
means longer training time and a slower run time. shorter vision means a more primitive snake. (3-10 is recommended)
### 'layerSizes[]'
This array is the list of layers and the number of neurons in each layer of the neural network. !DO NOT CHANGE LAYER 0!
This layer is decided by the snake's view range snakeVision and changing it will result in a segmentation fault.
the length of layerSizes may be changed from 4 so long as the 4rth argument in 'gummy.manualInit()' is equal to the number of layers
The last layer may not be changed from '4'. every layer must have a layerNumber assigned. More layers and higher numbers
will result in slower training and possibly a smarter snake. 

# File Descriptions 
## `gameData.csv`
This file is where gummy stores the data of games played that is used to train off of.
If this file is corrupt or empty or mal-formatted for any reason replace the data with a line of '0,0,0...'
it is important that this file not end in a blank line
## `snakeNet.csv`
This is where the data for the neural net's weights is stored. You should never have to open this file
