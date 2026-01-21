#include "jsonPlanetParser.h"
#include <fstream>

const std::string jsonPlanetParser::s_fileLocation = "./Assets/Data/planetSettings.json";
int jsonPlanetParser::s_chosenPlanet = 0;
int jsonPlanetParser::s_amountOfPlanets = 0;

PlanetData jsonPlanetParser::RetrieveData(bool newRandom)
{
    json j = readPlanetFile();

    if (newRandom)
    {
        if(s_amountOfPlanets > 0)
            s_chosenPlanet = rand() % (s_amountOfPlanets);
    }

    return PlanetData::jsonToPlanetData(j);
}

bool jsonPlanetParser::WritePlanetData(PlanetData t_newData, bool saveAsNew)
{
    if (saveAsNew)
    {
        s_chosenPlanet = s_amountOfPlanets;
    }
    else
    {
        // do nothing and instead use the current selected planet
    }
    json j = t_newData.planetDataToJson();
    json loaded = readPlanetFile();
    MergeFiles(loaded, j);

    return dumpFile(loaded);

}

json jsonPlanetParser::readPlanetFile()
{
    std::ifstream readFile(s_fileLocation);
    if (!readFile)
    {
        std::cout << "NO FILE" << std::endl;
        return json();
    }
    json loadedFile;
    readFile >> loadedFile;

    readFile.close();

    s_amountOfPlanets = loadedFile.size();

    return loadedFile;
}

void jsonPlanetParser::MergeFiles(json& base, json mergeFile)
{
    if (!mergeFile.empty())
        for (auto& [key, value] : mergeFile.items()) {
            base[key] = value;
        }
}

// dump file
bool jsonPlanetParser::dumpFile(const json& dumpFile)
{
    std::string pretty = dumpFile.dump(4);

    std::cout << "Entry Count : " << dumpFile.size() << "\n" << pretty << "\n";

    std::ofstream writeFile(s_fileLocation);
    if (writeFile.fail())
    {
        std::cout << "WRITE FILE COULD NOT BE LOADED" << std::endl;
        return false;
    }

    writeFile << pretty;
    writeFile.close();

    return true;
}

json PlanetData::planetDataToJson()
{
    std::string pos = std::to_string(jsonPlanetParser::s_chosenPlanet);
    json j;
    j[pos]["name"] = "First";
    j[pos]["planetRad"] = planetShape.planetRadius;
    j[pos]["oceanRad"] = oceanShapeSettings.planetRadius;
    j[pos]["planetPoints"] = planetPointCount;
    j[pos]["oceanPoints"] = oceanPointCount;

    for (int i = 0; i < planetShape.noiseLayers.size(); i++)
    {
        auto& layer = planetShape.noiseLayers.at(i);
        auto& setting = layer.noiseSettings;
        std::string posString = std::to_string(i);
        j[pos]["NoiseLayers"][posString]["enabled"] = layer.enabled;
        j[pos]["NoiseLayers"][posString]["mask"] = layer.useAsMask;
        j[pos]["NoiseLayers"][posString]["settings"]["roughness"] = setting.baseRoughness;
        j[pos]["NoiseLayers"][posString]["settings"]["center"]["x"] = setting.center.x;
        j[pos]["NoiseLayers"][posString]["settings"]["center"]["y"] = setting.center.y;
        j[pos]["NoiseLayers"][posString]["settings"]["center"]["z"] = setting.center.z;
        j[pos]["NoiseLayers"][posString]["settings"]["filterType"] = setting.filterType;
        j[pos]["NoiseLayers"][posString]["settings"]["minValue"] = setting.minValue;
        j[pos]["NoiseLayers"][posString]["settings"]["numLayers"] = setting.numLayers;
        j[pos]["NoiseLayers"][posString]["settings"]["persistence"] = setting.persistence;
        j[pos]["NoiseLayers"][posString]["settings"]["strength"] = setting.strength;
        j[pos]["NoiseLayers"][posString]["settings"]["weight"] = setting.weightMultiplier;
    }

    j[pos]["planetColour"]["lightIntensity"] = planetColour.lightIntensity;
    for (int i = 0; i < planetColour.COLOUR_MAX; i++)
    {
        std::string posString = std::to_string(i);
        j[pos]["planetColour"][posString]["shaderType"] = planetColour.m_shaderType.at(i);
        j[pos]["planetColour"][posString]["active"] = planetColour.active.at(i);
        j[pos]["planetColour"][posString]["colour"]["r"] = planetColour.m_colours.at(i).r;
        j[pos]["planetColour"][posString]["colour"]["g"] = planetColour.m_colours.at(i).g;
        j[pos]["planetColour"][posString]["colour"]["b"] = planetColour.m_colours.at(i).b;
        j[pos]["planetColour"][posString]["height"] = planetColour.m_heights.at(i);
        j[pos]["planetColour"][posString]["normalLoc"] = planetColour.m_normalLocation.at(i);
        j[pos]["planetColour"][posString]["normalStrength"] = planetColour.m_normalStrength.at(i);
        j[pos]["planetColour"][posString]["textureLocation"] = planetColour.m_textureLocation.at(i);
        j[pos]["planetColour"][posString]["textureScale"] = planetColour.m_textureScale.at(i);
    }

    return j;
}

PlanetData PlanetData::jsonToPlanetData(const json& input)
{
    PlanetData p;

    if (!input.is_object() || input.empty())
    {
        std::cout << "[jsonToPlanetData] Input JSON is not an object or is empty\n";
        return p;
    }

    std::string chosenLoc = std::to_string(jsonPlanetParser::s_chosenPlanet);

    const json* root = nullptr;
    if (input.contains(chosenLoc) && input[chosenLoc].is_object())
    {
        root = &input[chosenLoc];
    }
    else
    {
        for (auto& [k, v] : input.items())
        {
            if (v.is_object())
            {
                root = &v;
                break;
            }
        }
    }

    if (!root)
    {
        std::cout << "[jsonToPlanetData] No suitable root object found in JSON\n";
        return p;
    }

    const json& j = *root;

    // Top-level scalar fields
    if (j.contains("planetRad"))
    {
        if (j["planetRad"].is_number())
            p.planetShape.planetRadius = j["planetRad"].get<decltype(p.planetShape.planetRadius)>();
        else
            std::cout << "[jsonToPlanetData] planetRad present but not a number\n";
    }

    if (j.contains("oceanRad"))
    {
        if (j["oceanRad"].is_number())
            p.oceanShapeSettings.planetRadius = j["oceanRad"].get<decltype(p.oceanShapeSettings.planetRadius)>();
        else
            std::cout << "[jsonToPlanetData] oceanRad present but not a number\n";
    }

    if (j.contains("planetPoints"))
    {
        if (j["planetPoints"].is_number_integer())
            p.planetPointCount = j["planetPoints"].get<decltype(p.planetPointCount)>();
        else
            std::cout << "[jsonToPlanetData] planetPoints present but not an integer\n";
    }

    if (j.contains("oceanPoints"))
    {
        if (j["oceanPoints"].is_number_integer())
            p.oceanPointCount = j["oceanPoints"].get<decltype(p.oceanPointCount)>();
        else
            std::cout << "[jsonToPlanetData] oceanPoints present but not an integer\n";
    }

    // Noise layers
    if (j.contains("NoiseLayers") && j["NoiseLayers"].is_object())
    {
        const json& nlObj = j["NoiseLayers"];
        for (auto& [idx, nlJson] : nlObj.items())
        {
            if (!nlJson.is_object())
            {
                std::cout << "[jsonToPlanetData] NoiseLayers." << idx << " is not an object, skipping\n";
                continue;
            }

            // parse index from key (keys are numeric strings in saved JSON)
            int layerIndex = -1;
            try
            {
                layerIndex = std::stoi(idx);
            }
            catch (...)
            {
                std::cout << "[jsonToPlanetData] NoiseLayers key '" << idx << "' is not a number, skipping\n";
                continue;
            }
            if (layerIndex < 0)
            {
                std::cout << "[jsonToPlanetData] NoiseLayers index negative, skipping\n";
                continue;
            }

            if (layerIndex >= static_cast<int>(p.planetShape.noiseLayers.size()))
                p.planetShape.noiseLayers.resize(layerIndex + 1);

            auto& layer = p.planetShape.noiseLayers.at(layerIndex);

            if (nlJson.contains("enabled")) layer.enabled = nlJson.value("enabled", layer.enabled);
            if (nlJson.contains("mask")) layer.useAsMask = nlJson.value("mask", layer.useAsMask);

            if (nlJson.contains("settings") && nlJson["settings"].is_object())
            {
                const json& s = nlJson["settings"];

                if (s.contains("center") && s["center"].is_object())
                {
                    const json& c = s["center"];
                    if (c.contains("x") && c["x"].is_number()) layer.noiseSettings.center.x = c["x"].get<decltype(layer.noiseSettings.center.x)>();
                    if (c.contains("y") && c["y"].is_number()) layer.noiseSettings.center.y = c["y"].get<decltype(layer.noiseSettings.center.y)>();
                    if (c.contains("z") && c["z"].is_number()) layer.noiseSettings.center.z = c["z"].get<decltype(layer.noiseSettings.center.z)>();
                }

                if (s.contains("filterType") && s["filterType"].is_number())
                    layer.noiseSettings.filterType = s["filterType"].get<decltype(layer.noiseSettings.filterType)>();
                if (s.contains("minValue") && s["minValue"].is_number())
                    layer.noiseSettings.minValue = s["minValue"].get<decltype(layer.noiseSettings.minValue)>();
                if (s.contains("numLayers") && s["numLayers"].is_number_integer())
                    layer.noiseSettings.numLayers = s["numLayers"].get<decltype(layer.noiseSettings.numLayers)>();
                if (s.contains("persistence") && s["persistence"].is_number())
                    layer.noiseSettings.persistence = s["persistence"].get<decltype(layer.noiseSettings.persistence)>();
                if (s.contains("roughness") && s["roughness"].is_number())
                    layer.noiseSettings.baseRoughness = s["roughness"].get<decltype(layer.noiseSettings.baseRoughness)>();
                if (s.contains("strength") && s["strength"].is_number())
                    layer.noiseSettings.strength = s["strength"].get<decltype(layer.noiseSettings.strength)>();
                if (s.contains("weight") && s["weight"].is_number())
                    layer.noiseSettings.weightMultiplier = s["weight"].get<decltype(layer.noiseSettings.weightMultiplier)>();
            }
            else
            {
                std::cout << "[jsonToPlanetData] NoiseLayers." << idx << " missing settings or not an object\n";
            }
        }
    }
    else if (j.contains("NoiseLayers"))
    {
        std::cout << "[jsonToPlanetData] NoiseLayers present but not an object\n";
    }

    if (j.contains("planetColour") && j["planetColour"].is_object())
    {
        const json& pc = j["planetColour"];
        if (pc.contains("lightIntensity") && pc["lightIntensity"].is_number())
            p.planetColour.lightIntensity = pc["lightIntensity"].get<decltype(p.planetColour.lightIntensity)>();
        else if (pc.contains("lightIntensity"))
            std::cout << "[jsonToPlanetData] planetColour.lightIntensity present but not a number\n";

        for (auto& [k, v] : pc.items())
        {
            if (k == "lightIntensity") continue;
            if (!v.is_object())
            {
                std::cout << "[jsonToPlanetData] planetColour." << k << " is not an object, skipping\n";
                continue;
            }

            // parse numeric colour index from key
            int colourIndex = -1;
            try
            {
                colourIndex = std::stoi(k);
            }
            catch (...)
            {
                std::cout << "[jsonToPlanetData] planetColour key '" << k << "' is not a number, skipping\n";
                continue;
            }
            if (colourIndex < 0)
            {
                std::cout << "[jsonToPlanetData] planetColour index negative, skipping\n";
                continue;
            }

            // ensure all vectors have space for this index
            auto ensureSize = [&](size_t needed) {
                if (colourIndex >= static_cast<int>(needed))
                {
                    // Resize all colour-related vectors to hold index
                    p.planetColour.m_shaderType.resize(colourIndex + 1);
                    p.planetColour.active.resize(colourIndex + 1);
                    p.planetColour.m_colours.resize(colourIndex + 1);
                    p.planetColour.m_heights.resize(colourIndex + 1);
                    p.planetColour.m_normalLocation.resize(colourIndex + 1);
                    p.planetColour.m_normalStrength.resize(colourIndex + 1);
                    p.planetColour.m_textureLocation.resize(colourIndex + 1);
                    p.planetColour.m_textureScale.resize(colourIndex + 1);
                }
            };
            ensureSize(p.planetColour.m_shaderType.size());

            // assign values using .at(index) to preserve numeric keys' positions
            if (v.contains("shaderType") && v["shaderType"].is_number_integer())
                p.planetColour.m_shaderType.at(colourIndex) = v["shaderType"].get<int>();

            if (v.contains("active") && v["active"].is_boolean())
                p.planetColour.active.at(colourIndex) = v["active"].get<bool>();

            if (v.contains("colour") && v["colour"].is_object())
            {
                const json& c = v["colour"];
                if (c.contains("r") && c["r"].is_number()) p.planetColour.m_colours.at(colourIndex).r = c["r"].get<float>();
                if (c.contains("g") && c["g"].is_number()) p.planetColour.m_colours.at(colourIndex).g = c["g"].get<float>();
                if (c.contains("b") && c["b"].is_number()) p.planetColour.m_colours.at(colourIndex).b = c["b"].get<float>();
            }
            else
            {
                std::cout << "[jsonToPlanetData] planetColour." << k << ".colour missing or not object\n";
            }

            if (v.contains("height") && v["height"].is_number())
                p.planetColour.m_heights.at(colourIndex) = v["height"].get<float>();
            else
                p.planetColour.m_heights.at(colourIndex) = 0.0f;

            if (v.contains("normalLoc") && v["normalLoc"].is_string())
                p.planetColour.m_normalLocation.at(colourIndex) = v["normalLoc"].get<std::string>();
            else
                p.planetColour.m_normalLocation.at(colourIndex).clear();

            if (v.contains("normalStrength") && v["normalStrength"].is_number())
                p.planetColour.m_normalStrength.at(colourIndex) = v["normalStrength"].get<float>();
            else
                p.planetColour.m_normalStrength.at(colourIndex) = 0.0f;

            if (v.contains("textureLocation") && v["textureLocation"].is_string())
                p.planetColour.m_textureLocation.at(colourIndex) = v["textureLocation"].get<std::string>();
            else
                p.planetColour.m_textureLocation.at(colourIndex).clear();

            if (v.contains("textureScale") && v["textureScale"].is_number())
                p.planetColour.m_textureScale.at(colourIndex) = v["textureScale"].get<float>();
            else
                p.planetColour.m_textureScale.at(colourIndex) = 0.0f;
        }

    }
    else if (j.contains("planetColour"))
    {
        std::cout << "[jsonToPlanetData] planetColour present but not an object\n";
    }

    return p;
}
