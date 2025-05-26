#pragma once

#include "Defines.h"
#include "Observer.h"
#include <vector>

class TurnManager : public ISubject
{
private:
    PlayerId currentPlayer;
    int turnNumber;
    std::vector<IObserver*> observers;
public:
    TurnManager();

    void startGame(PlayerId startingPlayer);
    void nextTurn();
    PlayerId getCurrentPlayer() const;
    int getCurrentTurnNumber() const;
    void attach(IObserver* observer) override;
    void detach(IObserver* observer) override;
    void notify(const GameEvent& event) override;
};