# 373-24-jjigae

Team member:
- Brian Rahadi, bra40
- Andre Luong, ala230
- Lew Daniel Yang, ldy
- Ryan Ly, rla168
- Abdinajib Idle, ayi1
- Ishaan Makhija, ima23

## Project status

This project has some components working as intended but it is far from complete.  
Converting the syntax tree of a game file into game objects that form the GameContext works as expected.  
Strong typing for Expressions in dataVariant.h is implemented and functioning.  
The GameManager can set up and run games with partially tested rule support.  
The for loop Rule works as expected, while most other rules are untested.  
The integration of GameManager with the Lobby, LobbyManager, and web-socket-networking/tools/server/chatserver.cpp has not been completed, which was one of the main goals we had.  
Significant work remains to integrate and validate these components.  

## Setup

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

## Network

1. Start a server:
    ```
    web-socket-networking/bin/chatserver 8000 ../app/web-socket-networking/webchat.html
    ```

2. Join server as a client from a new terminal:
    ```
    web-socket-networking/bin/client-ncurses localhost 8000
    ```

## Script to run server and chat

Make server-script.sh directly under root containing these 4 lines below
```
cd build
cmake -DCMAKE_CXX_STANDARD=23 -DCMAKE_EXPORT_COMPILE_COMMANDS=on ../app
make
cd web-socket-networking
./bin/chatserver 8000 webchat.html
```

Make chat-script.sh
```
cd ./build/web-socket-networking
./bin/client-ncurses localhost 8000
```
To run server, go `./server-script.sh`

To run chat terminal, go `./chat-script.sh`

If there is permission error, do `chmod +x chat-script.sh` or `chmod +x server-script.sh`

## Script to run test

Make test-script.sh
```
cd ./build/web-socket-networking
./bin/client-ncurses localhost 8000

```

To run tests, go `./test-script.sh`

If there is permission error, do `chmod +x test-script.sh`

## Other

Instructions for unit testing with GoogleTest (Based on Source: Cmpt 473 exercise by Nick Sumner (2024)):

In 373-24-jjigae/CMakeLists.txt, add_subdirectory of the directory you want to test.\
Create CMakeLists.txt in the directory you want to test.  
It should look like the one in game-logic if you use the same structure.\
Create your test file in the test directory (like test_UserRoleManagement).\
In test/CMakeLists.txt, add your test file and directory (the name used for add_library()) you are testing to target_link_libraries and add_executable, respectively.\
Comment out other test files you want to ignore.\