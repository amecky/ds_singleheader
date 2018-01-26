#pragma once
#define DS_STRING_IMPLEMENTATION

namespace ds {

	static const char NULL_CHAR = '\0';

	class string {

	public:

		typedef char* iterator;
		typedef const char* const_iterator;
		typedef char* pointer;
		typedef const char* const_pointer;
		string();
		string(const char* s);
		~string();
		void append(char c);
		void append(int i);
		void append(float f);
		void append(double d);
		void append(const_pointer txt);
		string& operator+=(const string& other);

		iterator begin() { return _first; }
		iterator end() { return _last; }
		const_iterator begin() const { return _first; }
		const_iterator end() const { return _last; }

		const char& operator [] (size_t n) const;
		const char& at(size_t n) const;
		const char& back() const;
		const char& front() const;

		const_iterator c_str() const;
		bool is_small() const;
		size_t size() const;
		size_t capacity() const;
		bool is_empty() const;

		void resize(size_t s);
		void reserve(size_t s);
		void clear();

		//push_back
		//assign
		//insert
		//erase
		//replace
		//swap
		//pop_back

		bool find(const_pointer t, size_t offset = 0, size_t* index = 0) const;
		bool rfind(const_pointer t, size_t offset = 0, size_t* index = 0) const;
		bool find_first_of(const_pointer t, size_t offset = 0, size_t* index = 0) const;
		bool find_last_of(const_pointer t, size_t offset = 0, size_t* index = 0) const;
		bool find_last_not_of(const_pointer t, size_t offset = 0, size_t* index = 0) const;
		bool compare(const_pointer t) const;
		string substr(size_t start) const;
		string substr(size_t start, size_t end) const;

		unsigned int hash_code() const;
	private:
		void append(const char* first, const char* last);
		pointer _first;
		pointer _last;
		pointer _capacity;
		char _buffer[16];
	};

	namespace str {

		string value_of(int i) {
			char tmp[16];
			sprintf_s(tmp, "%d", i);
			return string(tmp);
		}

		string value_of(float f) {
			char tmp[16];
			sprintf_s(tmp, "%g", f);
			return string(tmp);
		}
	}

#ifdef DS_STRING_IMPLEMENTATION

	inline string::string() : _first(_buffer), _last(_buffer), _capacity(_buffer + 16) {
	}

	inline string::string(const char* s) : _first(_buffer), _last(_buffer), _capacity(_buffer + 16) {
		size_t len = 0;
		char temp = '\0';
		if (!s) {
			s = &temp;
		}
		for (const_iterator it = s; *it; ++it) {
			++len;
		}
		append(s, s + len);
	}

	inline string::~string() {
		if (!is_small()) {
			delete[] _first;
		}
	}

	inline const char& string::operator [] (size_t n) const {
		if (_first + n < _capacity) {
			return _first[n];
		}
		return NULL_CHAR;
	}

	inline const char& string::at(size_t n) const {
		if (_first + n < _capacity) {
			return _first[n];
		}
		return NULL_CHAR;
	}

	inline bool string::is_small() const {
		return _first == _buffer;
	}

	inline bool string::is_empty() const {
		return _first == _last;
	}

	inline size_t string::size() const {
		return (size_t)(_last - _first);
	}

	inline size_t string::capacity() const {
		return (size_t)(_capacity - _first);
	}

	inline string::const_iterator string::c_str() const {
		return _first;
	}

	inline void string::reserve(size_t newSize) {
		if (_first + newSize + 1 >= _capacity) {
			const size_t size = (size_t)(_last - _first);
			pointer newfirst = new char[newSize + 1];
			for (pointer it = _first, newit = newfirst, end = _last; it != end; ++it, ++newit) {
				*newit = *it;
			}
			if (!is_small()) {
				delete[] _first;
			}
			_first = newfirst;
			_last = newfirst + size;
			_capacity = _first + newSize;
		}
	}

	inline void string::append(const_pointer t) {
		size_t len = 0;
		char temp = '\0';
		if (!t) {
			t = &temp;
		}
		for (const_iterator it = t; *it; ++it) {
			++len;
		}
		append(t, t + len);
	}

	inline void string::append(const char* first, const char* last) {
		const size_t newsize = (size_t)((_last - _first) + (last - first) + 1);
		if (_first + newsize > _capacity) {
			reserve((newsize * 3) / 2);
		}
		for (; first != last; ++_last, ++first) {
			*_last = *first;
		}
		*_last = 0;
	}

	inline void string::append(char c) {
		append(&c, &c + 1);
	}

	inline void string::append(int i) {
		char temp[16];
		sprintf_s(temp, "%d", i);
		append(temp);
	}

	inline void string::append(float f) {
		char temp[16];
		sprintf_s(temp, "%g", f);
		append(temp);
	}

#endif
}