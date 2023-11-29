#include <iostream>
#include "MessageDispatcher.h"
#include <random>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>

int getRandomInteger(int min, int max) {
    std::random_device rd;  // A random device produces a random seed
    std::mt19937 gen(rd());  // Mersenne Twister 19937 generator with a random seed
    std::uniform_int_distribution<int> distribution(min, max);  // Distribution for integers between min and max

    return distribution(gen);  // Generate a random integer
}

void writeToCSV(std::ofstream& csvFile, const std::vector<Delay>& data) {
    // Write data to the CSV file
    for (const auto& row : data) {
        csvFile << row.measuredDelay <<","<< row.expectedDelay<< "\n";
    }
}

int main() {
    MessageDispatcher dispatcher;

    int numMailboxes = 100;
    int numTelegraphs = 100;
    int maxDelay = 500;
    int maxSubscriptions = 5;
    int duration = 3;
//    int messageIntensity = 5.0;

    for (int i = 0; i < numMailboxes; ++i) {
        dispatcher.addMailbox(i);
    }

    std::vector<Telegraph> telegraphs(numTelegraphs);
    for (int i = 0; i < numTelegraphs; ++i) {
        telegraphs[i] = Telegraph();

        int numSubs = getRandomInteger(0, maxSubscriptions);
        for (int s = 0; s < numSubs; ++s) {
            int msg = getRandomInteger(0, numMailboxes);
            int delay = getRandomInteger(16, maxDelay);
            dispatcher.addListener(telegraphs[i], msg, delay);
        }
    }

    auto start = std::chrono::system_clock::now();
    long long diff;
    int numMsgs = 0;
    do {
        auto now = std::chrono::system_clock::now();
        diff = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (getRandomInteger(0, 50000) == 1) {
            int msg = getRandomInteger(0, numMailboxes);
            dispatcher.dispatchMessage(msg);
            numMsgs++;
        }

        dispatcher.update();
    } while (diff < duration);

    std::vector<Delay> totalDelays;
    for (const auto&[msg, mailbox]: dispatcher.mailboxes) {
        totalDelays.insert(totalDelays.end(), mailbox.measuredDelays.begin(), mailbox.measuredDelays.end());
    }

    std::string filename = "exp13.csv";
    std::ofstream csvFile(filename);

    if (!csvFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
    }

    csvFile << "number of mailboxes: " << numMailboxes << "\n";
    csvFile << "number of telegraphs: " << numTelegraphs << "\n";
    csvFile << "maximum delay: " << maxDelay << "\n";
    csvFile << "maximum subscription: " << maxSubscriptions << "\n";
    csvFile << "duration: " << duration << "\n";
    csvFile << "number of telegrams sent: "<< numMsgs << "\n";
    csvFile << "received at,latency,delay\n";
    writeToCSV(csvFile, totalDelays);
    csvFile.close();

}
