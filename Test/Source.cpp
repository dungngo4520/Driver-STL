#include <iostream>
#include <Windows.h>

namespace memory {
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
}  // namespace memory


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
        explicit Iterable(T* Data, SIZE_T Size) : m_Data(Data), m_Size(Size) {}
        Iterator<T> begin() { return Iterator<T>(m_Data); }
        Iterator<T> end() { return Iterator<T>(m_Data + m_Size); }
        ConstIterator<T> begin() const { return ConstIterator<T>(m_Data); }
        ConstIterator<T> end() const { return ConstIterator<T>(m_Data + m_Size); }

    private:
        T* m_Data = nullptr;
        SIZE_T m_Size = 0;
    };

    template <typename T>
    class ReverseIterable {
    public:
        explicit ReverseIterable(T* Data, SIZE_T Size) : m_Data(Data), m_Size(Size) {}
        ReverseIterator<T> begin() { return ReverseIterator<T>(m_Data + m_Size - 1); }
        ReverseIterator<T> end() { return ReverseIterator<T>(m_Data - 1); }
        ReverseConstIterator<T> begin() const { return ReverseConstIterator<T>(m_Data + m_Size - 1); }
        ReverseConstIterator<T> end() const { return ReverseConstIterator<T>(m_Data - 1); }

    private:
        T* m_Data = nullptr;
        SIZE_T m_Size = 0;
    };
}  // namespace iterator

namespace strings {
    class StringView : public memory::StackBuffer, public iterator::Iterable<WCHAR> {
    public:
        StringView(_In_z_bytecount_c_(Length) const PWCHAR& Buffer, _In_ const SIZE_T SizeInChar) :
            memory::StackBuffer(const_cast<PWCHAR>(Buffer), SizeInChar * sizeof(WCHAR)),
            Iterable(const_cast<PWCHAR>(Buffer), SizeInChar)
        {
        }
        [[nodiscard]] PWCHAR Str() const { return static_cast<PWCHAR>(Ptr()); }
        [[nodiscard]] SIZE_T SizeInChar() const { return Size() / sizeof(WCHAR); }
        [[nodiscard]] SIZE_T SizeInByte() const { return Size(); }

        bool EndWith(
            _In_z_bytecount_c_(SuffixSizeInChar) const PWCHAR& Suffix,
            const SIZE_T SuffixSizeInChar,
            bool CaseSensitive
        ) const
        {
            if (SuffixSizeInChar > SizeInChar()) {
                return false;
            }

            Iterable<const WCHAR> suffixIterable(Suffix, SuffixSizeInChar);
            auto it = begin() + (SizeInChar() - SuffixSizeInChar);
            auto itSuffix = suffixIterable.begin();
            for (; itSuffix != suffixIterable.end(); ++it, ++itSuffix) {
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

        bool BeginWith(
            _In_z_bytecount_c_(PrefixSizeInChar) const PWCHAR& Prefix,
            const SIZE_T PrefixSizeInChar,
            bool CaseSensitive
        ) const
        {
            if (PrefixSizeInChar > SizeInChar()) {
                return false;
            }

            Iterable<const WCHAR> prefixIterable(Prefix, PrefixSizeInChar);
            auto it = begin();
            auto itPrefix = prefixIterable.begin();
            for (; itPrefix != prefixIterable.end(); ++it, ++itPrefix) {
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

        bool Contains(
            _In_z_bytecount_c_(SubStringSizeInChar) const PWCHAR& SubString,
            const SIZE_T SubStringSizeInChar,
            bool CaseSensitive
        ) const
        {
            if (SubStringSizeInChar > SizeInChar()) {
                return false;
            }

            Iterable<const WCHAR> subStringIterable(SubString, SubStringSizeInChar);

            for (auto it = begin(); it != end(); ++it) {
                auto itSubString = subStringIterable.begin();
                auto itTemp = it;
                for (; itSubString != subStringIterable.end(); ++itTemp, ++itSubString) {
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
                if (itSubString == subStringIterable.end()) {
                    return true;
                }
            }

            return false;
        }
    };
}  // namespace strings


int main()
{
    wchar_t str[] = L"Hello World!";
    strings::StringView view(str, wcslen(str));
    if (view.EndWith(const_cast<PWCHAR>(L"ld!"), 3, false)) {
        std::wcout << L"End with World" << std::endl;
    }
    else {
        std::wcout << L"Not end with World" << std::endl;
    }

    if (view.BeginWith(const_cast<PWCHAR>(L"Hello"), 5, false)) {
        std::wcout << L"Begin with Hello" << std::endl;
    }
    else {
        std::wcout << L"Not begin with Hello" << std::endl;
    }

    if (view.Contains(const_cast<PWCHAR>(L"World"), 5, false)) {
        std::wcout << L"Contains World" << std::endl;
    }
    else {
        std::wcout << L"Not contains World" << std::endl;
    }
}
