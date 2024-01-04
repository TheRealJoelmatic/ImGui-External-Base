#pragma once

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "../globals.hh"
#include "../json/json.hpp"
#include "../menu.hh"
#include <fstream>

using json = nlohmann::json;

void saveConfig(const json& data, const std::string& configName);
json readConfig(const std::string& configName);
void loadConfig(const std::string& configName);

bool configFileExists();

#endif