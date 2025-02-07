#pragma once
#include <wdm.h>

namespace log {
    enum class LogLevel : UINT8 {
        Error = 0,
        Warning,
        Info,
        Debug,
    };

    inline const char* ToString(const LogLevel Level)
    {
        switch (Level) {
            case LogLevel::Error:
                return "Error";
            case LogLevel::Warning:
                return "Warning";
            case LogLevel::Info:
                return "Info";
            case LogLevel::Debug:
                return "Debug";
            default:
                return "unknown";
        }
    }

    class Logger {
    public:
        virtual ~Logger() = default;
        virtual NTSTATUS Initialize(_In_ PVOID Config) = 0;
        virtual void Destroy() = 0;

        template <typename... Args>
        void Error(const char* const Format, Args... args)
        {
            Log(LogLevel::Error, Format, args);
        }

        template <typename... Args>
        void Warning(const char* const Format, Args... args)
        {
            Log(LogLevel::Warning, Format, args);
        }

        template <typename... Args>
        void Info(const char* const Format, Args... args)
        {
            Log(LogLevel::Info, Format, args);
        }

        template <typename... Args>
        void Debug(const char* const Format, Args... args)
        {
            Log(LogLevel::Debug, Format, args);
        }

    protected:
        virtual void Log(const LogLevel Level, const char* const Format, ...) = 0;

        LogLevel m_LogLevel = LogLevel::Info;
    };

    struct DebugLoggerConfig {
        SIZE_T StructSize;
        PUNICODE_STRING KeyPath;
        PUNICODE_STRING LogLevelValueName;
    };
    class DebugLogger : public Logger {
    public:
        NTSTATUS Initialize(_In_ PVOID Config) override;
        void Destroy() override;

    private:
        void Log(const LogLevel Level, const char* const Format, ...) override;

        DebugLoggerConfig m_Config = {};
    };

    struct FileLoggerConfig {
        SIZE_T StructSize;
        PUNICODE_STRING KeyPath;
        PUNICODE_STRING LogLevelValueName;
        PUNICODE_STRING LogFilePathValueName;
    };
    class FileLogger : public Logger {
    public:
        NTSTATUS Initialize(_In_ PVOID Config) override;
        void Destroy() override;

    private:
        void Log(const LogLevel Level, const char* const Format, ...) override;

        FileLoggerConfig m_Config = {};
    };
}  // namespace log
