#include "SpritesheetDividerParser.h"

std::vector<Ember::Rect> SpritesheetParser::Read(const std::string& file_path) {
	file = Cinder::CFSFile(file_path.c_str());
	std::vector<Ember::Rect> dividers;

	int c = 0;
	file.DoEachWord([&](std::string& word, uint32_t counter) {
		if (counter % 4 == 0) {
			dividers.push_back(Ember::Rect({ 0, 0, 0, 0 }));
			c = 0;
		}
		switch (c) {
		case 0:
			dividers.back().x = stoi(word);
			break;
		case 1:
			dividers.back().y = stoi(word);
			break;
		case 2:
			dividers.back().w = stoi(word);
			break;
		case 3:
			dividers.back().h = stoi(word);
			break;
		}

		c++;

		return true;
	});

	for (auto& d : dividers) {
		std::cout << d.pos << " " << d.size << std::endl;
	}

	file.CloseFile();

	return dividers;
}

void SpritesheetParser::Save(const std::string& file_path, std::vector<Ember::Rect>& dividers, const Ember::IVec2& texture_pos) {
	file = Cinder::CFSFile(file_path.c_str());

	for (auto& divider : dividers) {
		divider.x = (divider.x - texture_pos.x);
		divider.y = (divider.y - texture_pos.y);
		file.Write(divider.x);
		file.Write(" ");
		file.Write(divider.y);
		file.Write(" ");
		file.Write(divider.w);
		file.Write(" ");
		file.Write(divider.h);
		file.Write('\n');
	}

	file.CloseFile();
}