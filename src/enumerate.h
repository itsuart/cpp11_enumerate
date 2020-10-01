#pragma once
#include <cstddef>
#include <type_traits>
#include <iterator>
#include <string>

namespace helpers {

    namespace enumerate_impl {

        template<typename TValue>
        struct EnumeratedValue final {
            EnumeratedValue(std::size_t count, TValue& value)
                : count(count), value(value)
            {}

            constexpr EnumeratedValue() : count(), value(*(TValue*)nullptr) {}

            ~EnumeratedValue() = default;

            EnumeratedValue(const EnumeratedValue&) = default;
            //no copy-assigning
            EnumeratedValue& operator=(const EnumeratedValue&) = delete;

            //no moving
            EnumeratedValue(EnumeratedValue&&) = delete;
            EnumeratedValue& operator=(EnumeratedValue&&) = delete;

            // A number(count) assigned to the value in the traversed range.
            std::size_t count;

            // Reference to the value in the traversed range.
            TValue& value;

            operator TValue& () {
                return value;
            }
        };


        // In containers like vector, string, etc...
        template<typename Iterator>
        struct UnifiedIt {
            using iterator_category = typename Iterator::iterator_category;
            using difference_type = typename Iterator::difference_type;
            using value_type = typename Iterator::value_type;
            using reference = typename Iterator::reference;
        };

        template<typename T>
        struct UnifiedIt<T*> {
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using reference = T&;
        };

        template<typename T>
        struct UnifiedIt<const T*> {
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = const T;
            using reference = const T&;
        };


        template<typename ContainerIterator>
        class EnumeratingIterator final {
            using UnifiedIterator = UnifiedIt<ContainerIterator>;
            using T = typename std::remove_reference<typename UnifiedIterator::reference>::type;
            using InstantiatedEnumeratedValue = EnumeratedValue<T>;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = InstantiatedEnumeratedValue;
            using difference_type = typename UnifiedIterator::difference_type;
            using pointer = InstantiatedEnumeratedValue*;
            using reference = InstantiatedEnumeratedValue&;

            explicit EnumeratingIterator(ContainerIterator it, std::size_t countStart, std::ptrdiff_t step)
                : _count(countStart)
                , _step(step)
                , _it(static_cast<ContainerIterator&&>(it))
            {}

            reference operator*() {
                T& newItem = *_it;
                _item.~EnumeratedValue();
                new (&_item) InstantiatedEnumeratedValue(_count, newItem);
                return _item;
            }

            reference operator->() {
                return this->operator*();
            }

            void operator++() {
                ++_it;
                _count += _step;
            }

            bool operator!=(const EnumeratingIterator& other) const {
                return _it != other._it;
            }

        private:
            std::size_t _count;
            std::ptrdiff_t _step;
            ContainerIterator _it;
            InstantiatedEnumeratedValue _item;
        };


        template<typename ContainerIterator>
        class EnumeratingRange final {
        public:
            using iterator = EnumeratingIterator<ContainerIterator>;

            EnumeratingRange(
                ContainerIterator begin,
                ContainerIterator end,
                std::size_t countStart,
                std::ptrdiff_t step
            )
                : _begin( iterator(static_cast<ContainerIterator&&>(begin), countStart, step) )
                ,   _end( iterator(static_cast<ContainerIterator&&>(end),   countStart, step) )
            {}

            iterator begin() {
                return _begin;
            }

            iterator end() {
                return _end;
            }

        private:
            iterator _begin;
            iterator _end;
        };


        template<typename Container>
        class OwningEnumeratingRange final {
        public:
            using iterator = EnumeratingIterator<typename Container::const_iterator>;

            OwningEnumeratingRange(Container&& data, std::size_t countStart, std::ptrdiff_t step)
                : _data(static_cast<Container&&>(data))
                , _begin( iterator(_data.begin(), countStart, step) )
                , _end(   iterator(_data.end(),   countStart, step) )
            {}

            iterator begin() {
                return _begin;
            }

            iterator end() {
                return _end;
            }

        private:
            Container _data;
            iterator _begin;
            iterator _end;
        };


        template<typename Container>
        struct UnifiedContainerIterationInfo {
            using value_type = typename Container::value_type;
            using iterator = typename Container::iterator;
            using const_iterator = typename Container::const_iterator;
        };

        template<typename Container>
        struct UnifiedContainerIterationInfo<const Container> {
            using value_type = const typename Container::value_type;
            using iterator = typename Container::const_iterator;
            using const_iterator = typename Container::const_iterator;
        };

        template<typename T, std::size_t N>
        struct UnifiedContainerIterationInfo<T[N]> {
            using value_type = T;
            using iterator = T*;
            using const_iterator = const T*;
        };

        template<typename T, std::size_t N>
        struct UnifiedContainerIterationInfo<const T[N]> {
            using value_type = const T;
            using iterator = const T*;
            using const_iterator = const T*;
        };


    } // namespace counted_range_impl


    /*
        Returns iterable object that provides ref to original element and count.
    */
    template<
        typename Container,
        typename Ret = enumerate_impl::EnumeratingRange<
            typename enumerate_impl::UnifiedContainerIterationInfo<Container>::iterator
        >
    >
    Ret enumerate(Container& c, std::size_t countStart = 0, std::ptrdiff_t step = 1) {
        return Ret(std::begin(c), std::end(c), countStart, step);
    }

    /*
        Returns iterable object that provides ref to original element and count.
    */
    template<
        typename Iterator,
        typename Ret = enumerate_impl::EnumeratingRange<Iterator>
    >
    Ret enumerate(Iterator begin, Iterator end, std::size_t countStart = 0, std::ptrdiff_t step = 1) {
        return Ret(static_cast<Iterator&&>(begin), static_cast<Iterator&&>(end), countStart, step);
    }


    /*
        Returns iterable object that provides ref to original element and count. Object hosts the data from the rvalue.
    */
    template<
        typename Container,
        typename Ret = enumerate_impl::OwningEnumeratingRange<Container>
    >
    Ret enumerate(Container&& c, std::size_t countStart = 0, std::ptrdiff_t step = 1) {
        return Ret(static_cast<Container&&>(c), countStart, step);
    }

    /*
        Returns iterable object that provides ref to original element and count.
        Object owns the data pointed by initializer_list.
    */
    template<typename T, typename Ret = enumerate_impl::OwningEnumeratingRange<std::basic_string<T>>>
    Ret enumerate(std::initializer_list<T>&& c, std::size_t countStart = 0, std::ptrdiff_t step = 1) {
        return Ret( std::basic_string<T> ( static_cast<std::initializer_list<T>&&>(c) ), countStart, step);
    }

}