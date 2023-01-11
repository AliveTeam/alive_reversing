glslc ../Source/relive_lib/Renderer/Vulkan/shader.frag -o ./frag.spv
glslc ../Source/relive_lib/Renderer/Vulkan/shader.vert -o ./vert.spv

python bin2header.py -i frag.spv -o ../Source/relive_lib/Renderer/Vulkan/fragment_shader.h -v fragment_shader
python bin2header.py -i vert.spv -o ../Source/relive_lib/Renderer/Vulkan/vertex_shader.h -v vertex_shader
