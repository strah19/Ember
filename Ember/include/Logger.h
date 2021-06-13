#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <map>
#include <iostream>
#include <vector>

namespace Ember {
	class LogCommand {
	public:
		LogCommand() : command_name("") { }
		virtual void RunCommand(va_list& args, const char* input) = 0;
		virtual void ProcessArgs(va_list& args) = 0;
		virtual void AddToOutput(std::string& output, std::vector<LogCommand*>& commands);

		inline const std::string GetCommand() const { return command_name; }
		inline const std::string GetCommandOutput() const { return command_output; }
		inline size_t GetPosition() const { return starting_position; }

		inline void SetCommand(const std::string& command_name) { this->command_name = command_name; };
		inline void SetCommandOutput(const std::string& command_output) { this->command_output = command_output; };
		inline void SetPosition(size_t position) { starting_position = position; }
		inline void ResetPosition() { starting_position = reseting_position; }
		inline void GetResetingPosition() { reseting_position = starting_position; }
	protected:
		std::string command_name;
		std::string command_output;
		size_t starting_position = 0;
		size_t reseting_position = 0;
	};

	typedef LogCommand* (__stdcall* CreateLogCommandFn)(void);

	class TimestampLogCommand : public LogCommand {
	public:
		TimestampLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new TimestampLogCommand(); }
	};

	class UserDefinedStringCommand : public LogCommand {
	public:
		UserDefinedStringCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new UserDefinedStringCommand(); }
	};

	class UserLogCommand : public LogCommand {
	public:
		UserLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new UserLogCommand(); }
	};

	class NewLineLogCommand : public LogCommand {
	public:
		NewLineLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new NewLineLogCommand(); }
	};

	enum ColorCode {
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_YELLOW = 33,
		FG_DEFAULT = 37,

		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};

	class ColorController {
	public:
		ColorController() : code(FG_DEFAULT) { }

		inline void SetColor(ColorCode code) { this->code = code; }

		friend std::ostream& operator<<(std::ostream& os, const ColorController& controller) {
			return os << "\033[" << controller.code << "m";
		}
	private:
		ColorCode code;
	};

	class ColorLogCommand : public LogCommand {  
	public:
		ColorLogCommand() { }
		void RunCommand(va_list& args, const char* input) override;
		void ProcessArgs(va_list& args) override;
		static LogCommand* __stdcall Create() { return new ColorLogCommand(); }
	private:
		ColorController color_controller;
	};

	class LogFormat {
	public:
		void Init(const char* format, ...);
		std::vector<LogCommand*>& GetCommands() { return commands; }
		std::string GetLeftOutput() { return output; }
	private:
		std::vector<LogCommand*> commands;
		std::string output;
	};

	class Logger {
	public:
		void SetLogFormat(LogFormat* log_format);
		void Log(const char* fmt, ...); 
	private:
		LogFormat* formatter;
	};

	void InitializeLoggingSystem();

	class LogImpl {
	public:
		static void Init();

		static Logger& GetLogError();
		static Logger& GetLogWarning();
		static Logger& GetLogDef();
		static Logger& GetLogDefGood();
	};
}

#ifdef EMBER_DEBUG
	#define EMBER_LOG_ERROR(...) Ember::LogImpl::GetLogError().Log(__VA_ARGS__);
	#define EMBER_LOG_WARNING(...) Ember::LogImpl::GetLogWarning().Log(__VA_ARGS__);
	#define EMBER_LOG(...) Ember::LogImpl::GetLogDef().Log(__VA_ARGS__);
	#define EMBER_LOG_GOOD(...) Ember::LogImpl::GetLogDefGood().Log(__VA_ARGS__);
#endif // EMBER_DEBUG

#ifdef EMBER_RELEASE
	#define EMBER_LOG_ERROR(...) 
	#define EMBER_LOG_WARNING(...) 
	#define EMBER_LOG(...) 
	#define EMBER_LOG_GOOD(...) 
#endif // EMBER_RELEASE

#ifdef EMBERL_DIST
	#define EMBER_LOG_ERROR(...) 
	#define EMBER_LOG_WARNING(...) 
	#define EMBER_LOG(...) 
	#define EMBER_LOG_GOOD(...) 
#endif // EMBERL_DIST

#endif // !LOGGER_H
