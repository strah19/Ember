#include "FrameBuffer.h"
#include "Logger.h"

#include <iostream>
#include <glad/glad.h>

namespace Ember {
	FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) {
		Init(width, height);
	}

	void FrameBuffer::Init(uint32_t width, uint32_t height) {
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
			EMBER_LOG_ERROR("Failed to load framebuffer.");
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
		/*
		glBindTexture(GL_TEXTURE_2D, color_attachment);
		char unsigned pixels[20][20][3];

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				pixels[i][j][0] = 0;
				pixels[i][j][1] = 255;
				pixels[i][j][2] = 0;
			}
		}
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 20, 20, GL_RGB, GL_UNSIGNED_BYTE, (void*)&pixels);

		char unsigned read_pixels[1][1][3];
		glReadPixels(0, 0, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void*) &read_pixels);
		*/
	}

	RenderBuffer::RenderBuffer(uint32_t width, uint32_t height) {
		Init(width, height);
	}

	void RenderBuffer::Init(uint32_t width, uint32_t height) {
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}
}