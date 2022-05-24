#include "engine/utility/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "engine/utility/stb_image.h"

bool Image::Load(const std::string& filename, bool flip_y) {
	stbi_set_flip_vertically_on_load(flip_y);
	data = stbi_load(filename.c_str(), &width, &height, &num_channels, 0);

	return data != NULL;
}

void Image::Free() {
	free(data);
	data = NULL;
}

