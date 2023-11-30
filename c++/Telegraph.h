//
// Created by Alice Sze on 11/14/23.
//

#ifndef LIBGDX_TELEGRAPH_H
#define LIBGDX_TELEGRAPH_H


#include "Telegram.h"
#include <functional>
#include <iostream>

class Telegraph {
public:
     void handleMessage(const std::shared_ptr<Telegram>& msg) const {
//         int* value = static_cast<int*>(msg.get()->extraInfo.get());
//
//         // Use value as needed
//         std::cout << "Value: " << *value << std::endl;
     };

    bool operator==(const Telegraph& other) const {
        return this == &other;
    }
};

// Hash function specialization for Telegraph
namespace std {
    template <>
    struct hash<Telegraph> {
        std::size_t operator()(const Telegraph& t) const {
            return std::hash<long>()((long) (&t));
        }
    };
}



#endif //LIBGDX_TELEGRAPH_H
