#include "cell.hpp"
#include <cassert>
#include <limits>

namespace dg {
	CellBoard::CellBoard(const lubee::SizeI size):
		_board(size),
		_vnet({{}, size})
	{}
	CellBoard::CellBoard(const lubee::RectI* obs, const size_t n, const lubee::SizeI size):
		CellBoard(size)
	{
		for(size_t i=0 ; i<n ; i++)
			_board.placeObstacle(obs[i]);
	}
	CellBoard::PointV CellBoard::FindPlace(const NBoard& board, const lubee::SizeI& rs) {
		const auto size = board.getSize();
		const auto w = rs.width;
		std::vector<int32_t>	histH(size.area());
		const auto histHAt = [&histH, w=size.width](const int x, const int y) -> decltype(auto) {
			return histH[y*w + x];
		};
		// 横ヒストグラム
		for(int i=0 ; i<size.height ; i++) {
			uint32_t score = 0;
			int lc = 0,
			   rc = 0;		// 次に調べる場所
			do {
				assert(lc <= rc);
				assert(rc-lc < w);

				auto& c = board.cellAt(rc++, i);
				if(!c.used) {
					score += c.score;
					if(rc - lc == w) {
						histHAt(lc, i) = score;
						score -= board.cellAt(lc++, i).score;
					}
				} else {
					score = 0;
					while(lc != rc)
						histHAt(lc++, i) = -1;
				}
			} while(rc < size.width);
			while(lc != rc)
				histHAt(lc++, i) = -1;
		}

		PointV ret;
		uint32_t maxScore = 0;
		const auto h = rs.height;
		for(int i=0 ; i<size.width ; i++) {
			uint32_t score = 0;
			int tc = 0,
			   bc = 0;
			do {
				const auto sc = histHAt(i, bc++);
				if(sc == -1) {
					score = 0;
					tc = bc;
				} else {
					score += sc;
					if(bc - tc > h)
						score -= histHAt(i, tc++);
					if(bc - tc == h) {
						if(maxScore <= score) {
							if(maxScore < score) {
								maxScore = score;
								ret.clear();
							}
							ret.emplace_back(lubee::PointI{i, tc});
						}
					}
				}
			} while(bc < size.height);
		}
		return ret;
	}
	bool CellBoard::place(const lubee::SizeI& rs) {
		assert(rs.width > 0 && rs.height > 0);
		// 隣接スコアが一番高かった場所を取得
		const auto cand = FindPlace(_board, rs);
		if(cand.empty())
			return false;
		const auto place = [this, rs](const lubee::PointI& p){
			const Rect rect{p, rs};
			_board.placeObstacle(rect);
			_placedRect.emplace_back(rect);
			_vnet.takeIn(rect);
		};
		if(cand.size() == 1) {
			place(cand[0]);
		} else {
			// 仮想配線長で優先度を付ける
			const auto nCand = cand.size();
			VNet::VLength minlen = std::numeric_limits<VNet::VLength>::max();
			size_t idx;
			for(size_t i=0 ; i<nCand ; i++) {
				auto vn = _vnet;
				vn.takeIn(lubee::RectI(cand[i], rs));
				const auto len = vn.getLength();
				assert(len > 0);
				if(minlen > len) {
					minlen = len;
					idx = i;
				}
			}
			auto& best = cand[idx];
			place(best);
		}
		return true;
	}
	const CellBoard::RectV& CellBoard::placedRect() const noexcept {
		return _placedRect;
	}
	CellBoard::Score CellBoard::calcScore(const double rNet, const double rEmpty) const noexcept {
		return 1.0 / (1 + (rNet*_vnet.getLength() / VNet::N_Corner) + rEmpty*_board.getNEmptyCell());
	}
	size_t CellBoard::getNEmptyCell() const noexcept {
		return _board.getNEmptyCell();
	}
	const NBoard& CellBoard::nboard() const noexcept {
		return _board;
	}
	bool CellBoard::operator == (const CellBoard& cb) const noexcept {
		return _board == cb._board &&
				_vnet == cb._vnet &&
				_placedRect == cb._placedRect;
	}
	bool CellBoard::operator != (const CellBoard& cb) const noexcept {
		return !(this->operator == (cb));
	}
}
