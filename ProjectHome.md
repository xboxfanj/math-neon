This project aims to implement high performance approximations of higher order math functions (trig, exponential, powers, etc) in C and hand optimized assembly. Floating Point performance on ARM Cortex A8 equipped devices (OpenPandora, Beagle Board, iPhone 3GS, Palm Pre, etc) is heavily dependent on utilizing the pipelined NEON VFPU. By default GCC targets the fully IEEE754 compliant VFP-lite, which is an order of magnitude slower. Support for the NEON unit in GCC is limited, so hand written assembly will be required.

To determine if this project was worthwhile i investigated a cmath implementation as compiled by Code Sourcery 2009q1. I noticed the following problems: <br>
<b>1.</b> Depends on many floating point branches. Even simple things like "if (x < 0) y = -y;" are generating branches. -> Requires a 20 cycle stall. <br>
<b>2.</b> All integer work on floating point data is being done on the ARM. -> Requires a 20 cycle stall. <br>
<b>3.</b> Only VFP instructions are being produced / No vectorization. -> Slower than NEON. <br>
<b>4.</b> Overly robust, ie floorf() uses complex ARM based routine instead of VFP/NEON float->integer->float conversion -> Very slow.