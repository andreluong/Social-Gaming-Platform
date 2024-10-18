#include "inputRequestQueue.h"

InputRequestQueue::InputRequestQueue()
    : queue() {}

std::vector<std::pair<User, HumanInputType>> InputRequestQueue::getQueue() {
    return queue;
};

void InputRequestQueue::addInputRequest(User user, HumanInputType inputType) {
    queue.push_back(std::make_pair(user, inputType));
} 

std::optional<std::vector<std::pair<User, HumanInputType>>::iterator>
InputRequestQueue::getRequestFromMessage(const networking::Message& message) {
    auto userOwnsMessage = [&](std::pair<User, HumanInputType> userInputType) -> bool {
        return userInputType.first.getConnection() == message.connection;
    };
    auto userIt = std::find_if(queue.begin(), queue.end(), userOwnsMessage);

    // Return request
    if (userIt != queue.end()) {
        return std::optional<std::vector<std::pair<User, HumanInputType>>::iterator>{userIt};
    }
    
    // Return null
    return std::nullopt;
}

void InputRequestQueue::removeRequest(std::vector<std::pair<User, HumanInputType>>::iterator it) {
    queue.erase(it);
}

bool InputRequestQueue::isEmpty() {
    return queue.empty();
}
