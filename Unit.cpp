#include "Unit.h"
#include "Grid.h"
#include "AssetManager.h"
#include <iostream>
#include <algorithm>

Unit::Unit(UnitType type, PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition)
    : unitType(type),
    ownerId(owner),
    unitId(id),
    grid(gameGrid),
    health(100),
    maxHealth(100),
    attackPower(10),
    defense(5),
    movementRange(3),
    actionPoints(2),
    maxActionPoints(2),
    currentPosition(initialPosition),
    sprite(AssetManager::getDefaultTexture()),
    unitIsActive(true),
    unitIsAlive(true)
{
    loadSpriteForType();
    sprite.setPosition(sf::Vector2f(static_cast<float>(initialPosition.x) * CELL_SIZE, static_cast<float>(initialPosition.y) * CELL_SIZE));
    
    if (grid && grid->getCell(initialPosition))
    {
        grid->getCell(initialPosition)->setOccupyingUnit(this);
    }
}

Unit::~Unit()
{
    if (grid && grid->getCell(currentPosition) && grid->getCell(currentPosition)->getOccupyingUnit() == this)
    {
        grid->getCell(currentPosition)->setOccupyingUnit(nullptr);
    }
}

void Unit::loadSpriteForType()
{
    std::string textureName;

    switch (unitType)
    {
    case UnitType::WARRIOR:
        textureName = "assets/warrior.png";
        break;
    case UnitType::ARCHER:
        textureName = "assets/archer.png";
        break;
    case UnitType::TANK:
        textureName = "assets/tank.png";
        break;
    default:
        textureName = "";
        break;
    }

    if (!textureName.empty())
    {
        sprite.setTexture(AssetManager::getInstance().getTexture(textureName), true);
    }
}

int Unit::getHealth() const
{
    return health; 
}

int Unit::getMaxHealth() const 
{ 
    return maxHealth; 
}

int Unit::getAttackPower() const 
{ 
    return attackPower; 
}

int Unit::getDefense() const 
{ 
    return defense; 
}

int Unit::getMovementRange() const 
{ 
    return movementRange; 
}

int Unit::getActionPoints() const 
{ 
    return actionPoints; 
}

int Unit::getMaxActionPoints() const 
{ 
    return maxActionPoints; 
}

PlayerId Unit::getOwner() const 
{ 
    return ownerId;
}

UnitType Unit::getType() const 
{ 
    return unitType;
}

sf::Vector2i Unit::getPosition() const 
{ 
    return currentPosition;
}

int Unit::getId() const 
{ 
    return unitId;
}

bool Unit::isActive() const 
{ 
    return unitIsActive && unitIsAlive; 
}

bool Unit::isAlive() const 
{ 
    return unitIsAlive; 
}

sf::Sprite& Unit::getSprite() 
{ 
    return sprite; 
}

void Unit::takeDamage(int amount)
{
    if (!unitIsAlive)
    {
        return;
    }

    int damageTaken = amount - defense;

    if (damageTaken < 0)
    {
        damageTaken = 0;
    }

    health -= damageTaken;

    std::cout << "Юнит " << unitId << " получает " << damageTaken << " урона. ОЗ: " << health << "/" << maxHealth << std::endl;

    GameEvent healthEvent(EventType::UNIT_HEALTH_CHANGED);

    healthEvent.unitId = unitId;
    notify(healthEvent);

    if (health <= 0)
    {
        health = 0;
        unitIsAlive = false;
        unitIsActive = false;

        std::cout << "Юнит " << unitId << " погиб." << std::endl;

        if (grid && grid->getCell(currentPosition) && grid->getCell(currentPosition)->getOccupyingUnit() == this)
        {
            grid->getCell(currentPosition)->setOccupyingUnit(nullptr);
        }

        GameEvent deathEvent(EventType::UNIT_DIED);

        deathEvent.unitId = unitId;
        notify(deathEvent);
    }
}

bool Unit::attack(Unit& target)
{
    if (!isActive() || actionPoints <= 0)
    {
        return false;
    }

    std::cout << "Юнит " << unitId << " (" << (int)ownerId << ") атакует Юнита " << target.getId() << " (" << (int)target.getOwner() << ")" << std::endl;
    
    target.takeDamage(attackPower);
    spendActionPoints(1);

    GameEvent actionEvent(EventType::UNIT_ACTION_TAKEN);

    actionEvent.unitId = unitId;
    notify(actionEvent);
    
    return true;
}

bool Unit::moveTo(const sf::Vector2i& targetPosition)
{
    if (!isActive() || actionPoints <= 0)
    {
        return false;
    }

    if (!grid || !grid->isValidCoordinate(targetPosition))
    {
        return false;
    }

    Cell* targetCell = grid->getCell(targetPosition);

    if (!targetCell || !targetCell->isTraversable() || targetCell->getOccupyingUnit() != nullptr)
    {
        return false;
    }

    if (grid->getCell(currentPosition))
    {
        grid->getCell(currentPosition)->setOccupyingUnit(nullptr);
    }

    setPosition(targetPosition);

    if (grid->getCell(currentPosition))
    {
        grid->getCell(currentPosition)->setOccupyingUnit(this);
    }

    spendActionPoints(1);

    GameEvent moveEvent(EventType::UNIT_MOVED);

    moveEvent.unitId = unitId;
    notify(moveEvent);

    GameEvent actionEvent(EventType::UNIT_ACTION_TAKEN);

    actionEvent.unitId = unitId;
    notify(actionEvent);

    return true;
}

void Unit::setPosition(const sf::Vector2i& newPosition)
{
    currentPosition = newPosition;
    sprite.setPosition(sf::Vector2f(static_cast<float>(currentPosition.x) * CELL_SIZE, static_cast<float>(currentPosition.y) * CELL_SIZE));
}

void Unit::resetActionPoints()
{
    actionPoints = maxActionPoints;
    unitIsActive = true;
}

void Unit::spendActionPoints(int amount)
{
    actionPoints -= amount;

    if (actionPoints < 0)
    {
        actionPoints = 0;
    }
}

void Unit::setActive(bool active)
{
    unitIsActive = active;
}

void Unit::onTurnStart()
{
    if (unitIsAlive)
    {
        resetActionPoints();

        std::cout << "Юнит " << unitId << " начинает ход. ОД: " << actionPoints << std::endl;
    }
}

void Unit::onTurnEnd() {}

void Unit::update(sf::Time deltaTime) {}

void Unit::render(sf::RenderTarget& target)
{
    if (unitIsAlive)
    {
        target.draw(sprite);
    }
}

void Unit::attach(IObserver* observer)
{
    observers.push_back(observer);
}

void Unit::detach(IObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Unit::notify(const GameEvent& event)
{
    for (IObserver* obs : observers)
    {
        if (obs)
        {
            obs->onNotify(event);
        }
    }
}

Warrior::Warrior(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition) : Unit(UnitType::WARRIOR, owner, id, gameGrid, initialPosition)
{
    maxHealth = 120;
    health = maxHealth;
    attackPower = 25;
    defense = 10;
    movementRange = 3;
    maxActionPoints = 2;
    actionPoints = maxActionPoints;
    loadSpriteForType();
    sprite.setPosition(sf::Vector2f(static_cast<float>(initialPosition.x) * CELL_SIZE, static_cast<float>(initialPosition.y) * CELL_SIZE));
}

Archer::Archer(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition) : Unit(UnitType::ARCHER, owner, id, gameGrid, initialPosition)
{
    maxHealth = 80;
    health = maxHealth;
    attackPower = 20;
    defense = 5;
    movementRange = 4;
    maxActionPoints = 2;
    actionPoints = maxActionPoints;
    loadSpriteForType();
    sprite.setPosition(sf::Vector2f(static_cast<float>(initialPosition.x) * CELL_SIZE, static_cast<float>(initialPosition.y) * CELL_SIZE));
}

Tank::Tank(PlayerId owner, int id, Grid* gameGrid, sf::Vector2i initialPosition) : Unit(UnitType::TANK, owner, id, gameGrid, initialPosition)
{
    maxHealth = 180;
    health = maxHealth;
    attackPower = 15;
    defense = 15;
    movementRange = 2;
    maxActionPoints = 2;
    actionPoints = maxActionPoints;
    loadSpriteForType();
    sprite.setPosition(sf::Vector2f(static_cast<float>(initialPosition.x) * CELL_SIZE, static_cast<float>(initialPosition.y) * CELL_SIZE));
}