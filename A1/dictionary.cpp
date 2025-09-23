/*
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
*/
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "dictionary.h"
#include "settings.h"
namespace seneca {
    Dictionary::Dictionary() = default;
    Dictionary::~Dictionary() {
        deallocate();
    }
    void Dictionary::deallocate() {
        delete[] m_words;
        m_words = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
    Dictionary::Dictionary(const Dictionary& other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_words = new Word[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            m_words[i] = other.m_words[i];
        }
    }
    Dictionary& Dictionary::operator=(const Dictionary& other) {
        if (this != &other) {
            deallocate();
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_words = new Word[m_capacity];
            for (size_t i = 0; i < m_size; ++i) {
                m_words[i] = other.m_words[i];
            }
        }
        return *this;
    }
    Dictionary::Dictionary(Dictionary&& other) {
        *this = std::move(other);
    }
    Dictionary& Dictionary::operator=(Dictionary&& other) {
        if (this != &other) {
            deallocate();
            m_words = other.m_words;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            other.m_words = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }


    PartOfSpeech Dictionary::parsePOS(const std::string& str) {
        if (str == "n." || str == "n. pl.") return PartOfSpeech::Noun;
        if (str == "pron.") return PartOfSpeech::Pronoun;
        if (str == "a.") return PartOfSpeech::Adjective;
        if (str == "adv.") return PartOfSpeech::Adverb;
        if (str == "v." || str == "v. i." || str == "v. t." || str == "v. t. & i.") return PartOfSpeech::Verb;
        if (str == "prep.") return PartOfSpeech::Preposition;
        if (str == "conj.") return PartOfSpeech::Conjunction;
        if (str == "interj.") return PartOfSpeech::Interjection;
        return PartOfSpeech::Unknown;
    }

    Dictionary::Dictionary(const char* filename) {
        std::ifstream file(filename);
        if (!file) {
            *this = Dictionary(); //empty 
            return;
        }
        std::string line;
        size_t count = 0;
        while (getline(file, line)) {
            ++count;
        }
        file.clear();
        file.seekg(0);
        m_capacity = count;
        m_size = 0;
        m_words = new Word[m_capacity];

        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string word, pos, def;

            getline(ss, word, ',');
            getline(ss, pos, ',');
            getline(ss, def);

            m_words[m_size].m_word = word;
            m_words[m_size].m_pos = parsePOS(pos);
            m_words[m_size].m_definition = def;
            ++m_size;
        }
    }

    void Dictionary::searchWord(const char* word) const {
        bool found = false;
        std::string target = word;

        for (size_t i = 0; i < m_size; ++i) {
            if (m_words[i].m_word == target) {
                if (!found) {
                    std::cout << m_words[i].m_word;
                    found = true;
                }
                else {
                    std::cout << std::setw(static_cast<int>(target.size())) << " ";
                }

                if (g_settings.m_verbose && m_words[i].m_pos != PartOfSpeech::Unknown) {
                    std::cout << " - (";
                    switch (m_words[i].m_pos) {
                    case PartOfSpeech::Noun: std::cout << "noun"; break;
                    case PartOfSpeech::Pronoun: std::cout << "pronoun"; break;
                    case PartOfSpeech::Adjective: std::cout << "adjective"; break;
                    case PartOfSpeech::Adverb: std::cout << "adverb"; break;
                    case PartOfSpeech::Verb: std::cout << "verb"; break;
                    case PartOfSpeech::Preposition: std::cout << "preposition"; break;
                    case PartOfSpeech::Conjunction: std::cout << "conjunction"; break;
                    case PartOfSpeech::Interjection: std::cout << "interjection"; break;
                    default: break;
                    }
                    std::cout << ") ";
                }
                else {
                    std::cout << " - ";
                }

                std::cout << m_words[i].m_definition << '\n';

                if (!g_settings.m_show_all)
                    break;
            }
        }

        if (!found) {
            std::cout << "Word '" << word << "' was not found in the dictionary.\n";
        }
    }

}