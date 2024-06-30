//
// Created by jemek on 14.06.2024.
//

#include "Menu/MainMenu.h"

MainMenu::MainMenu(sf::RenderWindow* window, const sf::Font& font) : Menu(font), Window(window), WindowSize(Window->getSize()),
                                                                     FileManager(new FileInputOutputManager), InGameplay(false), NickNameIsValid(false){
    addButton("Start Game");
    addButton("Scoreboard");
    addButton("Exit");
}

MainMenu::~MainMenu() {
    delete FileManager;
}


void MainMenu::interact() {
    if(ItemSelected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(75));
        switch(getSelectedItemIndex()) {
            case 0:
                if (!NickNameIsValid) {
                    initiateNickINputScrean();
                }
                if (Window->isOpen()) { InGameplay = true; }
                else { InGameplay = false; }
                break;
            case 1:
                viewScoreboard();
                break;
            case 2:
                Window->close();
                break;
        }
    }
}

void MainMenu::initiateNickINputScrean() {
    sf::Text prompt;
    prompt.setFont(Font);
    prompt.setString("Nickname:");
    prompt.setCharacterSize(30);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition((WindowSize.x - prompt.getGlobalBounds().width) / 2, 150);

    sf::RectangleShape inputBox(sf::Vector2f(600, 50));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setPosition((WindowSize.x - inputBox.getSize().x) / 2, 250);

    sf::Text userInput;
    userInput.setFont(Font);
    userInput.setCharacterSize(30);
    userInput.setFillColor(sf::Color::Black);
    userInput.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 5);

    std::regex nicknameRegex("^[\\w]{1,32}$");
    while (!NickNameIsValid) {
        while (Window->pollEvent(Event)) {
            if (Event.type == sf::Event::Closed) {
                Window->close();
                NickNameIsValid = true;
                break;
            } else if (Event.type == sf::Event::TextEntered) {
                nicknameInput(nicknameRegex, prompt);
            }
        }
        userInput.setString(Nickname);

        Window->clear(sf::Color::Black);
        Window->draw(prompt);
        Window->draw(inputBox);
        Window->draw(userInput);
        Window->display();
    }
}

void MainMenu::nicknameInput(const std::regex& nicknameRegex, sf::Text& prompt) {
    if (Event.type == sf::Event::TextEntered) {
        if (Event.text.unicode == '\b') { // Backspace
            if (!Nickname.empty()) {
                Nickname.pop_back();
            }
        } else if (Event.text.unicode == '\r') { // Enter
            if (std::regex_match(Nickname, nicknameRegex)) {
                std::cout << "Wpisana nazwa: " << Nickname << std::endl;
                NickNameIsValid = true;
            } else {
                std::cout << "zla nazwa" << std::endl;
                prompt.setString("Invalid nickname");
                prompt.setFillColor(sf::Color::Red);
                prompt.setPosition((WindowSize.x - prompt.getGlobalBounds().width) / 2, 150);
            }
        } else  if (Nickname.size() < 32)  {
            Nickname += static_cast<char>(Event.text.unicode);
            if (!std::regex_match(Nickname, nicknameRegex)) {
                Nickname.pop_back(); // Remove last character if invalid
                prompt.setString("Invalid nickname, use: letters, numbers or this \"_\" character");
                prompt.setFillColor(sf::Color::Red);
                prompt.setPosition((WindowSize.x - prompt.getGlobalBounds().width) / 2, 150);
            } else {
                prompt.setString("Nickname:");
                prompt.setFillColor(sf::Color::White);
                prompt.setPosition((WindowSize.x - prompt.getGlobalBounds().width) / 2, 150);
            }
        } else {
            prompt.setString("Nickname is to long");
            prompt.setFillColor(sf::Color::Red);
            prompt.setPosition((WindowSize.x - prompt.getGlobalBounds().width) / 2, 150);
        }
    }
}

void MainMenu::manageNewScore(const int& score) {
    Scoreboard = FileManager->getScoreboard();
    char place = checkPlace(score);
//    for (auto a : Scoreboard){ std::cout << a.first << " " << a.second << std::endl; }
//    std::cout << int(place) << std::endl;
    if(place <= 10) { //add to scoreboard
        if (place > Scoreboard.size()) {
            Scoreboard.push_back({Nickname, score});
        } else {
            if (place == 10) { //last
                Scoreboard[place - 1] = {Nickname, score};
            } else {
                std::vector<std::pair<std::string, int>> holdScores;

                for (int i = place - 1; i < Scoreboard.size(); ++i) {
                    holdScores.push_back(Scoreboard[i]);
                }

                Scoreboard[place - 1] = {Nickname, score};
                int end = std::min(10, int(Scoreboard.size() + 1));

                for (int i = place; i < end; ++i) {
                    if (i < Scoreboard.size()) {
                        Scoreboard[i] = holdScores[i - place];
                    } else {
                        Scoreboard.push_back(holdScores[i - place]);
                    }
                }
            }
        }

    }
//    for (auto a : Scoreboard){ std::cout << a.first << " " << a.second << std::endl; }
    FileManager->saveNewScoreboard(Scoreboard);
}

char MainMenu::checkPlace(const int &score) {
    char place = 1;
    if (!Scoreboard.empty()) {
        for (const auto& highScore : Scoreboard) {
            if (highScore.second >= score) {
                place++;
            } else { break; }
        }
    }

    return place;
}

void MainMenu::viewScoreboard() {
    Menu* ScoreboardMenu = new Menu(Font);
    ScoreboardMenu->addButton("Go to menu");
    sf::Text headline;
    headline.setFont(Font);
    headline.setString("High Scores");
    headline.setCharacterSize(30);
    headline.setFillColor(sf::Color::White);
    headline.setPosition((WindowSize.x - headline.getGlobalBounds().width) / 2, 150);

    ScoreboardMenu->addText(headline);
    Scoreboard = FileManager->getScoreboard();
    for (int i = 0; i < Scoreboard.size(); ++i) {
        sf::Text line;
        line.setFont(Font);
        line.setString(FileManager->dataToLine(Scoreboard[i]));
        line.setCharacterSize(30);
        line.setFillColor(sf::Color::White);
        line.setPosition((WindowSize.x - line.getGlobalBounds().width) / 2, 250 + (75 * i));

        ScoreboardMenu->addText(line);
    }

    while (Window->isOpen()) {
        poolEvent();
        if(ItemSelected()) {
            break;
        }
        Window->clear(sf::Color::Black);
        ScoreboardMenu->draw(*Window);
        Window->display();
    }
    delete ScoreboardMenu;
}

void MainMenu::poolEvent() {
    while (Window->pollEvent(Event)) {
        if (Event.type == sf::Event::Closed) {
            Window->close();
            NickNameIsValid = true;
            break;
        }
    }
}

void MainMenu::viewEndScore(const int& score) {
    Menu* ScoreboardMenu = new Menu(Font);
    ScoreboardMenu->addButton("Next");
    sf::Text headline;
    headline.setFont(Font);
    headline.setString("Your score");
    headline.setCharacterSize(30);
    headline.setFillColor(sf::Color::White);
    headline.setPosition((WindowSize.x - headline.getGlobalBounds().width) / 2, 150);

    ScoreboardMenu->addText(headline);

    sf::Text line;
    line.setFont(Font);
    line.setString(std::to_string(score));
    line.setCharacterSize(30);
    line.setFillColor(sf::Color::White);
    line.setPosition((WindowSize.x - line.getGlobalBounds().width) / 2, 250);

    ScoreboardMenu->addText(line);

    while (Window->isOpen()) {
        poolEvent();
        if(ItemSelected()) {
            break;
        }
        Window->clear(sf::Color::Black);
        ScoreboardMenu->draw(*Window);
        Window->display();
    }
    delete ScoreboardMenu;
}