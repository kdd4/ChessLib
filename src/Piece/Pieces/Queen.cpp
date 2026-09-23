#include "ChessLib/Piece/Pieces/Queen.hpp"

namespace Chess
{
    namespace Pieces
    {
        Queen::Queen(Position pos, PieceColor color, std::weak_ptr<Board>& board)
            :
            MovablePiece(pos, PieceType::Queen, color, board)
        {}

        Queen::Queen(const Piece& piece, std::weak_ptr<Board> board)
            :
            MovablePiece(piece, board)
        {
            type = PieceType::Queen;
        }

        Queen::Queen(const MovablePiece& right)
            :
            MovablePiece(right)
        {
            type = PieceType::Queen;
        }

        std::shared_ptr<MovablePiece> Queen::clone(std::weak_ptr<Board> board) const
        {
            std::shared_ptr<MovablePiece> copy(new Queen(*this, board));
            return copy;
        }

        void Queen::getMoves(std::vector<std::shared_ptr<Move>>& vec, bool onlyAttack) const
        {
            if (this->deleted) return;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    if (dx == 0 && dy == 0) continue;

                    moveDirection(dx, dy, vec);
                }
            }
        }

        bool Queen::move(const Position& pos, std::vector<std::shared_ptr<Move>>& vec) const
        {
            if (!pos.check()) return false;

            std::shared_ptr<Piece> attackedPiece = board.lock()->getPiece(pos);

            if (attackedPiece != nullptr)
            {
                if (attackedPiece->color == color)
                    return false;
            }

            std::shared_ptr<ImplMove> move(new ImplMove(board));

            move->appendAttack(pos);
            move->appendStep(this->pos, pos);

            vec.push_back(move);

            return attackedPiece == nullptr;
        }

        void Queen::moveDirection(int dx, int dy, std::vector<std::shared_ptr<Move>>& vec) const
        {
            for (int i = 1; i < 8; ++i)
            {
                Position movePos(pos.x + dx * i, pos.y + dy * i);

                if (!move(movePos, vec)) break;
            }
        }
    }
}