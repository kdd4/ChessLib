#pragma once

#include "ChessLib/Piece/MovablePiece.hpp"
#include "ChessLib/Board.hpp"

#include "ChessLib/Piece/Enums.hpp"
#include "ChessLib/Position.hpp"
#include "ChessLib/ImplMove.hpp"

#include <vector>
#include <memory>

namespace Chess
{
    namespace Pieces
    {
        class Pawn : public IMovablePiece
        {
        public:
            Pawn(Position pos, PieceColor color, std::weak_ptr<Board>& board);
            Pawn(const Piece& piece, std::weak_ptr<Board> board);
            Pawn(const MovablePiece& right);

            std::shared_ptr<MovablePiece> clone(std::weak_ptr<Board> board) const override final;
            void getMoves(std::vector<std::shared_ptr<Move>>& vec, bool onlyAttack = false) const override final;

        private: 
            void forwardMove(std::vector<std::shared_ptr<Move>>& vec) const;
            void leftMove(std::vector<std::shared_ptr<Move>>& vec) const;
            void rightMove(std::vector<std::shared_ptr<Move>>& vec) const;

            void captureOnMoveLeft(std::vector<std::shared_ptr<Move>>& vec) const;
            void captureOnMoveRight(std::vector<std::shared_ptr<Move>>& vec) const;
        };
    }
}
