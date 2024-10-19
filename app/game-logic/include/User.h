#ifndef USER_H
#define USER_H

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "Lobby.h"
#include "Server.h"
#include "humanInputType.h"

class Lobby;

enum class RoleType {
    Player,
    Audience,
    Host
};

class Role {
public:
    virtual ~Role() = default;
    virtual RoleType getType() const = 0;
};

class Player : public Role {
public:
    RoleType getType() const override;
    void play();
};

class Audience : public Role {
public:
    RoleType getType() const override;
    void observe();
};

class Host : public Role {
public:
    RoleType getType() const override;
    void manageGame();
};

// Player and Audience roles cannot exist simultaneously
class User {
public:
    User(unsigned long int id, const std::string& name, networking::Connection connection);
    User(unsigned long int id, networking::Connection connection);

    unsigned long int getId() const;
    std::string getName() const;
    void setName(const std::string& name);
    void addRole(std::shared_ptr<Role> role);
    void removeRole(RoleType roleType);
    bool hasRole(RoleType roleType) const;
    void swapPlayerAudienceRoles();
    Lobby* getLobby() const;
    void setLobby(Lobby* newLobby);

    networking::Connection getConnection();
    void addResponse(networking::Message msg, HumanInputType inputType);
private:
    unsigned long int id;
    std::string name;
    std::vector<std::shared_ptr<Role>> roles;
    Lobby* lobby;

    networking::Connection connection;
    std::vector<std::pair<networking::Message, HumanInputType>> responses;
};

#endif
