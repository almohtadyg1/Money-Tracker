#include <iostream>
#include <map>
#include <vector>
#include <filesystem>
#include <cctype>
#include <string>
#include <algorithm>

#include "encrypter.h"
#include "savingFunctions.h"


const std::string DATA_FILE = "saves.data";



bool isNumberOnly(const std::string& str) {
    for (char ch : str) {
        if (!isdigit(ch)) return false;
    }
    return true;
}



std::string input(const char* question = nullptr) {
    if (question) {
        std::cout << question << std::flush;
    }

    std::string word;
    std::getline(std::cin, word);

    while (word.back() == ' ') {
        word.pop_back();
    }

    return word;
}



std::string lowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return str;
}



bool askForReset() {
    std::string resetAnswerLowered = lowercase(input("Would You Like To Reset Money Tracker?\nWarning! If Reset All Data Will Be Lost.\nReset? <Y/N>: "));
    if (resetAnswerLowered == "y" || resetAnswerLowered == "yes" ? true : false) {
        if (std::filesystem::remove(DATA_FILE)) {
            std::cout << "Reset Completed.\nReopen Money Tracker To A Fresh Start!";
            return true;
        }
        else {
            std::cout << "saves.data not found or could not be deleted.\n";
            return false;
        }
    }
    else {
        std::cout << "Didn't Reset, Data Still Saved";
        return false;
    }
}


// console is just temp until GUI


int main() {
    bool readErr = false;
    std::string saves = loadFile(DATA_FILE, readErr);

    if (!readErr) {
        std::cout << "Welcome to Money Tracker!\n";
        std::string key = input("Password: ");
        if (key.length() <= 32) {
            key.append(32 - key.length(), '*');
        }
        else {
            std::cout << "Max Password Size is 32!" << std::endl;
            return 1;
        }

        std::string decrypted = decryptAesCng(saves, key);

        std::string suffix = "valid";
        if (decrypted.size() >= suffix.size() &&
            decrypted.compare(decrypted.size() - suffix.size(), suffix.size(), suffix) == 0) {

            decrypted.erase(decrypted.size() - suffix.size());
        }
        else {
            std::cout << "Incorrect Password!!!\n";
            
            askForReset();

            return 1;
        }

        // remove the pad
        while (decrypted.back() == '*') {
            decrypted.pop_back();
        }

        std::map<std::string, std::string> dataMap;
        std::map<std::string, std::string> borrowersMap;
        std::vector<std::string> orderVector;

        stringToData(decrypted, dataMap, borrowersMap, orderVector);

        std::string choice, tempString, tempString1, tempString2;
        do {
            std::cout << "Choose:\n"
                << "1-  View Data\n"
                << "2-  Make A Transaction\n"
                << "3-  Write A Short Note\n"
                << "4-  Add/Edit A Borrower/Lender\n"
                << "5-  Save and Exit\n"
                << "6-  Exit Only\n"
                << "99- RESET and Exit\n";

            choice = input(">>> ");

            if (choice == "1") {
                std::cout << "\nData:\n";
                for (int i = 0; i < 3; i++) {
                    std::cout << orderVector[i] << ": " << dataMap[orderVector[i]] << "\n";
                }
                std::cout << "\nBorrowers/Lenders Data:";
                if (orderVector.size() != 3) {
                    for (size_t i = 3; i < orderVector.size(); i++) {
                        std::cout << "\n" << orderVector[i] << ": " << borrowersMap[orderVector[i]];
                    }
                }
                else {
                    std::cout << " <NONE>";
                }
                std::cout << "\n\n";
                
            }
            else if (choice == "2") {
                tempString = lowercase(input("Transaction Type (+/-): "));
                tempString1 = input("Transaction Value: ");
                if (tempString == "" || tempString1 == "") {
                    std::cout << "Invalid Input!\n";
                    continue;
                }
                if (!isNumberOnly(tempString1) || tempString1.length() > 18) {
                    std::cout << "Invalid Input!\n";
                    continue;
                }

                if (tempString == "+" || tempString == "p" || tempString == "positive") {
                    dataMap["Last Transaction"] = tempString1;
                    dataMap["Total Money"] = std::to_string(std::stoll(dataMap["Total Money"]) + std::stoll(tempString1));
                }
                else if (tempString == "-" || tempString == "n" || tempString == "negative") {
                    dataMap["Last Transaction"] = "-" + tempString1;
                    dataMap["Total Money"] = std::to_string(std::stoll(dataMap["Total Money"]) - std::stoll(tempString1));
                }
                else {
                    std::cout << "Invalid Input!\n";
                    continue;
                }
                std::cout << "New Total Money value: " << dataMap["Total Money"] << "\n";
                
                tempString = "";
                tempString1 = "";
            }
            else if (choice == "3") {
                dataMap["Short Note"] = input("Short Note: ");
                
                std::cout << "Short Note Saved!\n";
            }
            else if (choice == "4") {
                std::cout << "\nBorrowers/Lenders Data:";
                if (orderVector.size() != 3) {
                    for (size_t i = 3; i < orderVector.size(); i++) {
                        std::cout << "\n" << orderVector[i] << ": " << borrowersMap[orderVector[i]];
                    }
                    std::cout << "\nNote: Positive number = You Borrowed\n"
                        << " Negative number = He/She Borrowed";
                }
                else {
                    std::cout << " <NONE>";
                }
                std::cout << "\n\n";

                tempString = lowercase(input("Who Took Money From The Other?\n1- You\n2- Him/Her\nanswer: "));
                tempString1 = input("His/Her Name: ");
                tempString2 = input("Borrowed Value: ");
                if (tempString == "" || tempString1 == "" || tempString2 == "") {
                    std::cout << "Invalid Input!\n";
                    continue;
                }
                if (!isNumberOnly(tempString2) || tempString2.length() > 18) {
                    std::cout << "Invalid Input!\n";
                    continue;
                }

                if (tempString == "1" || tempString == "you" || tempString == "me") {
                    if (orderVector.size() > 3 && std::find(orderVector.begin() + 3, orderVector.end(), tempString1)
                            != orderVector.end()) {
                        borrowersMap[tempString1] = std::to_string(std::stoll(borrowersMap[tempString1]) + std::stoll(tempString2));
                    }
                    else {
                        borrowersMap[tempString1] = tempString2;
                        orderVector.push_back(tempString1);
                    }
                    dataMap["Total Money"] = std::to_string(std::stoll(dataMap["Total Money"]) + std::stoll(tempString2));
                }
                else if (tempString == "2" || tempString == "him" || tempString == "her") {
                    if (orderVector.size() > 3 && std::find(orderVector.begin() + 3, orderVector.end(), tempString1)
                        != orderVector.end()) {
                        borrowersMap[tempString1] = std::to_string(std::stoll(borrowersMap[tempString1]) - std::stoll(tempString2));
                    }
                    else {
                        borrowersMap[tempString1] = "-" + tempString2;
                        orderVector.push_back(tempString1);
                    }
                    dataMap["Total Money"] = std::to_string(std::stoll(dataMap["Total Money"]) - std::stoll(tempString2));
                }
                else {
                    std::cout << "Invalid Input!\n";
                }

                tempString = "";
                tempString1 = "";
                tempString2 = "";
                std::cout << "Data Saved!\n";
            }
            else if (choice == "5") {
                std::string dataString = dataToString(dataMap, borrowersMap, orderVector);
                // padding data so it's a multiple of 16 with '*'
                int remainder = dataString.length() % 16;
                if (remainder != 0) {
                    int starCount = 16 - remainder;
                    starCount = (starCount < 5 ? starCount + 11 : starCount - 5);
                    dataString.append(starCount, '*');
                }
                dataString += "valid";
                std::string encrypted = encryptAesCng(dataString, key);

                if (saveToFile(encrypted, DATA_FILE)) {
                    std::cout << "Unable to open file for writing.\n";
                    return 1;
                }

                return 0;
            }
            else if (choice == "6") return 0;
            else if (choice == "99") {
                if (askForReset()) return 0;
            }
            else {
                std::cout << "Invalid Input!";
            }

            std::cout << "\n";
        }
        while (true);

    }
    else {
        readErr = false;
        std::cout << "Welcome to Money Tracker!\n"
            << "Warning! Don't forget the password as your saved data would be unretrievable.\n";

        std::string key = input("Password (32 characters is max): "); // 32 characters
        if (key.length() <= 32) {
            key.append(32 - key.length(), '*');
        }
        else {
            std::cout << "Max Password Size is 32!" << std::endl;
            return 1;
        }

        std::string totalMoney = input("Current Total Money: ");
        if (!isNumberOnly(totalMoney) || totalMoney.length() > 18) {
            std::cout << "Invalid Input!\n";
            return 1;
        }

        std::map<std::string, std::string> dataMap = {
            {"Total Money", (totalMoney == "" ? "0" : totalMoney)},
            {"Last Transaction", "-"},
            {"Short Note", "-"}
        };
        std::map<std::string, std::string> borrowersMap;
        std::vector<std::string> orderVector = { "Total Money", "Last Transaction", "Short Note" };

        std::string dataString = dataToString(dataMap, borrowersMap, orderVector);
        // padding data so it's a multiple of 16 with '*'
        int remainder = dataString.length() % 16;
        if (remainder != 0) {
            int starCount = 16 - remainder;
            starCount = (starCount < 5 ? starCount + 11 : starCount-5);
            dataString.append(starCount, '*');
        }
        dataString += "valid";
        std::string encrypted = encryptAesCng(dataString, key);

        if (saveToFile(encrypted, DATA_FILE)) {
            std::cout << "Unable to open file for writing.\n";
            return 1;
        }
    }

    return 0;
}
