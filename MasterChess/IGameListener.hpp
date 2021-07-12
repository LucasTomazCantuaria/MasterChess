#pragma once
#include <vector>

namespace MasterChess
{
    struct Game;
    struct IMovement;
    struct IGameListener
    {
        /// <summary>
        /// Evento executado quando o jogo estiver prestes a come�ar.
        /// </summary>
        /// <param name="game">Inst�ncia do jogo</param>
        virtual void OnGameStart(Game* game) {  }

        /// <summary>
        /// Evento executado quando algum movimento � realizado.
        /// </summary>
        /// <param name="movement"></param>
        virtual void OnMovementExecution(IMovement* movement) {  }
    };    

}
