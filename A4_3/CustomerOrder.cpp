/*
-----------------------------------------------------------------------------------------------------------------------------------------------
I declare that this submission is the result of my own work and I only copied the code that my professor provided to complete my assignments.
This submitted piece of work has not been shared with any other student or 3rd party content provider.
-----------------------------------------------------------------------------------------------------------------------------------------------
*/
#include <iomanip>
#include "CustomerOrder.h"
#include "Station.h"
#include "Utilities.h"
#include <iostream>
#include <stdexcept>
#include <string>
namespace seneca {
    size_t CustomerOrder::m_widthField = 0;

    CustomerOrder::CustomerOrder(const std::string& str) {
        Utilities util;
        size_t pos = 0;
        bool more = true;

        m_name = util.extractToken(str, pos, more);
        m_product = util.extractToken(str, pos, more);

        size_t itemStartPos = pos;
        size_t tempPos = pos;
        size_t count = 0;

        while (more) {
            util.extractToken(str, tempPos, more);
            count++;
        }

        m_cntItem = count;

        m_lstItem = new Item*[m_cntItem];

        for (size_t i = 0; i < m_cntItem; ++i) {
            m_lstItem[i] = new Item(util.extractToken(str, itemStartPos, more));
        }

        if (m_widthField < util.getFieldWidth()) {
            m_widthField = util.getFieldWidth();
        }
    }

    CustomerOrder::CustomerOrder(const CustomerOrder&) {
        throw std::runtime_error("Copy constructor is not allowed for this class.");
    }

    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept {
        m_name = std::move(other.m_name);
        m_product = std::move(other.m_product);
        m_cntItem = other.m_cntItem;
        m_lstItem = other.m_lstItem;

        other.m_cntItem = 0;
        other.m_lstItem = nullptr;
    }

    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            if (m_lstItem) {
                for (size_t i = 0; i < m_cntItem; ++i) {
                    delete m_lstItem[i];
                }
                delete[] m_lstItem;
            }

            m_name = std::move(other.m_name);
            m_product = std::move(other.m_product);
            m_cntItem = other.m_cntItem;
            m_lstItem = other.m_lstItem;

            other.m_cntItem = 0;
            other.m_lstItem = nullptr;
        }
        return *this;
    }

    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; ++i) {
            delete m_lstItem[i];
        }
        delete[] m_lstItem;
    }

    bool CustomerOrder::isOrderFilled() const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (!m_lstItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    bool CustomerOrder::isItemFilled(const std::string &itemName) const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == itemName && !m_lstItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    void CustomerOrder::fillItem(Station &station, std::ostream &os) {

        for (size_t i = 0; i < m_cntItem; ++i) {
            if (m_lstItem[i]->m_itemName == station.getItemName()) {
                if (!m_lstItem[i]->m_isFilled) {
                    if (station.getQuantity() > 0) {
                        m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                        m_lstItem[i]->m_isFilled = true;

                        station.updateQuantity();

                        os << "    Filled " << m_name << ", " << m_product
                           << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;

                        break;

                    } else {
                        os << "    Unable to fill " << m_name << ", " << m_product
                           << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                    }
                }
            }
        }
    }

    void CustomerOrder::display(std::ostream &os) const {
        os << m_name << " - " << m_product << std::endl;

        for (size_t i = 0; i < m_cntItem; ++i) {
            os << "["
            << std::right << std::setw(6) << std::setfill('0') << m_lstItem[i]->m_serialNumber
            << "] "
            << std::left << std::setw(static_cast<int>(m_widthField)) << std::setfill(' ') << m_lstItem[i]->m_itemName
            << " - "
            << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED")
            << std::endl;
        }
    }



}
