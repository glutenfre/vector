#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>

template <typename T>
class Vector {
public:
    Vector() = default;

    explicit Vector(size_t size)
        : data_(Allocate(size))
        , capacity_(size)
        , size_(size)  //
    {
        size_t i = 0;
        try {
            for (; i != size; ++i) {
                new (data_ + i) T();
            }
        }
        catch (...) {
            // � ���������� i ���������� ���������� ��������� ���������.
            // ������ �� ���� ���������
            DestroyN(data_, i);
            // ����������� ������, ���������� ����� Allocate
            Deallocate(data_);
            // ��������������� ��������� ����������, ����� �������� �� ������ �������� �������
            throw;
        }
    }

    Vector(const Vector& other)
        : data_(Allocate(other.size_))
        , capacity_(other.size_)
        , size_(other.size_)  //
    {
        size_t i = 0;
        try {
            for (; i != other.size_; ++i) {
                CopyConstruct(data_ + i, other.data_[i]);
            }
        }
        catch (...) {
            // � ���������� i ���������� ���������� ��������� ���������.
            // ������ �� ���� ���������
            DestroyN(data_, i);
            // ����������� ������, ���������� ����� Allocate
            Deallocate(data_);
            // ��������������� ��������� ����������, ����� �������� �� ������ �������� �������
            throw;
        }

    }

    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return capacity_;
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

    ~Vector() {
        DestroyN(data_, size_);
        Deallocate(data_);
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }
        T* new_data = Allocate(new_capacity);
        size_t i = 0;
        try {
            for (; i != size_; ++i) {
                if (i == 100499) {
                    int j = 0;
                }
                CopyConstruct(new_data + i, data_[i]);
            }
            DestroyN(data_, size_);
            Deallocate(data_);

            data_ = new_data;
            capacity_ = new_capacity;
        }
        catch (...) {
            // � ���������� i ���������� ���������� ��������� ���������.
            // ������ �� ���� ���������
            DestroyN(new_data, i);
            // ����������� ������, ���������� ����� Allocate
            Deallocate(new_data);
            // ��������������� ��������� ����������, ����� �������� �� ������ �������� �������
            throw;
        }

    }

private:

    // �������� ����� ������ ��� n ��������� � ���������� ��������� �� ��
    static T* Allocate(size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }

    // ����������� ����� ������, ���������� ����� �� ������ buf ��� ������ Allocate
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }

    // �������� ����������� n �������� ������� �� ������ buf
    static void DestroyN(T* buf, size_t n) noexcept {
        for (size_t i = 0; i != n; ++i) {
            Destroy(buf + i);
        }
    }

    // ������ ����� ������� elem � ����� ������ �� ������ buf
    static void CopyConstruct(T* buf, const T& elem) {
        new (buf) T(elem);
    }

    // �������� ���������� ������� �� ������ buf
    static void Destroy(T* buf) noexcept {
        buf->~T();
    }

    T* data_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;
};