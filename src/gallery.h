#ifndef GALLERY_H
#define GALLERY_H

#include <condition_variable>
#include <deque>
#include <future>
#include <memory>
#include <opencv2/core.hpp>
#include <vector>

#include "museum.h"

// #include "painter.h"
class Painter;

template <class T>
class MessageQueue
{
public:
	T receive();
    void send(T &&msg);

private:
    std::mutex _mutex;
    std::condition_variable _cond;
    std::deque<T> _queue; // FIFO
};

class Gallery: public Museum{

public:
	Gallery(const int num_canvases);

	void Paint(const int id, const int skill, const std::string & name);
	void Draw(const int id, std::shared_ptr<Painter> painter);

	int waitForAccess();
	void simulate();
	void show();

private:
	void accessCanvas();
	std::shared_ptr<MessageQueue<int>> _message_queue;
	std::vector<cv::Mat> _canvases;
	std::mutex _mutex_canvas;
	int _num_canvases;
	int _canvas_width;
	int _canvas_height;
};

#endif /* GALLERY_H */
