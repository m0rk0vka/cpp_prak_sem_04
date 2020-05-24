all: data-server data-client

data-client:
	g++ -std=c++17 -Wall -o client structs.cpp structs.h table.cpp table.h client.cpp

data-server:
	g++ -std=c++17 -Wall -o server structs.cpp structs.h table.cpp table.h server.cpp
