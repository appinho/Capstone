#ifndef PAINTER_H
#define PAINTER_H

#include "museum.h"
#include "gallery.h"
#include <string>

// class Gallery;

class Painter: public Museum, public std::enable_shared_from_this<Painter>{
public:
	Painter(const std::string & name, const int skill);

	void setGallery(std::shared_ptr<Gallery> gallery);
	int getSkill();
	std::string getName();
	void simulate();
	std::shared_ptr<Painter> get_shared_this() { return shared_from_this(); }

private:
	void paint();

	std::shared_ptr<Gallery> _current_gallery;
	std::string _name;
	int _skill;
};

#endif /* PAINTER_H */