#include "File.h"

namespace Ember {
	bool Bind(File* file) {
		files.push_back(file);
		return true;
	}

	void Check(const char* host_file) {
		for (auto& i : files)
			if (i->Path() == host_file)
				i->CloseFile();
	}

	void Close() {
		for (auto i : files)
			i->CloseFile();
	}

	File::File(const char* file_path)
		: file_path(file_path) {
		Check(file_path);
		if (Bind(this))
			Open();
	}

	File::File(File& file) {
		this->file_path = file.file_path;
	}

	void File::operator=(const File& file) {
		this->file_path = file.file_path;
	}

	File::~File() {
		CloseFile();
	}

	void File::Open() {
		file = std::fstream(file_path, std::fstream::in | std::fstream::out | std::fstream::app);
		ASSERT_OPEN;
	}

	void File::CloseFile() {
		file.flush();
		file.close();
	}

	void File::Delete() {
		file.close();
		remove(file_path.c_str());
	}

	void File::EmptyFile() {
		CloseFile();
		file.open(file_path.c_str(), std::fstream::out | std::fstream::trunc);
		CloseFile();
		Open();
	}

	std::streamoff File::SizeInBytes() {
		ASSERT_OPEN;

		std::streampos beg, end;
		file.seekg(0, std::ios::beg);
		beg = file.tellg();
		file.seekg(0, std::ios::end);
		end = file.tellg();

		return end - beg;
	}

	std::streampos File::CurrentLocation() {
		return file.tellg();
	}

	bool File::IfEmpty() {
		return (SizeInBytes() == 0) ? true : false;
	}

	void File::ResetLocation() {
		ASSERT_OPEN;

		file.clear();
		file.seekg(0);
	}

	void File::ChangeLocation(const int location) {
		ASSERT_OPEN;
		file.seekg(location, std::ios::beg);
	}

	unsigned int File::WordCount() {
		ASSERT_OPEN;
		ResetLocation();

		std::string search_word;
		unsigned int word_counter = 0;
		while (file >> search_word)
			word_counter++;

		return word_counter;
	}

	__int64 File::LineCount() {
		ASSERT_OPEN;
		ResetLocation();

		return std::count(std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>(), '\n');
	}

	int File::LocationToWord(const char* word) {
		ASSERT_OPEN;
		ResetLocation();

		std::string word_search;
		int word_counter = 0;
		while (file >> word_search) {
			if (word_search == word)
				return word_counter;

			word_counter++;
		}

		return -1;
	}

	unsigned long File::CharacterCount() {
		ASSERT_OPEN;
		ResetLocation();

		char active_char;
		unsigned long number_chars = 0;

		while (file.get(active_char))
			if (active_char != '\n')
				number_chars++;

		return number_chars;
	}

	std::string File::ReadAll() {
		ASSERT_OPEN;
		ResetLocation();

		std::string buffer((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());

		return buffer;
	}

	std::string File::ReadWord(const int location) {
		ASSERT_OPEN;
		ResetLocation();

		std::string word_search;
		unsigned int word_counter = 0;
		while (file >> word_search) {
			if (word_counter == location)
				return word_search;
			word_counter++;
		}

		return file_path;
	}

	std::string File::ReadLine(const int line_number) {
		ASSERT_OPEN;
		ResetLocation();

		std::string line_search;
		unsigned int line_counter = 0;

		while (getline(file, line_search)) {
			if (line_counter == line_number)
				return line_search;
			line_counter++;
		}

		return file_path;
	}

	void File::DeleteLine(const int line_number) {
		ASSERT_OPEN;
		std::string file_data = ReadAll();

		std::string line_search;
		unsigned int line_counter = 0;

		std::vector<size_t> line_sizes;

		while (getline(file, line_search)) {
			line_sizes.push_back(line_search.length());

			if (line_counter == line_number) {
				size_t location = 0;
				for (size_t i = 0; i < line_sizes.size() - 1; i++) {
					location += line_sizes[i];
				}
				file_data.erase(location, line_sizes.back() + 1);
				break;
			}
			line_counter++;
		}

		line_sizes.clear();

		EmptyFile();
		file << file_data;
	}

	void File::DoEachWord(const std::function<bool(std::string& word, unsigned int counter)>& func) {
		ASSERT_OPEN;

		std::string word_search;
		unsigned int word_counter = 0;
		while (file >> word_search) {
			if (!func(word_search, word_counter))
				break;
			word_counter++;
		}

		ResetLocation();
	}

	void File::DoEachLine(const std::function<bool(std::string& line, unsigned int counter)>& func) {
		ASSERT_OPEN;

		std::string line;
		unsigned int line_counter = 0;
		while (getline(file, line)) {
			if (!func(line, line_counter))
				break;
			line_counter++;
		}

		ResetLocation();
	}

	bool CinderStructure::Load(const std::string& file_path) {
		core_file = new File(file_path.c_str());
		
		bool inside_section = false;
		bool reading_a_key = true;
		size_t current_section = -1;
		std::string save_key;
		core_file->DoEachWord([&](std::string& word, unsigned int counter){
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

		for (auto i : sections) {
			std::cout << i.section_name << std::endl;
			for (auto j : i.keys) {
				std::cout << j.key << " " << j.value << std::endl;
			}
		}

		return true;
	}

	CinderStructure::~CinderStructure() {
		delete core_file;
	}

	CinderStructure::CinderReturnCodes CinderStructure::WriteSection(const std::string& section_name) {
		for (size_t i = 0; i < sections.size(); i++) {
			if (sections[i].section_name == section_name) {
				std::string all = core_file->ReadAll();
				std::string::size_type found;
				found = all.find(section_name + ":");
				if (found != std::string::npos) 
					all.erase(found, section_name.length() + 1);
				
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
						sections[i].keys[j].value = value;

						

						return CinderStructure::CinderReturnCodes::Null;
					}
				}

				std::string code_input = section_name + ":\n";
				sections[i].keys.push_back(KeyToValues(key, value));
				core_file->WriteAfterWord(key + '=' + value + '\n', code_input);
				return CinderStructure::CinderReturnCodes::Null;
			}
		}

		return CinderStructure::CinderReturnCodes::Null;
	}
}