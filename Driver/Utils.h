#pragma once

namespace utils {
    template <class T, void (T::*EnterFn)(), void (T::*ExitFn)()>
    class ScopeGuard {
    public:
        explicit ScopeGuard(T &Lock) : m_Lock(Lock) { (m_Lock.*EnterFn)(); }
        ~ScopeGuard() { (m_Lock.*ExitFn)(); }

        ScopeGuard(const ScopeGuard &) = delete;
        ScopeGuard(ScopeGuard &&) = delete;
        ScopeGuard &operator=(const ScopeGuard &) = delete;
        ScopeGuard &operator=(ScopeGuard &&) = delete;

    private:
        T &m_Lock;
    };
}  // namespace utils
