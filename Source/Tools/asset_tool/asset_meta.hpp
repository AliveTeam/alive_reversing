#include <jsonxx/jsonxx.h>

#include <fstream>
#include <iostream>

struct MetaFrameInfo
{
    int original_width; // original width of the frames without padding
    int original_height; // original height of the frames without padding and not multiplied by 2
    int x_offset;
    int y_offset;
};

class AssetMeta
{
public:
    // extra info to make porting from this engine to the new engine easier
    std::string game;
    int padding;
    std::vector<MetaFrameInfo> metaFrameInfo;

    int size_width;
    int size_height;
    int offset_x;
    int offset_y;
    int frame_count;
    
    bool LoadJSONFromFile(const std::string& path)
    {
        std::ifstream file;
        file.open(path);

        if (!file.good())
        {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        std::string json = buffer.str();

        jsonxx::Object metaData;
        metaData.parse(json);

        jsonxx::Object metaOffset = metaData.get<jsonxx::Object>("offset");
        jsonxx::Object metaSize = metaData.get<jsonxx::Object>("size");

        offset_x = (int)metaOffset.get<jsonxx::Number>("x");
        offset_y = (int)metaOffset.get<jsonxx::Number>("y");

        size_width = (int)metaSize.get<jsonxx::Number>("w");
        size_height = (int)metaSize.get<jsonxx::Number>("h");

        frame_count = (int)metaData.get<jsonxx::Number>("frame_count");

        return true;
    }

    void SaveJSONToFile(const std::string& path, bool writeExtraData = false)
    {
        jsonxx::Object metaData_offset;
        metaData_offset << "x" << offset_x;
        metaData_offset << "y" << offset_y;

        jsonxx::Object metaData_originalSize;
        metaData_originalSize << "w" << size_width;
        metaData_originalSize << "h" << size_height;

        jsonxx::Object metaData;

        metaData << "offset" << metaData_offset;
        metaData << "size" << metaData_originalSize;

        metaData << "frame_count" << frame_count;

        if (writeExtraData)
        {
            jsonxx::Object extra;
            jsonxx::Array frameInfo;

            extra << "game" << game;
            extra << "padding" << padding;

            for (int i = 0; i < frame_count; i++)
            {
                jsonxx::Object info;
                info << "original_width" << metaFrameInfo.at(i).original_width;
                info << "original_height" << metaFrameInfo.at(i).original_height;
                info << "x_offset" << metaFrameInfo.at(i).x_offset;
                info << "y_offset" << metaFrameInfo.at(i).y_offset;
                frameInfo.append(info);
            }

            extra << "frames_info" << frameInfo;
            metaData << "extra" << extra;
        }

        std::ofstream file;
        file.open(path);
        file << metaData.json();
        file.close();
    }
};