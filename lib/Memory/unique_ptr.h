#pragma once

namespace arduino
{
namespace memory
{
template <class T>
class unique_ptr
{
public:
	unique_ptr() : ptr(nullptr) 
	{
	}

	unique_ptr(T * ptr) : ptr(ptr)
	{
	}

	unique_ptr(const unique_ptr & obj) = delete; 
	unique_ptr& operator=(const unique_ptr & obj) = delete; 

	unique_ptr(unique_ptr && dyingObj) 
	{
		this->ptr = dyingObj.ptr;
		dyingObj.ptr = nullptr; 
	}

	void operator=(unique_ptr && dyingObj)
	{
		__cleanup__();

		this->ptr = dyingObj.ptr;
		dyingObj.ptr = nullptr;
	}

	T* operator->()
	{
		return this->ptr;
	}

    T* operator->() const
	{
		return const_cast<unique_ptr<T> *>(this)->operator->();
	}

	T& operator*()
	{
		return *(this->ptr);
	}

    T& operator*() const
	{
        return const_cast<unique_ptr<T> *>(this)->operator*();
	}

	~unique_ptr()
	{
		__cleanup__();
	}

private:
	T * ptr = nullptr;

	void __cleanup__()
	{
		if (ptr != nullptr)
			delete ptr;
	}
};

template <class T>
class unique_ptr<T[]>
{
public:
	unique_ptr() : ptr(nullptr)
	{
	}

	unique_ptr(T * ptr) : ptr(ptr)
	{
	}

	unique_ptr(const unique_ptr & obj) = delete; 
	unique_ptr& operator=(const unique_ptr & obj) = delete;

	unique_ptr(unique_ptr && dyingObj)
	{
		__cleanup__();

		this->ptr = dyingObj.ptr;
		dyingObj.ptr = nullptr;
	}

	void operator=(unique_ptr && dyingObj)
	{
		__cleanup__();

		this->ptr = dyingObj.ptr;
		dyingObj.ptr = nullptr;
	}

	T* operator->()
	{
		return this->ptr;
	}

    T* operator->() const
	{
		return const_cast<unique_ptr<T> *>(this)->operator->();
	}

	T& operator*()
	{
		return *(this->ptr);
	}

    T& operator*() const
	{
		return const_cast<unique_ptr<T> *>(this)->operator*();
	}

	T& operator[](int index)
	{
		if(index < 0)
		{
            return nullptr; // can't throw exceptions in embeded systems
		}
		return this->ptr[index];
	}

    T& operator[](int index) const
	{
        return const_cast<unique_ptr<T> *>(this)[index];
	}

	~unique_ptr()
	{
		__cleanup__();
	}

private:
	T * ptr = nullptr;
	void __cleanup__()
	{
		if (ptr != nullptr)
			delete[] ptr;
	}
};
} // namespace memory
} // namespace arduino

