/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#include "logger.h"
namespace seneca{

    Logger::Logger() = default;
    Logger::~Logger() {
        delete[] m_events;
    }

    void Logger::resize() {
        size_t newCapacity = (m_capacity == 0) ? 5 : m_capacity * 2;
        Event* temp = new Event[newCapacity];
        for (size_t i = 0; i < m_size; ++i) {
            temp[i] = m_events[i];
        }
        delete[] m_events;
        m_events = temp;
        m_capacity = newCapacity;
    }

    void Logger::addEvent(const Event& event) {
        if (m_size == m_capacity) {
            resize();
        }
        m_events[m_size++] = event;
    }

    Logger::Logger(Logger&& other) {
        m_events = other.m_events;
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        other.m_events = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    Logger& Logger::operator=(Logger&& other) {
        if (this != &other) {
            delete[] m_events;
            m_events = other.m_events;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_events = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }

    std::ostream& operator<<(std::ostream& out, const Logger& logger) {
        for (size_t i = 0; i < logger.m_size; ++i) {
            out << logger.m_events[i] << '\n';
        }
        return out;
    }

}