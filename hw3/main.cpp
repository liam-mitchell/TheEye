#include <iostream>
#include "Image.h"
#include "Scene.h"
#include "Parser.h"

int main(int argc, char *argv[])
{
	if (argc != 2) //require scene file input
	{
		std::cout << "Error: incorrect number of arguments (format: raytracer.exe filename.txt)\n";
		return 0;
	}
	else
	{
		Scene scene;
		Parser parser;
		parser.buildSceneFromFile(scene, argv[1]);
		Image image = scene.render();
		image.writeImage("output.png");
		return 1;
	}
}
