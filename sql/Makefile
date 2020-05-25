all: data-server data-client

data-client:
	g++ -std=c++17 -Wall -o client structs.cpp client.cpp

data-server:
	g++ -std=c++17 -Wall -o server structs.cpp table.cpp server.cpp
