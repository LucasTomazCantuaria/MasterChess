#pragma once

namespace MasterChess
{
    struct Game;
    struct IGameListener
    {
        /// <summary>
        /// Função de evento executada pela classe "Game" quando o jogo estiver prestes a começar.
        /// </summary>
        /// <param name="game">Instância do jogo</param>
        virtual void OnGameStart(Game* game) = 0;
    };
}
