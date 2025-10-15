/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#ifndef SENECA_GUILD_H
#define SENECA_GUILD_H
#include "character.h"
#include <string>
#include <iostream>
namespace seneca {
    class Guild {
        std::string m_name;
        Character** m_members;
        size_t m_size;
        size_t m_capacity;
        void deallocate();
        void resize(size_t newCap);
        bool contains(const std::string& name) const;
    public:
        Guild();
        Guild(const char* name);
        ~Guild();
        Guild(const Guild& other);
        Guild& operator=(const Guild& other);
        Guild(Guild&& other) noexcept;
        Guild& operator=(Guild&& other) noexcept;
        void addMember(Character* c);
        void removeMember(const std::string& name);
        Character* operator[](size_t idx) const;
        void showMembers() const;
    };
}
#endif