#ifndef FILE_H
#define FILE_H

#include "Ember.h"

#define FILE_STATUS_DEBUG

#ifdef FILE_STATUS_DEBUG
#define ASSERT(condition, message) \
        do { \
            if (! (condition)) { \
                std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                          << " line " << __LINE__ << ": " << message << std::endl; \
                std::terminate(); \
            } \
        } while (false)
#define ASSERT_OPEN ASSERT(file.is_open(), "Could Not Open File " << file_path)
#endif

#ifndef  FILE_STATUS_DEBUG
#define ASSERT(condition, message) do { } while (false)
#define ASSERT_OPEN
#endif

namespace Ember {
	class File {
	public:
		File(const char* file_path);

		File(File& file);
		void operator=(const File& file);

		virtual ~File();

		void EmptyFile();
		void CloseFile();
		void Delete();
		bool IsEnd() { return file.eof(); }

		std::streampos CurrentLocation();
		void ChangeLocation(const int location);
		void ResetLocation();
		void Open();

		__int64 LineCount();
		unsigned int WordCount();
		unsigned long CharacterCount();

		std::streamoff SizeInBytes();
		bool IfEmpty();
		inline const std::string Path() const { return file_path; }

		std::string ReadAll();
		std::string ReadLine(const int line_number);
		std::string ReadWord(const int location);
		int LocationToWord(const char* word);
		bool AtEndOfFile() { return (CurrentLocation() == EOF); }

		void DoEachWord(const std::function<bool(std::string& word, unsigned int counter)>& func);
		void DoEachLine(const std::function<bool(std::string& line, unsigned int counter)>& func);

		template <typename T>
		void Write(const T& data) {
			ASSERT_OPEN;

			std::stringstream buffer;
			buffer << data;

			file << buffer.str();
		}

		template <typename T>
		void WriteAfterWord(const T& data, std::string& finder_word) {
			ASSERT_OPEN;

			std::string file_data = ReadAll();

			size_t loc_word = file_data.find(finder_word);
			std::stringstream buffer(data);

			if (loc_word != std::string::npos)
				file_data.insert(loc_word + finder_word.length(), buffer.str());

			EmptyFile();
			file << file_data;
		}

		template<typename T>
		void WriteAtEndLine(const T& data, int line_number) {
			ASSERT_OPEN;
			std::string file_data = ReadAll();

			int word_counter = 0;
			for (size_t i = 0; i < file_data.length(); i++) {
				if (file_data[i] == '\n') {
					if (line_number == word_counter) {
						std::stringstream buffer;
						buffer << data;
						file_data.insert(i, buffer.str());
						break;
					}
					word_counter++;
				}
			}

			EmptyFile();
			file << file_data;
		}

		template <typename T>
		void WriteObject(const T& obj) {
			ASSERT_OPEN;
			file.write((char*)&obj, sizeof(obj));
		}

		template <typename T>
		void ReadObject(T& obj) {
			ASSERT_OPEN;
			file.read((char*)&obj, sizeof(obj));
		}

		void DeleteLine(const int line_number);
	protected:
		std::string file_path;
		std::fstream file;
	};

	static std::vector<File*> files;

	static bool Bind(File* file);
	static void Check(const char* host_file);
	static void Close();

}

#endif // !FILE_H
