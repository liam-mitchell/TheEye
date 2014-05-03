#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include "Image.h"
#include "Scene.h"
#include "Parser.h"

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

		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
		Image image = scene.render();
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

		std::chrono::duration<double> time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		std::cout << "Time taken (single thread): " << time.count() << '\n';
		image.writeImage("output.png");
		return 0;
	} else if (argc == 3) {
		Scene scene;
		Parser parser;
		parser.buildSceneFromFile(scene, argv[1]);
		Image image = scene.generateBlankImage();
		
		std::string arg(argv[2]);
		if (arg != "-parallel") std::cout << "Error: unrecognized command line argument " << arg << '\n';
		std::size_t thread_count = std::thread::hardware_concurrency();
		if (thread_count == 0) thread_count = 1;

		std::vector<std::thread> threads(thread_count);
		int x = 0;
		int y = 0;
		int width = scene.width();
		int height = scene.height() / thread_count;

		std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

		for (std::size_t i = 0; i < thread_count - 1; ++i) {
			threads[i] = std::thread([x, y, width, height, &scene, &image] {
				scene.partialRender(image, x, y, width, height);
			});
			y += height;
		}

		height += scene.height() % thread_count;
		threads[thread_count - 1] = std::thread([x, y, width, height, &scene, &image] {
			scene.partialRender(image, x, y, width, height);
		});

		for (auto& thread: threads) {
			thread.join();
		}

		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

		std::chrono::duration<double> time = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		std::cout << "Time taken (mulitiple threads): " << time.count() << '\n';
		std::cout << "Number of hardware threads available for use: " << std::thread::hardware_concurrency() << '\n';

		image.writeImage("output.png");
		return 0;
	} else {
		std::cout << "Incorrect number of command line arguments\n";
		std::cout << "Usage: raytracer.exe scenefile.txt [-parallel]";
	}
			
}
