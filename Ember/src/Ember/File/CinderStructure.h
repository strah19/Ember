#ifndef CINDER_STRUCTURE_H
#define CINDER_STRCUTURE_H

#include "File/File.h"

namespace Ember {
	using CinderStructureType = std::string;

	struct KeyToValues {
		CinderStructureType key;
		CinderStructureType value;

		KeyToValues(CinderStructureType key, CinderStructureType value) : key(key), value(value) { }
	};

	struct Section {
		CinderStructureType section_name;
		std::vector<KeyToValues> keys;
		unsigned int word_position;
		Section(CinderStructureType section_name, unsigned int word_position) : section_name(section_name), word_position(word_position) { }
	};

	class CinderStructure {
	public:
		enum class CinderReturnCodes {
			DeletedSection, Null
		};

		bool Load(const std::string& file_path);
		~CinderStructure();

		CinderReturnCodes WriteSection(const CinderStructureType& section_name);
		CinderReturnCodes WriteKeyValueToSection(const CinderStructureType& section_name, const CinderStructureType& key, const CinderStructureType& value);
		CinderReturnCodes DeleteKey(const CinderStructureType& section_name, const CinderStructureType& key);
		std::string GetValue(const CinderStructureType& section_name, const CinderStructureType& key);

	private:
		File* core_file;

		std::vector<Section> sections;
	};
}

#endif // !CINDER_STRUCTURE_H
