#pragma once

namespace Editor
{

struct RenderState
{
    inline static bool IsGrid = false;
    inline static bool IsCollider = false;
    inline static bool IsTexture = true;

    inline static bool SnapToGrid = false;

    RenderState() = delete;
};

}
