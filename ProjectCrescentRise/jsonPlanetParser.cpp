#include "jsonPlanetParser.h"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

bool jsonPlanetParser::WritePlanetData(PlanetData t_newData)
{
    bool dataAvailable = false;
    std::string fileLoc = "./Assets/Data/planetSettings.json";

    std::ifstream readFile(fileLoc);
    if (!readFile)
    {
        std::cout << "NO FILE" << std::endl;
    }
    else
    {
        dataAvailable = true;
    }
    json loadedFile;
    readFile >> loadedFile;

    readFile.close();


    json j;
    j["project"] = "json_demo";
    j["version"] = 1.0;

    if(dataAvailable && !loadedFile.empty())
        for (auto& [key, value] : loadedFile.items()) {
            j[key] = value;
        }

    std::string pretty = j.dump(4);

    std::cout << "Entry Count : " << j.size() << "\n" << pretty << "\n";

    std::ofstream writeFile(fileLoc);
    if (!writeFile)
    {
        std::cout << "NO FILE" << std::endl;
    }
    writeFile << pretty;
    writeFile.close();
    return true;
}
