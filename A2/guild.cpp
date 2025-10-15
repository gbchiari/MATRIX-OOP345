/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include "guild.h"
namespace seneca {
    Guild::Guild()
        : m_name(),
        m_members(nullptr),
        m_size(0),
        m_capacity(0)
    {
    }
    Guild::Guild(const char* name)
        : m_name(name),
        m_members(nullptr),
        m_size(0),
        m_capacity(0)
    {
    }
    Guild::~Guild() {
        deallocate();
    }
    void Guild::deallocate() {
        delete[] m_members;
        m_members = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
    Guild::Guild(const Guild& other)
        : Guild()
    {
        *this = other;
    }
    Guild& Guild::operator=(const Guild& other) {
        if (this != &other) {
            deallocate();
            m_name = other.m_name;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            if (m_capacity > 0) {
                m_members = new Character * [m_capacity] {};
                for (size_t i = 0; i < m_size; ++i) {
                    m_members[i] = other.m_members[i];
                }
            }
        }
        return *this;
    }
    Guild::Guild(Guild&& other) noexcept
        : m_name(std::move(other.m_name)),
        m_members(other.m_members),
        m_size(other.m_size),
        m_capacity(other.m_capacity)
    {
        other.m_members = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    Guild& Guild::operator=(Guild&& other) noexcept {
        if (this != &other) {
            deallocate();
            m_name = std::move(other.m_name);
            m_members = other.m_members;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_members = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }
    bool Guild::contains(const std::string& name) const {
        for (size_t i = 0; i < m_size; ++i) {
            if (m_members[i]->getName() == name)
                return true;
        }
        return false;
    }
    void Guild::resize(size_t newCap) {
        Character** temp = new Character * [newCap] {};
        for (size_t i = 0; i < m_size; ++i)
            temp[i] = m_members[i];
        delete[] m_members;
        m_members = temp;
        m_capacity = newCap;
    }
    void Guild::addMember(Character* c) {
        if (!c || contains(c->getName())) return;
        if (m_size == m_capacity)
            resize(m_capacity == 0 ? 2 : m_capacity * 2);
        m_members[m_size++] = c;
        c->setHealthMax(c->getHealthMax() + 300);
    }
    void Guild::removeMember(const std::string& name) {
        for (size_t i = 0; i < m_size; ++i) {
            if (m_members[i]->getName() == name) {
                m_members[i]->setHealthMax(m_members[i]->getHealthMax() - 300);
                // shift left
                for (size_t j = i + 1; j < m_size; ++j)
                    m_members[j - 1] = m_members[j];
                --m_size;
                return;
            }
        }
    }
    Character* Guild::operator[](size_t idx) const {
        return (idx < m_size) ? m_members[idx] : nullptr;
    }
    void Guild::showMembers() const {
        if (m_size == 0) {
            if (m_name.empty()) {
                std::cout << "No guild." << std::endl;
            }
            else {
                std::cout << "[Guild] " << m_name << std::endl;
            }
            return;
        }
        std::cout << "[Guild] " << m_name << std::endl;
        for (size_t i = 0; i < m_size; ++i)
            std::cout << "    " << i + 1 << ": " << *m_members[i] << std::endl;
    }
}