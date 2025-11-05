/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include "book.h"
#include "settings.h"

namespace seneca {

    // Private constructor used by createItem
    Book::Book(const std::string& author, const std::string& title,
        const std::string& country, double price,
        unsigned short year, const std::string& summary)
        : MediaItem(title, summary, year), m_author(author), m_country(country), m_price(price) {
    }

    // Static function to create a Book from a CSV line
    Book* Book::createItem(const std::string& src) {
        std::string line = src;
        MediaItem::trim(line);
        if (line.empty() || line[0] == '#') throw "Not a valid book.";

        auto nextToken = [](std::string& s) {
            size_t pos = s.find(',');
            std::string token = (pos == std::string::npos) ? s : s.substr(0, pos);
            if (pos != std::string::npos) s.erase(0, pos + 1);
            MediaItem::trim(token);
            return token;
            };

        try {
            std::string author = nextToken(line);
            std::string title = nextToken(line);
            std::string country = nextToken(line);
            std::string priceStr = nextToken(line);
            std::string yearStr = nextToken(line);
            std::string summary = line;
            MediaItem::trim(summary);

            double price = std::stod(priceStr);
            int    y = std::stoi(yearStr);
            if (y < 0 || y > 65535) throw 0;
            unsigned short year = static_cast<unsigned short>(y);

            return new Book(author, title, country, price, year, summary);
        }
        catch (...) {
            throw "Not a valid book.";
        }
    }

    // Display function override
    void Book::display(std::ostream& out) const {
        if (g_settings.m_tableView) {
            out << "B | ";
            out << std::left << std::setfill('.');
            out << std::setw(50) << this->getTitle() << " | ";
            out << std::right << std::setfill(' ');
            out << std::setw(2) << m_country << " | ";
            out << std::setw(4) << this->getYear() << " | ";
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
            out << this->getTitle() << " [" << this->getYear() << "] ["
                << m_author << "] [" << m_country << "] [" << m_price << "]\n";
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