#include "Log.h"
#include "Strings.h"

#include <stdarg.h>  // NOLINT(modernize-deprecated-headers)

NTSTATUS log::DebugLogger::Initialize(_In_ const PVOID Config)
{
    const auto ConfigPtr = static_cast<DebugLoggerConfig*>(Config);
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
    strings::Free(m_Config.KeyPath, TAG_LOGGING);
    strings::Free(m_Config.LogLevelValueName, TAG_LOGGING);
}

void log::DebugLogger::Log(const LogLevel Level, const char* const Format, ...)
{
    if (Level > m_LogLevel) {
        return;
    }
    va_list Args;
    va_start(Args, Format);
    DbgPrint(Format, Args);
    va_end(Args);
}
