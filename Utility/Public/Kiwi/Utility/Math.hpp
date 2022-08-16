#ifndef KIWI_UTILITY_Math_HPP
#define KIWI_UTILITY_Math_HPP

#include <Kiwi/Utility/Config.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RIGHT_HANDED
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdint>
#include <limits>

namespace kiwi {

typedef float float32_t;
typedef double float64_t;

using Quat = glm::qua<float, glm::packed>;

using Mat2 = glm::mat<2, 2, float, glm::packed>;
using Mat3 = glm::mat<3, 3, float, glm::packed>;
using Mat4 = glm::mat<4, 4, float, glm::packed>;

using Vec2 = glm::vec<2, float, glm::packed>;
using Vec3 = glm::vec<3, float, glm::packed>;
using Vec4 = glm::vec<4, float, glm::packed>;

using Vec2i = glm::vec<2, int32_t, glm::packed>;
using Vec3i = glm::vec<3, int32_t, glm::packed>;
using Vec4i = glm::vec<4, int32_t, glm::packed>;

using Vec2u = glm::vec<2, uint32_t, glm::packed>;
using Vec3u = glm::vec<3, uint32_t, glm::packed>;
using Vec4u = glm::vec<4, uint32_t, glm::packed>;

template <typename T>
constexpr float NormalizeInteger(T value)
{
    return (
        value < 0
        ? -static_cast<float>(value) / static_cast<float>(std::numeric_limits<T>::min())
        :  static_cast<float>(value) / static_cast<float>(std::numeric_limits<T>::min())
    );
}

} // namespace kiwi

#endif // KIWI_UTILITY_Math_HPP