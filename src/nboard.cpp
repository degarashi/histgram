#include "nboard.hpp"
#include "lubee/compare.hpp"
#include <iostream>
#include <cassert>

namespace dg {
	// ------------ Cell ------------
	bool NBoard::Cell::operator == (const Cell& c) const noexcept {
		return score == c.score &&
				used == c.used;
	}
	std::ostream& NBoard::Cell::print(std::ostream& os) const {
		os << std::boolalpha;
		return os << "{" << int(score) << ", " << used << "}";
	}

	// ------------ NBoard ------------
	NBoard::NBoard(const lubee::SizeI s):
		_size(s.width+2, s.height+2),
		_cell(_size.area()),
		_nEmptyCell(s.area())
	{
		const auto w = _size.width,
					h = _size.height;
		for(int i=0 ; i<w ; i++) {
			_rawAt(i, 0).used = true;
			_rawAt(i, h-1).used = true;
		}
		for(int i=0 ; i<h ; i++) {
			_rawAt(0, i).used = true;
			_rawAt(w-1, i).used = true;
		}
		_calcScore({{1,1}, {1,h-2}});
		_calcScore({{w-2,1}, {1,h-2}});
		_calcScore({{1,1}, {w-2,1}});
		_calcScore({{1,h-2}, {w-2,1}});
	}
	const NBoard::Cell& NBoard::_rawAt(const int x, const int y) const noexcept {
		return const_cast<NBoard*>(this)->_rawAt(x,y);
	}
	NBoard::Cell& NBoard::_rawAt(const int x, const int y) noexcept {
		return _cell[y*_size.width + x];
	}
	void NBoard::_calcScore(const lubee::RectI& rect) {
		const auto bottom = rect.y1,
					right = rect.x1;
		assert(!(rect.x0 < 0 ||
				rect.y0 < 0 ||
				right > _size.width ||
				bottom > _size.height));

		for(int i=rect.y0 ; i<bottom ; i++) {
			for(int j=rect.x0 ; j<right ; j++) {
				int count = 0;
				if(!_rawAt(j, i).used) {
					for(int ly=-1 ; ly<=1 ; ly++) {
						for(int lx=-1 ; lx<=1 ; lx++) {
							count += static_cast<int>(_rawAt(j+lx,i+ly).used);
						}
					}
				}
				_rawAt(j,i).score = count;
			}
		}
	}

	NBoard::Cell& NBoard::cellAt(const int x, const int y) noexcept {
		return _cell[(y+1)*_size.width + x+1];
	}
	const NBoard::Cell& NBoard::cellAt(const int x, const int y) const noexcept {
		return const_cast<NBoard*>(this)->cellAt(x, y);
	}
	void NBoard::recalcScore() {
		_calcScore({{}, _size});
	}
	void NBoard::recountEmptyCells() noexcept {
		int nmp = 0;
		for(auto& c : _cell)
			nmp += static_cast<int>(!c.used);
		_nEmptyCell = nmp;
	}
	void NBoard::placeObstacle(lubee::RectI rect) {
		rect = rect.clip(lubee::RectI{{0,0}, {_size.width-2, _size.height-2}});
		if(rect.area() == 0)
			return;

		auto emp = _nEmptyCell;
		for(int oy=rect.y0 ; oy<rect.y1 ; oy++) {
			for(int ox=rect.x0 ; ox<rect.x1 ; ox++) {
				auto& c = cellAt(ox, oy);
				if(!c.used) {
					c.used = true;
					--emp;
				}
			}
		}
		_nEmptyCell = emp;

		auto tr = ToRaw(lubee::RectI{rect.x0, rect.x1, rect.y0, rect.y1});
		tr.expand(1);
		_calcScore(tr);
	}
	lubee::RectI NBoard::ToRaw(const lubee::RectI& rect) noexcept {
		return {{rect.x0+1, rect.y0+1}, rect.size()};
	}
	lubee::SizeI NBoard::getSize() const noexcept {
		auto ts = _size;
		ts.expand(-2);
		return ts;
	}
	size_t NBoard::getNEmptyCell() const noexcept {
		return _nEmptyCell;
	}
	bool NBoard::operator == (const NBoard& b) const noexcept {
		return _size == b._size &&
				_nEmptyCell == b._nEmptyCell &&
				_cell == b._cell;
	}
	std::ostream& NBoard::print(std::ostream& os) const {
		const auto size = _size;
		for(int i=0 ; i<size.height ; i++) {
			os << "{";
			for(int j=0 ; j<size.width ; j++) {
				_rawAt(j,i).print(os);
			}
			os << "}" << std::endl;
		}
		os << getNEmptyCell() << std::endl;
		os << getSize() << std::endl;
		return os;
	}
	void NBoard::refresh() {
		recalcScore();
		recountEmptyCells();
	}
}
