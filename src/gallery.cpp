#include "gallery.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

template <typename T>
T MessageQueue<T>::receive()
{
    // Create a lock and pass it to the condition variable
    std::unique_lock<std::mutex> uLock(_mutex);
    _cond.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

    // Get the latest element and remove it from the queue
    T msg = std::move(_queue.front());
    _queue.pop_front();

    return msg; // will not be copied due to return value optimization (RVO) in C++
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // Prevent data race
    std::lock_guard<std::mutex> uLock(_mutex);

    // Move into queue
    _queue.push_back(std::move(msg));
    _cond.notify_one(); // notify client
}

Gallery::Gallery(const int num_canvases):
    _num_canvases(num_canvases){

    for(int i = 0; i < _num_canvases; i++){
        _canvases.push_back(cv::Mat(500, 500, CV_8UC3, cv::Scalar(255, 255, 255)));
    }
    _message_queue = std::make_shared<MessageQueue<int>>();
}

int Gallery::waitToPaint(){

    while (true) {

         // Wait between two cycles to relax CPU load
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // _message_queue.get()->send(Shape::kCircle);
        int canvas_id = _message_queue.get()->receive();
        if(canvas_id >= 0){
            return canvas_id;
        }
    }
}

void Gallery::Paint(const int id, Painter * painter){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_coord(0, 500);
    std::uniform_int_distribution<> dis_rad(20, 60);
    std::uniform_int_distribution<> dis_color(0, 255);
    int x  = dis_coord(gen);
    int y  = dis_coord(gen);
    int rad = dis_rad(gen);
    int r = dis_color(gen);
    int g = dis_color(gen);
    int b = dis_color(gen);
    std::cout << x << " " << y << " " << rad << " " << r << " " << g << " " << b << "\n";
    std::lock_guard<std::mutex> uLock(_mutex_canvas);
    cv::circle(_canvases[id], cv::Point(x , y ), rad, cv::Scalar(r, g, b), CV_FILLED);
}

void Gallery::Draw(const int id, std::shared_ptr<Painter> painter){

    // std::lock_guard<std::mutex> uLock(_mutex_canvas);
    // painter->drawStuff(_canvases[id]);
    // cv::circle(_canvases[id], cv::Point(200 , 200 ), 30, cv::Scalar(255, 0, 0), CV_FILLED);
}

void Gallery::simulate(){
	std::cout << "Simulate" << "\n";
	_threads.emplace_back(std::thread(&Gallery::accessCanvas, this));
}

void Gallery::accessCanvas(){

	std::cout << "accessCanvas" << "\n";
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_time(10, 20);
    std::uniform_int_distribution<> dis_canvas(1, _num_canvases);
    double cycleDuration = dis_time(gen); // duration of a single simulation cycle in ms

    // Timer to measure the time between two loop cycles
    std::chrono::time_point<std::chrono::system_clock> lastUpdate = std::chrono::system_clock::now();
    
    // Infinite loop
    while (true) {

    	// Wait between two cycles to relax CPU load
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // Timer to measure the time between two loop cycles
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();

    	// std::cout << "Loop " << timeSinceLastUpdate << " " << cycleDuration << "\n";
        // When waiting long enough
        if (timeSinceLastUpdate >= cycleDuration){

        	int msg = dis_canvas(gen) - 1;
        	std::cout << "Access to canvas " << msg << "\n";
            auto has_sent = std::async(std::launch::async, 
                &MessageQueue<int>::send, 
                _message_queue, 
                std::move(msg));
            has_sent.wait();

            cycleDuration = dis_time(gen);
            lastUpdate = std::chrono::system_clock::now();
        }

    }

}

void Gallery::show(){
    std::cout << "Show" << "\n";

    // Infinite loop
    while (true) {

        // Wait between two cycles to relax CPU load
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        for(int i = 0; i < _num_canvases; i++){
            std::string title = "Canvas " + std::to_string(i);
            cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
            cv::imshow(title, _canvases[i]);
            cv::waitKey(33);
        }
    }
}