/**
 * @file Font.cpp
 * @author strah19
 * @date October 3 2021
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file load a .ttf file and builds it into an OpenGL texture.
 */

#include "Font.h"
#include "Assets.h"
#include "Logger.h"
#include "TextureAtlas.h"
#include <glad/glad.h>
#include <algorithm>

#define ASCII_SIZE 128

namespace Ember {
    /**
    * Creates a texture that stores all the font glypths.
    * 
    * @param const char* file of the font.
    * @param uint32_t size of the font (height).
    */
    void Font::Init(const char* filepath, uint32_t size) {
        FT_Library* ft = GetFreeType();
        FT_Face face;
        if (FT_New_Face(*ft, filepath, 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            EMBER_LOG_ERROR("Failed to load font '%s'.", filepath);
        }
        else
            EMBER_LOG_GOOD("Loaded font '%s'.", filepath);

        this->size = size;
        FT_Set_Pixel_Sizes(face, 0, size);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  
        uint32_t w = 0, h = 0;
        for (unsigned char c = 0; c < ASCII_SIZE; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                EMBER_LOG_ERROR("Failed to load glyph '%c'.", c);
                continue;
            }

            if (face->glyph->bitmap.rows > h)
                h = face->glyph->bitmap.rows;

            w += face->glyph->bitmap.width;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            w,
            h,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        uint32_t ox = 0;
        for (unsigned char c = 0; c < ASCII_SIZE; c++) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                EMBER_LOG_ERROR("Failed to load glyph '%c'.", c);
                continue;
            }

            Glyph glyph = {
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                { face->glyph->advance.x, face->glyph->advance.y }, 
                0.0f
            };

            glyph.offset = TextureAtlas::CalculateSpriteCoordinate({ ox, 0 }, w, h).x;
            glyphs.insert(std::pair<char, Glyph>(c, glyph));
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, ox, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
            ox += face->glyph->bitmap.width;
        }

        width = w;
        height = h;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        FT_Done_Face(face);
	}

    /**
    * Font Deconstructor.
    */
    Font::~Font() {
        glDeleteTextures(1, &texture);
    }

    /**
    * Calculate the size of string.
    * 
    * @param const std::string& text to figure out length.
    */
    uint32_t Font::GetSizeOfText(const std::string& text) {
        uint32_t w = 0;
        for (auto& c : text) {
            Glyph character = glyphs[c];

            w += character.size.x;
        }

        return w;
    }
}