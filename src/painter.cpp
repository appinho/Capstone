#include "painter.h"
#include <iostream>
#include <opencv2/imgproc.hpp>

Painter::Painter(const std::string & name):
	_name(name){
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

        int id =_current_gallery->waitToPaint();
        std::cout << _name << " paints" << "\n";
        _current_gallery->Paint(id, this);

        // request entry to the current intersection (using async)
        // auto ftrEntryGranted = std::async(&Gallery::Draw, _current_gallery, id, get_shared_this());

        // wait until entry has been granted
        // ftrEntryGranted.get();

        std::cout << _name << " painted " << id << "\n";
    }
}

void Painter::drawStuff(cv::Mat m){

	cv::circle(m, cv::Point(200 , 200 ), 30, cv::Scalar(255, 0, 0), CV_FILLED);
}