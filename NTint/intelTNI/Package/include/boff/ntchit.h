#ifndef NTCHIT_H
#define NTCHIT_H

#include <DirectXMath.h>

namespace boff {
    struct NTCHit {
        DirectX::XMFLOAT3 position; 
        DirectX::XMFLOAT3 normal;   
        float distance;             
        int materialID;            
        NTCHit()
            : position{0.0f, 0.0f, 0.0f}, normal{0.0f, 0.0f, 0.0f}, distance(0.0f), materialID(-1) {}
    };
}

#endif // NTCHIT_H