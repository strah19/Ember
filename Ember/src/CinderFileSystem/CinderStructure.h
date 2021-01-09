#ifndef CINDER_STRUCTURE_H
#define CINDER_STRUCTURE_H

#include "CFSFile.h"
#include <unordered_map>

namespace Cinder {
    struct KeyToValue;
    using CinderType = std::string;

    struct Section {
        std::unordered_map<CinderType, CinderType> key_to_value;
        CinderType section_name;
        size_t word_position;
        Section(const CinderType& section_name, size_t word_position) : section_name(section_name), word_position(word_position) { }
    };

    struct KeyToValue {
        CinderType key;
        CinderType value;
    };

    class CinderStructureInputException {
    public:
        void DisplayErrorMessage() {
            std::cerr << "Exception : " << msg << std::endl;
        }
    private:
        std::string msg = "User input is not correct.";
    };

    class CinderStructure {
    public:
        /**
        * Will go through the file and get all the sections and key-values to memory.
        * 
        * @param The filepath for the data.
        */
        void LoadCinderStructure(const CinderType& file_path);

        /**
        * Will print the sections and key-value info into the console.
        */
        void Log();

        /**
        * Write a new section if a section already doesn't exist.
        *
        * @param A name for the new section.
        */
        void WriteSection(const CinderType& section_name);

        /**
        * Will return the section_index if it exists, otherwise it returns -1.
        * 
        * @param The section name to find.
        * 
        * @return The section index.
        */
        size_t CheckIfSectionExists(const CinderType& section_name);

        /**
        * Will go through sections keys and find it's value, otherwise will return "".
        *
        * @param The section index.
        * @param The key in the section.
        * 
        * @return Will return the key if it exists.
        */
        CinderType CheckIfKeyExists(size_t section_index, const CinderType& key);

        /**
        * Will write a key and value to a section. If it already exists it does nothing to the key but the value can keep being updated.
        * 
        * @param The section name.
        * @param The key that will go into the section.
        * @param The value for the key.
        */
        void WriteKeyValueToSection(const CinderType& section_name, const CinderType& key, const CinderType& value);

        /**
        * Will delete the given section if it exists.
        *
        * @param The section name.
        */
        void DeleteSection(const CinderType& section_name);

        /**
        * Will delete a key in a section if it exists.
        *
        * @param The section name.
        * @param The key to delete in the section.
        */
        void DeleteKey(const CinderType& section_name, const CinderType& key);

        /**
        * If the key is found in a section, it will return the key's value.
        * 
        * @param The section name.
        * @param The key in the section.
        * 
        * @return The key's value.
        */
        CinderType GetValue(const CinderType& section_name, const CinderType& key);
    private:
        void FillMemoryFromFile();
        bool IsWordSection(const CinderType& possible_section_name);
        void AddSection(const CinderType& section_name);
        void AddKey(const CinderType& section_name, const KeyToValue& key_value, size_t section_index, const CinderType& file_stream);
        CinderType GetWholeSectionInStream(const CinderType& file_stream, std::pair<size_t, size_t>& section_positions);
        void GenerateKey(const CinderType& section_name, const KeyToValue& key_value, size_t section_index);
        void InsertSectionToStream(CinderType& file_stream, std::pair<size_t, size_t>& section_positions, CinderType& section);

        std::pair<size_t, size_t> GetCharacterLocationOfSection(const CinderType& file_stream, const CinderType& section_name, size_t section_index);
    private:
        CFSFile file;
        std::vector<Section> sections;

        const CinderType SECTION_SIGNATURE = ":";
        const CinderType KEY_TO_VALUE_DIVIDER = "=";
    };
}

#endif // !CINDER_STRUCTURE_H
