/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include "team.h"
namespace seneca {
    Team::Team() : m_members(nullptr), m_size(0), m_capacity(0) {}
    Team::Team(const char* name) : m_name(name), m_members(nullptr), m_size(0), m_capacity(0) {}
    Team::~Team() {
        deallocate();
    }
    void Team::deallocate() {
        for (size_t i = 0; i < m_size; ++i)
            delete m_members[i];
        delete[] m_members;
        m_members = nullptr;
        m_size = 0;
        m_capacity = 0;
    }
    Team::Team(const Team& other)
        : m_members(nullptr),
        m_size(0),
        m_capacity(0)
    {
        *this = other;
    }
    Team& Team::operator=(const Team& other) {
        if (this != &other) {
            deallocate();
            m_name = other.m_name;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_members = new Character * [m_capacity] {};
            for (size_t i = 0; i < m_size; ++i)
                m_members[i] = other.m_members[i]->clone();
        }
        return *this;
    }
    Team::Team(Team&& other) noexcept {
        *this = std::move(other);
    }
    Team& Team::operator=(Team&& other) noexcept {
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
    bool Team::nameExists(const std::string& name) const {
        for (size_t i = 0; i < m_size; ++i)
            if (m_members[i]->getName() == name)
                return true;
        return false;
    }
    void Team::resize(size_t newCap) {
        Character** temp = new Character * [newCap] {};
        for (size_t i = 0; i < m_size; ++i)
            temp[i] = m_members[i];
        delete[] m_members;
        m_members = temp;
        m_capacity = newCap;
    }
    void Team::addMember(const Character* c) {
        if (!c || nameExists(c->getName())) return;

        if (m_size == m_capacity)
            resize(m_capacity == 0 ? 2 : m_capacity * 2);

        m_members[m_size++] = c->clone();
    }
    void Team::removeMember(const std::string& name) {
        for (size_t i = 0; i < m_size; ++i) {
            if (m_members[i]->getName() == name) {
                delete m_members[i];
                for (size_t j = i + 1; j < m_size; ++j)
                    m_members[j - 1] = m_members[j];
                --m_size;
                return;
            }
        }
    }
    Character* Team::operator[](size_t idx) const {
        return (idx < m_size) ? m_members[idx] : nullptr;
    }
    void Team::showMembers() const {
        if (m_size == 0) {
            std::cout << "No team." << std::endl;
            return;
        }

        std::cout << "[Team] " << m_name << std::endl;
        for (size_t i = 0; i < m_size; ++i)
            std::cout << "    " << i + 1 << ": " << *m_members[i] << std::endl;
    }
}