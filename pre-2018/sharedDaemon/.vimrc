ca make !(cd ./build && make)
ca cmake !(cd ./build && cmake ..)
ca makelog !(cd ./build && make 2>log.txt)<CR> :e ./build/log.txt
ca run !./build/daemonLoader
