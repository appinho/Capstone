#ifndef PAINTER_H
#define PAINTER_H

#include "major.h"
#include "gallery.h"
#include <string>

class Painter: public Major, public std::enable_shared_from_this<Painter>{
public:
	Painter(const std::string & name);
	void setGallery(std::shared_ptr<Gallery> gallery);
	void simulate();

	std::shared_ptr<Painter> get_shared_this() { return shared_from_this(); }
	void drawStuff(cv::Mat m);
private:
	void paint();
	std::shared_ptr<Gallery> _current_gallery;
	std::string _name;
};

#endif /* PAINTER_H */