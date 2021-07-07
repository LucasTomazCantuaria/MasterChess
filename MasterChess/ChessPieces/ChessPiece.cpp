#include "ChessPiece.hpp"
#include "MasterChess/ChessPlayer.hpp"

#include <cassert>

namespace MasterChess
{

    ChessPiece::Movement::Movement(ChessPiece* piece, const Vector2Int& destination) : piece(piece), origin(piece->Position()), destination(destination)
    {

    }

    ChessPiece* ChessPiece::Movement::Piece() const
    {
        return piece;
    }

    void ChessPiece::Movement::Execute()
    {
        assert(piece->Position() == origin);
        auto board = piece->Board();
        assert(board);
        board->RepositionPiece(piece, destination);
        ++piece->movementCount;
    }

    void ChessPiece::Movement::Undo()
    {
        assert(piece->Position() == destination);
        auto board = piece->Board();
        assert(board);
        board->RepositionPiece(piece, origin);
        --piece->movementCount;
    }

    Vector2Int ChessPiece::Movement::Origin() const
    {
        return origin;
    }

    Vector2Int ChessPiece::Movement::Destination() const
    {
        return destination;
    }

    ChessPiece::CaptureMovement::CaptureMovement(ChessPiece* piece, const Vector2Int& destination, IPiece* captured) : Movement(piece, destination), captured(captured), capturedDestination(captured->Position())
    {
        assert(piece->Board() == captured->Board());
    }

    void ChessPiece::CaptureMovement::Execute()
    {
        auto board = captured->Board();
        board->BeginUpdate();
        board->RemovePiece(captured);
        Movement::Execute();
        board->EndUpdate();
    }

    void ChessPiece::CaptureMovement::Undo()
    {
        auto board = captured->Board();
        board->BeginUpdate();
        Movement::Undo();
        captured->Board()->AddPiece(captured, capturedDestination);
        board->EndUpdate();
    }

    IPiece* ChessPiece::CaptureMovement::Captured() const
    {
        return captured;
    }

    ChessPiece::ChessPiece(IPlayer* player) : movementCount(0), player(player), board(nullptr), game(nullptr)
    {

    }

    IPlayer* ChessPiece::Player() const
    {
        return player;
    }

    IBoard* ChessPiece::Board() const
    {
        return board;
    }

    Vector2Int ChessPiece::Position() const
    {
        return Board()->FindPieceLocation((IPiece*)this);
    }

    int ChessPiece::MovementCount() const
    {
        return movementCount;
    }

    void ChessPiece::OnAddToBoard(IBoard* board)
    {
        this->board = board;
    }

    void ChessPiece::OnRemoveFromBoard()
    {
        board = nullptr;
    }

    void ChessPiece::OnGameStart(Game* game)
    {
        this->game = game;
    }

    unique_ptr<IMovement> ChessPiece::CreateMovement(const Vector2Int& position)
    {
        assert(IsEmpty(position) || IsCapturable(position));
        if (IsEmpty(position))
            return std::make_unique<Movement>(this, position);
        return std::make_unique<CaptureMovement>(this, position, board->At(position));
    }

    bool ChessPiece::IsEmpty(const Vector2Int& position) const
    {
        assert(board);
        auto& area = board->BoardArea();
        assert(area.IncludesPosition(position));
        return !board->At(position);
    }

    bool ChessPiece::IsCapturable(const Vector2Int& position) const
    {
        assert(board);
        auto& area = board->BoardArea();
        assert(area.IncludesPosition(position));
        return IsCapturable(board->At(position));
    }

    bool ChessPiece::IsCapturable(IPiece* piece) const
    {
        return piece && piece->Player()->Team() != Player()->Team();
    }
}
