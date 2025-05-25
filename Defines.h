#pragma once

#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;
const float CELL_SIZE = 64.0f;
const float WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE;
const float WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE + 100.0f;

enum class PlayerId
{
    PLAYER_ONE,
    PLAYER_TWO,
    NEUTRAL
};

enum class UnitType
{
    WARRIOR,
    ARCHER,
    TANK
};

enum class GameStateId
{
    NONE,
    MAIN_MENU,
    GAMEPLAY,
    GAME_OVER
};

enum class AiStateId
{
    IDLE,
    MOVING_TO_TARGET,
    ATTACKING
};

enum class AiStrategyType
{
    AGGRESSIVE,
    DEFENSIVE
};

enum class GameplayMode
{
    PVP,
    PVE
};

struct PathNode
{
    sf::Vector2i position;
    int gCost;
    int hCost;
    PathNode* parent;

    PathNode(sf::Vector2i pos, PathNode* p = nullptr) : position(pos), gCost(0), hCost(0), parent(p) {}

    int getFCost() const
    {
        return gCost + hCost;
    }

    bool operator==(const PathNode& other) const
    {
        return position.x == other.position.x && position.y == other.position.y;
    }
};

struct ComparePathNode
{
    bool operator()(const PathNode* a, const PathNode* b) const
    {
        return a->getFCost() > b->getFCost();
    }
};

enum class EventType
{
    UNIT_HEALTH_CHANGED,
    UNIT_DIED,
    UNIT_MOVED,
    UNIT_ACTION_TAKEN,
    TURN_CHANGED,
    GAME_OVER_EVENT
};

struct GameEvent
{
    EventType type;
    int unitId;
    PlayerId playerId;

    GameEvent(EventType t) : type(t), unitId(-1), playerId(PlayerId::NEUTRAL) {}
};

class Unit;
class Grid;
class GameState;
class AssetManager;
class InputHandler;
class TurnManager;
class PlayerController;
class AiController;
class Game;
class IObserver;
class ISubject;