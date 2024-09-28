#include "User.h"

#include <stdexcept>
#include <algorithm>

// Server should generate a unique id upon User construction
User::User(unsigned long int id, const std::string& name) : id(id), name(name) {}
User::User(unsigned long int id) : id(id), name("Unnamed " + std::to_string(id)) {}

unsigned long int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

void User::setName(const std::string& name) {
    this->name = name;
}

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
