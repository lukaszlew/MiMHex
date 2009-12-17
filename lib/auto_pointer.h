#ifndef AUTO_POINTER_H
#define AUTO_POINTER_H

template <typename T>
class AutoPointer {
public:
	AutoPointer() : _ptr(NULL) { }
	AutoPointer(T* ptr) : _ptr(ptr) { }
	~AutoPointer() { delete _ptr; }

	AutoPointer& operator=(T* ptr) {
		if (ptr == _ptr)
			return *this;
		delete _ptr;
		_ptr = ptr;
		return *this;
	}
	T& operator*() { return *_ptr; }
	T* operator->() { return _ptr; }
	bool operator==(T* ptr) { return ptr == _ptr; }
	bool operator!=(T* ptr) { return ptr != _ptr; }

	T* GetPointer() { return _ptr; }
	T* Detach() {
		T* ptr = _ptr;
		_ptr = NULL;
		return ptr;
	}
private:
	T* _ptr;
};

template <typename T>
class AutoTable {
public:
	AutoTable() : _ptr(NULL) { }
	AutoTable(T* ptr) : _ptr(ptr) { }
	~AutoTable() { delete[] _ptr; }

	AutoTable& operator=(T* ptr) {
		if (ptr == _ptr)
			return *this;
		delete[] _ptr;
		_ptr = ptr;
		return *this;
	}
	T& operator*() { return *_ptr; }
	T* operator->() { return _ptr; }
	T& operator[](int idx) { return _ptr[idx]; }
	bool operator==(T* ptr) { return ptr == _ptr; }
	bool operator!=(T* ptr) { return ptr != _ptr; }

	T* GetPointer() { return _ptr; }
	T* Detach() {
		T* ptr = _ptr;
		_ptr = NULL;
		return ptr;
	}
private:
	T* _ptr;
};

#endif
