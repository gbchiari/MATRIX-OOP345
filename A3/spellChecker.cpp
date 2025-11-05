/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <fstream>
#include <iomanip>
#include "spellChecker.h"

namespace seneca {

    SpellChecker::SpellChecker(const char* filename) {
        std::ifstream fin(filename);
        if (!fin)
            throw "Bad file name!";

        for (size_t i = 0; i < 6 && fin; ++i) {
            fin >> m_badWords[i] >> m_goodWords[i];
            m_replaced[i] = 0;
        }
    }

    void SpellChecker::operator()(std::string& text) {
        for (size_t i = 0; i < 6; ++i) {
            size_t pos{};
            while ((pos = text.find(m_badWords[i], pos)) != std::string::npos) {
                text.replace(pos, m_badWords[i].size(), m_goodWords[i]);
                pos += m_goodWords[i].size();
                ++m_replaced[i];
            }
        }
    }

    void SpellChecker::showStatistics(std::ostream& out) const {
        out << "Spellchecker Statistics\n";
        for (size_t i = 0; i < 6; ++i) {
            out << std::setw(15) << std::left << m_badWords[i]
                << ": " << m_replaced[i] << " replacements\n";
        }
    }
}