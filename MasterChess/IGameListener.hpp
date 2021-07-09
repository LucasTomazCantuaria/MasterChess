#pragma once
#include <vector>

namespace MasterChess
{
    struct Game;
    struct IMovement;
    struct IGameListener
    {
        /// <summary>
        /// Evento executado quando o jogo estiver prestes a começar.
        /// </summary>
        /// <param name="game">Instância do jogo</param>
        virtual void OnGameStart(Game* game) {  }

        /// <summary>
        /// Evento executado quando algum movimento é realizado.
        /// </summary>
        /// <param name="movement"></param>
        virtual void OnMovementExecution(IMovement* movement) {  }
    };

    struct GameBroadcastListener : IGameListener
    {
        void OnGameStart(Game* game) override { for (auto listener : listeners) listener->OnGameStart(game); }
        void OnMovementExecution(IMovement* movement) override { for (auto listener : listeners) listener->OnMovementExecution(movement); }
        void AddListener(IGameListener* listener) { listeners.emplace_back(listener); }
    private:
        std::vector<IGameListener*> listeners;
    };

}
