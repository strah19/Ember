#include "CinderStructure.h"

namespace Ember {
	bool CinderStructure::Load(const std::string& file_path) {
		core_file = new File(file_path.c_str());

		bool inside_section = false;
		bool reading_a_key = true;
		size_t current_section = -1;
		std::string save_key;
		core_file->DoEachWord([&](std::string& word, unsigned int counter) {
			if (word.back() == ':') {
				std::string copy_word = word;
				copy_word.pop_back();
				sections.push_back(Section(copy_word, counter));

				inside_section = true;
				current_section++;
			}
			else if (inside_section) {
				if (word.back() == ':')
					inside_section = false;
				else {
					if (reading_a_key) {
						save_key = word;
						reading_a_key = false;
					}
					else if (!reading_a_key && word != "=") {
						sections[current_section].keys.push_back(KeyToValues(save_key, word));
						reading_a_key = true;
					}
				}
			}

			return true;
			});

		return true;
	}

	CinderStructure::~CinderStructure() {
		delete core_file;
	}

	CinderStructure::CinderReturnCodes CinderStructure::WriteSection(const CinderStructureType& section_name) {
		for (size_t i = 0; i < sections.size(); i++) {
			if (sections[i].section_name == section_name) {
				std::string all = core_file->ReadAll();

				unsigned top, bottom;
				top = all.find(section_name + ":");
				if (i == sections.size() - 1)
					bottom = all.find(all.length() - top);
				else
					bottom = all.find(sections[i + 1].section_name + ":");

				all.erase(top, bottom - top);

				core_file->EmptyFile();
				core_file->Write(all);

				sections.erase(sections.begin() + i);
				return CinderStructure::CinderReturnCodes::DeletedSection;
			}
		}

		std::string code_input = section_name + ":\n";
		core_file->Write(code_input);
		sections.push_back(Section(section_name, core_file->WordCount() - 1));

		return CinderStructure::CinderReturnCodes::Null;
	}

	CinderStructure::CinderReturnCodes CinderStructure::WriteKeyValueToSection(const CinderStructureType& section_name, const CinderStructureType& key, const CinderStructureType& value) {
		for (size_t i = 0; i < sections.size(); i++) {
			if (sections[i].section_name == section_name) {
				for (size_t j = 0; j < sections[i].keys.size(); j++) {
					if (sections[i].keys[j].key == key) {
						std::string all = core_file->ReadAll();
						unsigned top_section = all.find(section_name + ":");
						unsigned bottom_section;
						if (i == sections.size() - 1)
							bottom_section = all.find(all.length() - top_section);
						else
							bottom_section = all.find(sections[i + 1].section_name + ":");

						std::string section = all.substr(top_section, bottom_section - top_section);

						unsigned find_key = section.find(key);
						section.erase(find_key + key.size() + 3, sections[i].keys[j].value.size());

						section.insert(find_key + key.size() + 3, value);

						all.erase(top_section, bottom_section - top_section);
						all.insert(top_section, section);

						core_file->EmptyFile();
						core_file->Write(all);
						sections[i].keys[j].value = value;

						return CinderStructure::CinderReturnCodes::Null;
					}
				}

				std::string code_input = section_name + ":\n";
				sections[i].keys.push_back(KeyToValues(key, value));
				core_file->WriteAfterWord(key + " = " + value + '\n', code_input);
				return CinderStructure::CinderReturnCodes::Null;
			}
		}

		return CinderStructure::CinderReturnCodes::Null;
	}

	CinderStructure::CinderReturnCodes CinderStructure::DeleteKey(const CinderStructureType& section_name, const CinderStructureType& key) {
		for (size_t i = 0; i < sections.size(); i++) {
			if (sections[i].section_name == section_name) {
				for (size_t j = 0; j < sections[i].keys.size(); j++) {
					if (sections[i].keys[j].key == key) {
						std::string all = core_file->ReadAll();
						unsigned top_section = all.find(section_name + ":");
						unsigned bottom_section;
						if (i == sections.size() - 1)
							bottom_section = all.find(all.length() - top_section);
						else
							bottom_section = all.find(sections[i + 1].section_name + ":");

						std::string section = all.substr(top_section, bottom_section - top_section);

						unsigned find_key = section.find(key);
						section.erase(find_key, key.size() + 3 + sections[i].keys[j].value.size());

						all.erase(top_section, bottom_section - top_section);
						all.insert(top_section, section);

						core_file->EmptyFile();
						core_file->Write(all);
					}
				}
			}
		}

		return CinderStructure::CinderReturnCodes::Null;
	}

	std::string CinderStructure::GetValue(const CinderStructureType& section_name, const CinderStructureType& key) {
		for (size_t i = 0; i < sections.size(); i++) {
			if (sections[i].section_name == section_name) {
				for (size_t j = 0; j < sections[i].keys.size(); j++) {
					if (sections[i].keys[j].key == key) {
						return sections[i].keys[j].value;
					}
				}
			}
		}

		return "";
	}
}