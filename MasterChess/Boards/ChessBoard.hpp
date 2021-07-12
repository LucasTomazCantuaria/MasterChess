#pragma once
#include "MasterChess/IBoard.hpp"
#include "Math/Vector2Int.hpp"

#include <string>
#include <unordered_set>
#include <stack>

namespace MasterChess
{
    using std::string;
    using std::unordered_set;
    using Math::Vector2Int;

    /// <summary>
    /// Representa um tabuleiro tradicional de xadrez.
    /// </summary>
    struct ChessBoard : IBoard
    {
        vector<IPiece*> Pieces() const override;
        const Area& BoardArea() const override;
        IPiece* At(const Vector2Int& position) const override;
        Vector2Int FindPieceLocation(IPiece* piece) const override;
        void AddPiece(IPiece* piece, const Vector2Int& position) override;
        bool ContainsPiece(IPiece* piece) const override;
        void RemovePiece(IPiece* piece) override;
        void RepositionPiece(IPiece* piece, const Vector2Int& position) override;
    private:
        template<class T>
        struct _matrix8x8
        {
            union
            {
                T m[8][8];
                T v[8 * 8];
            };
            _matrix8x8() : m{  } {  }
            T& operator[](const Vector2Int& v) { return m[v.x][v.y]; }
            T  operator[](const Vector2Int& v) const { return m[v.x][v.y]; }
            T& operator[](int i) { return v[i]; }
            T  operator[](int i) const { return v[i]; }
        };
        _matrix8x8<IPiece*> board;
    };
    
}

