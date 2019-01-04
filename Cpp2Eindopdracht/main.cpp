//
//  main.cpp
//  socketexample
//
//  Created by Bob Polis on 16/09/14.
//  Revised by Jeroen de Haas on 22/11/2016
//  Copyright (c) 2014 Avans Hogeschool, 's-Hertogenbosch. All rights reserved.
//
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <thread>
#include <iostream>
#include <exception>
#include <memory>
#include <utility>
#include <chrono>
#include "Game.h"
#include "Fileparser.h"
using namespace std;

#include "Socket.h"
#include "Sync_queue.h"
#include "ClientCommand.h"
#include "Player.h"
#include "ClientInfo.h"

namespace machiavelli {
    const int tcp_port {1080};
    const string prompt {"machiavelli> "};
}

static bool running = true;

static Sync_queue<ClientCommand> queue;

void consume_command(Game *game) // runs in its own thread
{
    try {
        while (running) {
            ClientCommand command {queue.get()}; // will block here unless there are still command objects in the queue
            if (auto clientInfo = command.get_client_info().lock()) {
                auto &client = clientInfo->get_socket();
                auto &player = clientInfo->get_player();
				auto otherPlayer = game->getOtherPlayer(clientInfo);

                try {
	                if (game->getCurrentPlayer() == clientInfo)
	                {
						// TODO handle command here
						client << player.get_name() << ", you wrote: '" << command.get_cmd() << "', but I'll ignore that for now.\r\n" << machiavelli::prompt;
						otherPlayer.get()->get_socket() << player.get_name() << " wrote " << command.get_cmd() << "', but I'll ignore that for now.\r\n" << machiavelli::prompt;
	                }
	                else client << "Jouw tegenstander is aan de beurt.\r\n" << machiavelli::prompt;
                    

                } catch (const exception& ex) {
                    cerr << "*** exception in consumer thread for player " << player.get_name() << ": " << ex.what() << '\n';
                    if (client.is_open()) {
                        client.write("Sorry, something went wrong during handling of your request.\r\n");
                    }
                } catch (...) {
                    cerr << "*** exception in consumer thread for player " << player.get_name() << '\n';
                    if (client.is_open()) {
                        client.write("Sorry, something went wrong during handling of your request.\r\n");
                    }
                }
            }
        }
    } catch (...) {
        cerr << "consume_command crashed\n";
    }
}

std::shared_ptr<ClientInfo> init_client_session(Socket client) {
    client.write("Welcome to Server 1.0! To quit, type 'quit'.\r\n");
    client.write("What's your name?\r\n");
    client.write(machiavelli::prompt);
    string name;
    bool done { false };
    while(!done) {
        done = client.readline([&name](std::string input) {
            name = input;
        });
    }
	client.write("What's your age?\r\n");
	client.write(machiavelli::prompt);
	int age;
	done = false;
	while (!done) {
		try
		{
			done = client.readline([&age](std::string input) {
				age = stoi(input);
			});
		}
		catch (...)
		{
			client.write("What's your age?\r\n");
			client.write(machiavelli::prompt);
		}
		
	}
    return make_shared<ClientInfo>(move(client), Player { name,age});
}

void handle_client(Socket client, Game *game) // this function runs in a separate thread
{
    try {
        auto client_info = init_client_session(move(client));
        auto &socket = client_info->get_socket();
        auto &player = client_info->get_player();
		game->addPlayer(client_info);
        socket << "Welcome, " << player.get_name() << ", have fun playing our game!\r\n" << machiavelli::prompt;
	    if (game->amountOfPlayers() >= 2)
	    {
			game->startGame();
	    }

        while (running) { // game loop
            try {
                // read first line of request
                std::string cmd;
                if (socket.readline([&cmd,&client_info](std::string input) { cmd=input; })) {
                    cerr << '[' << socket.get_dotted_ip() << " (" << socket.get_socket() << ") " << player.get_name() << "] " << cmd << "\r\n";

                    if (cmd == "quit") {
                        socket.write("Bye!\r\n");
						auto otherPlayer = game->getOtherPlayer(client_info);
						otherPlayer.get()->get_socket() << client_info->get_player().get_name() << " heeft het spel verlaten.\n" << machiavelli::prompt;
						
                        break; // out of game loop, will end this thread and close connection
                    }
                    else if (cmd == "quit_server") {
                        running = false;
                    }

                    ClientCommand command {cmd, client_info};
                    queue.put(command);
                };

            } catch (const exception& ex) {
                socket << "ERROR: " << ex.what() << "\r\n";
            } catch (...) {
                socket.write("ERROR: something went wrong during handling of your request. Sorry!\r\n");
            }
        }
        // close weg
    } 
    catch(std::exception &ex) {
        cerr << "handle_client " << ex.what() << "\n";
    }
    catch (...) {
        cerr << "handle_client crashed\n";
    }
}

void start ()
{
	Game game;
	// start command consumer thread
	vector<thread> all_threads;
	all_threads.emplace_back(consume_command, &game);

	// create a server socket
	ServerSocket server{ machiavelli::tcp_port };

	try {
		cerr << "server listening" << '\n';
		while (running) {
			// wait for connection from client; will create new socket
			server.accept([&all_threads,&game](Socket client) {				
				std::cerr << "Connection accepted from " << client.get_dotted_ip() << "\n";
				all_threads.emplace_back(handle_client, move(client),&game);
			});
			this_thread::sleep_for(chrono::milliseconds(100));
		}
	}
	catch (const exception& ex) {
		cerr << ex.what() << ", resuming..." << '\n';
	}
	catch (...) {
		cerr << "problems, problems, but: keep calm and carry on!\n";
	}

	for (auto &t : all_threads) {
		t.join();
	}
}

int main(int argc, const char * argv[])
{    
	start();
	_CrtDumpMemoryLeaks();
    return 0;
}

