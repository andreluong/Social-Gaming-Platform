#ifndef USER_H
#define USER_H

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "Server.h"
#include "humanInput.h"

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
    User(unsigned long int id, const std::string& name);
    User(unsigned long int id);
    unsigned long int getId() const;
    std::string getName() const;
    void setName(const std::string& name);
    void addRole(std::shared_ptr<Role> role);
    void removeRole(RoleType roleType);
    bool hasRole(RoleType roleType) const;
    void swapPlayerAudienceRoles();

    networking::Connection getConnection() { return connection; };
    void addResponse(networking::Message msg, HumanInputType inputType) { responses.insert({msg, inputType}); };
    
private:
    unsigned long int id;
    std::string name;
    std::vector<std::shared_ptr<Role>> roles;

    networking::Connection connection;
    std::unordered_map<networking::Message, HumanInputType> responses;
};

#endif
