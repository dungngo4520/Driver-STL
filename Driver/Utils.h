#pragma once

namespace utils {
    template <class T, void (T::*EnterFn)(), void (T::*ExitFn)()>
    class ObjectScopeGuard {
    public:
        explicit ObjectScopeGuard(T &Object) : m_Object(Object) { (m_Object.*EnterFn)(); }
        ~ObjectScopeGuard() { (m_Object.*ExitFn)(); }

        ObjectScopeGuard(const ObjectScopeGuard &) = delete;
        ObjectScopeGuard(ObjectScopeGuard &&) = delete;
        ObjectScopeGuard &operator=(const ObjectScopeGuard &) = delete;
        ObjectScopeGuard &operator=(ObjectScopeGuard &&) = delete;

    private:
        T &m_Object;
    };
}  // namespace utils
