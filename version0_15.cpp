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
#include <iomanip>

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

    {
        int pos = rand() % (word.length() - 2) + 1; // เลือกตำแหน่งแบบสุ่ม ยกเว้นตัวแรกและตัวสุดท้าย
        if (find(hiddenPositions.begin(), hiddenPositions.begin() + hiddenCount, pos) == hiddenPositions.begin() + hiddenCount)
        {
            hiddenPositions[hiddenCount] = pos;
            hiddenWord[pos] = '_'; // ซ่อนตัวอักษรที่ตำแหน่งนั้น
            hiddenCount++;
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
    count = 0;
    while (getline(tokenStream, token, delimiter))
    {
        tokens[count++] = token;
    }
    return tokens;
}

// ฟังก์ชันโหลดสถิติจากไฟล์
vector<GameStat> loadStatistics()
{
    ifstream statFile("game_stats.txt");
    vector<GameStat> stats;

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
            stat.timeTaken = stod(splitString(line, ' ', count)[1]); // ตัดเวลาออก
            stats[numStats++] = stat; // เก็บข้อมูลสถิติ
        }
    }

    statFile.close();
    return stats;
}

// ฟังก์ชันบันทึกสถิติลงไฟล์
void saveStatistics(const GameStat &stat)
{
    ofstream statFile("game_stats.txt", ios::app); // เปิดไฟล์แบบ append
    if (!statFile.is_open())                       // ตรวจสอบว่าไฟล์ถูกเปิดหรือไม่
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
    string question;
    int count = 0;

    if (!file.is_open())
    {
        cout << "Unable to open file: " << filename << "\n";
        return 0;
    }

    while (getline(file, question) && count < MAX_QUESTIONS)
    {
        questions[count++] = question;
    }

    file.close();
    return count; // ส่งกลับจำนวนคำถามที่โหลดได้
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
    system("cls");
    int numStats;
    auto stats = loadStatistics(numStats);
    if (numStats == 0)
    {
        cout << "No statistics to display.\n";
        char Wait;
        cin.get(Wait);
        cout << "\nPress Enter to continue";
        cin.get(Wait);
        return;
    }

    // เรียงลำดับตามคะแนนรวม
    sort(stats.begin(), stats.begin() + numStats, compareByScore);

    // แสดงผลอันดับ
    string line(60, '=');
    cout << right << setw(10) << "\n===== Ranking by Total Score =====\n";
    cout << line;
    cout << "\nRank\tDate\t\t\tDifficulty\tScore\tTime\n";
    cout << line << endl;
    for (size_t i = 0; i < numStats; ++i)
    {
        cout << (i + 1) << "\t" << stats[i].date << "\t" << stats[i].difficulty << "\t"
             << stats[i].totalScore << "\t" << stats[i].timeTaken << " sec\n";
    }
    char Wait;
    cin.get(Wait);
    cout << "\nSaved, Press Enter to continue";
    cin.get(Wait);
}

// ฟังก์ชันแสดงอันดับตามเวลา
void displayRankingsByTime()
{
    system("cls");
    int numStats;
    auto stats = loadStatistics(numStats);
    if (numStats == 0)
    {
        cout << "No statistics to display.\n";
        char Wait;
        cin.get(Wait);
        cout << "\nPress Enter to continue";
        cin.get(Wait);
        return;
    }

    // เรียงลำดับตามเวลา
    sort(stats.begin(), stats.begin() + numStats, compareByTime);

    // แสดงอันดับตามเวลา
    string line(60, '=');
    cout << "\n===== Ranking by Time Taken (Fastest) =====\n";
    cout << line;
    cout << "\nRank\tDate\t\t\tDifficulty\tScore\tTime\n";
    cout << line << endl;
    for (size_t i = 0; i < numStats; ++i)
    {
        cout << (i + 1) << "\t" << stats[i].date << "\t" << stats[i].difficulty << "\t"
             << stats[i].totalScore << "\t" << stats[i].timeTaken << " sec\n";
    }
    char Wait;
    cin.get(Wait);
    cout << "\nSaved, Press Enter to continue";
    cin.get(Wait);
}

// ฟังก์ชันแสดงสถิติ
void viewStatistics()
{
    system("cls");
    ifstream statFile("game_stats.txt");

    // ตรวจสอบว่าไฟล์สถิติสามารถเปิดได้หรือไม่
    if (!statFile.is_open())
    {
        cout << "No statistics available.\n";
        char Wait;
        cin.get(Wait);
        return;
    }

    // อ่านและแสดงผลสถิติจากไฟล์
    string line;
    while (getline(statFile, line))
    {
        cout << line << endl;
    }

    statFile.close(); // ปิดไฟล์
    char Wait;
    cin.get(Wait);
}

// ฟังก์ชันการเล่นเกม
void playGame()
{
    int correctAnswers = 0;
    int wrongAnswers = 0;
    int score = 0;

    auto startTime = high_resolution_clock::now();

    for (int i = 0; i < totalQuestions; ++i)
    {
        string hiddenWord = generateHiddenWord(questions[i], difficulty);
        cout << "Guess the word: " << hiddenWord << endl;

        string answer;
        cout << "Your answer: ";
        cin >> answer;

        // ตรวจสอบคำตอบ
        if (answer == questions[i])
        {
            cout << "Correct!\n";
            correctAnswers++;
            score += 10; // เพิ่มคะแนน
        }
        else
        {
            cout << "Wrong! The correct answer was: " << questions[i] << endl;
            wrongAnswers++;
        }
    }

    auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(endTime - startTime); // คำนวณเวลาที่ใช้

    // แสดงผลลัพธ์
    cout << "\nGame Over!\n";
    cout << "Correct Answers: " << correctAnswers << endl;
    cout << "Wrong Answers: " << wrongAnswers << endl;
    cout << "Total Score: " << score << endl;
    cout << "Time Taken: " << duration.count() << " seconds\n";

    // บันทึกสถิติ
    GameStat stat;
    stat.date = to_string(time(nullptr)); // วันที่เป็น Unix timestamp
    stat.difficulty = difficulty;
    stat.correctAnswers = correctAnswers;
    stat.wrongAnswers = wrongAnswers;
    stat.totalScore = score;
    stat.timeTaken = duration.count();
    saveStatistics(stat);
}

// ฟังก์ชันหลัก
int main()
{
    srand(static_cast<unsigned int>(time(0))); // เริ่มต้นการสุ่ม

    array<string, MAX_QUESTIONS> questions; // ตัวแปรเก็บคำถาม
    int totalQuestions = loadQuestions("questions.txt", questions); // โหลดคำถามจากไฟล์

    if (totalQuestions == 0)
    {
        cout << "No questions available to play.\n";
        return 1; // ออกจากโปรแกรมถ้าคำถามไม่มี
    }

    string difficulty;
    cout << "Select difficulty (easy, medium, hard): ";
    cin >> difficulty;

    playGame(questions, totalQuestions, difficulty); // เริ่มเกม

    // แสดงสถิติ
    char viewStatChoice;
    cout << "Do you want to view statistics? (y/n): ";
    cin >> viewStatChoice;
    if (viewStatChoice == 'y' || viewStatChoice == 'Y')
    {
        viewStatistics();
    }

    // แสดงอันดับ
    char rankingChoice;
    cout << "Do you want to view rankings? (y/n): ";
    cin >> rankingChoice;
    if (rankingChoice == 'y' || rankingChoice == 'Y')
    {
        displayRankingsByScore();
        displayRankingsByTime();
    }

    return 0;
}
