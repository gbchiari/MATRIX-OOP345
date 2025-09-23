/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#ifndef SENECA_EVENT_H
#define SENECA_EVENT_H
#include <string>
#include <chrono>
#include <iostream>
namespace seneca {
    class Event {
        std::string m_name{};
        std::chrono::nanoseconds m_duration{};
    public:
        Event() = default;
        Event(const char* name, const std::chrono::nanoseconds& duration);
        friend std::ostream& operator<<(std::ostream& out, const Event& e);
    };
}
#endif