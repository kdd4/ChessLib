#include "ChessLib/Piece/Pieces/Pawn.hpp"

namespace Chess
{
    namespace Pieces
    {
        Pawn::Pawn(Position pos, PieceColor color, std::weak_ptr<Board>& board)
            :
            MovablePiece(pos, PieceType::Pawn, color, board)
        {}

        Pawn::Pawn(const Piece& piece, std::weak_ptr<Board> board)
            :
            MovablePiece(piece, board)
        {
            type = PieceType::Pawn;
        }

        Pawn::Pawn(const MovablePiece& right)
            :
            MovablePiece(right)
        {
            type = PieceType::Pawn;
        }

        std::shared_ptr<MovablePiece> Pawn::clone(std::weak_ptr<Board> board) const
        {
            std::shared_ptr<MovablePiece> copy(new Pawn(*this, board));
            return copy;
        }

        void Pawn::getMoves(std::vector<std::shared_ptr<Move>>& vec, bool onlyAttack) const
        {
            if (this->deleted) return;

            if (!onlyAttack) {
                this->forwardMove(vec);
            }

            this->rightMove(vec);
            this->leftMove(vec);

            this->captureOnMoveLeft(vec);
            this->captureOnMoveRight(vec);
        }

        void Pawn::forwardMove(std::vector<std::shared_ptr<Move>>& vec) const
        {
            Position move_step = Position(pos.x, pos.y + getDirection());

            if (!move_step.check()) return;
            
            std::shared_ptr<Piece> move_place = board.lock()->getPiece(move_step);

            if (move_place != nullptr) return;
            
            std::shared_ptr<ImplMove> move = std::shared_ptr<ImplMove>(new ImplMove(board));
            move->appendStep(this->pos, move_step);

            if (move_step.y == 7 || move_step.y == 0)
            {
                move->changeType(pos, type, PieceType::Queen); // CHANGE IT
            }

            vec.push_back(move);

            Position move_step_far = Position(pos.x, pos.y + getDirection() * 2);

            if (this->getLastMoveMoment() != -1 || !move_step_far.check()) return;
            
            std::shared_ptr<Piece> attacked_place = board.lock()->getPiece(move_step_far);

            if (attacked_place != nullptr) return;
            
            std::shared_ptr<ImplMove> move_far = std::shared_ptr<ImplMove>(new ImplMove(board));

            move_far->appendStep(this->pos, move_step_far);

            vec.push_back(move_far);
        }

        void Pawn::leftMove(std::vector<std::shared_ptr<Move>>& vec) const
        {
            Position move_step = Position(pos.x - 1, pos.y + getDirection());

            if (!move_step.check()) return;
            
            std::shared_ptr<Piece> attacked_place = board.lock()->getPiece(move_step);

            if (attacked_place == nullptr) return;
            
            if (attacked_place->color == color) return;
                
            std::shared_ptr<ImplMove> move = std::shared_ptr<ImplMove>(new ImplMove(board));

            move->appendStep(this->pos, move_step);
            move->appendAttack(move_step);

            if (move_step.y == 7 || move_step.y == 0)
            {
                move->changeType(pos, type, PieceType::Queen); // CHANGE IT
            }

            vec.push_back(move);
        }

        void Pawn::rightMove(std::vector<std::shared_ptr<Move>>& vec) const
        {
            Position move_step = Position(pos.x + 1, pos.y + getDirection());

            if (!move_step.check()) return;
            
            std::shared_ptr<Piece> attacked_place = board.lock()->getPiece(move_step);

            if (attacked_place == nullptr) return;
            
            if (attacked_place->color == color) return;
                
            std::shared_ptr<ImplMove> move = std::shared_ptr<ImplMove>(new ImplMove(board));

            move->appendStep(this->pos, move_step);
            move->appendAttack(move_step);

            if (move_step.y == 7 || move_step.y == 0)
            {
                move->changeType(pos, type, PieceType::Queen); // CHANGE IT
            }

            vec.push_back(move);
        }

        void Pawn::captureOnMoveLeft(std::vector<std::shared_ptr<Move>>& vec) const
        {
            Position move_step = Position(pos.x - 1, pos.y + getDirection());

            if (!move_step.check()) return;

            std::shared_ptr<Piece> move_place = board.lock()->getPiece(move_step);

            if (move_place != nullptr) return;

            Position attacked_position = Position(pos.x - 1, pos.y);

            if (!attacked_position.check()) return;
            
            std::shared_ptr<Piece> attacked_place = board.lock()->getPiece(attacked_position);

            if (attacked_place == nullptr) return;

            if (attacked_place->color == color || attacked_place->type != PieceType::Pawn) return;

            if (attacked_place->getLastMoveMoment() != board.lock()->getMoveCount() - 1 \
                || attacked_place->getMoveCount() != 1) return;

            std::shared_ptr<ImplMove> move = std::shared_ptr<ImplMove>(new ImplMove(board));

            move->appendStep(this->pos, move_step);
            move->appendAttack(Position(pos.x - 1, pos.y));

            vec.push_back(move);
        }

        void Pawn::captureOnMoveRight(std::vector<std::shared_ptr<Move>>& vec) const
        {
            Position move_step = Position(pos.x + 1, pos.y + getDirection());

            if (!move_step.check()) return;

            std::shared_ptr<Piece> move_place = board.lock()->getPiece(move_step);

            if (move_place != nullptr) return;

            Position attacked_position = Position(pos.x + 1, pos.y);

            if (!attacked_position.check()) return;

            std::shared_ptr<Piece> attacked_place = board.lock()->getPiece(attacked_position);

            if (attacked_place == nullptr) return;

            if (attacked_place->color == color || attacked_place->type != PieceType::Pawn) return;

            if (attacked_place->getLastMoveMoment() != board.lock()->getMoveCount() - 1 \
                || attacked_place->getMoveCount() != 1) return;

            std::shared_ptr<ImplMove> move = std::shared_ptr<ImplMove>(new ImplMove(board));

            move->appendStep(this->pos, move_step);
            move->appendAttack(Position(pos.x + 1, pos.y));

            vec.push_back(move);
        }

    }
}
