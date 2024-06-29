//
// Created by jemek on 14.06.2024.
//

#ifndef PROJEKT_FILEINPUTOUTPUTMANAGER_H
#define PROJEKT_FILEINPUTOUTPUTMANAGER_H
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

namespace fs = std::filesystem;

class FileInputOutputManager {
    fs::path MainFileDirectory;
    fs::path TopPlayersScoreboard;
    std::regex pattern;
public:
    FileInputOutputManager();
    std::vector<std::pair<std::string, int>> getScoreboard();
    void saveNewScoreboard(const std::vector<std::string>& dataToSave);
    void saveNewScoreboard(const std::vector<std::pair<std::string, int>>& dataToSave);
    std::pair<std::string, int> lineToData(const std::string& line);
    std::string dataToLine(const std::pair<std::string, int>& data);
};


#endif //PROJEKT_FILEINPUTOUTPUTMANAGER_H
