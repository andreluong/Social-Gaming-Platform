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
\
Create a build directory on the same level of this project\
cd into it\
run:\
cmake ../373-24-jjigae\
make\
test/runAllTests
