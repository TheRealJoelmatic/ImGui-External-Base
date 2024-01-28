#pragma once

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "../json/json.hpp"

using json = nlohmann::json;

void saveConfig(const json& data, const std::string& configName);
json readConfig(const std::string& configName);
void loadConfig(const std::string& configName);

bool configFileExists();

#endif