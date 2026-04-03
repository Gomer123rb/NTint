def c1, 1.50000000, 4.00000000, 0.15915491, -0.25000000
def c2, 0.00000000, 0.50000000, 1.00000000, -1.00000000
def c3, 24.98080063, -24.98080063, -60.14580917, 60.14580917
def c4, 85.45378876, -85.45378876, -64.93934631, 64.93934631
def c5, 19.73921013, -19.73921013, -9.00000000, 0.75000000
def c6, 0.25000000, 1.94999993, 2.79999995, 0.15000001
mov r0.y, c0.x
mov r0.x, c0
mul r0.y, c6, r0
mul r0.x, c1, r0
mad r0.y, v0.z, c6.z, r0
mul r0.y, r0, c1.z
frc r1.y, r0
mov r1.w, r1.y
mad r0.x, v0, c1.y, r0
mad r0.x, r0, c1.z, c1.w
frc r0.y, r0.x
mov r0.w, r0.y
add r0.xyz, -r1.w, c2
mul r0.xyz, r0, r0
mad r3.xyz, r0, c3.xyxw, c3.zwzw
mad r3.xyz, r3, r0, c4.xyxw
mad r3.xyz, r3, r0, c4.zwzw
mad r3.xyz, r3, r0, c5.xyxw
add r1.xyz, -r0.w, c2
mul r1.xyz, r1, r1
mad r2.xyz, r1, c3.xyxw, c3.zwzw
mad r2.xyz, r2, r1, c4.xyxw
mad r2.xyz, r2, r1, c4.zwzw
mad r2.xyz, r2, r1, c5.xyxw
mad r3.xyz, r3, r0, c2.wzww
slt r4.x, r1.w, c6
sge r4.yz, r1.w, c5.xzww
mov r0.xz, r4
dp3 r0.y, r4, c2.wzww
dp3 r1.w, r3, -r0
sge r0.yz, r0.w, c5.xzww
mad r1.xyz, r2, r1, c2.wzww
slt r0.x, r0.w, c6
dp3 r2.y, r0, c2.wzww
mov r2.xz, r0
dp3 r0.x, r1, -r2
add r0.y, r0.x, r1.w
mov r0.xz, v0
mad r0.y, r0, c6.w, v0
mov oPos.xyz, r0
mov oT1.xyz, r0
mov oD0, v5
mov oT2.xyz, c2.xzxw
mov oT0.xy, v7
mov oPos.w, c2.z