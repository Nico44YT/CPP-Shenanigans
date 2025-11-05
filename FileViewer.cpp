#include <iostream>
#include <conio.h>
#include <filesystem>

std::filesystem::directory_entry get_entry_at(const std::filesystem::path& path, std::size_t index) {
    auto it = std::filesystem::directory_iterator(path);
    std::advance(it, index);
    return *it;
}

constexpr int ENTER = 13;

constexpr int ESCAPE_CHAR = 224;
constexpr int UP_ARROW = 72;
constexpr int LEFT_ARROW = 75;
constexpr int RIGHT_ARROW = 77;
constexpr int DOWN_ARROW = 80;

int currentIndex = -1;
int maxFiles = 1;

void handleInput(const int &ch) {
    if (ch == UP_ARROW || ch == 'w') currentIndex--;
    if (ch == DOWN_ARROW || ch == 's') currentIndex++;

    if (currentIndex < -1) currentIndex = maxFiles - 1;
    if (currentIndex > maxFiles-1) currentIndex = -1;
}

void drawFolders(const std::filesystem::path path) {
    if (is_directory(path)) {

        std::string indicator = "  ";
        if (-1 == currentIndex) indicator = "> ";
        std::cout << indicator << "..." << '\n';

        int index = 0;
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            indicator = "  ";
            if (index == currentIndex) indicator = "> ";
            std::cout << indicator << entry.path().filename().string() << (is_directory(entry.path()) ? "/" : "") << '\n';
            index++;
        }

        maxFiles = index;
    }
}

int main(int argc, char *argv[]) {
    std::filesystem::path currentPath = std::filesystem::current_path();
    currentPath = currentPath.parent_path();

    while (true) {
        system("CLS");

        std::cout << "Current Path: " << currentPath << "\n";
        drawFolders(currentPath);

        int ch = getch();

        if (ch == 'q' || ch == 27) break;

        if (ch == ESCAPE_CHAR) {
            ch = getch();

            handleInput(ch);
            continue;
        }

        if (ch == ENTER) {
            if (currentIndex == -1) {
                currentPath = currentPath.parent_path();
                continue;
            }

            std::filesystem::directory_entry entry = get_entry_at(currentPath, currentIndex);

            auto newPath = currentPath / entry;

            if (entry.is_directory()) {
                currentIndex = -1;
                currentPath = newPath;
            } else {
                std::string command = "edit ";
                command += newPath.string();

                system(command.c_str());
            }
        }
    }

    return 0;
}
