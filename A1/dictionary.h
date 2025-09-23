/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#ifndef SENECA_DICTIONARY_H
#define SENECA_DICTIONARY_H
#include <string>
namespace seneca {
    enum class PartOfSpeech { //profs gave this
        Unknown,
        Noun,
        Pronoun,
        Adjective,
        Adverb,
        Verb,
        Preposition,
        Conjunction,
        Interjection,
    };
    struct Word {
        std::string m_word{};
        std::string m_definition{};
        PartOfSpeech m_pos = PartOfSpeech::Unknown;
    };

    class Dictionary {
        Word* m_words{};
        size_t m_size{};
        size_t m_capacity{};
        void deallocate(); //free the memory
        PartOfSpeech parsePOS(const std::string&);
    public:
        Dictionary();
        Dictionary(const char* filename);
        Dictionary(const Dictionary& other);
        Dictionary& operator=(const Dictionary& other);
        virtual ~Dictionary();
        Dictionary(Dictionary&& other);
        Dictionary& operator=(Dictionary&& other);

        void searchWord(const char* word) const;
    };
}
#endif