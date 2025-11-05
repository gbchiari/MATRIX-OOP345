/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "movie.h"
#include "settings.h"

namespace seneca {

    Movie::Movie(const std::string& title,
        unsigned short year,
        const std::string& summary)
        : MediaItem(title, summary, year) {
    }

    Movie* Movie::createItem(const std::string& src) {
        std::string line = src;
        MediaItem::trim(line);
        if (line.empty() || line[0] == '#') throw "Not a valid movie.";

        auto nextToken = [](std::string& s) {
            size_t pos = s.find(',');
            std::string token = (pos == std::string::npos) ? s : s.substr(0, pos);
            if (pos != std::string::npos) s.erase(0, pos + 1);
            MediaItem::trim(token);
            return token;
            };

        try {
            std::string title = nextToken(line);
            std::string yearStr = nextToken(line);
            std::string summary = line;
            MediaItem::trim(summary);

            int y = std::stoi(yearStr);
            if (y < 0 || y > 65535) throw 0;
            unsigned short year = static_cast<unsigned short>(y);

            return new Movie(title, year, summary);
        }
        catch (...) {
            throw "Not a valid movie.";
        }
    }

    void Movie::display(std::ostream& out) const {
        if (g_settings.m_tableView) {
            out << "M | ";
            out << std::left << std::setfill('.');
            out << std::setw(50) << this->getTitle() << " | ";
            out << std::right << std::setfill(' ');
            out << std::setw(9) << this->getYear() << " | ";
            out << std::left;
            if (g_settings.m_maxSummaryWidth > -1) {
                if (static_cast<short>(this->getSummary().size()) <= g_settings.m_maxSummaryWidth)
                    out << this->getSummary();
                else
                    out << this->getSummary().substr(0, g_settings.m_maxSummaryWidth - 3) << "...";
            }
            else {
                out << this->getSummary();
            }
            out << std::endl;
        }
        else {
            size_t pos = 0;
            out << this->getTitle() << " [" << this->getYear() << "]\n";
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << "" << '\n';
            while (pos < this->getSummary().size()) {
                out << "    " << this->getSummary().substr(pos, g_settings.m_maxSummaryWidth) << '\n';
                pos += g_settings.m_maxSummaryWidth;
            }
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << ""
                << std::setfill(' ') << '\n';
        }
    }
}