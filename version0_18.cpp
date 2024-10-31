#include <iostream>     // ใช้สำหรับการรับและแสดงผลข้อมูลทางหน้าจอ
#include <fstream>      // ใช้ในการจัดการไฟล์
#include <vector>       // ที่ใช้เก็บข้อมูลแบบ array ที่ขยายขนาดได้ ซึ่งมีการจัดการเมมโมรีอัตโนมัติ
#include <string>       // ใช้สำหรับการจัดการข้อมูลแบบข้อความ
#include <ctime>        // ใช้สำหรับการจัดการวันที่และเวลา
#include <chrono>       // ใช้สำหรับการวัดเวลาโดยละเอียด ซึ่งมักใช้ในการจับเวลา (timer) และการประมวลผลเวลาที่แม่นยำมากขึ้นกว่าการใช้ <ctime>
#include <algorithm>        // ใช้สำหรับฟังก์ชันการจัดการข้อมูล เช่นการเรียงลำดับ (std::sort), ค้นหา (std::find), และอื่น ๆ ที่ช่วยให้การจัดการข้อมูลทำได้สะดวกและรวดเร็ว
#include <sstream>      // ใช้สำหรับการจัดการข้อมูลประเภทสตริง โดยสามารถแปลงสตริงเป็นตัวเลขหรือกลับกันได้ ซึ่งสะดวกในการแปลงข้อมูลชนิดต่าง ๆ
#include <cctype>       // ใช้จัดการตัวอักษร เช่นการเช็คว่าเป็นตัวอักษรหรือตัวเลข (std::isalpha, std::isdigit) และการปรับเปลี่ยนตัวอักษรเป็นตัวพิมพ์เล็กหรือตัวพิมพ์ใหญ่ (std::toupper, std::tolower)
#include <random>       // ใช้ในการสุ่มตัวเลข ซึ่งมีเครื่องมือในการสุ่มตัวเลขที่ซับซ้อนและแม่นยำมากขึ้นกว่าการใช้ <cstdlib>
#include <iomanip>      // ใช้สำหรับจัดการการแสดงผลตัวเลขแบบกำหนดตำแหน่งทศนิยม เช่น std::setprecision เพื่อกำหนดจำนวนจุดทศนิยมที่ต้องการ
#include <cstdlib>      // ใช้สำหรับฟังก์ชันทั่วไป เช่นการสุ่ม (std::rand, std::srand) และการแปลงชนิดข้อมูล

using namespace std;
using namespace chrono;

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

// เมนูหลัก
void mainMenu();
// ฟังก์ชันการเล่นเกม
void playGame();
// ฟังก์ชันแสดงสถิติ
void viewStatistics();
// ฟังก์ชันแสดงอันดับตามคะแนนรวม
void displayRankingsByScore();
// ฟังก์ชันแสดงอันดับตามเวลา
void displayRankingsByTime();
// ฟังก์ชันรีเซ็ตอันดับ (ลบไฟล์ game_stats.txt)
void resetRankings();
// คำอธิบายเกม
void gameDescription();
// คำสั่งล้างหน้าจอ
void clearScreen();
// ฟังก์ชันการแสดงคำพร้อมช่องว่างตามระดับความยาก
string generateHiddenWord(const string &word, const string &difficulty);
// ฟังก์ชันแยก string
vector<string> splitString(const string &str, char delimiter);
// ฟังก์ชันโหลดสถิติจากไฟล์
vector<GameStat> loadStatistics();
// ฟังก์ชันบันทึกสถิติลงไฟล์
void saveStatistics(const GameStat &stat);
// ฟังก์ชันโหลดคำถามจากไฟล์
vector<string> loadQuestions(const string &filename);
// ฟังก์ชันเปรียบเทียบสำหรับการเรียงลำดับตามคะแนนรวม (มากไปน้อย)
bool compareByScore(const GameStat &a, const GameStat &b);
// ฟังก์ชันเปรียบเทียบสำหรับการเรียงลำดับตามเวลา (น้อยไปมาก)
bool compareByTime(const GameStat &a, const GameStat &b);

// หลัก
int main()
{
    mainMenu();
    return 0;
}

// เมนูหลัก
void mainMenu()
{
    int choice;
    string line(39, '=');
    do
    {
        // เรียกใช้คำสั่ง
        clearScreen();
        // แสดงเมนู
        cout << line;
        cout << "\n===== 3-Minute Word Guessing Game =====\n";
        cout << line << endl;
        cout << "1 - Play Game\n";
        cout << "2 - View Statistics\n";
        cout << "3 - Display Rankings by Score\n";
        cout << "4 - Display Rankings by Time\n";
        cout << "5 - Reset Rankings\n";
        cout << "6 - Game Description\n";
        cout << "7 - Exit\n";
        cout << line << endl;
        cout << "Enter Choose: ";
        cin >> choice;

        if (cin.fail()) // ตรวจสอบว่าการป้อนข้อมูลผิดพลาดหรือไม่ ใช้ตรวจสอบว่าการรับข้อมูลจากผู้ใช้ผ่าน std::cin สำเร็จหรือไม่ โดยเฉพาะเมื่อเราพยายามรับข้อมูลประเภทใดประเภทหนึ่ง เช่น ตัวเลข แต่ผู้ใช้ป้อนข้อมูลที่ไม่สามารถแปลงเป็นตัวเลขได้ เช่น ตัวอักษร ซึ่งจะทำให้ cin เกิดสถานะ "ล้มเหลว" และ cin.fail() จะคืนค่าเป็น true
        {
            cin.clear();                                         // เคลียร์สถานะของ cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ละเว้นข้อมูลที่ไม่ถูกต้อง
            char Wait;
            cout << "Invalid input! Please enter a number.\n";
            cout << "\nPress Enter to continue";
            cin.get(Wait);
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
            resetRankings();
            break;
        case 6:
            gameDescription();
            break;
        case 7:
            cout << "Exiting the game. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            char Wait;
            cin.get(Wait);
            cout << "\nPress Enter to continue";
            cin.get(Wait);
            break;
        }
    } while (choice != 7);
}

// ฟังก์ชันการเล่นเกม
void playGame()
{
    // เรียกใช้คำสั่ง
    clearScreen();
    string difficulty;
    string dfcl;
    vector<string> words;  // ใช้สร้างตัวแปร words ซึ่งเป็น vector ของ string โดย vector นี้สามารถเก็บข้อมูลประเภท string ได้หลาย ๆ ค่า และยังสามารถขยายขนาดได้ตามที่ต้องการ

    // เลือกระดับความยาก
    cout << "Select difficulty (1. easy, 2. medium, 3. hard, 4. back): ";
    cin >> dfcl;
    transform(dfcl.begin(), dfcl.end(), dfcl.begin(), ::tolower); // แปลงอักษรเป็นตัวเล็ก
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
    }
    else if (dfcl == "4")
    {
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
        char Wait;
        cin.get(Wait);
        cout << "\nPress Enter to continue";
        cin.get(Wait);
        return;
    }

    // สุ่มคำถาม
    random_device rd;       // สร้างค่าที่สุ่มขึ้นมาเพื่อนำไปเป็น seed สำหรับตัวสร้างตัวเลขสุ่ม
    mt19937 g(rd());        // ใช้ mt19937 (Mersenne Twister) ในการสร้างตัวเลขสุ่มโดยใช้ค่า seed จาก rd เพื่อให้การสุ่มมีความแม่นยำสูง
    shuffle(words.begin(), words.end(), g);     // ฟังก์ชัน shuffle ใช้สำหรับสุ่มลำดับข้อมูลใน words โดยเรียงคำถามใหม่แบบสุ่ม ทำให้คำถามที่ได้มีลำดับใหม่ทุกครั้งที่เรียกใช้

    int correctAnswers = 0, wrongAnswers = 0;       // สร้างตัวแปรสำหรับนับจำนวนคำตอบที่ถูกและผิด เริ่มต้นเป็น 0
    auto startTime = high_resolution_clock::now();      // ใช้ high_resolution_clock จาก <chrono> เพื่อบันทึกเวลาที่เริ่มทำแบบทดสอบ
    int timeLimit = 3 * 60; // กำหนดเวลา 3 นาที 180 วินาที

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
        auto currentTime = high_resolution_clock::now();        // high_resolution_clock เป็นคลาสจากไลบรารี <chrono> ที่ใช้เพื่อจับเวลาที่มีความละเอียดสู
        double elapsedTime = duration_cast<seconds>(currentTime - startTime).count();       // duration_cast<seconds>(...) ใช้เพื่อแปลงค่าที่ได้ให้เป็นหน่วยวินาที
        if (elapsedTime >= timeLimit)
        {
            cout << "Time's up!\n";
            break;
        }
    }

    // คำนวณคะแนนรวม
    int totalScore = correctAnswers * 10 - wrongAnswers * 5;
    if (totalScore < 0)
    {
        totalScore = 0; // หากคะแนนน้อยกว่า 0 ให้ตั้งเป็น 0
    }
    auto endTime = high_resolution_clock::now();
    double duration = duration_cast<seconds>(endTime - startTime).count();

    // บันทึกสถิติ
    time_t now = time(0);
    char *dt = ctime(&now);     // ctime(&now) ใช้เพื่อแปลงค่า time_t เป็นสตริงที่แสดงวันที่และเวลาปัจจุบันในรูปแบบที่อ่านง่าย (เช่น "Wed Oct 31 10:45:37 2024")
    string dateTime(dt);        // ใช้ remove เพื่อย้ายตัวอักษร '\n' (newline) ไปที่ท้ายสตริง แล้ว erase จะลบส่วนที่อยู่ในอาเรย์หลังจากตัวอักษรที่ไม่ต้องการออก
    dateTime.erase(remove(dateTime.begin(), dateTime.end(), '\n'), dateTime.end()); // ลบ '\n' ออกจากวันที่
    GameStat stat = {dateTime, difficulty, correctAnswers, wrongAnswers, totalScore, duration};
    saveStatistics(stat);

    // แสดงผลคะแนน
    cout << "\nGame Over! You got " << correctAnswers << " correct and " << wrongAnswers << " wrong.\n";
    cout << "Your total score: " << totalScore << "\n";
    cout << "Time taken: " << duration << " seconds.\n";
    char Wait;
    cin.get(Wait);
    cout << "\nSaved, Press Enter to continue";
    cin.get(Wait);
}

// ฟังก์ชันแสดงสถิติ
void viewStatistics()
{
    // เรียกใช้คำสั่ง
    clearScreen();
    ifstream statFile("game_stats.txt");

    // ตรวจสอบว่าไฟล์ว่าง
    statFile.seekg(0, ios::end);
    if (statFile.tellg() == 0) // หากขนาดไฟล์เป็น 0
    {
        cout << "No statistics to display.\n";
        // คันด้วยกดปุ่มEnter
        char Wait;
        cin.get(Wait);
        cout << "\nPress Enter to continue";
        cin.get(Wait);
        statFile.close();
        return;
    }

    // รีเซ็ตตำแหน่งไฟล์ไปที่จุดเริ่มต้น
    statFile.seekg(0, ios::beg);

    cout << "\n===== View Statistics =====\n";

    string line;
    while (getline(statFile, line))
    {
        cout << line << endl;
    }
    // หยุด ไปกดEnter
    statFile.close();
    char Wait;
    cin.get(Wait);
    cout << "\nPress Enter to continue";
    cin.get(Wait);
}

// ฟังก์ชันแสดงอันดับตามคะแนนรวม
void displayRankingsByScore()
{
    // เรียกใช้คำสั่ง
    clearScreen();
    vector<GameStat> stats = loadStatistics();
    if (stats.empty())
    {
        cout << "No statistics to display.\n";
        // คันด้วยกดปุ่มEnter
        char Wait;
        cin.get(Wait);
        cout << "\nPress Enter to continue";
        cin.get(Wait);
        return;
    }

    // เรียงลำดับตามคะแนนรวม
    sort(stats.begin(), stats.end(), compareByScore);

    // แสดงผลอันดับ
    string line(60, '=');
    cout << right << setw(10) << "\n===== Ranking by Total Score =====\n";
    cout << line;
    cout << "\nRank\tDate\t\t\tDifficulty\tScore\tTime\n";
    cout << line << endl;
    for (size_t i = 0; i < stats.size(); ++i)
    {
        cout << (i + 1) << "\t" << stats[i].date << "\t" << stats[i].difficulty << "\t"
             << stats[i].totalScore << "\t" << stats[i].timeTaken << " sec\n";
    }
    // คันด้วยกดปุ่มEnter
    char Wait;
    cin.get(Wait);
    cout << "\nPress Enter to continue";
    cin.get(Wait);
}

// ฟังก์ชันแสดงอันดับตามเวลา
void displayRankingsByTime()
{
    // เรียกใช้คำสั่ง
    clearScreen();
    vector<GameStat> stats = loadStatistics();
    if (stats.empty())
    {
        cout << "No statistics to display.\n";
        // คันด้วยกดปุ่มEnter
        char Wait;
        cin.get(Wait);
        cout << "\nPress Enter to continue";
        cin.get(Wait);
        return;
    }

    // เรียงลำดับตามเวลา
    sort(stats.begin(), stats.end(), compareByTime);

    // แสดงอันดับตามเวลา
    string line(60, '=');
    cout << "\n===== Ranking by Time Taken (Fastest) =====\n";
    cout << line;
    cout << "\nRank\tDate\t\t\tDifficulty\tScore\tTime\n";
    cout << line << endl;
    for (size_t i = 0; i < stats.size(); ++i)
    {
        cout << (i + 1) << "\t" << stats[i].date << "\t" << stats[i].difficulty << "\t"
             << stats[i].totalScore << "\t" << stats[i].timeTaken << " sec\n";
    }
    // คันด้วยกดปุ่มEnter
    char Wait;
    cin.get(Wait);
    cout << "\nPress Enter to continue";
    cin.get(Wait);
}

// ฟังก์ชันรีเซ็ตอันดับ (ลบไฟล์ game_stats.txt)
void resetRankings()
{
    string choiseDelete;
    cout << "Do you want to delete your data? (Yes :: No): ";
    cin >> choiseDelete;
    transform(choiseDelete.begin(), choiseDelete.end(), choiseDelete.begin(), ::tolower); // แปลงตัวอักษรเป็นตัวพิมพ์เล็ก

    if (choiseDelete == "yes" || choiseDelete == "y")
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
    else
    {
        cout << "Operation cancelled.\n";
    }

    // คันด้วยกดปุ่มEnter
    char Wait;
    cin.get(Wait);
    cout << "\nPress Enter to continue";
    cin.get(Wait);
}

// คำอธิบายเกม
void gameDescription()
{
    // เรียกใช้คำสั่ง
    clearScreen();
    string line(100, '=');
    cout << line << endl;
    cout << "Game Description" << endl;
    cout << "Game Name: 3-Minute Vocabulary Guessing" << endl;
    cout << "Objective:" << endl;
    cout << "Players must guess the hidden words based on the chosen question's " << endl;
    cout << "difficulty level (easy, medium, hard). The goal is to answer as many " << endl;
    cout << "questions correctly as possible within the given time limit to achieve " << endl;
    cout << "the highest total score." << endl;
    cout << "Scoring System:" << endl;
    cout << "Points for Correct Answers: When a player answers a question correctly," << endl;
    cout << "they will receive 10 points for each correct answer." << endl;
    cout << "Points Deducted for Incorrect Answers: If a player answers a question incorrectly," << endl;
    cout << "5 points will be deducted for each wrong answer." << endl;
    cout << line << endl;
    // คันด้วยกดปุ่มEnter
    char Wait;
    cin.get(Wait);
    cout << "\nPress Enter to continue";
    cin.get(Wait);
}

// คำสั่งล้างหน้าจอ
void clearScreen()
{
#ifdef _WIN32
    system("cls"); // ใช้ cls บน Windows
#else
    system("clear"); // ใช้ clear บน Linux และ macOS
#endif
}

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

    string line;
    GameStat stat;
    while (getline(statFile, line))
    {
        if (line.find("Date:") != string::npos)
        {
            stat.date = line.substr(6); // ตัด "Date: " ออก  ฟังก์ชัน find() จะค้นหาสตริง "Date:" ภายในสตริง lineถ้าพบ "Date:" จะคืนค่าตำแหน่ง (index) ที่เริ่มต้นของสตริง "Date:" ใน line.ถ้าไม่พบ จะคืนค่า string::npos, ซึ่งเป็นค่าคงที่ที่แสดงถึงตำแหน่งที่ไม่ถูกต้อง
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