#include "Logger.h"
#include <cstdarg>
#include <ctime>
#include <sstream>
#include <vector>

namespace Ember {
    void TimestampLogCommand::RunCommand(va_list& args, const char* input) {
        const std::time_t now = std::time(nullptr); 
        const std::tm calendar_time = *std::localtime(std::addressof(now));

        command_output = std::to_string(calendar_time.tm_hour) + ":" + std::to_string(calendar_time.tm_min) + ":" + std::to_string(calendar_time.tm_sec);
    }

    void TimestampLogCommand::ProcessArgs(va_list& args) { }

    void LogCommand::AddToOutput(std::string& output, std::vector<LogCommand*>& commands) {
        size_t pos = GetPosition();
        size_t size = GetCommandOutput().size();
        output.insert(GetPosition(), GetCommandOutput());
        for (auto& command : commands)
            if (pos < command->GetPosition())
                command->SetPosition(command->GetPosition() + size);
    }

    void UserDefinedStringCommand::RunCommand(va_list& args, const char* input) { }

    void UserDefinedStringCommand::ProcessArgs(va_list& args) {
          command_output = va_arg(args, char*);
    }

    void UserLogCommand::RunCommand(va_list& args, const char* input) {
        std::string user_in;
        bool found_new_arg = false;
        while (*input != '\0') {
            if (*input == '%') {
                found_new_arg = true;
            }
            else if (found_new_arg) {
                switch (*input) {
                case 'd': user_in += std::to_string(va_arg(args, int)); break;
                case 'f': user_in += std::to_string(va_arg(args, double)); break;
                case 's': user_in += va_arg(args, char*); break;
                }
                found_new_arg = false;
            }
            else
                user_in += *input;


            ++input;
        }
        SetCommandOutput(user_in);
    }

    void UserLogCommand::ProcessArgs(va_list& args) { }

    void NewLineLogCommand::RunCommand(va_list& args, const char* input) {
        command_output = '\n';
    }

    void NewLineLogCommand::ProcessArgs(va_list& args) { }

    const std::pair<std::string, ColorCode> COLORS[] = {
        std::make_pair("{cR}", ColorCode::FG_RED),
        std::make_pair("{cDef}", ColorCode::FG_DEFAULT),
        std::make_pair("{cB}", ColorCode::FG_BLUE),
        std::make_pair("{cG}", ColorCode::FG_GREEN),
        std::make_pair("{cY}", ColorCode::FG_YELLOW),
    };

    void ColorLogCommand::RunCommand(va_list& args, const char* input) {
        for (auto& color : COLORS) {
            if (color.first == command_name) {
                color_controller.SetColor(color.second);
                break;
            }
        }

        std::stringstream ss;
        ss << color_controller;
        command_output = ss.str();
    }

    void ColorLogCommand::ProcessArgs(va_list& args) { }

    static std::map<std::string, CreateLogCommandFn> COMMANDS;

    void InitCommands();

    void InitializeLoggingSystem() {
        InitCommands();
    }

    void InitCommands() {
        COMMANDS["{ts}"] = TimestampLogCommand::Create;
        COMMANDS["{s}"] = UserDefinedStringCommand::Create;
        COMMANDS["{l}"] = UserLogCommand::Create;
        COMMANDS["\n"] = NewLineLogCommand::Create;

        for(auto& color : COLORS)
            COMMANDS[color.first] = ColorLogCommand::Create;
    }

    void FindAllOccurances(std::vector<size_t>& vec, std::string data, std::string to_search) {
        size_t pos = data.find(to_search);
        while (pos != std::string::npos)
        {
            vec.push_back(pos);
            pos = data.find(to_search, pos + to_search.size());
        }
    }

    void LogFormat::Init(const char* format, ...) {
        va_list args;
        va_start(args, format);
 
        std::string input = format;

        for (auto& current_command : COMMANDS) {
            std::vector<size_t> positions;
            FindAllOccurances(positions, input, current_command.first);
            for (auto& position : positions) {
                commands.push_back(current_command.second());
                commands.back()->SetCommand(current_command.first);
                commands.back()->SetPosition(position);
                commands.back()->ProcessArgs(args);
            }
        }

        for (auto& command : commands) {
            input.erase(command->GetPosition(), command->GetCommand().size());
            for (auto& pos_test : commands)
                if (command->GetPosition() < pos_test->GetPosition())
                    pos_test->SetPosition(pos_test->GetPosition() - command->GetCommand().size());
        }

        for (auto& command : commands) 
            command->GetResetingPosition();

        output = input;

        va_end(args);
	}

    void Logger::SetLogFormat(LogFormat* log_format) {
        formatter = log_format;
    }

    void Logger::Log(const char* fmt, ...) {
        if (formatter) {
            va_list args;
            va_start(args, fmt);
            std::string output = formatter->GetLeftOutput();

            for (auto& command : formatter->GetCommands())
                command->ResetPosition();

            for (auto& command : formatter->GetCommands()) {
                command->RunCommand(args, fmt);
                command->AddToOutput(output, formatter->GetCommands());
            }

            printf("%s", output.c_str());
            va_end(args);
        }
    }

    static Logger error_log;
    static LogFormat error_format;

    static Logger warning_log;
    static LogFormat warning_format;

    static Logger def_log;
    static LogFormat def_format;

    static Logger def_log_good;
    static LogFormat def_format_good;

    void LogImpl::Init() {
        InitializeLoggingSystem();

        error_format.Init("{cR}[{ts}]::{s}::{s}::{l}{cDef}\n", "Ember", "ERROR");
        error_log.SetLogFormat(&error_format);

        warning_format.Init("{cY}[{ts}]::{s}::{s}::{l}{cDef}\n", "Ember", "WARNING");
        warning_log.SetLogFormat(&warning_format);

        def_format.Init("{cDef}[{ts}]::{s}::{l}{cDef}\n", "Ember");
        def_log.SetLogFormat(&def_format);

        def_format_good.Init("{cG}[{ts}]::{s}::{l}{cDef}\n", "Ember");
        def_log_good.SetLogFormat(&def_format_good);
    }

    Logger& LogImpl::GetLogError() { return error_log; }

    Logger& LogImpl::GetLogWarning() { return warning_log; }

    Logger& LogImpl::GetLogDef() { return def_log; }

    Logger& LogImpl::GetLogDefGood() { return def_log_good; }
}