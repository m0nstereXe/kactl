alias c='g++ -Wall -Wconversion -Wfatal-errors -g -std=c++17 \
	-fsanitize=undefined,address'
export CXXFLAGS='-fsanitize=address -g -Wall -Wextra -std=gnu++20' #nick make
xmodmap -e 'clear lock' -e 'keycode 66=less greater' #caps = <>
