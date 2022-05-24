#ifndef __UUID_H__
#define __UUID_H__


#include <unordered_map>

class UUID {
	unsigned _id;
public:
	const static UUID INVALID;
	static unsigned get();
	static void seed(unsigned seed);


		
	UUID();
	UUID(unsigned id);
	bool IsValid() const;
	bool IsInvalid() const;
	UUID& operator=(const UUID& o);
	bool operator==(const UUID& o) const;
	bool operator!=(const UUID& o) const;
	bool operator<(const UUID& o) const;
	bool operator<=(const UUID& o) const;
	operator unsigned() const;
	struct hash_fn
	{
		unsigned operator() (const UUID &node) const {
			return std::hash<unsigned>()(node._id);
		}
	};
};

#endif