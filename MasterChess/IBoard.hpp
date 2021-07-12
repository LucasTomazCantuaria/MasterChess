#pragma once
#include "IGameListener.hpp"
#include "IPiece.hpp"
#include "Math/Area.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

namespace MasterChess
{
    struct Game;
    struct HitInfo;
    struct IPiece;
    struct IPlayer;
    struct IMovement;

    using std::vector;
    using std::unique_ptr;
    using Math::Vector2Int;
    using Math::Ray;
    using Math::Area;

    /// <summary>
    /// Interface de um tabuleiro de dimensões não específicadas.
    /// </summary>
    struct IBoard : IGameListener
    {
        virtual ~IBoard() = default;

        /// <summary>
        /// Obtém as peças atuais do tabuleiro.
        /// </summary>
        /// <returns>Um vetor com todas as peças do tabuleiro.</returns>
        virtual vector<IPiece*> Pieces() const = 0;

        /// <summary>
        /// Obtém a area válida do tabuleiro.
        /// </summary>
        /// <returns></returns>
        virtual const Area& BoardArea() const = 0;

        /// <summary>
        /// Retorna a peça que está no posição solicitada.
        /// </summary>
        /// <param name="position">Posição que deve ser checada.</param>
        /// <returns>Um pointer para a peça que está na posição checada ou nullptr se a casa estiver vazia.</returns>
        /// <remarks>A posição deve ser uma posição válida!</remarks>
        virtual IPiece* At(const Vector2Int& position) const = 0;

        /// <summary>
        /// Retorna a posição de uma peça que está no tabuleiro.
        /// </summary>
        /// <param name="piece">Peça a ser localizada.</param>
        /// <returns>Um vetor com a posição da peça.</returns>
        /// <remarks>A peça deve estar no tabuleiro na hora da checagem!</remarks>
        virtual Vector2Int FindPieceLocation(IPiece* piece) const = 0;

        /// <summary>
        ///	Adiciona uma peça no tabuleiro no local solicitado.
        /// </summary>
        /// <param name="piece">Peça a ser adicionada.</param>
        /// <param name="position">Posição a ser ocupada.</param>
        /// <remarks>A casa da posição solicitada deve estar vazia!</remarks>
        virtual void AddPiece(IPiece* piece, const Vector2Int& position) = 0;

        /// <summary>
        /// Checa se uma peça está incluida no tabuleiro.
        /// </summary>
        /// <param name="piece">Peça a ser testada.</param>
        /// <returns>Verdadeira caso esteja e falso caso contrário.</returns>
        virtual bool ContainsPiece(IPiece* piece) const = 0;

        /// <summary>
        /// Remove uma peça do tabuleiro.
        /// </summary>
        /// <param name="piece">Peça a ser removida.</param>
        /// <remarks>A peça deve estar no tabuleiro!</remarks>
        virtual void RemovePiece(IPiece* piece) = 0;

        /// <summary>
        /// Reposiciona uma peça no tabuleiro.
        /// </summary>
        /// <param name="piece">Peça a ser reposicionada.</param>
        /// <param name="position">Posição destinada a peça</param>
        /// <remarks>A peça deve estar no tabuleiro atual e a posição solicitada deve ser válida!</remarks>
        virtual void RepositionPiece(IPiece* piece, const Vector2Int& position) = 0;

        /// <summary>
        /// Traça um raio que colide com a primeira peça que esteja no caminho
        /// </summary>
        /// <param name="ray"></param>
        /// <param name="hitInfo"></param>
        /// <returns></returns>
        bool CastRay(const Ray& ray, HitInfo* hitInfo) const;

        void OnGameStart(Game* game) override
        {
            for (auto ptr : Pieces())
                ptr->OnGameStart(game);
        }

    };

}
