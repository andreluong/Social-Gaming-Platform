# Social Gaming Platform

A platform where users can develop highly customizable social games and play with other users. 

Our focus is the engine, that parses game files from a syntax tree structure, extracting the configuration, constants, variables, and rules into interpretable models. Users can then initialize and run the game through a client/server chat room.

### Team jjigae for CMPT 373

Team member:
- Brian Rahadi, bra40
- Andre Luong, ala230
- Lew Daniel Yang, ldy
- Ryan Ly, rla168
- Abdinajib Idle, ayi1
- Ishaan Makhija, ima23

### Project status

This project has some components working as intended but it is far from complete:  
- Converting the syntax tree of a game file into game objects that form the GameContext works as expected.  
- Strong typing for Expressions in dataVariant.h is implemented and functioning.  
- The GameManager can set up and run games with partially tested rule support.  
- The for loop Rule works as expected, while most other rules are untested.  
- The integration of GameManager with the Lobby, LobbyManager, and web-socket-networking/tools/server/chatserver.cpp has not been completed, which was one of the main goals we had.  
- Significant work remains to integrate and validate these components.  

### Setup

1. Create a build directory located on the root folder and change into it:
    ```
    mkdir build && cd build
    ```

2. Run cmake and make:
    ```
    cmake ../app && make
    ```

3. Run demo of game-specification:
    ```
    game-specification/demo <game_file_path>
    ```

4. Run tests:
    ```
    test/runAllTests
    ```

### Network

1. Start a server:
    ```
    web-socket-networking/bin/chatserver 8000 ../app/web-socket-networking/webchat.html
    ```

2. Join server as a client from a new terminal:
    ```
    web-socket-networking/bin/client-ncurses localhost 8000
    ```
