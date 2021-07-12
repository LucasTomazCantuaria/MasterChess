#include "ChessBoard.hpp"

#include <algorithm>
#include <sstream>

#include <cassert>

namespace MasterChess
{
    static const Area ChessBoardArea = []
    {
        Area a;
        for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            a.AddPosition({ i, j });
        return a;
    }();

    vector<IPiece*> ChessBoard::Pieces() const
    {
        vector<IPiece*> pieces;
        for (auto p : board.v) if (p)
            pieces.push_back(p);
        return pieces;
    }

    const Area& ChessBoard::BoardArea() const
    {
        return ChessBoardArea;
    }

    IPiece* ChessBoard::At(const Vector2Int& position) const
    {
        assert(ChessBoardArea.IncludesPosition(position));
        return board[position];
    }

    Vector2Int ChessBoard::FindPieceLocation(IPiece* piece) const
    {
        assert(ContainsPiece(piece));
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) if (board[{ i, j }] == piece)
                return { i, j };
        return {};
    }

    void ChessBoard::AddPiece(IPiece* piece, const Vector2Int& position)
    {
        assert(!ContainsPiece(piece));
        assert(!board[position]);
        board[position] = piece;
        piece->OnAddToBoard(this);
    }

    bool ChessBoard::ContainsPiece(IPiece* piece) const
    {
        using namespace std;
        auto& v = board.v;
        auto it = find(begin(v), end(v), piece);
        return it != end(v);
    }

    void ChessBoard::RemovePiece(IPiece* piece)
    {
        assert(ContainsPiece(piece));
        board[FindPieceLocation(piece)] = nullptr;
        piece->OnRemoveFromBoard();
    }

    void ChessBoard::RepositionPiece(IPiece* piece, const Vector2Int& position)
    {
        assert(ContainsPiece(piece));
        assert(At(position) == nullptr);
        board[FindPieceLocation(piece)] = nullptr;
        board[position] = piece;
    }

}
