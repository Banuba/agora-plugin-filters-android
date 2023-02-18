/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <bnb/utils/defs.hpp>
#include <cstdint>
#include <memory>
#include <string>

namespace bnb { namespace interfaces {

class mesh;

/**Class, represents face resources */
class BNB_EXPORT face {
public:
    virtual ~face() {}

    /**@return face name (string) */
    virtual std::string get_name() const = 0;

    /**@return face index (int) */
    virtual int32_t get_index() const = 0;

    /**
     *sets face index for face. Face mesh and textures will be updated according their faces index. 
     *If index will be greater than max faces can be found face resources update will be ignored.
     *@param index (int): face index
     */
    virtual void set_index(int32_t index) = 0;

    /**
     *sets face mesh. Must be only "$builtin$meshes/face.stream" mesh.
     *@param mesh (mesh): face mesh
     */
    virtual void set_face_mesh(const std::shared_ptr<mesh> & mesh) = 0;

    /**@return face mesh (mesh) */
    virtual std::shared_ptr<mesh> get_face_mesh() = 0;
};

} }  // namespace bnb::interfaces
/// @}

