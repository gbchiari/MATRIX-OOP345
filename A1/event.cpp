/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#include "event.h"
#include "settings.h"
namespace seneca {
    Event::Event(const char* name, const std::chrono::nanoseconds& duration)
        : m_name{ name }, m_duration{ duration } {
    }
    std::ostream& operator<<(std::ostream& out, const Event& e) {
        static int counter = 1;
        //40 caracteres, right
        out.width(2);
        out << counter++ << ": ";
        out.width(40);
        out << std::right << e.m_name << " -> ";
        //choose the time base i g_settings
        if (g_settings.m_time_units == "seconds") {
            out.width(2);
            out << std::right << std::chrono::duration_cast<std::chrono::seconds>(e.m_duration).count();
            out << " seconds";
        }
        else if (g_settings.m_time_units == "milliseconds") {
            out.width(5);
            out << std::right << std::chrono::duration_cast<std::chrono::milliseconds>(e.m_duration).count();
            out << " milliseconds";
        }
        else if (g_settings.m_time_units == "microseconds") {
            out.width(8);
            out << std::right << std::chrono::duration_cast<std::chrono::microseconds>(e.m_duration).count();
            out << " microseconds";
        }
        else {
            out.width(11);
            out << std::right << e.m_duration.count();
            out << " nanoseconds";
        }
        return out;
    }
}