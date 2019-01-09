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
					if (command.get_cmd() == "help")
					{
						client << "\r\nBasis van een beurt:\r\n"
							<< "Pak 2 goudstukken of pak 2 kaarten en leg daar 1 van terug.\r\n"
							<< "Leg 1 bouwkaart neer en betaal de waarde.\r\n"
							<< "Een karaktereigenschap is op elk moment te gebruiken.\r\n"
							<< "\r\n"
							<< "Karakterss:\r\n"
							<< "1: Moordenaar, vermoord een ander karakter.\r\n"
							<< "2: Dief, steelt van een andere speler.\r\n"
							<< "3: Magiër, ruilt bouwkaarten om.\r\n"
							<< "4: Koning, begint de volgende ronde en ontvangt van monumenten.\r\n"
							<< "5: Prediker, is beschermd tegen condotière en ontvangt van kerkelijke gebouwen.\r\n"
							<< "6: Koopman, ontvangt 1 extra goudstuk en ontvangt van commerciële gebouwen.\r\n"
							<< "7: Bouwmeester, trekt 2 extra kaarten en mag 3 gebouwen bouwen.\r\n"
							<< "8: Condotière, vernietigd een gebouw en ontvangt van militaire gebouwen.\r\n"
							<< machiavelli::prompt;
					}
	                else if (game->getCurrentPlayer() == clientInfo)
	                {
						if (game->getState() == "start" && command.get_cmd() == "kies karakters") game->roundSetup();
						if (game->getState() == "start" && command.get_cmd() == "cheatmodus") game->cheatSetup();
						if (game->getState() == "setup" && command.get_cmd() == "start ronde") game->startRound();
	                }
					else if (game->getCurrentPlayer() != clientInfo)
					{
						client << "\r\nDe tegenstander is aan de beurt.\r\n";
					}					
					else client << "\r\nOpdracht niet herkent.\r\n";
                    

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
    client.write("Welkom op de server! Stop het spel met 'quit'.\r\n");
    client.write("Wat is jouw naam?\r\n");
    client.write("machiavelli> ");
    string name;
    bool done { false };
    while(!done) {
        done = client.readline([&name](std::string input) {
            name = input;
        });
    }
	client.write("\r\n\Hoe oud ben je?(oudste begint)\r\n");
	client.write("machiavelli> ");
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
			client.write("\r\n\Hoe oud ben je?(oudste begint)\r\n");
			client.write("machiavelli> ");
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
        socket << "Welkom, " << player.get_name() << ", veel plezier met machiavelli!\r\n\r\n";
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
                        socket.write("\r\nHoudoe!\r\n");
						if(game->amountOfPlayers() >= 2)
						{
							auto otherPlayer = game->getOtherPlayer(client_info);
							otherPlayer.get()->get_socket() << "\r\n"<< client_info->get_player().get_name() << " heeft het spel verlaten.\n" << machiavelli::prompt;
						}						
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

