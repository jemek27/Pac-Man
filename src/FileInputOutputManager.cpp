//
// Created by jemek on 14.06.2024.
//

#include "FileInputOutputManager.h"

FileInputOutputManager::FileInputOutputManager() :  pattern(std::regex(R"(([^\s]+)\s+(\d+))")),
                                                    MainFileDirectory("GameData"),
                                                    TopPlayersScoreboard(MainFileDirectory / "TopPlayersScoreboard.txt"){}

std::vector<std::pair<std::string, int>> FileInputOutputManager::getScoreboard() {
    std::vector<std::pair<std::string, int>> scoreboard;

    if (fs::exists(TopPlayersScoreboard)) {
        std::ifstream inFile(TopPlayersScoreboard);
        std::string currentLine;
        while (std::getline(inFile, currentLine)) {
            scoreboard.push_back(lineToData(currentLine));
        }
        inFile.close();
    } else {
        std::cerr << "TopPlayersScoreboard could not be loaded" << std::endl;
    }

    return scoreboard;
}

void FileInputOutputManager::saveNewScoreboard(const std::vector<std::string>& dataToSave) {
    if (!fs::exists(MainFileDirectory)) {
        fs::create_directories(MainFileDirectory);
        std::cout << MainFileDirectory << " was created" << std::endl;
    }

    if (!fs::exists(TopPlayersScoreboard)) {
        std::ofstream(TopPlayersScoreboard).close();
        std::cout << TopPlayersScoreboard << " was created" << std::endl;
    }

    std::ofstream outFile(TopPlayersScoreboard);
    if (outFile.is_open()) {
        for (const auto& outputLine : dataToSave) {
            outFile << outputLine + "\n";
        }
    } else {
        std::cerr << "TopPlayersScoreboard could not be saved" << std::endl;
    }

    outFile.close();
}

void FileInputOutputManager::saveNewScoreboard(const std::vector<std::pair<std::string, int>> &dataToSave) {
    std::vector<std::string> stingDataToSave;
    for(const auto& data : dataToSave) {
        stingDataToSave.push_back(dataToLine(data));
    }
    saveNewScoreboard(stingDataToSave);
}

std::pair<std::string, int> FileInputOutputManager::lineToData(const std::string &line) {
    std::smatch matches;
    std::string nick;
    int score;
    if (std::regex_search(line, matches, pattern)) {
        nick = matches[1];
        score = std::stoi(matches[2]);
//        std::cout << "Nick: " << nick;
//        std::cout << " Score: " << score << std::endl;
    } else {
        std::cout << "No match found." << std::endl;
    }
    return std::pair<std::string, int>({nick, score});
}

std::string FileInputOutputManager::dataToLine(const std::pair<std::string, int>& data) {
    return std::string(data.first + " " + std::to_string(data.second));
}

