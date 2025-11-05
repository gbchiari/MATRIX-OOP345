/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#ifndef SENECA_COLLECTION_H
#define SENECA_COLLECTION_H

#include <string>
#include <vector>
#include "mediaItem.h"

namespace seneca {

    class Collection {
        std::string m_name{};
        std::vector<MediaItem*> m_items{};
        void (*m_observer)(const Collection&, const MediaItem&) = nullptr;

    public:
        Collection(const std::string& name);
        ~Collection();

        Collection(const Collection&) = delete;
        Collection& operator=(const Collection&) = delete;
        Collection(Collection&&) = delete;
        Collection& operator=(Collection&&) = delete;

        const std::string& name() const;
        size_t size() const;

        void setObserver(void (*observer)(const Collection&, const MediaItem&));

        Collection& operator+=(MediaItem* item);

        MediaItem* operator[](size_t idx) const;                // throws std::out_of_range
        MediaItem* operator[](const std::string& title) const;  // TASK: STL algorithm, no manual loop

        void removeQuotes();                                     // TASK: STL algorithm
        void sort(const std::string& field);                     // TASK: STL algorithm
    };

    std::ostream& operator<<(std::ostream& out, const Collection& col);
}

#endif