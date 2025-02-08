#include "Log.h"
#include "Map.h"
#include "Strings.h"
#include "Synchronize.h"

EXTERN_C_START
DRIVER_INITIALIZE DriverEntry;
EXTERN_C_END

#pragma alloc_text(INIT, DriverEntry)

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
    ExInitializeDriverRuntime(DrvRtPoolNxOptIn);

    // iterator
    auto TestIterator = []() -> bool {
        const wchar_t str[] = L"test string 123";
        const wchar_t str2[] = L"test string 123";
        iterator::Iterable<wchar_t> ForwardIterator(const_cast<wchar_t*>(str), sizeof(str) / sizeof(wchar_t) - 1);
        iterator::ReverseIterable<wchar_t> BackwardIterator(
            const_cast<wchar_t*>(str), sizeof(str) / sizeof(wchar_t) - 1
        );

        int i = 0;
        for (auto it = ForwardIterator.begin(); it != ForwardIterator.end(); ++it, i++) {
            if (*it != str2[i]) {
                return false;
            }
        }
        for (auto c : ForwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }
        for (auto& c : ForwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }
        for (const auto c : ForwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }
        for (const auto& c : ForwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }

        i = sizeof(str) / sizeof(wchar_t) - 1;
        for (auto it = BackwardIterator.begin(); it != BackwardIterator.end(); ++it, i--) {
            if (*it != str2[i]) {
                return false;
            }
        }
        for (auto c : BackwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }
        for (auto& c : BackwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }
        for (const auto c : BackwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }
        for (const auto& c : BackwardIterator) {
            UNREFERENCED_PARAMETER(c);
        }

        return true;
    };


    auto TestLog = []() -> bool {
        log::DebugLogger logger;
        log::DebugLoggerConfig config{ .StructSize = sizeof(log::DebugLoggerConfig),
                                       .KeyPath = nullptr,
                                       .LogLevelValueName = nullptr };
        if (!NT_SUCCESS(logger.Initialize(&config))) {
            return false;
        }
        logger.Debug("test: %s", "test str");
        logger.Info("test: %s", "test str");
        logger.Error("test: %s", "test str");
        logger.Warning("test: %s", "test str");

        return true;
    };

    auto TestMap = []() -> bool {
        PUNICODE_STRING test = nullptr;
        DECLARE_CONST_UNICODE_STRING(source, L"TEST");
        strings::Clone(test, &source, memory::PoolType::NonPaged, 1234, nullptr);
        strings::MakeEmpty(test, 123, memory::PoolType::NonPaged, 1234, nullptr);

        container::Map<int, memory::PoolType::NonPaged, 1234> m;
        m.Insert(1);
        m.Insert(2);
        if (m.Size() != 2) {
            return false;
        }
        return true;
    };

    auto TestMemory = []() -> bool {
        auto ptr = memory::AllocPaged(10, 0);
        memory::Free(ptr, 0);

        ptr = memory::AllocNonPaged(10, 0);
        memory::Free(ptr, 0);

        ptr = memory::Alloc(memory::PoolType::NonPaged, 10, 0);
        memory::Free(ptr, 0);

        class Test : public memory::Alloctable<memory::PoolType::NonPaged, 0> {
        public:
            int Data;
        };

        const auto ptr2 = new Test();
        delete ptr2;

        wchar_t data[] = L"test data";
        memory::StackBuffer stack(data, sizeof(data));
        if (stack.Ptr() != data) {
            return false;
        }
        if (stack.Size() != sizeof(data)) {
            return false;
        }

        auto heap = memory::HeapBuffer::Create(10, memory::PoolType::NonPaged, 0);
        if (heap.Tag() != 0) {
            return false;
        }
        if (heap.Size() != 10) {
            return false;
        }
        return true;
    };

    auto TestString = []() -> bool {
        PUNICODE_STRING str = nullptr;
        strings::MakeEmpty(str, 10, memory::PoolType::NonPaged, 0);
        if (str == nullptr) {
            return false;
        }
        strings::Free(str, 0);

        DECLARE_CONST_UNICODE_STRING(str2, L"test string");
        strings::Clone(str, &str2, memory::PoolType::NonPaged, 0);
        if (str == nullptr) {
            return false;
        }
        wchar_t data[] = L"test data";
        strings::StringView view(data, sizeof(data) / sizeof(wchar_t));
        strings::StringView view3(str);
        strings::StringView view2(str2);
        strings::Free(str, 0);
        return true;
    };

    auto TestSync = []() -> bool {
        sync::FastMutex mutex;
        sync::SpinLock spin;
        sync::Event event(false, false);
        sync::GuardedMutex guarded_mutex;
        sync::ResourceLock resource_lock;
        sync::Semaphore semaphore(10);

        {
            sync::LockGuard<sync::FastMutex> lock(mutex);
        }
        {
            sync::LockGuard<sync::SpinLock> lock(spin);
        }
        {
            sync::LockGuard<sync::Event> lock(event);
        }
        {
            sync::LockGuard<sync::GuardedMutex> lock(guarded_mutex);
        }
        {
            sync::LockGuard<sync::ResourceLock> lock(resource_lock);
        }
        {
            sync::LockGuard<sync::Semaphore> lock(semaphore);
        }
        return true;
    };

    auto TestUtils = []() -> bool {
        class Test {
        public:
            void Enter() {}
            void Exit() {}
        };

        Test t;
        utils::ObjectScopeGuard<Test, &Test::Enter, &Test::Exit> ScopeGuard(t);
        return true;
    };

    // Run tests
    TestIterator();
    TestLog();
    TestMap();
    TestMemory();
    TestString();
    TestSync();
    TestUtils();
    
    return STATUS_SUCCESS;
}
