/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include "collection.h"
#include <algorithm>
#include <stdexcept>
#include <string>

namespace seneca {

    Collection::Collection(const std::string& name) : m_name(name) {}

    Collection::~Collection() {
        for (auto* p : m_items) {
            delete p;
        }
    }

    const std::string& Collection::name() const { return m_name; }

    size_t Collection::size() const { return m_items.size(); }

    void Collection::setObserver(void (*observer)(const Collection&, const MediaItem&)) {
        m_observer = observer;
    }

    Collection& Collection::operator+=(MediaItem* item) {
        if (!item) return *this;

        // Check duplicate title
        auto it = std::find_if(m_items.begin(), m_items.end(),
            [t = item->getTitle()](MediaItem* p) { return p->getTitle() == t; });

        if (it != m_items.end()) {
            delete item; // already present -> delete parameter
        }
        else {
            m_items.push_back(item);
            if (m_observer)
                m_observer(*this, *item);
        }
        return *this;
    }

    MediaItem* Collection::operator[](size_t idx) const {
        if (idx >= m_items.size()) {
            throw std::out_of_range("Bad index " + std::to_string(idx) +
                ". Collection has " + std::to_string(m_items.size()) + " items.");
        }
        return m_items[idx];
    }

    // TASK: STL algorithm, no manual loops, lambda no ref-capture
    MediaItem* Collection::operator[](const std::string& title) const {
        auto it = std::find_if(m_items.begin(), m_items.end(),
            [title](MediaItem* p) { return p->getTitle() == title; });
        return (it == m_items.end()) ? nullptr : *it;
    }

    // TASK: STL algorithm, remove quotes from title & summary
    void Collection::removeQuotes() {
        auto strip = [](std::string& s) {
            if (!s.empty() && s.front() == '"') s.erase(0, 1);
            if (!s.empty() && s.back() == '"') s.pop_back();
            };

        std::for_each(m_items.begin(), m_items.end(),
            [strip](MediaItem* p) {
                std::string t = p->getTitle();
                std::string s = p->getSummary();
                strip(t);
                strip(s);
                p->setTitle(t);
                p->setSummary(s);
            });
    }

    // TASK: STL algorithm, sort by field
    void Collection::sort(const std::string& field) {
        std::sort(m_items.begin(), m_items.end(),
            [field](MediaItem* a, MediaItem* b) {
                if (field == "title")   return a->getTitle() < b->getTitle();
                if (field == "year")    return a->getYear() < b->getYear();
                if (field == "summary") return a->getSummary() < b->getSummary();
                return a->getTitle() < b->getTitle(); // default
            });
    }

    std::ostream& operator<<(std::ostream& out, const Collection& col) {
        for (size_t i = 0; i < col.size(); ++i)
            out << *col[i];
        return out;
    }
}