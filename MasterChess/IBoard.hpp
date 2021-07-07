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
        /// Ilumina uma certa área do tabuleiro.
        /// </summary>
        /// <param name="area">Area a ser pintada.</param>
        /// <param name="color">Cor desejada.</param>
        virtual void LightenArea(const Area& area, uint32_t color) = 0;

        /// <summary>
        /// Salva as cores do tabuleiro.
        /// </summary>
        virtual void PushColors() = 0;

        /// <summary>
        /// Restaura as cores do tabuleiro.
        /// </summary>
        virtual void PopColors() = 0;

        /// <summary>
        /// Retorna as cores atuais do tabuleiro.
        /// </summary>
        /// <returns>Um mapa que relaciona as diferentes cores com posições do tabuleiro.</returns>
        virtual std::unordered_map<uint32_t, Area> Colors() const = 0;

        /// <summary>
        /// Traça um raio que colide com a primeira peça que esteja no caminho
        /// </summary>
        /// <param name="ray"></param>
        /// <param name="hitInfo"></param>
        /// <returns></returns>
        bool CastRay(const Ray& ray, HitInfo* hitInfo) const;

        /// <summary>
        /// Começa uma atualização do tabuleiro.
        /// Deve ser usada quando se tiver que fazer multiplas alterações no tabuleiro para que somente no final sejam aplicadas essas mudanças de uma só vez.
        /// </summary>
        /// <remarks>Deve ser usada em conjunto com a função "EndUpdate"</remarks>
        /// <example>
        ///     board->BeginUpdate();
        ///     board->AddPiece(p1, { 0, 0 });
        ///     board->AddPiece(p2, { 1, 0 });
        ///     board->AddPiece(p3, { 2, 0 });
        ///     board->LightenArea(area, color);
        ///     board->EndUpdate(); //Somente aqui as mudanças são aplicadas no visual do tabuleiro.
        /// </example>
        virtual void BeginUpdate() = 0;

        /// <summary>
        /// Termina a atualização do tabuleiro e aplica as mudanças feitas
        /// </summary>
        /// <remarks>Deve ser usada em conjunto com a função "BeginUpdate"</remarks>
        virtual void EndUpdate() = 0;        

        void OnGameStart(Game* game) override
        {
            for (auto ptr : Pieces())
                ptr->OnGameStart(game);
        }

    };

}
