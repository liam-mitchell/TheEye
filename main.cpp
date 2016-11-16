#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>

#include "Scene.h"
#include "Parser.h"
#include "Window.h"

int main(int argc, char *argv[])
{
	if (argc < 2) //require scene file input
	{
		std::cout << "Error: incorrect number of arguments (format: raytracer.exe filename.txt [-parallel])\n";
		return 0;
	}
	else if (argc == 2)
	{
		Scene scene;
		Parser parser;
		parser.buildSceneFromFile(scene, argv[1]);

                double time = glfwGetTime();
                while (true) {
                        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
                        double dt = glfwGetTime() - time;

                        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
                        glClear(GL_COLOR_BUFFER_BIT);

                        scene.render();
                        scene.update(dt);

                        time += dt;

                        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
                        std::chrono::duration<double> time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
                        std::cout << "Time taken (GPU): " << time.count() << '\n';

                        glfwPollEvents();
                }

		return 0;
	} else {
		std::cout << "Incorrect number of command line arguments\n";
		std::cout << "Usage: raytracer.exe scenefile.txt [-parallel]";
	}
			
}
