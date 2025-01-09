#pragma once

#include <vector>
#include <memory>
#include <algorithm>

template<typename T>
class SecureMemory {
public:
    SecureMemory() = default;
    
    explicit SecureMemory(size_t size) : data_(size) {
        std::fill(data_.begin(), data_.end(), 0);
    }
    
    SecureMemory(const SecureMemory& other) : data_(other.data_) {}
    
    SecureMemory& operator=(const SecureMemory& other) {
        if (this != &other) {
            clear();
            data_ = other.data_;
        }
        return *this;
    }
    
    SecureMemory(SecureMemory&& other) noexcept : data_(std::move(other.data_)) {}
    
    SecureMemory& operator=(SecureMemory&& other) noexcept {
        if (this != &other) {
            clear();
            data_ = std::move(other.data_);
        }
        return *this;
    }
    
    ~SecureMemory() {
        clear();
    }
    
    void clear() {
        if (!data_.empty()) {
            volatile T* ptr = data_.data();
            for (size_t i = 0; i < data_.size(); ++i) {
                ptr[i] = 0;
            }
            data_.clear();
        }
    }
    
    void resize(size_t size) {
        clear();
        data_.resize(size);
        std::fill(data_.begin(), data_.end(), 0);
    }
    
    T* data() {
        return data_.data();
    }
    
    const T* data() const {
        return data_.data();
    }
    
    size_t size() const {
        return data_.size();
    }
    
    bool empty() const {
        return data_.empty();
    }
    
    T& operator[](size_t index) {
        return data_[index];
    }
    
    const T& operator[](size_t index) const {
        return data_[index];
    }
    
    bool operator==(const SecureMemory& other) const {
        return data_ == other.data_;
    }
    
    bool operator!=(const SecureMemory& other) const {
        return !(*this == other);
    }

private:
    std::vector<T> data_;
}
