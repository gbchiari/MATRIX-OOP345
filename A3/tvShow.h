/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#ifndef SENECA_TVSHOW_H
#define SENECA_TVSHOW_H

#include <string>
#include <vector>
#include <list>
#include "mediaItem.h"

namespace seneca {

    struct TvEpisode {
        const class TvShow* m_show{};
        unsigned short m_numberOverall{};
        unsigned short m_season{};
        unsigned short m_numberInSeason{};
        std::string    m_airDate{};
        unsigned int   m_length{}; // seconds
        std::string    m_title{};
        std::string    m_summary{};
    };

    class TvShow : public MediaItem {
        unsigned int        m_id{};
        std::vector<TvEpisode> m_episodes;

        // Private ctor used by createItem()
        TvShow(unsigned int id,
            const std::string& title,
            unsigned short year,
            const std::string& summary);

    public:
        // Rule of 0 – defaulted destructor is fine
        ~TvShow() override = default;

        void display(std::ostream& out) const override;

        static TvShow* createItem(const std::string& strShow);

        template <typename Collection_t>
        static void addEpisode(Collection_t& col, const std::string& strEpisode);

        double getEpisodeAverageLength() const;
        std::list<std::string> getLongEpisodes() const;
    };

    // Template implementation must be in header
    template <typename Collection_t>
    void TvShow::addEpisode(Collection_t& col, const std::string& src) {
        std::string line = src;
        MediaItem::trim(line);
        if (line.empty() || line[0] == '#') throw "Not a valid episode.";

        auto nextToken = [](std::string& s) {
            size_t pos = s.find(',');
            std::string token = (pos == std::string::npos) ? s : s.substr(0, pos);
            if (pos != std::string::npos) s.erase(0, pos + 1);
            MediaItem::trim(token);
            return token;
            };

        try {
            TvEpisode ep{};

            std::string idStr = nextToken(line);
            std::string overallStr = nextToken(line);
            std::string seasonStr = nextToken(line);
            std::string inSeasonStr = nextToken(line);
            std::string airDate = nextToken(line);
            std::string lengthStr = nextToken(line);
            std::string title = nextToken(line);
            std::string summary = line;
            MediaItem::trim(summary);

            unsigned int showId = static_cast<unsigned int>(std::stoul(idStr));
            ep.m_numberOverall = static_cast<unsigned short>(std::stoi(overallStr));
            ep.m_season = seasonStr.empty() ? 1 : static_cast<unsigned short>(std::stoi(seasonStr));
            ep.m_numberInSeason = static_cast<unsigned short>(std::stoi(inSeasonStr));
            ep.m_airDate = airDate;
            ep.m_length = [&lengthStr]() -> unsigned int {
                if (lengthStr.empty()) return 0u;
                auto pos1 = lengthStr.find(':');
                if (pos1 == std::string::npos)
                    return static_cast<unsigned int>(std::stoul(lengthStr));
                auto pos2 = lengthStr.find(':', pos1 + 1);
                unsigned int h = static_cast<unsigned int>(std::stoul(lengthStr.substr(0, pos1)));
                unsigned int m = static_cast<unsigned int>(std::stoul(lengthStr.substr(pos1 + 1, pos2 - pos1 - 1)));
                unsigned int s = static_cast<unsigned int>(std::stoul(lengthStr.substr(pos2 + 1)));
                return h * 3600u + m * 60u + s;
                }();
            ep.m_title = title;
            ep.m_summary = summary;

            TvShow* where = nullptr;
            for (size_t i = 0; i < col.size() && !where; ++i) {
                if (auto* tv = dynamic_cast<TvShow*>(col[i]); tv && tv->m_id == showId)
                    where = tv;
            }
            if (!where) throw 0;

            ep.m_show = where;
            where->m_episodes.push_back(ep);
        }
        catch (...) {
            throw "Not a valid episode.";
        }
    }
}
#endif