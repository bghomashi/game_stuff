#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <cstdint>
#include <string>

class Image {
public:
	int width, height, num_channels;
	std::uint8_t* data;
	
	bool Load(const std::string& filename, bool flip_y = false);
	void Free();
};



#endif