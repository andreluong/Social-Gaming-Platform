cmake -DCMAKE_CXX_STANDARD=23 -DCMAKE_EXPORT_COMPILE_COMMANDS=on ../app
make
./web-socket-networking/bin/chatserver 8000 ./web-socket-networking/webchat.html
