#include "Log.h"
#include "Include.h"

#include <stdarg.h>

NTSTATUS log::DebugLogger::Initialize(_In_ PVOID Config)
{
    PAGED_CODE()

    auto ConfigPtr = static_cast<DebugLoggerConfig*>(Config);
    if (ConfigPtr == nullptr) {
        return STATUS_INVALID_PARAMETER;
    }
    if (ConfigPtr->StructSize != sizeof(DebugLoggerConfig)) {
        return STATUS_INFO_LENGTH_MISMATCH;
    }

    auto Status = strings::Clone(m_Config.KeyPath, ConfigPtr->KeyPath, memory::PoolType::Paged, TAG_LOGGING);
    if (!NT_SUCCESS(Status)) {
        return Status;
    }

    Status =
        strings::Clone(m_Config.LogLevelValueName, ConfigPtr->LogLevelValueName, memory::PoolType::Paged, TAG_LOGGING);
    if (!NT_SUCCESS(Status)) {
        strings::Free(m_Config.KeyPath, TAG_LOGGING);
        return Status;
    }

    return Status;
}

void log::DebugLogger::Destroy()
{
    PAGED_CODE()
    strings::Free(m_Config.KeyPath, TAG_LOGGING);
    strings::Free(m_Config.LogLevelValueName, TAG_LOGGING);
}

void log::DebugLogger::Log(const LogLevel Level, const char* const Format, ...)
{
    PAGED_CODE()
    if (Level > m_LogLevel) {
        return;
    }
    va_list Args;
    va_start(Args, Format);
    DbgPrint(Format, Args);
    va_end(Args);
}

NTSTATUS log::FileLogger::Initialize(_In_ PVOID Config)
{
    UNREFERENCED_PARAMETER(Config);
    return NTSTATUS();
}

void log::FileLogger::Destroy() {}

void log::FileLogger::Log(const LogLevel Level, const char* const Format, ...)
{
    UNREFERENCED_PARAMETER(Level);
    UNREFERENCED_PARAMETER(Format);
}
