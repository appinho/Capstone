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
    _num_canvases(num_canvases),
    _canvas_width(500),
    _canvas_height(500){

    for(int i = 0; i < _num_canvases; i++){
        _canvases.push_back(cv::Mat(_canvas_width, _canvas_height, CV_8UC3, 
            cv::Scalar(255, 255, 255)));
    }
    _message_queue = std::make_shared<MessageQueue<int>>();
}

int Gallery::waitForAccess(){

    while (true) {

         // Wait between two cycles to relax CPU load
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        int canvas_id = _message_queue.get()->receive();
        if(canvas_id >= 0){
            return canvas_id;
        }
    }
}


void Gallery::Paint(const int id, const int skill, const std::string & name){

    // Get crazy drawing parameters
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_coord(0, _canvas_width);
    std::uniform_int_distribution<> dis_rad(20, 60);
    std::uniform_int_distribution<> dis_color(0, 255);
    int x1  = dis_coord(gen);
    int y1  = dis_coord(gen);
    int x2  = dis_coord(gen);
    int y2  = dis_coord(gen);
    int rad = dis_rad(gen);
    int rad2 = dis_rad(gen);
    int r = dis_color(gen);
    int g = dis_color(gen);
    int b = dis_color(gen);

    cv::Point p1(x1 , y1);
    cv::Point p2(x2 , y2);
    cv::Scalar color(r, g, b);

    // Lock canvas
    std::lock_guard<std::mutex> uLock(_mutex_canvas);

    // Draw depending on painters skill
    switch(skill){
        case 0:
            cv::arrowedLine(_canvases[id], p1, p2, color, 3);
            break;
        case 1:
            cv::circle(_canvases[id], p1, rad, color, CV_FILLED);
            break;
        case 2:
            cv::rectangle(_canvases[id], p1, p2, color, CV_FILLED);
            break;
        case 3:
            cv::ellipse(_canvases[id], p1, cv::Size(rad, rad2), 0.2, 0.5, 0.8, CV_FILLED);
            break;
        case 4:
            cv::line(_canvases[id], p1, p2, color, 3);
            break;
        case 5:
            cv::arrowedLine(_canvases[id], p1, p2, color, 1);
            break;
        case 6:
            cv::circle(_canvases[id], p1, rad, color, 3);
            break;
        case 7:
            cv::rectangle(_canvases[id], p1, p2, color, 3);
            break;
        case 8:
            cv::ellipse(_canvases[id], p1, cv::Size(rad, rad2), 0.2, 0.5, 0.8, 3);
            break;
        case 9:
            cv::line(_canvases[id], p1, p2, color, 1);
            break;
    }

    // Sign canvas
    cv::Point sign_1(_canvas_width * 0.55, _canvas_height * 0.9);
    cv::Point sign_2(_canvas_width, _canvas_height);
    cv::Point sign_3(_canvas_width * 0.57, _canvas_height * 0.97);
    cv::rectangle(_canvases[id], sign_1, sign_2, cv::Scalar(200,200,200), CV_FILLED);
    cv::putText(_canvases[id], name, sign_3, 2, 1, cv::Scalar(0, 0, 0));
}

void Gallery::simulate(){
	_threads.emplace_back(std::thread(&Gallery::accessCanvas, this));
}

void Gallery::accessCanvas(){

	std::cout << "accessCanvas" << "\n";
	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_time(100, 300);
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

        // When waiting time is long enough
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