#include "message_queue.h"

template <typename T>
T MessageQueue2<T>::receive()
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
void MessageQueue2<T>::send(T &&msg)
{
    // Prevent data race
    std::lock_guard<std::mutex> uLock(_mutex);

    // Move into queue
    _queue.push_back(std::move(msg));
    _cond.notify_one(); // notify client
}