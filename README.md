# LLPO
![Screenshot of application](https://github.com/IsaacDexter/LLPO/assets/90466022/46d7252d-7b63-4fae-b66c-880b6338e24f)
Optimization project for *Low-Level Platform Optimization* module.
In this project I worked low level to try to squeeze performance out of a provided OpenGL physics framework. Performance of the calculation was recorded. This was implemented through:
## Memory Management
I provided a memory management solution to improve the memory efficiency of the framework. This framework included:
- Memory integrity checking.
- Walking the heap.
- Memory pools.
- `new` and `delete` overrides.
## Multithreading
- Multiple asynchronous threads are managed with `std::barrier`.
- These handle the collision resolution.
- Thread count is modifiable.
## Collision Optimization
- Collision code was optimized to reduce expense.
- Oct-trees were researched but ultimately unimplemented.

# Summary
Overall, my Low-Level Platform Optimization project was a success, and I am very happy with the optimizations I completed. If I were to improve the project further, I would further augment the collision code with Oct/Quad-tress, and consider switching from `std::barrier` to `std::future` to handle asynchronous threads.
![Screenshot of console output](https://github.com/IsaacDexter/LLPO/assets/90466022/e6038ed3-4455-424b-9c9c-13616c3181c5)

___

# PlayStation 4 Port
An additional port of the project was made to PS4 Architecture, using the PS4 SDK. This involved rewriting the graphics and physics code to make the most of PS4's Graphics API. Unfortunately, this code can not be shared as a result of NDA. Regardless, the port went well. The code functioned identically to the OpenGL version, although I was unable to port all of the optimizations, such as multithreading due to the limited timeframe of the project, as well as incompatibilities with `std::barrier` on PS4 SDK. Regardless, I am still very proud of my work.
![GIF of PS4 Port](https://github.com/IsaacDexter/LLPO/assets/90466022/78b0fe53-a8d3-45bf-9d65-a48909de6092)

