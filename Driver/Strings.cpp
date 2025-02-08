#include "Strings.h"
#include "Iterator.h"

#include <stdarg.h>  // NOLINT(modernize-deprecated-headers)

NTSTATUS strings::MakeEmpty(
    _Outptr_result_buffer_maybenull_(ByteAllocated) PUNICODE_STRING& Destination,
    _In_ const ULONG SizeInChar,
    _In_ const memory::PoolType PoolType,
    _In_ const ULONG Tag,
    _Out_opt_ const PULONG ByteAllocated
)
{
    if (ByteAllocated)
        *ByteAllocated = 0;
    auto BufferSize = static_cast<ULONG>(sizeof(UNICODE_STRING) + SizeInChar * sizeof(WCHAR));
    switch (PoolType) {
        case memory::PoolType::NonPaged:
            Destination = static_cast<PUNICODE_STRING>(memory::AllocNonPaged(BufferSize, Tag));
            break;
        case memory::PoolType::Paged:
            Destination = static_cast<PUNICODE_STRING>(memory::AllocPaged(BufferSize, Tag));
            break;
        default:
            return STATUS_INVALID_PARAMETER;
    }
    if (Destination == nullptr) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    Destination->Length = 0;
    Destination->MaximumLength = static_cast<USHORT>(SizeInChar * sizeof(WCHAR));
    Destination->Buffer = reinterpret_cast<PWCH>(Destination + 1);
    if (ByteAllocated != nullptr) {
        *ByteAllocated = BufferSize;
    }
    return STATUS_SUCCESS;
}

NTSTATUS strings::Clone(
    _Outptr_result_maybenull_ PUNICODE_STRING& Destination,
    _In_ const PCUNICODE_STRING& Source,
    _In_ const memory::PoolType PoolType,
    _In_ const ULONG Tag,
    _Out_opt_ const PULONG BytesCopied
)
{
    if (BytesCopied)
        *BytesCopied = 0;

    if (Source == nullptr) {
        return STATUS_INVALID_PARAMETER;
    }

    const auto StringSize = min(Source->Length, Source->MaximumLength);
    if (const auto status = MakeEmpty(Destination, StringSize / sizeof(WCHAR), PoolType, Tag, BytesCopied); !NT_SUCCESS(status)) {
        return status;
    }

    if (ExGetPreviousMode() == KernelMode) {
        RtlCopyMemory(Destination->Buffer, Source->Buffer, StringSize);
    }
    else {
        __try {  // NOLINT(clang-diagnostic-language-extension-token)
            RtlCopyMemory(Destination->Buffer, Source->Buffer, StringSize);
        }
        __except (EXCEPTION_EXECUTE_HANDLER) {
            Free(Destination, Tag);
            if (BytesCopied != nullptr) {
                *BytesCopied = 0;
            }
            return GetExceptionCode();
        }
    }
    Destination->Length = Destination->MaximumLength;
    return STATUS_SUCCESS;
}

void strings::Free(_Inout_updates_to_(nullptr, 0) PUNICODE_STRING& String, _In_ const ULONG Tag)
{
    memory::Free(String, Tag);
    String = nullptr;
}

bool strings::StringView::EndWith(
    _In_z_bytecount_c_(SuffixSizeInChar) const PWCHAR& Suffix,
    const SIZE_T SuffixSizeInChar,
    const bool CaseSensitive
) const
{
    if (SuffixSizeInChar > SizeInChar()) {
        return false;
    }

    Iterable<const WCHAR> SuffixIterable(Suffix, SuffixSizeInChar);
    auto it = begin() + static_cast<int>(SizeInChar() - SuffixSizeInChar);
    for (auto itSuffix = SuffixIterable.begin(); itSuffix != SuffixIterable.end(); ++it, ++itSuffix) {
        if (CaseSensitive) {
            if (*it != *Suffix) {
                return false;
            }
        }
        else {
            if (towlower(*it) != towlower(*itSuffix)) {
                return false;
            }
        }
    }

    return true;
}

bool strings::StringView::BeginWith(
    _In_z_bytecount_c_(PrefixSizeInChar) const PWCHAR& Prefix,
    const SIZE_T PrefixSizeInChar,
    const bool CaseSensitive
) const
{
    if (PrefixSizeInChar > SizeInChar()) {
        return false;
    }

    Iterable<const WCHAR> PrefixIterable(Prefix, PrefixSizeInChar);
    auto it = begin();
    auto itPrefix = PrefixIterable.begin();
    for (; itPrefix != PrefixIterable.end(); ++it, ++itPrefix) {
        if (CaseSensitive) {
            if (*it != *Prefix) {
                return false;
            }
        }
        else {
            if (towlower(*it) != towlower(*itPrefix)) {
                return false;
            }
        }
    }
    return true;
}

bool strings::StringView::Contains(
    _In_z_bytecount_c_(SubStringSizeInChar) const PWCHAR& SubString,
    const SIZE_T SubStringSizeInChar,
    const bool CaseSensitive
) const
{
    if (SubStringSizeInChar > SizeInChar()) {
        return false;
    }

    Iterable<const WCHAR> SubStringIterable(SubString, SubStringSizeInChar);

    for (auto it = begin(); it != end(); ++it) {
        auto itSubString = SubStringIterable.begin();
        auto itTemp = it;
        for (; itSubString != SubStringIterable.end(); ++itTemp, ++itSubString) {
            if (CaseSensitive) {
                if (*itTemp != *itSubString) {
                    break;
                }
            }
            else {
                if (towlower(*itTemp) != towlower(*itSubString)) {
                    break;
                }
            }
        }
        if (itSubString == SubStringIterable.end()) {
            return true;
        }
    }

    return false;
}
