#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <string>

using json = nlohmann::json;

const std::string CONFIG_FILE_PATH = "../config.json";

struct Color 
{
    int r, g, b;
};


class ConfigReader
{
	json _config;

public:
    ConfigReader(std::string filepath = CONFIG_FILE_PATH);

    // --- Window ---
    int getWindowWidth() const {
        return _config["window"]["width"];
    }

    int getWindowHeight() const {
        return _config["window"]["height"];
    }

    // --- Paths ---
    std::string getFontPath() const {
        return _config["paths"]["font"];
    }

    std::vector<std::string> getLogoPaths() const {
        return _config["paths"]["logos"].get<std::vector<std::string>>();
    }

    // --- Animation speed ---
 
    float getSpeed() const {
        return _config["animation"]["speed"];
    }


    // --- Logo ---
    float getLogoScaleMultiplier() const {
        return _config["logo"]["scaleMultiplier"];
    }

    unsigned int getLogoWidth() const {
        return _config["logo"]["size"]["width"];
    }

    unsigned int getLogoHeight() const {
        return _config["logo"]["size"]["height"];
    }

    float getLogoPositionX() const {
        return _config["logo"]["position"]["x"];
    }

    float getLogoPositionY() const {
        return _config["logo"]["position"]["y"];
    }

    Color getLogoColor() const
    {
        return {
            _config["logo"]["color"]["r"],
            _config["logo"]["color"]["g"],
            _config["logo"]["color"]["b"]
        };
    }

	// --- Text ---
    std::string getText() const {
        return _config["text"]["content"];
    }

    float getTextPositionX() const {
        return _config["text"]["position"]["x"];
    }

    float getTextPositionY() const {
        return _config["text"]["position"]["y"];
    }

    unsigned int getFontSize() const {
        return _config["text"]["fontSize"];
    }
};