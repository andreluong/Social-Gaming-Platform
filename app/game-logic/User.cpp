#include "include/User.h"
#include <spdlog/spdlog.h>

#include <stdexcept>
#include <algorithm>

// Server should generate a unique id upon User construction
// "Main Lobby" is number 0
User::User(unsigned long int id, const std::string& name, networking::Connection connection) : 
    id(id), 
    name(name),
    lobbyNum(0),
    connection(connection),
    responses() 
    {
        spdlog::debug("User created using first constructor");
    }

User::User(unsigned long int id, networking::Connection connection) : 
    id(id), 
    name(std::to_string(id)), // only 1 word names allowed
    lobbyNum(0),
    connection(connection),
    responses() 
    {
        spdlog::debug("User created using second constructor");
    }

User::~User(){
    spdlog::debug("User #{} is deleted", id);
}

unsigned long int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

// Lobby* User::getLobby() const{
//     return lobby;
// }

// void User::setLobby(Lobby* newLobby) {
//     lobby = newLobby;
// }

void User::setName(const std::string& name) {
    this->name = name;
}

unsigned int User::getLobby() const {
    return this->lobbyNum;
}

void User::setLobby(unsigned int lobbyNum){
    this->lobbyNum = lobbyNum;
}

networking::Connection User::getConnection() {
    return connection;
}

void User::addResponse(networking::Message msg, HumanInputType inputType) {
    responses.push_back(std::make_pair(msg, inputType)); 
};

void User::addRole(std::shared_ptr<Role> role) {
    RoleType newRoleType = role->getType();

    for (const auto& existingRole : roles) {
        if (existingRole->getType() == RoleType::Audience && newRoleType == RoleType::Player 
            || existingRole->getType() == RoleType::Player && newRoleType == RoleType::Audience) {
            throw std::runtime_error("User cannot have both Player and Audience roles.");
        }
    }
    
    roles.push_back(role);
}

void User::removeRole(RoleType roleType) {
    roles.erase(std::remove_if(roles.begin(), roles.end(), [roleType](const std::shared_ptr<Role>& role) { return role->getType() == roleType; }),
                roles.end());
}

bool User::hasRole(RoleType roleType) const {
    for (const auto& role : roles) {
        if (role->getType() == roleType) {
            return true;
        }
    }
    return false;
}

void User::swapPlayerAudienceRoles() {
    bool hasPlayer = hasRole(RoleType::Player);
    bool hasAudience = hasRole(RoleType::Audience);

    if (hasPlayer && !hasAudience) {
        removeRole(RoleType::Player);
        addRole(std::make_shared<Audience>());
    } else if (!hasPlayer && hasAudience) {
        removeRole(RoleType::Audience);
        addRole(std::make_shared<Player>());
    }
}

// Player role methods
RoleType Player::getType() const {
    return RoleType::Player;
}

void Player::play() {
    // TODO
}

// Audience role methods
RoleType Audience::getType() const {
    return RoleType::Audience;
}

void Audience::observe() {
    // TODO
}

// Host role methods
RoleType Host::getType() const {
    return RoleType::Host;
}

void Host::manageGame() {
    // TODO
}
