#pragma once
#include "nboard.hpp"
#include "vnet.hpp"

namespace dg {
	class CellBoard {
		public:
			// 矩形が置ける場所の検索
			using PointV = std::vector<lubee::PointI>;
			static PointV FindPlace(const NBoard& board, const lubee::SizeI& rs);

		private:
			using Score = double;
			using RectV = std::vector<lubee::RectI>;

			NBoard		_board;
			VNet		_vnet;
			RectV		_placedRect;

		public:
			CellBoard(const CellBoard&) = default;
			// サイズと配置領域制限を渡す(size+周囲1マス分の配列を確保)
			CellBoard(const lubee::RectI* obs, size_t n, lubee::SizeI size);
			CellBoard(lubee::SizeI size);

			bool operator == (const CellBoard& cb) const noexcept;
			bool operator != (const CellBoard& cb) const noexcept;
			// Rectに対して最適な配置位置を計算、配置し
			// 隣接値, 空きセル数, 仮想ネット矩形を更新
			// 置けたらtrueを返す
			bool place(const lubee::SizeI& rs);
			// これまでに置いた矩形リスト
			const RectV& placedRect() const noexcept;
			// 仮想配線長と空きマス数に応じたスコア
			Score calcScore(double rNet, double rEmpty) const noexcept;

			const NBoard& nboard() const noexcept;
			// 空きマス数(from NBoard)
			size_t getNEmptyCell() const noexcept;
	};
}
