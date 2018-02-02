#pragma once

namespace ds {

	template <typename TYPE> void Construct(void* dest) {
		new ((TYPE*)dest) TYPE;
	}

	template <typename TYPE> void Destruct(void* dest) {
		((TYPE*)dest)->~TYPE();
	}

	template<typename T, int N = 16>
	class smallvector {

	public:

		using iterator = T*;
		using const_iterator = const T *;
		using reference = T &;
		using const_reference = const T &;
		using pointer = T *;
		using const_pointer = const T *;

		smallvector() : _begin((T*)_buffer), _end((T*)_buffer), _capacity(N) {
			_constructor = !__has_trivial_constructor(T);
			_destructor = !__has_trivial_destructor(T);
			if (_constructor) {
				for (int i = 0; i < N; ++i) {
					Construct<T>(&_buffer[i]);
				}
			}
		}

		~smallvector() {
			if (!is_small()) {
				T* ptr = _begin;
				size_t s = size();
				for (size_t i = 0; i < s; ++i) {
					Destruct<T>(ptr);
					++ptr;
				}
				delete[] _begin;
			}
			if (_destructor) {
				for (int i = 0; i < N; ++i) {
					Destruct<T>(&_buffer[i]);
				}
			}
		}

		void clear() {
			_end = _begin;
		}

		void reset() {
			_begin = _buffer;
			_end = _begin;
			_capacity = N;
		}

		pointer data() {
			return _begin;
		}

		const_pointer data() const {
			return _begin;
		}

		void push_back(const_reference t) {
			if ((size() + 1) >= _capacity) {
				grow(_capacity * 2);
			}
			memcpy(_end, &t, sizeof(T));
			++_end;
		}

		void pop_back() {
			if (size() > 0) {
				--_end;
			}
		}

		iterator erase(iterator it) {
			if (_destructor) {
				Destruct<T>(it);
			}
			size_t s = end() - it;
			if (s > 0) {
				memcpy(it, it + 1, s * sizeof(T));
				--_end;
			}
			return it;
		}

		bool is_empty() const {
			return _begin == _end;
		}

		bool remove(size_t index) {
			if (index < size()) {
				T* p = _begin + index;
				if (_destructor) {
					Destruct<T>(p);
				}
				size_t s = size() - index;
				if (s > 0) {
					memcpy(p, p + 1, s * sizeof(T));
					--_end;
				}
				return true;
			}
			return false;
		}

		size_t size() const {
			return _end - _begin;
		}

		size_t capacity() const {
			return _capacity;
		}

		size_t size_in_bytes() const {
			return size_t((char*)_end - (char*)_begin);
		}

		iterator begin() {
			return _begin;
		}

		const_iterator begin() const {
			return _begin;
		}

		bool contains(const T& t) const {
			const_iterator it = begin();
			while (it != end()) {
				if (*it == t) {
					return true;
				}
				++it;
			}
			return false;
		}

		const_pointer find(const_reference t) const {
			const_iterator it = begin();
			while (it != end()) {
				if (*it == t) {
					return it;
				}
				++it;
			}
			return 0;
		}

		const_iterator end() const {
			return _end;
		}

		iterator end() {
			return _end;
		}

		reference operator[](size_t idx) {
			// FIXME: how to handle out of index
			return begin()[idx];
		}

		const_reference operator[](size_t idx) const {
			// FIXME: how to handle out of index
			return begin()[idx];
		}

		reference at(size_t idx) {
			// FIXME: how to handle out of index
			return begin()[idx];
		}

		const_reference at(size_t idx) const {
			// FIXME: how to handle out of index
			return begin()[idx];
		}

		bool is_small() const {
			return _begin == &_buffer[0];
		}

	private:

		void grow(size_t newCapacity) {
			if (newCapacity > N) {
				T* tmp = new T[newCapacity];
				size_t s = size();
				memcpy(tmp, _begin, _capacity * sizeof(T));
				if (!is_small()) {
					T* ptr = _begin;
					size_t s = size();
					for (size_t i = 0; i < s; ++i) {
						Destruct<T>(ptr);
						++ptr;
					}
					delete[] _begin;
				}
				_begin = tmp;
				_end = _begin + s;
				_capacity = newCapacity;
			}
		}
		T _buffer[N];
		size_t _capacity;
		bool _small;
		T* _begin;
		T* _end;
		bool _constructor;
		bool _destructor;
	};

}