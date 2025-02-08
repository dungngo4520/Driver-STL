#pragma once
#include <wdm.h>

constexpr ULONG UlongByteSwap(const ULONG x)
{
    return (((x << 24) & 0xFF000000) | ((x << 8) & 0x00FF0000) | ((x >> 8) & 0x0000FF00) | ((x >> 24) & 0x000000FF));
}

// ReSharper disable once CppMultiCharacterLiteral
inline constexpr ULONG TAG_LOGGING = UlongByteSwap('KLOG');  // NOLINT(clang-diagnostic-four-char-constants)

namespace memory {
    enum class PoolType : UINT8 { NonPaged, Paged };

    inline PVOID AllocNonPaged(const SIZE_T& Size, const ULONG& Tag)
    {
        return ExAllocatePool2(POOL_FLAG_NON_PAGED, Size, Tag);
    }


    inline PVOID AllocPaged(const SIZE_T& Size, const ULONG& Tag)
    {
        return ExAllocatePool2(POOL_FLAG_PAGED, Size, Tag);
    }

    inline void Free(const PVOID& Ptr, const ULONG& Tag) { ExFreePoolWithTag(Ptr, Tag); }

    inline PVOID ReAllocNonPaged(const PVOID& Ptr, const SIZE_T& Size, const ULONG& Tag)
    {
        if (Ptr) {
            Free(Ptr, Tag);
        }
        return AllocNonPaged(Size, Tag);
    }

    inline PVOID ReAllocPaged(const PVOID& Ptr, const SIZE_T& Size, const ULONG& Tag)
    {
        if (Ptr) {
            Free(Ptr, Tag);
        }
        return AllocPaged(Size, Tag);
    }

    inline PVOID Alloc(const PoolType Type, const SIZE_T Size, const ULONG Tag)
    {
        switch (Type) {
            case PoolType::Paged:
                return AllocPaged(Size, Tag);
            case PoolType::NonPaged:
                return AllocNonPaged(Size, Tag);
            default:
                return nullptr;
        }
    }

    template <PoolType Type, ULONG Tag>
    class Alloctable {
    public:
        void* operator new(const size_t Size) { return Alloc(Type, Size, Tag); }
        void operator delete(void* Ptr) { Free(Ptr, Tag); }
    };

    class Buffer {
    public:
        [[nodiscard]] PVOID Ptr() const { return m_Buffer; }
        [[nodiscard]] SIZE_T Size() const { return m_Size; }

    protected:
        void SetPtr(_In_ const PVOID& Ptr) { m_Buffer = Ptr; }
        void SetSize(_In_ const SIZE_T& Size) { m_Size = Size; }

    private:
        PVOID m_Buffer = nullptr;
        SIZE_T m_Size = 0;
    };

    class StackBuffer : public Buffer {
    public:
        explicit StackBuffer(_In_ const PVOID& Ptr = nullptr, _In_ const SIZE_T& Size = 0)
        {
            SetPtr(Ptr);
            SetSize(Size);
        }
    };

    class HeapBuffer : public Buffer {
    public:
        HeapBuffer(_In_ const PVOID& Ptr, _In_ const SIZE_T& Size, _In_ const ULONG& Tag) : m_Tag(Tag)
        {
            SetPtr(Ptr);
            SetSize(Size);
        }
        ~HeapBuffer()
        {
            if (Ptr() != nullptr) {
                Free(Ptr(), m_Tag);
            }
        }

        static HeapBuffer Create(_In_ const SIZE_T& Size, _In_ const PoolType& PoolType, _In_ const ULONG& Tag)
        {
            PVOID Ptr = nullptr;
            switch (PoolType) {
                case PoolType::NonPaged:
                    Ptr = AllocNonPaged(Size, Tag);
                    break;
                case PoolType::Paged:
                    Ptr = AllocPaged(Size, Tag);
                    break;
            }

            return HeapBuffer{ Ptr, Size, Tag };
        }

        HeapBuffer(_In_ const HeapBuffer&) = delete;
        HeapBuffer(_In_ HeapBuffer&&) = delete;
        HeapBuffer& operator=(_In_ const HeapBuffer&) = delete;
        HeapBuffer& operator=(_In_ HeapBuffer&&) = delete;


        [[nodiscard]] ULONG Tag() const { return m_Tag; }

    private:
        ULONG m_Tag;
    };
}  // namespace memory

void* operator new(const size_t Size);
void* operator new(const size_t Size, void* Ptr);
void operator delete(void* Ptr, size_t);
