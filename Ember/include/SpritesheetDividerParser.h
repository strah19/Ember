#ifndef SPRITESHEET_PARSER_H
#define SPRITESHEET_PARSER_H

#include "CinderFileSys.h"
#include <Ember.h>

class SpritesheetParser {
public:
	std::vector<Ember::Rect> Read(const std::string& file_path);
	void Save(const std::string& file_path, std::vector<Ember::Rect>& dividers, const Ember::IVec2& texture_pos);
private:
	Cinder::CFSFile file;
};

class UserDividedSpritesheet {
public:
	inline void SetDividers(const std::vector<Ember::Rect>& dividers) { this->dividers = dividers; }
	inline void SetSelected(size_t index) { selected = index; }
	inline size_t GetMaxSize() const { return selected; }
	inline Ember::Rect ReturnSourceRect() const { return dividers[selected]; }
private:
	size_t selected;
	std::vector<Ember::Rect> dividers;
};

#endif // !SPRITESHEET_PARSER_H
