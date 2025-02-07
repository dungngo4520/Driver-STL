#pragma once
#include <wdm.h>

#include "Iterator.h"
#include "Memory.h"

namespace strings {
    NTSTATUS Clone(
        _Outptr_result_maybenull_ PUNICODE_STRING& Destination,
        _In_ const PCUNICODE_STRING& Source,
        _In_ const memory::PoolType PoolType,
        _In_ const ULONG Tag,
        _Out_opt_ PULONG BytesCopied = nullptr
    );
    VOID FreeClonedUnicodeString(_Inout_ PUNICODE_STRING& String, _In_ const ULONG Tag);

    class StringView : public memory::StackBuffer, public iterator::Iterable<WCHAR> {
    public:
        StringView(_In_ const PWCHAR& Buffer, _In_ const SIZE_T SizeInChar) :
            memory::StackBuffer(Buffer, SizeInChar * sizeof(WCHAR)),
            Iterable(Buffer, SizeInChar)
        {
        }
        explicit StringView(_In_ const UNICODE_STRING& String) :
            memory::StackBuffer(String.Buffer, String.Length),
            Iterable(String.Buffer, String.Length / sizeof(WCHAR))
        {
        }
        explicit StringView(_In_ const PUNICODE_STRING& String) :
            memory::StackBuffer(String->Buffer, String->Length),
            Iterable(String->Buffer, String->Length / sizeof(WCHAR))
        {
        }
        [[nodiscard]] PWCHAR Str() const { return static_cast<PWCHAR>(Ptr()); }
        [[nodiscard]] SIZE_T SizeInChar() const { return Size() / sizeof(WCHAR); }
        [[nodiscard]] SIZE_T SizeInByte() const { return Size(); }

        [[nodiscard]] bool EndWith(
            _In_z_bytecount_c_(SuffixSizeInChar) const PWCHAR& Suffix,
            SIZE_T SuffixSizeInChar,
            bool CaseSensitive = false
        ) const;
        [[nodiscard]] bool BeginWith(
            _In_z_bytecount_c_(PrefixSizeInChar) const PWCHAR& Prefix,
            SIZE_T PrefixSizeInChar,
            bool CaseSensitive = false
        ) const;
        [[nodiscard]] bool Contains(
            _In_z_bytecount_c_(SubStringSizeInChar) const PWCHAR& SubString,
            SIZE_T SubStringSizeInChar,
            bool CaseSensitive = false
        ) const;
    };
}  // namespace strings
