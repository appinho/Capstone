#ifndef MAJOR_H
#define MAJOR_H

#include <thread>
#include <vector>

class Major{
public:
	// constructor / desctructor
    Major();
    ~Major();

	virtual void simulate(){};
protected:
	std::vector<std::thread> _threads;
};

#endif /* MAJOR_H */