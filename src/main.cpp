#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "painter.h"
#include "gallery.h"

int main() {

	int num_canvases;
	int num_painters;
	std::cout << "Enter the number of canvases, between 1-5, you want to be painted!" << "\n";
	std::cin >> num_canvases;
	if(num_canvases < 1){
		std::cout << "Number too small! Chose minimum: 1 canvas" << "\n";
		num_canvases = 1;
	}
	if(num_canvases > 5){
		std::cout << "Number too big! Chose maximum: 5 canvases" << "\n";
		num_canvases = 5;
	}
	std::cout << "Enter the number of painters, between 1-10, you want to be painted!" << "\n";
	std::cin >> num_painters;
	if(num_painters < 1){
		std::cout << "Number too small! Chose minimum: 1 painter" << "\n";
		num_painters = 1;
	}
	if(num_painters > 10){
		std::cout << "Number too big! Chose maximum: 10 painters" << "\n";
		num_painters = 10;
	}

    std::vector<std::shared_ptr<Gallery>> galleries;
    std::vector<std::shared_ptr<Painter>> painters;

    galleries.push_back(std::make_shared<Gallery>(num_canvases));
    std::for_each(galleries.begin(), galleries.end(), [](std::shared_ptr<Gallery> &g) {
        g->simulate();
    });

    std::vector<std::string> names = {"Van Gogh", "Picasso", "Munch"};
    for(int i = 0; i < num_painters; i++){
    	painters.push_back(std::make_shared<Painter>(names[i%names.size()]));
    	painters[i]->setGallery(galleries[i % galleries.size()]);
    }
    std::for_each(painters.begin(), painters.end(), [](std::shared_ptr<Painter> &p) {
        p->simulate();
    });

    std::for_each(galleries.begin(), galleries.end(), [](std::shared_ptr<Gallery> &g) {
        g->show();
    });

	return 0;
}