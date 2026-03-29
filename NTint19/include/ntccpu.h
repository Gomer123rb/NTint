#ifndef NTCCPU_H
#define NTCCPU_H

#include <DirectXMath.h>
#include <vector>

namespace boff {
    class cpurender {
    public:
        static void cpuRender(float* xrender, float* yrender, float* zrender);
        static void boffdenp(DirectX::XMFLOAT4X3A* hipcup);
    };
}

#endif // NTCCPU_H