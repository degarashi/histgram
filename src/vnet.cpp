#include "vnet.hpp"

namespace dg {
	VNet::VNet(const lubee::RectI& r) {
		_rect[LT] = {
			{r.x0, r.y0},
			{}
		};
		_rect[RT] = {
			{r.x1, r.y0},
			{}
		};
		_rect[LB] = {
			{r.x0, r.y1},
			{}
		};
		_rect[RB] = {
			{r.x1, r.y1},
			{}
		};
	}
	void VNet::takeIn(const lubee::RectI& r) noexcept {
		_rect[LT].takeIn({r.x0, r.y0});
		_rect[RT].takeIn({r.x1, r.y0});
		_rect[LB].takeIn({r.x0, r.y1});
		_rect[RB].takeIn({r.x1, r.y1});
	}
	VNet::VLength VNet::getLength() const noexcept {
		VLength ret = 0;
		for(auto& r : _rect) {
			const auto s = r.size();
			ret += s.width + s.height;
		}
		return ret;
	}
	bool VNet::operator == (const VNet& v) const noexcept {
		for(int i=0 ; i<N_Corner ; i++)
			if(_rect[i] != v._rect[i])
				return false;
		return true;
	}
}
