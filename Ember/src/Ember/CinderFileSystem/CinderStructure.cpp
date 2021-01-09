#include "CinderStructure.h"

namespace Cinder {
    void CinderStructure::LoadCinderStructure(const CinderType& file_path) {
        file = CFSFile(file_path.c_str());
        FillMemoryFromFile();
    }

    void CinderStructure::FillMemoryFromFile() {
        bool reading_section = false;
        bool reading_a_key = true;
        std::string current_key;

        file.DoEachWord([&](std::string& word, const uint32_t counter) {
            if (IsWordSection(word)) {
                word.pop_back();
                sections.push_back(Section(word, counter));

                reading_section = true;
            }
            else if (reading_section) {
                if (IsWordSection(word))
                    reading_section = false;
                else {
                    if (reading_a_key) {
                        current_key = word;
                        reading_a_key = false;
                    }
                    else if (!reading_a_key && word != KEY_TO_VALUE_DIVIDER) {
                        sections.back().key_to_value[current_key] = word;
                        reading_a_key = true;
                    }
                }
            }

            return true;
        });
    }

    bool CinderStructure::IsWordSection(const CinderType& possible_section_name) {
        return (possible_section_name.back() == SECTION_SIGNATURE.front());
    }

    void CinderStructure::WriteSection(const CinderType& section_name) {
        size_t section_index = CheckIfSectionExists(section_name);
        if (section_index == -1) 
            AddSection(section_name);
    }

    void CinderStructure::AddSection(const CinderType& section_name) {
        try {
            if (section_name == "")
                throw CinderStructureInputException();
            std::string code_input;
            code_input = section_name + SECTION_SIGNATURE + '\n';

            file.Write(code_input);
            sections.push_back(Section(section_name, (size_t)(file.GetWordCount() - 1)));
        }
        catch (CinderStructureInputException& exception) {
            exception.DisplayErrorMessage();
        }
    }

    void CinderStructure::DeleteSection(const CinderType& section_name) {
        size_t section_index = CheckIfSectionExists(section_name);
        if (section_index != -1) {
            std::string file_stream = file.ReadFile();
            std::pair<size_t, size_t> segment = GetCharacterLocationOfSection(file_stream, section_name, section_index);

            file_stream.erase(segment.first, segment.second - segment.first);

            file.EmptyFile();
            file.Write(file_stream);
        }
    }

    void CinderStructure::WriteKeyValueToSection(const CinderType& section_name, const CinderType& key, const CinderType& value) {
        try {
            if (section_name == "" || key == "")
                throw CinderStructureInputException();
            size_t section_index = CheckIfSectionExists(section_name);
            if (section_index != -1) {
                CinderType key_value = CheckIfKeyExists(section_index, key);
                if (key_value != "") {
                    std::string stream = file.ReadFile();
                    std::pair<size_t, size_t> segment = GetCharacterLocationOfSection(stream, section_name, section_index);

                    std::string section = GetWholeSectionInStream(stream, segment);

                    size_t find_key = section.find(key);
                    section.erase(find_key, key_value.size() + key.size() + 3);
                    section.insert(find_key, key + " = " + value);

                    InsertSectionToStream(stream, segment, section);
                    AddKey(section_name, { key, value }, section_index, stream);
                }
                else
                    GenerateKey(section_name, { key, value }, section_index);
            }
        }
        catch (CinderStructureInputException& exception) {
            exception.DisplayErrorMessage();
        }
    }

    void CinderStructure::GenerateKey(const CinderType& section_name, const KeyToValue& key_value, size_t section_index) {
        std::string stream = file.ReadFile();
        std::pair<size_t, size_t> segment = GetCharacterLocationOfSection(stream, section_name, section_index);

        std::string section = stream.substr(segment.first, segment.second - segment.first);
        section.append(key_value.key + " = " + key_value.value + '\n');

        InsertSectionToStream(stream, segment, section);
        AddKey(section_name, { key_value.key, key_value.value }, section_index, stream);
    }

    void CinderStructure::AddKey(const CinderType& section_name, const KeyToValue& key_value, size_t section_index, const CinderType& file_stream) {
        file.EmptyFile();
        file.Write(file_stream);
        sections[section_index].key_to_value[key_value.key] = key_value.value;
    }

    void CinderStructure::DeleteKey(const CinderType& section_name, const CinderType& key) {
        size_t section_index = CheckIfSectionExists(section_name);
        if (section_index != -1) {
            CinderType key_value = CheckIfKeyExists(section_index, key);
            if (key_value != "") {
                std::string stream = file.ReadFile();
                std::pair<size_t, size_t> segment = GetCharacterLocationOfSection(stream, section_name, section_index);

                std::string section = GetWholeSectionInStream(stream, segment);

                size_t find_key = section.find(key);
                section.erase(find_key, key_value.size() + key.size() + 4);

                InsertSectionToStream(stream, segment, section);

                file.EmptyFile();
                file.Write(stream);
                for (auto& key_v : sections[section_index].key_to_value) {
                    if (key_v.second == key_value) {
                        sections[section_index].key_to_value.erase(key_v.first);
                        break;
                    }
                }
            }
        }
    }

    CinderType CinderStructure::GetWholeSectionInStream(const CinderType& file_stream, std::pair<size_t, size_t>& section_positions) {
        return (file_stream.substr(section_positions.first, section_positions.second - section_positions.first));
    }

    void CinderStructure::InsertSectionToStream(CinderType& file_stream, std::pair<size_t, size_t>& section_positions, CinderType& section) {
        file_stream.erase(section_positions.first, section_positions.second - section_positions.first);
        file_stream.insert(section_positions.first, section);
    }

    std::pair<size_t, size_t> CinderStructure::GetCharacterLocationOfSection(const CinderType& file_stream, const CinderType& section_name, size_t section_index) {
        size_t top_section = file_stream.find(section_name + SECTION_SIGNATURE);
        size_t bottom_section;
        if (section_index == sections.size() - 1)
            bottom_section = file_stream.length() - top_section;
        else
            bottom_section = file_stream.find(sections[section_index + 1].section_name + ":");

        return std::make_pair(top_section, bottom_section);
    }

    size_t CinderStructure::CheckIfSectionExists(const CinderType& section_name) {
        for (size_t i = 0; i < sections.size(); i++) {
            if (sections[i].section_name == section_name) {
                return i;
            }
        }

        return -1;
    }

    void CinderStructure::Log() {
        for (auto& section : sections) {
            std::cout << "Section: " << section.section_name << std::endl;
            std::cout << "Section Position: " << section.word_position << std::endl;
            for (auto& key : section.key_to_value)
                std::cout << "Key: " << key.first << ", Value: " << key.second << std::endl;
        }
    }

    CinderType CinderStructure::CheckIfKeyExists(size_t section_index, const CinderType& key) {
        if (sections[section_index].key_to_value.find(key) != sections[section_index].key_to_value.end())
            return sections[section_index].key_to_value[key];
        return "";
    }

    CinderType CinderStructure::GetValue(const CinderType& section_name, const CinderType& key) {
        size_t section_index = CheckIfSectionExists(section_name);
        if (section_index != -1) {
            CinderType key_value = CheckIfKeyExists(section_index, key);
            if (key_value != "") {
                return key_value;
            }
        }

        return "";
    }

}