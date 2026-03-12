# TTT-masters

## How to set up project
When opening the TTT-masters repository for the first time the first thing you will notice is the CROSS and DOT folders. These are used to load the AI's for thier respective peices.
Hence the AI loaded in the CROSS folder will play as CROSS and the AI loaded into the DOT folder will be playing as DOT. If no AI file is found the program will load an anonymus HAND where the player themselfs have to operate the program by placing pieces by hand.

To load an AI there are only to things nessecery.
- config.csv
- AI shared object
  
#### config.csv
The config.csv is responsible for setting up 3 things:
- a player name
- a creator tag
- a profile picture file path

When opening a new config.csv it should look like this:
```csv
creator_tag,bot_name,pic_path
...
```
Replacing the ... a new line including the creator tag, player name, and profile picture file path for your bot.
if any of these are left empty they will get replaced by defaults:
- player_name: ANONYMOUS
- creator_tag: Anonymous
- pic_path: assets/no_user.png

#### Setting up project
When setting up the files for your project there are a few whing you will need to consider.
First: 
The name of the function that will return the move, should look like this:
```cpp
move <player_name>(raw_boardstate boardstate, std::atomic<int> *time_left){
//ai stuff

  return (move)<your move>;
};
```
Data types like move and raw_bordstate are given in the globals.cpp and globals.hpp amongst many other useful functions and structs.
including them into your project is *almost nessecery* to get cleanly running code. Simply copying the files from src should be enough. Look for an exampel of this in the AI rando_bot.
Its **very important** to name the function that will return the move for the given boardstate the same name as the one configured as player_name in config.csv.

#### SO - files
When using TTT-masters it's very important to compile your AI build into a shared object, with the same name as the 'player_name' set by your config file. 
"/<player_name>.so"

When creating your function it is important to add extern "C" in front of the function decleration in your header file.
If no SO file is given, a player will be be set to play by hand instead.

How the so-file is compiled and build is up to you but it might be helpful to use the structure and build.bash used for the exampel-bot rando_bot.

#### Useful things to know
The raw_boardstate is a struct contaning three things:
```cpp
struct raw_boardstate {
    piece board[9][9];
    piece turn; 
    short current; 
};
```
- A 9 by 9 grid contaning all the different spot in the game
- Witch player has the current turn
- Witch sub_board is currently active, if this is -1 it means all of the sub_boards are active. If not -1 the values should be between 0 and 8.

By now you have properbly noticed the piece type. piece is an enum defind as follows: 
```cpp
enum piece{
    CROSS = 1,
    EMPTY = 0,
    DOT = -1
};
```
It simply Defines the values 1 to CROSS and DOT to -1 whilst keeping EMPTY 0.
This alowes for cleaner code, whilst also keeping some simplicity.

At last we have the move struct, defind like this:
```cpp
struct move{
    int sub; 
    int spot;
};
```
It is simply a struct used to store a move. A move is given by 2 numbers: 
- sub, the sub_board where the move should take place.
- spot, the spot within the sub where thw move shall occur.

Note it dosent say whitch piece should be placed, this is automaticly figured out by the TTT-masters program.

#### Settings
In the main directory, you will also see the settings.json file.
The following will try to explain the function of each 'setting':
  timeout (int): The amount of time before a given AI is forced to forfiet the game. This timeout does not concern a player playing by hand.
  starting_pieces (int): The amount of pieces place before the game begins. This is so derterministick AI's can play more than one game.
  seed (int): The seed used to determien the random outcome of the pre-placed pieces. If this is set to -1, the seed is whill be randomly assigned.
  dot_path (string): The path to the folder containing the, SO and config of the player playing dot.
  cross_path (string): The path to the folder containing the, SO and config of the player playing cross.
  starting (string): The starting player. This will also flip the pre-placed pieces. it can be either "dot" or "cross"
