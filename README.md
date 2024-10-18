# 373-24-jjigae

Team member:
- Brian Rahadi, bra40
- Andre Luong, ala230
- Lew Daniel Yang, ldy
- Hieu Duong, qhduong
- Ryan Ly, rla168
- Abdinajib Idle, ayi1
- Ishaan Makhija, ima23


Instructions for unit testing with GoogleTest (Based on Source: Cmpt 473 exercise by Nick Sumner (2024)):

In 373-24-jjigae/CMakeLists.txt, add_subdirectory of the directory you want to test.\
Create CMakeLists.txt in the directory you want to test.  
It should look like the one in game-logic if you use the same structure.\
Create your test file in the test directory (like test_UserRoleManagement).\
In test/CMakeLists.txt, add your test file and directory (the name used for add_library()) you are testing to target_link_libraries and add_executable, respectively.\
Comment out other test files you want to ignore.\

## Setup

1. Create a build directory located on the root folder and change into it:
    ```
    mkdir build && cd build
    ```

2. Run cmake and make:
    ```
    cmake ../app && make
    ```

3. Start a server:
    ```
    web-socket-networking/bin/chatserver 8000 ../app/web-socket-networking/webchat.html
    ```

4. Join server as a client:
    ```
    web-socket-networking/bin/client-ncurses localhost 8000
    ```

    Run tests:
    ```
    test/runAllTests
    ```