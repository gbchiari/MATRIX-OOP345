/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include "LineManager.h"
#include "Utilities.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

namespace seneca {

    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream input(file);
        if (!input)
            throw std::runtime_error("Cannot open file: " + file);

        Utilities util;
        std::string line;
        std::vector<std::string> loadedStations;

        while (std::getline(input, line)) {
            size_t next_pos = 0;
            bool more = true;
            std::string current = util.extractToken(line, next_pos, more);
            std::string next = more ? util.extractToken(line, next_pos, more) : "";

            auto currentWS = std::find_if(stations.begin(), stations.end(),
                [&](const Workstation* ws) { return ws->getItemName() == current; });

            auto nextWS = std::find_if(stations.begin(), stations.end(),
                [&](const Workstation* ws) { return ws->getItemName() == next; });

            if (currentWS != stations.end()) {
                if (nextWS != stations.end()) {
                    (*currentWS)->setNextStation(*nextWS);
                }

                m_activeLine.push_back(*currentWS);
                loadedStations.push_back(current);
            }
        }

        m_firstStation = *std::find_if(m_activeLine.begin(), m_activeLine.end(),
            [&](const Workstation* ws) {
                return std::none_of(m_activeLine.begin(), m_activeLine.end(),
                    [&](const Workstation* other) {
                        return other->getNextStation() == ws;
                    });
            });

        m_cntCustomerOrder = g_pending.size();
    }

    void LineManager::reorderStations() {
        std::vector<Workstation*> reordered;

        Workstation *current = m_firstStation;

        while (current) {
            reordered.push_back(current);
            current = current->getNextStation();
        }

        m_activeLine = std::move(reordered);
    }

    [[nodiscard]] bool LineManager::run(std::ostream& os) {
        static size_t iteration = 0;

        os << "Line Manager Iteration: " << ++iteration << std::endl;

        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        for (auto& station : m_activeLine) {
            station->fill(os);
        }

        for (auto& station : m_activeLine) {
            station->attemptToMoveOrder();
        }

        if (g_completed.size() + g_incomplete.size() == m_cntCustomerOrder) {
            return true;
        }

        return false;
    }

    void LineManager::display(std::ostream& os) const {
        for (const auto& station : m_activeLine) {
            station->display(os);
        }
    }

}
