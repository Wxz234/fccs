#pragma once
#define FCCS_API
#include <type_traits>

#define FCCS_BITMASK_OPS(_BITMASK)																					\
[[nodiscard]] constexpr _BITMASK operator&(_BITMASK _Left, _BITMASK _Right) noexcept {								\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) & static_cast<_IntTy>(_Right));							\
}																													\
																													\
[[nodiscard]] constexpr _BITMASK operator|(_BITMASK _Left, _BITMASK _Right) noexcept { /* return _Left | _Right */	\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) | static_cast<_IntTy>(_Right));							\
}																													\
																													\
[[nodiscard]] constexpr _BITMASK operator^(_BITMASK _Left, _BITMASK _Right) noexcept { /* return _Left ^ _Right */	\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) ^ static_cast<_IntTy>(_Right));							\
}																													\
																													\
constexpr _BITMASK& operator&=(_BITMASK& _Left, _BITMASK _Right) noexcept { /* return _Left &= _Right */			\
    return _Left = _Left & _Right;																					\
}																													\
																													\
constexpr _BITMASK& operator|=(_BITMASK& _Left, _BITMASK _Right) noexcept { /* return _Left |= _Right */			\
    return _Left = _Left | _Right;																					\
}																													\
																													\
constexpr _BITMASK& operator^=(_BITMASK& _Left, _BITMASK _Right) noexcept { /* return _Left ^= _Right */			\
    return _Left = _Left ^ _Right;																					\
}																													\
																													\
[[nodiscard]] constexpr _BITMASK operator~(_BITMASK _Left) noexcept { /* return ~_Left */							\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(~static_cast<_IntTy>(_Left));														\
} 

#define FCCSGLOBALCONST constexpr

namespace fccs {
    class IResource {
    public:
        virtual ~IResource();
    };
    FCCS_API void DestroyResource(IResource* pResource);

    struct Color
    {
        float r, g, b, a;

        Color() : r(0.f), g(0.f), b(0.f), a(0.f) { }
        Color(float c) : r(c), g(c), b(c), a(c) { }
        Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) { }

        bool operator ==(const Color& _b) const { return r == _b.r && g == _b.g && b == _b.b && a == _b.a; }
        bool operator !=(const Color& _b) const { return !(*this == _b); }
    };
}