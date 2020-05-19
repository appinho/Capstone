#ifndef MUSEUM_H
#define MUSEUM_H

#include <thread>
#include <vector>

class Museum{
public:
    Museum();
    ~Museum();

	virtual void simulate(){};
protected:
	std::vector<std::thread> _threads;
};

#endif /* MUSEUM_H */