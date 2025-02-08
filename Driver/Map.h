#pragma once
#include <ntddk.h>

#include "memory.h"

namespace container {
    // Map Entry must have these 3 functions made as static
    template <class T, memory::PoolType Type, ULONG Tag>
    class Map {
    public:
        explicit Map(const PVOID Context = nullptr) : m_Context(Context)
        {
            RtlInitializeGenericTableAvl(&m_Table, CompareRoutine, AllocateRoutine, FreeRoutine, m_Context);
        }

        bool Insert(T Entry)
        {
            BOOLEAN isInserted;
            RtlInsertElementGenericTableAvl(&m_Table, static_cast<PVOID>(&Entry), sizeof(T), &isInserted);
            return isInserted;
        }

        bool Delete(const T &Entry) { return RtlDeleteElementGenericTableAvl(&m_Table, static_cast<PVOID>(&Entry)); }
        bool Exist(const T &Entry)
        {
            return RtlLookupElementGenericTableAvl(&m_Table, static_cast<PVOID>(&Entry)) != nullptr;
        }
        T *Find(const T &Entry)
        {
            return static_cast<T *>(RtlLookupElementGenericTableAvl(&m_Table, static_cast<PVOID>(&Entry)));
        }
        SIZE_T Size() { return RtlNumberGenericTableElementsAvl(&m_Table); }

        template <typename Func>
        void ForEach(Func F)
        {
            for (auto p = RtlEnumerateGenericTableAvl(&m_Table, TRUE); p != nullptr;
                 p = RtlEnumerateGenericTableAvl(&m_Table, FALSE)) {
                F();
            }
        }

        template <typename Func, typename Condition>
        void ForEachStopIf(Func F, Condition StopCondition)
        {
            for (auto p = RtlEnumerateGenericTableAvl(&m_Table, TRUE); p != nullptr;
                 p = RtlEnumerateGenericTableAvl(&m_Table, FALSE)) {
                if (StopCondition())
                    return;
                F();
            }
        }

    private:
        static RTL_GENERIC_COMPARE_RESULTS
            // ReSharper disable once CppParameterMayBeConstPtrOrRef
            CompareRoutine(_In_ struct _RTL_AVL_TABLE *Table, _In_ PVOID FirstStruct, _In_ PVOID SecondStruct)
        {
            UNREFERENCED_PARAMETER(Table);
            if (*static_cast<T *>(FirstStruct) > *static_cast<T *>(SecondStruct))
                return GenericGreaterThan;
            if (*static_cast<T *>(FirstStruct) < *static_cast<T *>(SecondStruct))
                return GenericLessThan;
            return GenericEqual;
        }
        // ReSharper disable once CppParameterMayBeConstPtrOrRef
        static PVOID AllocateRoutine(_In_ struct _RTL_AVL_TABLE *Table, _In_ CLONG ByteSize)
        {
            UNREFERENCED_PARAMETER(Table);
            const auto Ptr = memory::Alloc(Type, ByteSize, Tag);
            auto ObjectPtr = reinterpret_cast<T *>(static_cast<PRTL_BALANCED_LINKS>(Ptr) + 1);
            new (ObjectPtr) T();
            return Ptr;
        }
        // ReSharper disable once CppParameterMayBeConstPtrOrRef
        // ReSharper disable once CppParameterMayBeConst
        static VOID FreeRoutine(_In_ struct _RTL_AVL_TABLE *Table, _In_ __drv_freesMem(Mem) _Post_invalid_ PVOID Buffer)
        {
            UNREFERENCED_PARAMETER(Table);
            T* ObjectPtr = reinterpret_cast<T *>(static_cast<PRTL_BALANCED_LINKS>(Buffer) + 1);
            ObjectPtr->~T();
        }

    private:
        RTL_AVL_TABLE m_Table;
        PVOID m_Context;
    };
}  // namespace container
