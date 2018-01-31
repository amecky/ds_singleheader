#pragma once
// -----------------------------------------------------------------
//
// This is a lightweight string implementation. It uses the small
// string approach introduced by LLVM. This means that it uses
// an internal buffer of 16 chars and will only allocate memory
// if the string will exceed this limit. 
// 
// It is a singleheader implementation. You need to define
// #define DS_STRING_IMPLEMENTATION inside one of your source
// files to include the entire implementation. The best place
// would be the main.cpp 
// Example:
// #define DS_STRING_IMPLEMENTATION
// include <ds_string.h>
//
// author : amecky@gmail.com
// version: 1.0
// git    : https://github.com/amecky/ds_singleheader
// 
// Changelog:
//   - initial release
// -----------------------------------------------------------------
#define DS_STRING_IMPLEMENTATION

namespace ds {

	static const unsigned int SMALL_BUFFER_SIZE = 16;

	static const char NULL_CHAR = '\0';

	static const unsigned int FNV_Prime = 0x01000193; //   16777619
	static const unsigned int FNV_Seed = 0x811C9DC5; // 2166136261

	class string {

	public:

		typedef char* iterator;
		typedef const char* const_iterator;
		typedef char* pointer;
		typedef const char* const_pointer;
		string();
		string(const char* s);
		string(const string& other);
		~string();
		void append(char c);
		void append(int i);
		void append(float f);
		void append(double d);
		void append(const_pointer txt);
		void append(const string& txt);
		string sprintf(const char* fmt, ...);
		string& operator+=(const string& other);

		iterator begin() { return _first; }
		iterator end() { return _last; }
		const_iterator begin() const { return _first; }
		const_iterator end() const { return _last; }

		const char& operator [] (size_t n) const;
		const char& at(size_t n) const;

		const_iterator c_str() const;
		bool is_small() const;
		size_t size() const;
		size_t capacity() const;
		bool is_empty() const;

		void resize(size_t s);
		void reserve(size_t s);
		void clear();

		string& assign(const string& other); // FIXME
		string& insert(size_t pos, const string& str);
		string& insert(size_t pos, const char* str); 
		string& erase(size_t pos, size_t len = 0);
		string& replace(const char old_char, const char new_char, bool caseSensitive = true);

		bool find(const_pointer t, size_t offset = 0, size_t* index = 0) const;
		bool compare(const_pointer t, bool caseSensitive = true) const;
		bool compare(const string& t, bool caseSensitive = true) const;
		string substr(size_t start) const; 
		string substr(size_t start, size_t end) const;

		unsigned int hash_code(unsigned int hash = FNV_Seed) const;
	private:
		void append(const char* first, const char* last);
		size_t get_length(const char* t) const;
		pointer _first;
		pointer _last;
		pointer _capacity;
		char _buffer[SMALL_BUFFER_SIZE];
	};

	namespace str {

		inline string value_of(int i) {
			char tmp[16];
			sprintf_s(tmp, "%d", i);
			return string(tmp);
		}

		inline string value_of(float f) {
			char tmp[16];
			sprintf_s(tmp, "%g", f);
			return string(tmp);
		}

		
	}

#ifdef DS_STRING_IMPLEMENTATION

#include <stdarg.h>

	inline string::string() : _first(_buffer), _last(_buffer), _capacity(_buffer + SMALL_BUFFER_SIZE) {
	}

	inline string::string(const char* s) : _first(_buffer), _last(_buffer), _capacity(_buffer + SMALL_BUFFER_SIZE) {
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

	inline string::string(const string& s) : _first(_buffer), _last(_buffer), _capacity(_buffer + SMALL_BUFFER_SIZE) {
		size_t len = s.size();		
		append(s.c_str(), s.c_str() + len);
	}

	inline string::~string() {
		if (!is_small()) {
			delete[] _first;
		}
	}

	inline void string::clear() {
		_last = _first;
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

	inline void string::resize(size_t size) {
		reserve(size);
		for (pointer it = _last, end = _first + size + 1; it < end; ++it) {
			*it = 0;
		}
		_last = _first + size;
	}

	inline size_t string::get_length(const char* t) const {
		size_t len = 0;
		char temp = '\0';
		if (!t) {
			t = &temp;
		}
		for (const_iterator it = t; *it; ++it) {
			++len;
		}
		return len;
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

	inline string& string::operator+=(const string& other) {
		size_t len = other.size();
		append(other.c_str(), other.c_str() + len);
		return *this;
	}

	inline void string::append(const string& t) {
		size_t len = t.size();
		append(t.c_str(), t.c_str() + len);
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

	inline void string::append(double d) {
		char temp[16];
		sprintf_s(temp, "%g", d);
		append(temp);
	}

	inline string string::sprintf(const char* fmt, ...) {
		size_t fs = get_length(fmt);
		size_t size = fs * 2 + 50;   
		string str;
		va_list ap;
		while (1) {     
			str.resize(size);
			va_start(ap, fmt);
			int n = vsnprintf((char *)str.c_str(), size, fmt, ap);
			va_end(ap);
			if (n > -1 && n < size) {  
				str.resize(n);
				append(str);
				break;
			}
			if (n > -1) {
				size = n + 1;
			}
			else {
				size *= 2;
			}
		}
		return str;
	}

	inline bool string::find(const_pointer t, size_t offset, size_t* index) const {
		size_t tl = get_length(t);
		size_t cl = size();
		if (tl > cl) {
			return false;
		}	
		char first = t[0];
		for (size_t i = offset; i <= cl - tl; ++i) {
			char current = _first[i];
			bool found = true;
			if (current == first) {
				for (unsigned j = 1; j < tl; ++j) {
					current = _first[i + j];
					char d = t[j];
					if (current != d) {
						found = false;
						break;
					}
				}
				if (found) {
					if (index != 0) {
						*index = i;
					}
					return true;
				}
			}
		}
		return false;
	}

	inline bool string::compare(const_pointer t, bool caseSensitive) const {
		size_t tl = get_length(t);
		if (tl != size()) {
			return false;
		}
		for (size_t i = 0; i < tl; ++i) {
			if (caseSensitive) {
				if (tolower(_first[i]) != tolower(t[i])) {
					return false;
				}
			}
			else {
				if (_first[i] != t[i]) {
					return false;
				}
			}
		}
		return true;
	}

	inline bool string::compare(const string& t, bool caseSensitive) const {
		return compare(t.c_str(), caseSensitive);
	}

	inline unsigned int string::hash_code(unsigned int hash) const {
		const unsigned char* ptr = (const unsigned char*)_first;
		while (*ptr) {
			hash = (*ptr++ ^ hash) * FNV_Prime;
		}
		return hash;
	}

	inline string string::substr(size_t start, size_t end) const {
		string str;
		if (start < size()) {
			if (end > size()) {
				end = size();
			}
			size_t delta = end - start + 1;
			str.reserve(delta);
			str.append(_first + start, _first + start + delta);
		}
		return str;
	}

	inline string string::substr(size_t start) const {
		return substr(start, size());
	}

	string& string::replace(const char old_char, const char new_char, bool caseSensitive) {
		for (pointer it = _first; it != _last; ++it) {
			if (!caseSensitive) {
				if (tolower(*it) == tolower(old_char)) {
					*it = new_char;
				}
			}
			if (*it == old_char) {
				*it = new_char;
			}
		}
		return *this;
	}

	inline string& string::erase(size_t pos, size_t len) {
		if (pos < size()) {
			size_t e = pos + len + 1;
			if (e > size()) {
				e = size();
			}
			pointer newit = _first + e;
			pointer end = _last;
			for (pointer it = _first + pos; it != end; ++it, ++newit) {
				*it = *newit;
			}
			--_last;
			*_last = '\0';
		}
		return *this;
	}

	string& string::insert(size_t pos, const string& str) {
		size_t l = str.size();
		size_t e = size();
		if (pos > e) {
			append(str);
		}
		else {
			resize(size() + l);
			size_t delta = e - pos;
			pointer it = _first + pos;
			pointer next = _first + pos + l;
			for (size_t i = delta; i > 0; --i) {
				_first[pos + l + i - 1] = _first[pos + i - 1];
			}
			for (size_t i = 0; i < l; ++i) {
				_first[pos + i] = str[i];
				++_last;
			}
			*_last = '\0';
		}
		return *this;
	}

	string& string::insert(size_t pos, const char* str) {
		size_t l = strlen(str);
		size_t e = size();
		if (pos > e) {
			append(str);
		}
		else {
			resize(size() + l);
			size_t delta = e - pos;
			pointer it = _first + pos;
			pointer next = _first + pos + l;
			for (size_t i = delta; i > 0; --i) {
				_first[pos + l + i - 1] = _first[pos + i - 1];
			}
			for (size_t i = 0; i < l; ++i) {
				_first[pos + i] = str[i];
				++_last;
			}
			*_last = '\0';
		}
		return *this;
	}

	string& string::assign(const string& other) {
		clear();
		resize(other.size());
		append(other);
		return *this;
	}

#endif
}