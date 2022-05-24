#include "engine/utility/UUID.h"

namespace utl {

	const UUID UUID::INVALID = { 0 };
	static unsigned s_currentID = 1;

	unsigned UUID::get() {
		return s_currentID++;
	}
	void UUID::seed(unsigned seed) {
		s_currentID = seed;
	}

	UUID::UUID() : _id(UUID::INVALID) {}
	UUID::UUID(unsigned id) : _id(id) {}
	bool UUID::IsValid() const {
		return _id != UUID::INVALID;
	}
	bool UUID::IsInvalid() const {
		return _id == UUID::INVALID;
	}
	UUID& UUID::operator=(const UUID& o) {
		_id = o._id;
		return *this;
	}
	bool UUID::operator==(const UUID& o) const {
		return _id == o._id;
	}
	bool UUID::operator!=(const UUID& o) const {
		return _id != o._id;
	}
	bool UUID::operator< (const UUID& o) const {
		return _id < o._id;
	}
	bool UUID::operator<= (const UUID& o) const {
		return _id <= o._id;
	}
	UUID::operator unsigned() const {
		return _id;
	}
}