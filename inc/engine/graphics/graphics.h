#ifndef __OGL_GRAPHICS_H_
#define __OGL_GRAPHICS_H_

#include "glw/window.h"
#include "glw/vertex_buffer.h"
#include "glw/shader.h"
#include "glw/matrix_stack.h"
#include "glw/draw_commands.h"
#include "glw/texture2d.h"
#include "glw/color.h"
#include "glw/device.h"
#include "glw/framebuffer.h"

namespace OGL {
	struct PixelType {
		enum {
			Unsigned_Byte,
			Float,
			Unsigned_Int,
			Unsigned_Int_24_8,

			NUM
		};
	};
	struct PixelFormat {
		enum {
			RGB,
			RGBA,

			Depth_Component,
			Stencil_Index,
			Depth24_Stencil8,

			NUM
		};
	};
	struct InternalFormat {
		enum {
			RGB,
			RGBA,

			Depth_Component,
			Stencil_Copmonent,
			Depth_Stencil,

			NUM
		};
	};
	struct Filter {
		enum {
			Nearest,    // Returns the pixel that is closest to the coordinates.
			Linear,     // Returns the weighted average of the 4 pixels surrounding the given coordinates.

			// mipmaped
			Nearest_Mipmap_Nearest,
			Nearest_Mipmap_Linear,
			Linear_Mipmap_Nearest,
			Linear_Mipmap_Linear,

			NUM
		};
	};
	struct Wrapping {
		enum {
			Repeat,
			Mirrored_Repeat,
			Clamp_To_Edge,
			Clamp_To_Border,

			NUM
		};
	};

	struct DepthTest {
		enum {
			Always, 	// The depth test always passes
			Never, 		// The depth test never passes
			Less,  		// Passes if the fragment's depth value is less than the stored depth value
			Equal,  	// Passes if the fragment's depth value is equal to the stored depth value
			LEqual, 	// Passes if the fragment's depth value is less than or equal to the stored depth value
			Greater,	// Passes if the fragment's depth value is greater than the stored depth value
			NotEqual,	// Passes if the fragment's depth value is not equal to the stored depth value
			GEqual,		// Passes if the fragment's depth value is greater than or equal to the stored depth value

			NUM
		};
	};
	struct ClearBit {
		enum {
			Color,
			Depth,
			Stencil,
			Accum,

			NUM
		};
	};

	struct BlendEquation {
		enum {
			Add,
			Subtract,
			ReverseSubtract,
			Min,
			Max,

			NUM,
		};
	};
	struct BlendFunc {
		enum {
			Zero,					// Factor is equal to 0.
			One,					// Factor is equal to 1
			Src_Color, 				// Factor is equal to the source color vector Csource
			One_Minus_Src_Color, 	// Factor is equal to 1 minus the source color vector: 1−Csource
			Dst_Color, 				// Factor is equal to the destination color vector C¯destination
			One_Minus_Dst_Color, 	// Factor is equal to 1 minus the destination color vector: 1−C¯destination
			Src_Alpha, 				// Factor is equal to the alpha component of the source color vector Csource
			One_Minus_Src_Alpha,	// Factor is equal to 1−alpha of the source color vector Csource
			Dst_Alpha, 				// Factor is equal to the alpha component of the destination color vector Cdestination
			One_Minus_Dst_Alpha, 	// Factor is equal to 1−alpha of the destination color vector C destination
			Constant_Color, 		// Factor is equal to the constant color vector Cconstant
			One_Minus_Const_Color,	// Factor is equal to 1 - the constant color vector Cconstant
			Const_Alpha, 			// Factor is equal to the alpha component of the constant color vector Cconstant
			One_Minus_Const_Alpha,	// Factor is equal to 1−alpha of the constant color vector Cconstant.

			NUM
		};
	};
};

#endif