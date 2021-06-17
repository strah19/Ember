#include "OpenGLFrameBuffer.h"

#include <iostream>
#include <glad/glad.h>

namespace EmberGL {
	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) {
		glGenFramebuffers(1, &frame_buffer_id);
		Bind();

		glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment);
		glBindTexture(GL_TEXTURE_2D, color_attachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment, 0);
		
		glCreateTextures(GL_TEXTURE_2D, 1, &depth_stencil_attachment);
		glBindTexture(GL_TEXTURE_2D, depth_stencil_attachment);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
			GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_stencil_attachment, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		UnBind();
	}

	FrameBuffer::~FrameBuffer() {
		glDeleteFramebuffers(1, &frame_buffer_id);
		glDeleteTextures(1, &color_attachment);
		glDeleteTextures(1, &depth_stencil_attachment);
	}

	void FrameBuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id);
	}

	void FrameBuffer::UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}