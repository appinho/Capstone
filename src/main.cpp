#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "painter.h"
#include "gallery.h"

int main() {

	// I/O to user 
	int num_canvases;
	int num_painters;
	std::cout << "Welcome to the Crazy Museum!" << "\n";
	std::cout << "Enter the number of crazy canvases, between 1-5, you want to look at!" << "\n";
	std::cin >> num_canvases;
	if(num_canvases < 1){
		std::cout << "Not crazy enough! Chose minimum of 1 crazy canvas" << "\n";
		num_canvases = 1;
	}
	if(num_canvases > 5){
		std::cout << "Too crazy! Chose maximum of 5 crazy canvases" << "\n";
		num_canvases = 5;
	}
	std::cout << "Enter the number of crazy painters, between 1-10, that can show their passion!" << "\n";
	std::cin >> num_painters;
	if(num_painters < 1){
		std::cout << "Come on! Chose minimum of 1 crazy painter" << "\n";
		num_painters = 1;
	}
	if(num_painters > 10){
		std::cout << "Ok, hold on! Chose maximum of 10 crazy painters" << "\n";
		num_painters = 10;
	}

	// Initialize threads
    std::vector<std::shared_ptr<Gallery>> galleries;
    std::vector<std::shared_ptr<Painter>> painters;

    // Set up galleries
    galleries.push_back(std::make_shared<Gallery>(num_canvases));
    std::for_each(galleries.begin(), galleries.end(), [](std::shared_ptr<Gallery> &g) {
        g->simulate();
    });

    // Set up painters
    std::vector<std::string> painter_names = {"Van Gogh", "Picasso", "Da Vinci", "Monet", "Munch",
		"Michelangelo", "Dali", "Frida", "Rembrandt", "Vermeer"};
    for(int i = 0; i < num_painters; i++){
    	painters.push_back(std::make_shared<Painter>(painter_names[i % painter_names.size()], i));
    	painters[i]->setGallery(galleries[i % galleries.size()]);
    }

    // Simulate
    std::for_each(painters.begin(), painters.end(), [](std::shared_ptr<Painter> &p) {
        p->simulate();
    });
    std::for_each(galleries.begin(), galleries.end(), [](std::shared_ptr<Gallery> &g) {
        g->show();
    });

	return 0;
}