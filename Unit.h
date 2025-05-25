#pragma once

#include "Defines.h"
#include "Observer.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Time.hpp>
#include <string>
#include <vector>

class Grid;
class Ability;

class Unit : public ISubject
{
protected:
    UnitType unitType;
    PlayerId ownerId;
    int unitId;
    Grid* grid;

    int health;
    int maxHealth;
    int attackPower;
    int defense;
    int movementRange;
    int actionPoints;
    int maxActionPoints;

    sf::Vector2i currentPosition;
    sf::Sprite sprite;
    bool unitIsActive;
    bool unitIsAlive;

    std::vector<IObserver*> observers;

    void setPosition(const sf::Vector2i& newPosition);
    void loadSpriteForType();
public:
    Unit(UnitType type, PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition);
    virtual ~Unit();

    int getHealth() const;
    int getMaxHealth() const;
    int getAttackPower() const;
    int getDefense() const;
    int getMovementRange() const;
    int getActionPoints() const;
    int getMaxActionPoints() const;
    PlayerId getOwner() const;
    UnitType getType() const;
    sf::Vector2i getPosition() const;
    int getId() const;
    bool isActive() const;
    bool isAlive() const;

    sf::Sprite& getSprite();

    virtual void takeDamage(int amount);
    virtual bool attack(Unit& target);
    virtual bool moveTo(const sf::Vector2i& targetPosition);

    virtual void resetActionPoints();
    virtual void spendActionPoints(int amount);
    virtual void setActive(bool active);

    virtual void onTurnStart();
    virtual void onTurnEnd();

    virtual void update(sf::Time deltaTime);
    virtual void render(sf::RenderTarget& target);

    void attach(IObserver* observer) override;
    void detach(IObserver* observer) override;
    void notify(const GameEvent& event) override;
};

class Warrior : public Unit
{
public:
    Warrior(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition);
};

class Archer : public Unit
{
public:
    Archer(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition);
};

class Tank : public Unit
{
public:
    Tank(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition);
};