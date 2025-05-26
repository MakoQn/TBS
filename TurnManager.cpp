#include "TurnManager.h"
#include <iostream>
#include <algorithm>

TurnManager::TurnManager() : currentPlayer(PlayerId::PLAYER_ONE), turnNumber(0) {}

void TurnManager::startGame(PlayerId startingPlayer)
{
    currentPlayer = startingPlayer;
    turnNumber = 1;

    std::cout << "Игра началась. Ход " << turnNumber << ". Ход Игрока " << (int)currentPlayer << "." << std::endl;

    GameEvent event(EventType::TURN_CHANGED);

    event.playerId = currentPlayer;
    notify(event);
}

void TurnManager::nextTurn()
{
    if (currentPlayer == PlayerId::PLAYER_ONE)
    {
        currentPlayer = PlayerId::PLAYER_TWO;
    }
    else
    {
        currentPlayer = PlayerId::PLAYER_ONE;
        turnNumber++;
    }

    std::cout << "Ход " << turnNumber << ". Ход Игрока " << (int)currentPlayer << "." << std::endl;

    GameEvent event(EventType::TURN_CHANGED);

    event.playerId = currentPlayer;
    notify(event);
}

PlayerId TurnManager::getCurrentPlayer() const
{
    return currentPlayer;
}

int TurnManager::getCurrentTurnNumber() const
{
    return turnNumber;
}

void TurnManager::attach(IObserver* observer)
{
    observers.push_back(observer);
}

void TurnManager::detach(IObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void TurnManager::notify(const GameEvent& event)
{
    for (IObserver* obs : observers)
    {
        if (obs) obs->onNotify(event);
    }
}