"C:\Program Files (x86)\Microsoft DirectX SDK (August 2009)\Utilities\bin\x86\fxc.exe" /Fo pixel_shader.bin /Ges -T ps_2_0 -E PS ..\Source\AliveLibAE\Renderer\pixel_shader.hlsl
python bin2header.py -i pixel_shader.bin -o ..\Source\AliveLibAE\Renderer\pixel_shader.h -v pixel_shader
pause
