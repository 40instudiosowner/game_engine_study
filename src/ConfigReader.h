#include "json.hpp"
#include <fstream>
#include <string>

using json = nlohmann::json;

const std::string CONFIG_FILE_PATH = "config.json";

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

    // --- Animation speeds ---
    float getFadeSpeed() const {
        return _config["animation"]["speed"]["fade"];
    }

    float getMoveSpeed() const {
        return _config["animation"]["speed"]["move"];
    }

    float getScaleSpeed() const {
        return _config["animation"]["speed"]["scale"];
    }

    // --- Logo ---
    float getLogoScaleMultiplier() const {
        return _config["logo"]["scaleMultiplier"];
    }

    int getLogoWidth() const {
        return _config["logo"]["size"]["width"];
    }

    int getLogoHeight() const {
        return _config["logo"]["size"]["height"];
    }

    int getLogoPositionX() const {
        return _config["logo"]["position"]["x"];
    }

    int getLogoPositionY() const {
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

};