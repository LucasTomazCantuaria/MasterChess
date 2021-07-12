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

}
