/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <iomanip>
#include <numeric>
#include <algorithm>
#include "tvShow.h"
#include "settings.h"

namespace seneca {

    // Private
    TvShow::TvShow(unsigned int id,
        const std::string& title,
        unsigned short year,
        const std::string& summary)
        : MediaItem(title, summary, year), m_id(id) {
    }

    // Static creator from CSV line: ID,TITLE,YEAR,SUMMARY
    TvShow* TvShow::createItem(const std::string& src) {
        std::string line = src;
        MediaItem::trim(line);
        if (line.empty() || line[0] == '#') throw "Not a valid show.";

        auto nextToken = [](std::string& s) {
            size_t pos = s.find(',');
            std::string token = (pos == std::string::npos) ? s : s.substr(0, pos);
            if (pos != std::string::npos) s.erase(0, pos + 1);
            MediaItem::trim(token);
            return token;
            };

        try {
            std::string idStr = nextToken(line);
            std::string title = nextToken(line);
            std::string yearStr = nextToken(line);
            std::string summary = line;
            MediaItem::trim(summary);

            unsigned int id = static_cast<unsigned int>(std::stoul(idStr));
            int y = std::stoi(yearStr);
            if (y < 0 || y > 65535) throw 0;
            unsigned short year = static_cast<unsigned short>(y);

            return new TvShow(id, title, year, summary);
        }
        catch (...) {
            throw "Not a valid show.";
        }
    }

    // Display implementation as provided in the readme
    void TvShow::display(std::ostream& out) const {
        if (g_settings.m_tableView) {
            out << "S | ";
            out << std::left << std::setfill('.');
            out << std::setw(50) << this->getTitle() << " | ";
            out << std::right << std::setfill(' ');
            out << std::setw(2) << this->m_episodes.size() << " | ";
            out << std::setw(4) << this->getYear() << " | ";
            out << std::left;
            if (g_settings.m_maxSummaryWidth > -1) {
                if (static_cast<short>(this->getSummary().size()) <= g_settings.m_maxSummaryWidth)
                    out << this->getSummary();
                else
                    out << this->getSummary().substr(0, g_settings.m_maxSummaryWidth - 3) << "...";
            }
            else
                out << this->getSummary();
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
            for (auto& item : m_episodes) {
                out << std::setfill('0') << std::right;
                out << "    " << 'S' << std::setw(2) << item.m_season
                    << 'E' << std::setw(2) << item.m_numberInSeason << ' ';
                if (item.m_title != "")
                    out << item.m_title << '\n';
                else
                    out << "Episode " << item.m_numberOverall << '\n';

                pos = 0;
                while (pos < item.m_summary.size()) {
                    out << "            " << item.m_summary.substr(pos, g_settings.m_maxSummaryWidth - 8) << '\n';
                    pos += g_settings.m_maxSummaryWidth - 8;
                }
            }
            out << std::setw(this->getTitle().size() + 7) << std::setfill('-') << ""
                << std::setfill(' ') << '\n';
        }
    }

    // TASK 1: average length using STL algorithms, no manual loops, no reference captures
    double TvShow::getEpisodeAverageLength() const {
        if (m_episodes.empty()) return 0.0;
        using ull = unsigned long long;
        ull total = std::accumulate(
            m_episodes.begin(), m_episodes.end(), ull{ 0 },
            [](ull acc, const TvEpisode& e) { return acc + static_cast<ull>(e.m_length); }
        );
        return static_cast<double>(total) / static_cast<double>(m_episodes.size());
    }

    // TASK 2: list of episode titles >= 1 hour using STL algorithms, no manual loops, no reference captures
    std::list<std::string> TvShow::getLongEpisodes() const {
        std::vector<TvEpisode> tmp;
        std::copy_if(m_episodes.begin(), m_episodes.end(),
            std::back_inserter(tmp),
            [](const TvEpisode& e) { return e.m_length >= 3600u; });

        std::list<std::string> out;
        std::transform(tmp.begin(), tmp.end(),
            std::back_inserter(out),
            [](const TvEpisode& ep) {
                return ep.m_title.empty()
                    ? std::string("Episode ") + std::to_string(ep.m_numberOverall)
                    : ep.m_title;
            });
        return out;
    }
}