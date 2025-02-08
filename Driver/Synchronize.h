#pragma once
#include <wdm.h>

#include "Utils.h"

namespace sync {
    template <class T>
    using LockGuard = utils::ScopeGuard<T, &T::Acquire, &T::Release>;

    class Lock {
    public:
        Lock() = default;
        virtual ~Lock() = default;
        virtual void Acquire() = 0;
        virtual void Release() = 0;

        Lock(const Lock &) = delete;
        Lock(Lock &&) = delete;
        Lock &operator=(const Lock &) = delete;
        Lock &operator=(Lock &&) = delete;
    };

    class SpinLock final : public Lock {
    public:
        SpinLock() : m_OldIrql(PASSIVE_LEVEL) { KeInitializeSpinLock(&m_Lock); }
        void Acquire() override { KeAcquireSpinLock(&m_Lock, &m_OldIrql); }
        void Release() override { KeReleaseSpinLock(&m_Lock, m_OldIrql); }

    private:
        KSPIN_LOCK m_Lock;
        KIRQL m_OldIrql;
    };

    class FastMutex final : public Lock {
    public:
        FastMutex() { ExInitializeFastMutex(&m_Lock); }
        void Acquire() override { ExAcquireFastMutex(&m_Lock); }
        void Release() override { ExReleaseFastMutex(&m_Lock); }

    private:
        FAST_MUTEX m_Lock;
    };

    class GuardedMutex final : public Lock {
    public:
        GuardedMutex() { KeInitializeGuardedMutex(&m_Lock); }
        void Acquire() override { KeAcquireGuardedMutex(&m_Lock); }
        void Release() override { KeReleaseGuardedMutex(&m_Lock); }

    private:
        KGUARDED_MUTEX m_Lock;
    };

    class ResourceLock final : public Lock {
    public:
        ResourceLock() { ExInitializeResourceLite(&m_Lock); }
        void Acquire() override { ExEnterCriticalRegionAndAcquireResourceExclusive(&m_Lock); }
        void Release() override { ExReleaseResourceAndLeaveCriticalRegion(&m_Lock); }
        void AcquireShared() { ExEnterCriticalRegionAndAcquireResourceShared(&m_Lock); }
        void ReleaseShared() { ExReleaseResourceAndLeaveCriticalRegion(&m_Lock); }

    private:
        ERESOURCE m_Lock;
    };

    class Semaphore final : public Lock {
    public:
        explicit Semaphore(const LONG MaximumCount, const LONG InitialCount = 0)
        {
            KeInitializeSemaphore(&m_Semaphore, InitialCount, MaximumCount);
        }
        void Acquire() override { KeWaitForSingleObject(&m_Semaphore, Executive, KernelMode, FALSE, nullptr); }
        void Release() override { KeReleaseSemaphore(&m_Semaphore, IO_NO_INCREMENT, 1, FALSE); }

    private:
        KSEMAPHORE m_Semaphore;
    };

    class Event final : public Lock {
    public:
        explicit Event(const bool ManualReset, const bool InitialState)
        {
            KeInitializeEvent(&m_Event, ManualReset ? NotificationEvent : SynchronizationEvent, InitialState);
        }
        void Acquire() override { KeWaitForSingleObject(&m_Event, Executive, KernelMode, FALSE, nullptr); }
        void Release() override { KeSetEvent(&m_Event, IO_NO_INCREMENT, FALSE); }
        void Reset() { KeClearEvent(&m_Event); }

    private:
        KEVENT m_Event;
    };

}  // namespace sync
