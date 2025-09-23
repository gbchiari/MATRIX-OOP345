/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#ifndef SENECA_LOGGER_H
#define SENECA_LOGGER_H
#include "event.h"
namespace seneca {
    class Logger {
        Event* m_events{};
        size_t m_size{};
        size_t m_capacity{};
        void resize(); //private function 

    public:
        Logger();
        virtual ~Logger();
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&& other); //move constructor
        Logger& operator=(Logger&& other); //move assignment
        void addEvent(const Event& event);
        friend std::ostream& operator<<(std::ostream& out, const Logger& logger);
    };
}
#endif