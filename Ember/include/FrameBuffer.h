#ifndef OPENGL_FRAME_BUFFER_H
#define OPENGL_FRAME_BUFFER_H

#include <memory>

namespace Ember {
	class FrameBuffer {
	public:
		FrameBuffer(uint32_t width, uint32_t height);
		FrameBuffer() = default;

		void Init(uint32_t width, uint32_t height);
		virtual ~FrameBuffer();

		void Bind();
		void UnBind();
		uint32_t GetColorAttachment() { return color_attachment; }
		uint32_t GetBufferStencilAttachment() const { return depth_stencil_attachment; }
	private:
		uint32_t frame_buffer_id;
		uint32_t color_attachment;
		uint32_t depth_stencil_attachment;
	};

	class RenderBuffer {
	public:
		RenderBuffer(uint32_t width, uint32_t height);
		RenderBuffer() = default;

		void Init(uint32_t width, uint32_t height);
	private:
		uint32_t rbo;
	};
}

#endif // !OPENGL_FRAME_BUFFER_H
