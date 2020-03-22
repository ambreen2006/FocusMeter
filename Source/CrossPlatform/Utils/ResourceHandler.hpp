//
//  ResourceHandler.hpp
//  focusmeter
//
//  Created by Ambreen Haleem on 3/19/20.
//  Copyright © 2020 Ambreen Haleem. All rights reserved.
//

#ifndef ResourceHandler_h
#define ResourceHandler_h

template <typename T>
struct ResourceHandleTraits
{
    using Type = T;
    static Type Invalid() noexcept {
        return nullptr;
    }
};

template <typename Traits>
class ResourceHandle
{
    using Type = decltype(Traits::Invalid());
    Type m_value;
    void Close() noexcept {
        Traits::Close(m_value);
    }

public:

    ResourceHandle(ResourceHandle const &) = delete;
    ResourceHandle & operator=(ResourceHandle const &) = delete;

    explicit ResourceHandle(Type value = Traits::Invalid()) noexcept : m_value (value) {}

    Type Get() const noexcept {
        return m_value;
    }
    
    ~ResourceHandle() {
        Close();
    }
    
    Type * Set() noexcept {
        return &m_value;
    }

    Type Detach() noexcept {
        Type value = m_value;
        m_value = Traits::Invalid();
        return value;
    }
    
    bool Reset(Type value = Traits::Invalid()) noexcept {
        if (m_value != value) {
            Close();
            m_value = value;
        }
    }
    
    ResourceHandle& operator=(ResourceHandle &&other) noexcept {
        if (this != &other) {
            Reset(other.Detach());
        }
        return *this;
    }
};

template <typename Traits>
bool operator==(ResourceHandle<Traits> const& left, ResourceHandle<Traits> const& right) noexcept {
    return left.Get() == right.Get();
}

template <typename Traits>
bool operator!=(ResourceHandle<Traits> const& left, ResourceHandle<Traits> const& right) noexcept {
    return !(left.Get() == right.Get());
}

#endif /* ResourceHandler_h */
