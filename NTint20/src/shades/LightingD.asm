def c2, 0.00000000, 1.00000000, 0.45454544, 0
dp3 r0.x, c0, c0
rsq r0.x, r0.x
mul r0.x, r0, c0.y
max r0.x, r0, c2
mul r0.xyz, r0.x, c1
mul r0.xyz, v5, r0
min r0.xyz, r0, c2.y
max r0.xyw, r0.xyzz, c2.x
mov r1.xy, r0.x
mov r1.z, c2
lit r0.z, r1.xyzz
mov r1.xy, r0.y
mov r1.z, c2
lit r1.z, r1.xyzz
mov oD0.x, r0.z
mov r0.z, c2
mov r0.xy, r0.w
mov oPos, v0
mov oD0.y, r1.z
lit oD0.z, r0.xyzz
mov oT1.xyz, v0
mov oT2.xyz, c2.xyxw
mov oT0.xy, v7
mov oD0.w, c2.y