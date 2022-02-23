struct CustomRenderSpriteFormat
{
    void* originalPoly;
    int x;
    int y;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    float scale;
    bool flip;
    void* origPtr;
    int resource_id;
    int frametable_offset;
    int frame;
    bool emissive;
};

CustomRenderSpriteFormat* CustomRender_GetCommand(void* ptr);
void CustomRender_AddCommand(void* ptr, CustomRenderSpriteFormat command);
void CustomRender_RemoveCommand(void* ptr);
void CustomRender_ClearCommands();