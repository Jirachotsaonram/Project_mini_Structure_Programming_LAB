#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <random>

using namespace std;
using namespace chrono;

// ฟังก์ชันการแสดงคำพร้อมช่องว่างตามระดับความยาก
string generateHiddenWord(const string &word, const string &difficulty)
{
    string hiddenWord = word;

    int numCharsToHide = 0;
    if (difficulty == "easy")
    {
        numCharsToHide = 1 + (rand() % 2); // ซ่อน 1-2 ตัวอักษร
    }
    else if (difficulty == "medium")
    {
        numCharsToHide = 2 + (rand() % 2); // ซ่อน 2-3 ตัวอักษร
    }
    else if (difficulty == "hard")
    {
        numCharsToHide = 3 + (rand() % 2); // ซ่อน 3-4 ตัวอักษร
    }

    vector<int> hiddenPositions;
    while (hiddenPositions.size() < numCharsToHide)
    {
        int pos = rand() % (word.length() - 2) + 1; // เลือกตำแหน่งแบบสุ่ม ยกเว้นตัวแรกและตัวสุดท้าย
        if (find(hiddenPositions.begin(), hiddenPositions.end(), pos) == hiddenPositions.end())
        {
            hiddenPositions.push_back(pos);
            hiddenWord[pos] = '_'; // ซ่อนตัวอักษรที่ตำแหน่งนั้น
        }
    }

    return hiddenWord;
}

// โครงสร้างสำหรับเก็บสถิติ
struct GameStat
{
    string date;
    string difficulty;
    int correctAnswers;
    int wrongAnswers;
    int totalScore;
    double timeTaken;
};

// ฟังก์ชันแยก string
vector<string> splitString(const string &str, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

// ฟังก์ชันโหลดสถิติจากไฟล์
vector<GameStat> loadStatistics()
{
    ifstream statFile("game_stats.txt");
    vector<GameStat> stats;

    if (!statFile.is_open())
    {
        cout << "No statistics available.\n";
        return stats;
    }

    string line;
    GameStat stat;
    while (getline(statFile, line))
    {
        if (line.find("Date:") != string::npos)
        {
            stat.date = line.substr(6); // ตัด "Date: " ออก
        }
        else if (line.find("Difficulty:") != string::npos)
        {
            stat.difficulty = line.substr(12); // ตัด "Difficulty: " ออก
        }
        else if (line.find("Correct:") != string::npos)
        {
            stat.correctAnswers = stoi(line.substr(9)); // ตัด "Correct: " ออก
        }
        else if (line.find("Wrong:") != string::npos)
        {
            stat.wrongAnswers = stoi(line.substr(7)); // ตัด "Wrong: " ออก
        }
        else if (line.find("Total Score:") != string::npos)
        {
            stat.totalScore = stoi(line.substr(13)); // ตัด "Total Score: " ออก
        }
        else if (line.find("Time:") != string::npos)
        {
            stat.timeTaken = stod(splitString(line, ' ')[1]); // ตัดเวลาออก
            stats.push_back(stat);                            // เก็บข้อมูลสถิติ
        }
    }

    statFile.close();
    return stats;
}

// ฟังก์ชันบันทึกสถิติลงไฟล์
void saveStatistics(const GameStat &stat)
{
    ofstream statFile("game_stats.txt", ios::app); // เปิดไฟล์แบบ append
    if (!statFile.is_open()) // ตรวจสอบว่าไฟล์ถูกเปิดหรือไม่
    {
        cout << "Error: Unable to open file for saving statistics.\n";
        return;
    }

    statFile << "Date: " << stat.date << "\n"
             << "Difficulty: " << stat.difficulty << "\n"
             << "Correct: " << stat.correctAnswers << "\n"
             << "Wrong: " << stat.wrongAnswers << "\n"
             << "Total Score: " << stat.totalScore << "\n"
             << "Time: " << stat.timeTaken << " sec\n\n";
             
    statFile.close(); // ปิดไฟล์หลังจากเขียนข้อมูลเสร็จ
}


// ฟังก์ชันโหลดคำถามจากไฟล์
vector<string> loadQuestions(const string &filename)
{
    ifstream file(filename);
    vector<string> questions;
    string question;

    if (!file.is_open())
    {
        cout << "Unable to open file: " << filename << "\n";
        return questions;
    }

    while (getline(file, question))
    {
        questions.push_back(question);
    }

    file.close();
    return questions;
}

// ฟังก์ชันเปรียบเทียบสำหรับการเรียงลำดับตามคะแนนรวม (มากไปน้อย)
bool compareByScore(const GameStat &a, const GameStat &b)
{
    return a.totalScore > b.totalScore; // คะแนนมากที่สุดอยู่อันดับบน
}

// ฟังก์ชันเปรียบเทียบสำหรับการเรียงลำดับตามเวลา (น้อยไปมาก)
bool compareByTime(const GameStat &a, const GameStat &b)
{
    return a.timeTaken < b.timeTaken; // เวลาน้อยที่สุดอยู่อันดับบน
}

// ฟังก์ชันแสดงอันดับตามคะแนนรวม
void displayRankingsByScore()
{
    vector<GameStat> stats = loadStatistics();
    if (stats.empty())
    {
        cout << "No statistics to display.\n";
        return;
    }

    // เรียงลำดับตามคะแนนรวม
    sort(stats.begin(), stats.end(), compareByScore);

    // แสดงผลอันดับ
    cout << "\n=== Ranking by Total Score ===\n";
    cout << "Rank\tDate\t\t\tDifficulty\tScore\tTime\n";
    for (size_t i = 0; i < stats.size(); ++i)
    {
        cout << (i + 1) << "\t" << stats[i].date << "\t" << stats[i].difficulty << "\t"
             << stats[i].totalScore << "\t" << stats[i].timeTaken << " sec\n";
    }
}

// ฟังก์ชันแสดงอันดับตามเวลา
void displayRankingsByTime()
{
    vector<GameStat> stats = loadStatistics();
    if (stats.empty())
    {
        cout << "No statistics to display.\n";
        return;
    }

    // เรียงลำดับตามเวลา
    sort(stats.begin(), stats.end(), compareByTime);

    // แสดงอันดับตามเวลา
    cout << "\n=== Ranking by Time Taken (Fastest) ===\n";
    cout << "Rank\tDate\t\t\tDifficulty\tScore\tTime\n";
    for (size_t i = 0; i < stats.size(); ++i)
    {
        cout << (i + 1) << "\t" << stats[i].date << "\t" << stats[i].difficulty << "\t"
             << stats[i].totalScore << "\t" << stats[i].timeTaken << " sec\n";
    }
}

// ฟังก์ชันแสดงสถิติ
void viewStatistics()
{
    ifstream statFile("game_stats.txt");
    if (!statFile.is_open())
    {
        cout << "No statistics available.\n";
        return;
    }

     // ตรวจสอบว่าไฟล์ว่าง
    statFile.seekg(0, ios::end);
    if (statFile.tellg() == 0) // หากขนาดไฟล์เป็น 0
    {
        cout << "No statistics to display.\n";
        statFile.close();
        return;
    }

    // รีเซ็ตตำแหน่งไฟล์ไปที่จุดเริ่มต้น
    statFile.seekg(0, ios::beg);

    cout << "\n=== View Statistics ===\n";

    string line;
    while (getline(statFile, line))
    {
        cout << line << endl;
    }

    statFile.close();
}

// ฟังก์ชันการเล่นเกม
void playGame()
{
    string difficulty;
    string dfcl;
    vector<string> words;

    // เลือกระดับความยาก
    cout << "Select difficulty (1. easy, 2. medium, 3. hard, 4. back): ";
    cin >> dfcl;
    transform(dfcl.begin(), dfcl.end(), dfcl.begin(), ::tolower); //แปลงอักษรเป็นตัวเล็ก
    // ดึงคำถามจากไฟล์ตามระดับความยาก
    if (dfcl == "1" || dfcl == "easy")
    {
        difficulty = "easy";
        words = loadQuestions("easy_questions.txt");
    }
    else if (dfcl == "2" || dfcl == "medium")
    {
        difficulty = "medium";
        words = loadQuestions("medium_questions.txt");
    }
    else if (dfcl == "3" || dfcl == "hard")
    {
        difficulty = "hard";
        words = loadQuestions("hard_questions.txt");
    }else if(dfcl == "4"){
        return;
    }
    else
    {
        cout << "Invalid difficulty.\n";
        return;
    }

    // ตรวจสอบว่าได้คำถามหรือไม่
    if (words.empty())
    {
        cout << "No questions available for this difficulty.\n";
        return;
    }

    // สุ่มคำถาม
    random_device rd;
    mt19937 g(rd());
    shuffle(words.begin(), words.end(), g);

    int correctAnswers = 0, wrongAnswers = 0;
    auto startTime = high_resolution_clock::now();
    int timeLimit = 3 * 60; // กำหนดเวลา 3 นาที

    for (size_t i = 0; i < words.size(); ++i)
    {
        string guess;
        string word = words[i];
        string hiddenWord = generateHiddenWord(word, difficulty); // แสดงคำพร้อมช่องว่างตามระดับความยาก

        cout << "Question " << (i + 1) << ": " << hiddenWord << "\n";
        cout << "Your guess: ";
        cin >> guess;

        // ตรวจสอบคำตอบ
        transform(guess.begin(), guess.end(), guess.begin(), ::tolower); // แปลงเป็นตัวพิมพ์เล็ก
        transform(word.begin(), word.end(), word.begin(), ::tolower);    // แปลงเป็นตัวพิมพ์เล็ก
        if (guess == word)
        {
            correctAnswers++;
            cout << "Correct!\n";
        }
        else
        {
            wrongAnswers++;
            cout << "Wrong! The correct word was: " << word << "\n";
            if (wrongAnswers >= 3)
            {
                cout << "You have reached the maximum wrong answers.\n";
                break;
            }
        }

        // ตรวจสอบเวลา
        auto currentTime = high_resolution_clock::now();
        double elapsedTime = duration_cast<seconds>(currentTime - startTime).count();
        if (elapsedTime >= timeLimit)
        {
            cout << "Time's up!\n";
            break;
        }
    }

    // คำนวณคะแนนรวม
    int totalScore = correctAnswers * 10 - wrongAnswers * 5;
    auto endTime = high_resolution_clock::now();
    double duration = duration_cast<seconds>(endTime - startTime).count();

    // บันทึกสถิติ
    time_t now = time(0);
    char *dt = ctime(&now);
    string dateTime(dt);
    dateTime.erase(remove(dateTime.begin(), dateTime.end(), '\n'), dateTime.end()); // ลบ '\n' ออกจากวันที่
    GameStat stat = {dateTime, difficulty, correctAnswers, wrongAnswers, totalScore, duration};
    saveStatistics(stat);

    // แสดงผลคะแนน
    cout << "\nGame Over! You got " << correctAnswers << " correct and " << wrongAnswers << " wrong.\n";
    cout << "Your total score: " << totalScore << "\n";
    cout << "Time taken: " << duration << " seconds.\n";
}

// ฟังก์ชันรีเซ็ตอันดับ (ลบไฟล์ game_stats.txt)
void resetRankings()
{
    ofstream statFile("game_stats.txt", ios::trunc); // เปิดไฟล์แบบ trunc เพื่อลบข้อมูลทั้งหมด
    if (statFile.is_open())
    {
        cout << "All rankings and statistics have been reset.\n";
    }
    else
    {
        cout << "Error resetting rankings. Could not open the file.\n";
    }
    statFile.close();
}

// เมนูหลัก
void mainMenu()
{
    int choice;
    do
    {
        cout << "\n=== Word Guessing Game ===\n";
        cout << "1. Play Game\n";
        cout << "2. View Statistics\n";
        cout << "3. Display Rankings by Score\n";
        cout << "4. Display Rankings by Time\n";
        cout << "5. Reset Rankings\n"; // เพิ่มตัวเลือกรีเซ็ตอันดับ
        cout << "6. Exit\n";
        cout << "Select an option: ";
        cin >> choice;

        if (cin.fail()) // ตรวจสอบว่าการป้อนข้อมูลผิดพลาดหรือไม่
        {
            cin.clear(); // เคลียร์สถานะของ cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ละเว้นข้อมูลที่ไม่ถูกต้อง
            cout << "Invalid input! Please enter a number.\n";
            continue; // กลับไปที่ลูป
        }

        switch (choice)
        {
        case 1:
            playGame();
            break;
        case 2:
            viewStatistics();
            break;
        case 3:
            displayRankingsByScore();
            break;
        case 4:
            displayRankingsByTime();
            break;
        case 5:
            resetRankings(); // เรียกฟังก์ชันรีเซ็ตอันดับ
            break;
        case 6:
            cout << "Exiting the game. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
    } while (choice != 6);
}

int main()
{
    mainMenu();
    return 0;
}
