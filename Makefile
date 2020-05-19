all: data-server data-client

data-client:
	g++ -std=c++17 -Wall -o DATA structs.cpp structs.h table.cpp table.h client.cpp

data-server:
	g++ -std=c++17 -Wall -o SERVER structs.cpp structs.h table.cpp table.h server.cpp
