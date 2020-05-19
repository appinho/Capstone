#include "painter.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <future>

Painter::Painter(const std::string & name, const int skill):
	_name(name),
	_skill(skill){
}

int Painter::getSkill(){
    return _skill;
}

std::string Painter::getName(){
    return _name;
}

void Painter::setGallery(std::shared_ptr<Gallery> gallery){
	_current_gallery = gallery;
}

void Painter::simulate(){
    _threads.emplace_back(std::thread(&Painter::paint, this));
}

void Painter::paint(){

	while (true) {

    	// Wait between two cycles to relax CPU load
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // Wait to access gallery
        int id =_current_gallery->waitForAccess();
        // cv::Mat canvas = _current_gallery->permitAccess();


        // Painter enters gallery and paints on canvas
        _current_gallery->Paint(id, _skill, _name);
        std::cout << _name << " painted canvas " << id << "\n";
    }
}
