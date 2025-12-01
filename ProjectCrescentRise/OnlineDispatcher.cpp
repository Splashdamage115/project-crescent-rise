#include "OnlineDispatcher.h"
#include "ChatBoxText.h"
#include "OnlineConnection.h"

std::string OnlineDispatcher::playerIdentifier;

void OnlineDispatcher::Init()
{
	playerIdentifier = "Player 1";
	OnlineConnection::Start();
}

std::string OnlineDispatcher::GetIdentifier()
{
	return (playerIdentifier.size()) ? playerIdentifier : "_UNIDENTIFIED_PLAYER_";
}

void OnlineDispatcher::pushPlanet(const PlanetPayload& t_data)
{
	std::string t;
	t += "/planetMod ";
	std::vector<char> buffer = serializePlanetSettings(t_data);
	std::string str(buffer.begin(), buffer.end());
	t += str;
	OnlineConnection::pushTcp(t);
}



void OnlineDispatcher::Dispatch(DispatchType t_type, std::string data)
{
	std::string sendable;

	switch (t_type)
	{
	case OnlineDispatcher::DispatchType::ChatText:
		sendable += data;
		OnlineConnection::pushTcp(sendable);
		break;
	case OnlineDispatcher::DispatchType::movement:
		OnlineConnection::pushPosition(data);
		break;
	default:
		break;
	}
}

void OnlineDispatcher::RecieveDispatch(DispatchType t_type, std::string data)
{
	std::string itemSent = "";
	std::string name = "";
	bool reached = false;
	PlanetPayload out;
	switch (t_type)
	{
	case OnlineDispatcher::DispatchType::ChatText:
		std::cout << data << std::endl;
		for (auto& l : data)
		{
			if (l != '~' && !reached)
			{
				name += l;
			}
			if (l == '~' && !reached)
			{
				reached = true;
				continue;
			}
			if (reached)
			{
				itemSent += l;
			}
		}
		if (itemSent.at(0) == '/')
		{
			interpretCommandsFromServer(itemSent);
			break;
		}
		ChatBoxText::SentNewText(name, itemSent);
		break;
	case OnlineDispatcher::DispatchType::PlanetData:
		deSerializePlanet(data.c_str(), data.length());
		break;
	default:
		break;
	}
}

std::vector<char> OnlineDispatcher::serializePlanetSettings(const PlanetPayload& t_planetPayload)
{
    std::string result;
    // planetPointCount
    result += std::to_string(t_planetPayload.planetPointCount) + ",";
    // planetRadius
    result += std::to_string(t_planetPayload.shapeSettings.planetRadius) + ",";
    // noiseLayers count
    result += std::to_string(t_planetPayload.shapeSettings.noiseLayers.size()) + ",";
    // noiseLayers
    for (const auto& nl : t_planetPayload.shapeSettings.noiseLayers) {
        result += std::to_string(nl.enabled) + "," + std::to_string(nl.useAsMask) + ",";
        // NoiseSettings fields
        result += std::to_string(static_cast<int>(nl.noiseSettings.filterType)) + ",";
        result += std::to_string(nl.noiseSettings.strength) + ",";
        result += std::to_string(nl.noiseSettings.roughness) + ",";
        result += std::to_string(nl.noiseSettings.numLayers) + ",";
        result += std::to_string(nl.noiseSettings.persistence) + ",";
        result += std::to_string(nl.noiseSettings.baseRoughness) + ",";
        result += std::to_string(nl.noiseSettings.center.x) + ",";
        result += std::to_string(nl.noiseSettings.center.y) + ",";
        result += std::to_string(nl.noiseSettings.center.z) + ",";
        result += std::to_string(nl.noiseSettings.minValue) + ",";
        result += std::to_string(nl.noiseSettings.weightMultiplier) + ",";
    }
    // m_colours count
    result += std::to_string(t_planetPayload.planetColour.m_colours.size()) + ",";
    // m_colours
    for (const auto& c : t_planetPayload.planetColour.m_colours) {
        result += std::to_string(c.x) + "," + std::to_string(c.y) + "," + std::to_string(c.z) + ",";
    }
    // m_heights count
    result += std::to_string(t_planetPayload.planetColour.m_heights.size()) + ",";
    // m_heights
    for (const auto& h : t_planetPayload.planetColour.m_heights) {
        result += std::to_string(h) + ",";
    }
    // active count
    result += std::to_string(t_planetPayload.planetColour.active.size()) + ",";
    // active
    for (const auto& a : t_planetPayload.planetColour.active) {
        result += std::to_string(a) + ",";
    }
    if (!result.empty() && result.back() == ',') result.pop_back();

    std::vector<char> characters;
    for (auto l : result)
        characters.push_back(l);

    return characters;
}

void OnlineDispatcher::interpretCommandsFromServer(std::string data)
{
    bool arg1 = false;
    std::string command = "";
    std::string modifier = "";
    for (auto& l : data)
    {
        if (l != ' ')
        {
            if (arg1)
            {
                modifier += l;
            }
            else
            {
                command += l;
            }
        }
        else
        {
            if (arg1) break;

            arg1 = true;
        }
    }

    if (command == "/planetMod")
    {
        Window::Get().passNewPlanetSettings(deSerializePlanet(modifier.c_str(), modifier.length()));
    }
}

PlanetPayload OnlineDispatcher::deSerializePlanet(const char* data, size_t size)
{
    PlanetPayload p;
    std::string str(data, size);
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = str.find(',', start)) != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    if (start < str.size()) tokens.push_back(str.substr(start));

    size_t idx = 0;
    // planetRadius
    p.planetPointCount = std::stoi(tokens[idx]);
    idx++;
    p.shapeSettings.planetRadius = std::stof(tokens[idx++]);
    // noiseLayers count
    size_t noiseLayerLen = std::stoul(tokens[idx++]);
    p.shapeSettings.noiseLayers.resize(noiseLayerLen);
    for (size_t i = 0; i < noiseLayerLen; ++i) {
        p.shapeSettings.noiseLayers[i].enabled = std::stoi(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].useAsMask = std::stoi(tokens[idx++]);
        // NoiseSettings fields
        p.shapeSettings.noiseLayers[i].noiseSettings.filterType = static_cast<NoiseSettings::FilterType>(std::stoi(tokens[idx++]));
        p.shapeSettings.noiseLayers[i].noiseSettings.strength = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.roughness = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.numLayers = std::stoi(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.persistence = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.baseRoughness = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.center.x = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.center.y = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.center.z = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.minValue = std::stof(tokens[idx++]);
        p.shapeSettings.noiseLayers[i].noiseSettings.weightMultiplier = std::stof(tokens[idx++]);
    }
    // m_colours count
    size_t colourLen = std::stoul(tokens[idx++]);
    p.planetColour.m_colours.resize(colourLen);
    for (size_t i = 0; i < colourLen; ++i) {
        float x = std::stof(tokens[idx++]);
        float y = std::stof(tokens[idx++]);
        float z = std::stof(tokens[idx++]);
        p.planetColour.m_colours[i] = glm::vec3(x, y, z);
    }
    // m_heights count
    size_t heightsLen = std::stoul(tokens[idx++]);
    p.planetColour.m_heights.resize(heightsLen);
    for (size_t i = 0; i < heightsLen; ++i) {
        p.planetColour.m_heights[i] = std::stof(tokens[idx++]);
    }
    // active count
    size_t activeLen = std::stoul(tokens[idx++]);
    p.planetColour.active.resize(activeLen);
    for (size_t i = 0; i < activeLen; ++i) {
        p.planetColour.active[i] = std::stoi(tokens[idx++]);
    }
    return p;
}
