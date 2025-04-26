#include "../Public/ShaderLib.h"

void ShaderLib::init()
{
        std::ifstream ShaderList;
        std::string line;
        ShaderType type = ShaderType::NONE;
        std::string vs;
        std::string fs;
        //0 = Top JSON level - searching for shader record
        //1 = Middle JSON level - searching for field inside record
        //2 = Low JSON level - getting value of field
        int SearchMode = 0;
        
        ShaderList.open("shaders/ShaderList.json");

        while(!ShaderList.eof())
        {
            line.clear();
            std::getline(ShaderList, line);

            if(line.find('\"') != std::string::npos && SearchMode == 0)
            {
                SearchMode = 1;
                std::getline(ShaderList, line); // extracting opening bracket
                type = ShaderType::NONE;
                vs.clear();
                fs.clear();
                continue;
            }
            //finding fields and extracting values
            if(line.find("index") != std::string::npos && SearchMode == 1)
            {
                SearchMode = 2;
                const size_t pos = line.find(':') + 2;
                std::string value = line.substr(pos, 1);
                type = static_cast<ShaderType>(std::atoi(value.data()));
            }
            if(line.find("vsPath") != std::string::npos && SearchMode == 2)
            {
                const size_t pos = line.find(':') + 3;
                vs = line.substr(pos);
                vs.pop_back(); //remove comma
                vs.pop_back(); //remove quotation mark
            }
            if(line.find("fsPath") != std::string::npos && SearchMode == 2)
            {
                const size_t pos = line.find(':') + 3;
                fs = line.substr(pos);
                fs.pop_back(); //remove quotation mark
                SearchMode = 0;
            }

            //pushing new shader record to map, once the record is complete
            if(type != ShaderType::NONE &&
               !vs.empty() &&
               !fs.empty())
            {
                m_shaderLibrary.emplace(
                    type,
                    std::pair<const std::string,const std::string>
                    {vs, fs}
                    );
                type = ShaderType::NONE;
                vs.clear();
                fs.clear();
            }
        }// end of file loop
        ShaderList.close();
}