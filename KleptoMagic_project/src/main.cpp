// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include "sdlutils/sdlutils_demo.h"
#include "ecs/Game.h"

int main(int ac, char **av) {
	try {
		//sdlutils_basic_demo();

			if (!Game::Init()) {
				std::cerr << "Error al inicializar el juego Init." << std::endl;
				return 1;  // Salir con error si no se pudo inicializar
			}

			if (!game().initGame()) {
				std::cerr << "Error al inicializar el juego initGame." << std::endl;
				return 1;
			}

			// Usar el singleton para ejecutar el juego
			game().start();

			// Liberar el singleton al finalizar
			Game::Release();
		}
		catch (const std::string& e) { // catch exceptions thrown as strings
			std::cerr << e << std::endl;
		}
		catch (const char* e) { // catch exceptions thrown as char*
			std::cerr << e << std::endl;
		}
		catch (const std::exception& e) { // catch exceptions thrown as a sub-type of std::exception
			std::cerr << e.what();
		}
		catch (...) {
			std::cerr << "Caught and exception of unknown type ...";
		}

		return 0;
		
		
		/*
		Game game;
		game.run();
	} catch (const std::string &e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	} catch (const char *e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	} catch (const std::exception &e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	} catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
	*/
}

