#include <jsonxx/jsonxx.h>

#include <fstream>
#include <iostream>

class AssetMeta
{
public:
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

    void SaveJSONToFile(std::string path)
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

        std::ofstream file;
        file.open(path);
        file << metaData.json();
        file.close();
    }
};