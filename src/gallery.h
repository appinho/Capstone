#ifndef GALLERY_H
#define GALLERY_H

#include <future>
#include <vector>
#include <memory>
#include "major.h"
#include <opencv2/core.hpp>
#include <deque>
#include <condition_variable>

// forward declarations to avoid include cycle
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

class Gallery: public Major{

public:
	Gallery(const int num_canvases);

	void Paint(const int id, Painter * painter);
	void Draw(const int id, std::shared_ptr<Painter> painter);

	int waitToPaint();
	void simulate();
	void paint();

	void show();

private:
	void accessCanvas();
	std::shared_ptr<MessageQueue<int>> _message_queue;
	std::vector<cv::Mat> _canvases;
	std::mutex _mutex_canvas;
	int _num_canvases;
};

#endif /* GALLERY_H */
