#pragma once

#include "ChessLib/Move.hpp"
#include "ChessLib/Board.hpp"
#include "ChessLib/Piece/Piece.hpp"

#include <vector>
#include <memory>

namespace Chess
{
    class IMovablePiece : public IPiece
    {
    public:
        IMovablePiece(Position pos, PieceType type, PieceColor color, std::weak_ptr<Board> board, std::vector<int> moveMoment = {}, bool deleted = false);
        IMovablePiece(const IPiece& piece, const std::weak_ptr<Board>& board);
        IMovablePiece(const std::shared_ptr<IMovablePiece>& right);

        virtual std::shared_ptr<IMovablePiece> clone(std::weak_ptr<Board> board) const = 0;
        virtual void getMoves(std::vector<std::shared_ptr<Move>>& vec, bool onlyAttack = false) const = 0;

    protected:
        std::weak_ptr<Board> board;
    };

    typedef IMovablePiece MovablePiece;
}
