#include "../header Files/functions/configManger.h"
#include "../header Files/functions/utils.h"

void saveConfig(const json& data, const std::string& configName) {
    // Specify the file name
    const std::string fileName = "config.ini";

    // Read the existing content of the file
    std::ifstream configFileRead(fileName);
    std::string fileContent;

    if (configFileRead.is_open()) {
        // Read the entire content of the file
        fileContent.assign(std::istreambuf_iterator<char>(configFileRead), std::istreambuf_iterator<char>());
        configFileRead.close();
    }

    // Parse the existing content as JSON
    json existingData;
    try {
        existingData = json::parse(fileContent);
    }
    catch (const std::exception& e) {
        // Handle JSON parsing error, if any
        std::cerr << "Error parsing existing data: " << e.what() << std::endl;
    }

    // Open the file for writing
    std::ofstream configFile(fileName);

    if (!configFile.is_open()) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }

    // Update existing data or add new section
    existingData[configName] = data;

    // Write the updated data to the file
    configFile << std::setw(4) << existingData << std::endl;

    // Close the file
    configFile.close();

    std::cout << "Data saved to " << fileName << " with configName '" << configName << "' successfully." << std::endl;
}

json readConfig(const std::string& configName) {
    // Specify the file name
    const std::string fileName = "config.ini";

    // Read the existing content of the file
    std::ifstream configFileRead(fileName);
    std::string fileContent;

    if (configFileRead.is_open()) {
        // Read the entire content of the file
        fileContent.assign(std::istreambuf_iterator<char>(configFileRead), std::istreambuf_iterator<char>());
        configFileRead.close();
    }

    // Parse the existing content as JSON
    json existingData;
    try {
        existingData = json::parse(fileContent);
    }
    catch (const std::exception& e) {
        // Handle JSON parsing error, if any
        std::cerr << "Error parsing existing data: " << e.what() << std::endl;
    }

    // Check if the specified configName exists in the JSON data
    if (existingData.find(configName) != existingData.end()) {
        // Return the JSON data associated with the configName
        return existingData[configName];
    }
    else {
        std::cerr << "Config with name '" << configName << "' not found." << std::endl;
        return json(); // Return an empty JSON object if configName is not found
    }
}

void loadConfig(const std::string& configName) {

	if (!(configFileExists())) {
        json defualtData = {
            {"console", "false"}
        };
        saveConfig(defualtData, "1");
	}


    std::cout << "Config file exists." << std::endl;
    json configData = readConfig(configName);

    if (configData.empty()){
        json defualtData = {
            {"backgroundColor", "{0.117647, 0.117647, 0.141176, 1}"},
            {"console", "false"},
            {"roundedCorners", "3.000000"}

        };
        saveConfig(defualtData, "1");
        return;
    }

    std::cout << "Retrieved data for configName '" << configName << "': " << std::setw(4) << configData << std::endl;

    if (configData.is_object() && configData.contains("console")) {
        std::cout << "Value of 'console': " << configData["console"] << std::endl;

        if (configData["console"] == "true") {
			showConsole();
        }
        else
        {
            hideConsole();
        }
    }
    else {
        std::cerr << "Key 'console' not found or not a valid value." << std::endl;
    }

    if (configData.is_object() && configData.contains("backgroundColor")) {
        std::cout << "Value of 'backgroundColor': " << configData["backgroundColor"] << std::endl;
        ApplyCustomColors(StrToImVec4(configData["backgroundColor"]), ImGuiCol_WindowBg);
    }
    else {
        std::cerr << "Key 'backgroundColor' not found or not a valid value." << std::endl;
    }

    try {
        if (configData.is_object() && configData.contains("roundedCorners")) {
            std::cout << "Value of 'roundedCorners': " << configData["roundedCorners"] << std::endl;

            // Convert the string to a float
            float roundedCorners = std::stof(configData["roundedCorners"].get<std::string>());

            ApplyRoundedCorners(roundedCorners);
        }
        else {
            std::cerr << "Error: 'roundedCorners' key not found or not an object" << std::endl;
        }
    }
    catch (const nlohmann::json::exception& e) {
        // Handle JSON library exception
        std::cerr << "JSON exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        // Handle other exceptions
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

bool configFileExists() {
    const std::string fileName = "config.ini";
    std::ifstream configFile(fileName);
    return configFile.good(); // Returns true if the file exists
}