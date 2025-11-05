/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#ifndef SENECA_SPELLCHECKER_H
#define SENECA_SPELLCHECKER_H

#include <string>
#include <iostream>

namespace seneca {
    class SpellChecker {
        std::string m_badWords[6]{};
        std::string m_goodWords[6]{};
        size_t      m_replaced[6]{};  // How many replacements for each bad word

    public:
        SpellChecker(const char* filename);   // May throw const char*  "Bad file name!"
        void operator()(std::string& text);   // Replace + count
        void showStatistics(std::ostream& out) const;
    };
}

#endif