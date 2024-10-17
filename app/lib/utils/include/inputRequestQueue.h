#ifndef INPUT_REQUEST_QUEUE_H
#define INPUT_REQUEST_QUEUE_H

#include <vector>
#include "User.h"
#include "humanInput.h"
#include <optional>
#include "Server.h"

class InputRequestQueue {
public:
    InputRequestQueue();

    std::vector<std::pair<User, HumanInputType>> getQueue();
    void addInputRequest(User user, HumanInputType inputType);
    std::optional<std::vector<std::pair<User, HumanInputType>>::iterator> getRequestFromMessage(const networking::Message& message);
    void removeRequest(std::vector<std::pair<User, HumanInputType>>::iterator it);
    bool isEmpty();
private:
    std::vector<std::pair<User, HumanInputType>> queue;
};

#endif