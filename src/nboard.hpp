#pragma once
#include "lubee/rect.hpp"
#include <vector>

namespace dg {
	// 隣接値つきセル配列
	class NBoard {
		private:
			struct Cell {
				uint8_t	score;
				bool	used;

				bool operator == (const Cell& c) const noexcept;
				std::ostream& print(std::ostream& os) const;
			};
			using CellV = std::vector<Cell>;

			lubee::SizeI	_size;
			CellV			_cell;
			size_t			_nEmptyCell;

			Cell& _rawAt(int x, int y) noexcept;
			const Cell& _rawAt(int x, int y) const noexcept;
			void _calcScore(const lubee::RectI& rect);
			static lubee::RectI ToRaw(const lubee::RectI& rect) noexcept;

		public:
			NBoard(lubee::SizeI size);

			Cell& cellAt(int x, int y) noexcept;
			const Cell& cellAt(int x, int y) const noexcept;
			// 隣接値の計算
			void recalcScore();
			void recountEmptyCells() noexcept;
			// recalcScore() + recountEmptyCells()
			void refresh();
			// 矩形が占める領域を塗り潰し、残りセル数から引く(隣接値の差分更新)
			void placeObstacle(lubee::RectI rect);

			lubee::SizeI getSize() const noexcept;
			size_t getNEmptyCell() const noexcept;
			bool operator == (const NBoard& b) const noexcept;
			std::ostream& print(std::ostream& os) const;
	};
}
