#pragma once
namespace iterator {
    template <typename T>
    class Iterator {
    public:
        explicit Iterator(T* Ptr) : m_Ptr(Ptr) {}
        Iterator operator++()
        {
            Iterator i = *this;
            ++m_Ptr;
            return i;
        }
        Iterator operator++(int)
        {
            Iterator i = *this;
            ++m_Ptr;
            return i;
        }
        Iterator operator+=(int Step)
        {
            m_Ptr += Step;
            return *this;
        }
        Iterator operator+(int Step)
        {
            Iterator it = *this;
            it += Step;
            return it;
        }
        Iterator operator--()
        {
            Iterator i = *this;
            --m_Ptr;
            return i;
        }
        Iterator operator--(int)
        {
            Iterator i = *this;
            --m_Ptr;
            return i;
        }
        Iterator operator-=(int Step)
        {
            m_Ptr -= Step;
            return *this;
        }
        Iterator operator-(int Step)
        {
            Iterator it = *this;
            it -= Step;
            return it;
        }
        T& operator*() { return *m_Ptr; }
        T* operator->() { return m_Ptr; }
        bool operator==(const Iterator& Rhs) { return m_Ptr == Rhs.m_Ptr; }
        bool operator!=(const Iterator& Rhs) { return m_Ptr != Rhs.m_Ptr; }

    private:
        T* m_Ptr;
    };

    template <typename T>
    class ConstIterator {
    public:
        explicit ConstIterator(T* Ptr) : m_Ptr(Ptr) {}
        ConstIterator operator++()
        {
            ConstIterator i = *this;
            ++m_Ptr;
            return i;
        }
        ConstIterator operator++(int)
        {
            ConstIterator i = *this;
            ++m_Ptr;
            return i;
        }
        ConstIterator operator+=(int Step)
        {
            m_Ptr += Step;
            return *this;
        }
        ConstIterator operator+(int Step)
        {
            ConstIterator it = *this;
            it += Step;
            return it;
        }
        ConstIterator operator--()
        {
            ConstIterator i = *this;
            --m_Ptr;
            return i;
        }
        ConstIterator operator--(int)
        {
            ConstIterator i = *this;
            --m_Ptr;
            return i;
        }
        ConstIterator operator-=(int Step)
        {
            m_Ptr -= Step;
            return *this;
        }
        ConstIterator operator-(int Step)
        {
            ConstIterator it = *this;
            it -= Step;
            return it;
        }
        const T& operator*() { return *m_Ptr; }
        const T* operator->() { return m_Ptr; }
        bool operator==(const ConstIterator& Rhs) { return m_Ptr == Rhs.m_Ptr; }
        bool operator!=(const ConstIterator& Rhs) { return m_Ptr != Rhs.m_Ptr; }

    private:
        T* m_Ptr;
    };

    template <typename T>
    class ReverseIterator {
    public:
        explicit ReverseIterator(T* Ptr) : m_Ptr(Ptr) {}
        ReverseIterator operator++()
        {
            ReverseIterator i = *this;
            --m_Ptr;
            return i;
        }
        ReverseIterator operator++(int)
        {
            ReverseIterator i = *this;
            --m_Ptr;
            return i;
        }
        ReverseIterator operator+=(int Step)
        {
            m_Ptr -= Step;
            return *this;
        }
        ReverseIterator operator+(int Step)
        {
            ReverseIterator it = *this;
            it += Step;
            return it;
        }
        ReverseIterator operator--()
        {
            ReverseIterator i = *this;
            ++m_Ptr;
            return i;
        }
        ReverseIterator operator--(int)
        {
            ReverseIterator i = *this;
            ++m_Ptr;
            return i;
        }
        ReverseIterator operator-=(int Step)
        {
            m_Ptr += Step;
            return *this;
        }
        ReverseIterator operator-(int Step)
        {
            ReverseIterator it = *this;
            it -= Step;
            return it;
        }
        T& operator*() { return *m_Ptr; }
        T* operator->() { return m_Ptr; }
        bool operator==(const ReverseIterator& Rhs) { return m_Ptr == Rhs.m_Ptr; }
        bool operator!=(const ReverseIterator& Rhs) { return m_Ptr != Rhs.m_Ptr; }

    private:
        T* m_Ptr;
    };

    template <typename T>
    class ReverseConstIterator {
    public:
        explicit ReverseConstIterator(T* Ptr) : m_Ptr(Ptr) {}
        ReverseConstIterator operator++()
        {
            ReverseConstIterator i = *this;
            --m_Ptr;
            return i;
        }
        ReverseConstIterator operator++(int)
        {
            ReverseConstIterator i = *this;
            --m_Ptr;
            return i;
        }
        ReverseConstIterator operator+=(int Step)
        {
            m_Ptr -= Step;
            return *this;
        }
        ReverseConstIterator operator+(int Step)
        {
            ReverseConstIterator it = *this;
            it += Step;
            return it;
        }
        ReverseConstIterator operator--()
        {
            ReverseConstIterator i = *this;
            ++m_Ptr;
            return i;
        }
        ReverseConstIterator operator--(int)
        {
            ReverseConstIterator i = *this;
            ++m_Ptr;
            return i;
        }
        ReverseConstIterator operator-=(int Step)
        {
            m_Ptr += Step;
            return *this;
        }
        ReverseConstIterator operator-(int Step)
        {
            ReverseConstIterator it = *this;
            it -= Step;
            return it;
        }
        const T& operator*() { return *m_Ptr; }
        const T* operator->() { return m_Ptr; }
        bool operator==(const ReverseConstIterator& Rhs) { return m_Ptr == Rhs.m_Ptr; }
        bool operator!=(const ReverseConstIterator& Rhs) { return m_Ptr != Rhs.m_Ptr; }

    private:
        T* m_Ptr;
    };

    template <typename T>
    class Iterable {
    public:
        explicit Iterable(T* Data, const SIZE_T Size) : m_Data(Data), m_Size(Size) {}
        [[nodiscard]] Iterator<T> begin() { return Iterator<T>(m_Data); }
        [[nodiscard]] Iterator<T> end() { return Iterator<T>(m_Data + m_Size); }
        [[nodiscard]] ConstIterator<T> begin() const { return ConstIterator<T>(m_Data); }
        [[nodiscard]] ConstIterator<T> end() const { return ConstIterator<T>(m_Data + m_Size); }

    private:
        T* m_Data = nullptr;
        SIZE_T m_Size = 0;
    };

    template <typename T>
    class ReverseIterable {
    public:
        explicit ReverseIterable(T* Data, const SIZE_T Size) : m_Data(Data), m_Size(Size) {}
        [[nodiscard]] ReverseIterator<T> begin() { return ReverseIterator<T>(m_Data + m_Size - 1); }
        [[nodiscard]] ReverseIterator<T> end() { return ReverseIterator<T>(m_Data - 1); }
        [[nodiscard]] ReverseConstIterator<T> begin() const { return ReverseConstIterator<T>(m_Data + m_Size - 1); }
        [[nodiscard]] ReverseConstIterator<T> end() const { return ReverseConstIterator<T>(m_Data - 1); }

    private:
        T* m_Data = nullptr;
        SIZE_T m_Size = 0;
    };
}  // namespace iterator
