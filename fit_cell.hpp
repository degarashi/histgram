#pragma once
#include "cell.hpp"

namespace dg {
	using SizeV = std::vector<lubee::SizeI>;
	class Fit_Cell {
		private:
			SizeV			_toPlace;
			CellBoard		_initial;
			double			_cfNet,
							_cfEmpty;

		public:
			Fit_Cell(const SizeV& toPlace, const CellBoard& initial,
					const double cfNet, const double cfEmpty):
				_toPlace(toPlace),
				_initial(initial),
				_cfNet(cfNet),
				_cfEmpty(cfEmpty)
			{}
			template <class Gene>
			double operator()(const Gene& g) const {
				const auto len = g.length();
				assert(len == _toPlace.size());
				auto cb = _initial;
				for(size_t i=0 ; i<len ; i++)
					cb.place(_toPlace[g[i]]);

				return cb.calcScore(_cfNet, _cfEmpty);
			}
	};
}
