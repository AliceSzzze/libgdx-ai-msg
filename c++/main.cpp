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
        csvFile << row.measuredDelayMicros <<","<< row.expectedDelay<< "\n";
    }
}

int main() {
    MessageDispatcher dispatcher;

    int numMailboxes = 500;
    int numTelegraphs = 2000;
    int maxDelay = 500;
    int maxSubscriptions = 100;
    int duration = 10;

    for (int i = 0; i < numMailboxes; ++i) {
//        std::cout <<"adding mailbox "<<i <<"\n";
        dispatcher.addMailbox(i);
    }

    std::vector<std::shared_ptr<Telegraph>> telegraphs(numTelegraphs);
    for (int i = 0; i < numTelegraphs; ++i) {
        telegraphs[i] = std::make_shared<Telegraph>();

        int numSubs = getRandomInteger(0, maxSubscriptions);
        for (int s = 0; s < numSubs; ++s) {
            int msg = getRandomInteger(0, numMailboxes-1);
            int delay = getRandomInteger(16, maxDelay);
            dispatcher.addListener(telegraphs[i], msg, delay);
        }
    }

    auto start = std::chrono::steady_clock::now();
    long long diff;
    int numMsgs = 0;
    int unsubs = 0;
    do {
        auto now = std::chrono::steady_clock::now();
        diff = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        if (getRandomInteger(0, 2000) == 1) {
            int msg = getRandomInteger(0, numMailboxes-1);
            dispatcher.dispatchMessage(msg);
            numMsgs++;
        }

        if (getRandomInteger(0, 1000) == 1) {
            int msg = getRandomInteger(0, numMailboxes-1);
            int t = getRandomInteger(0, numTelegraphs-1);
            unsubs += dispatcher.removeListener(telegraphs[t], msg);
        }

//        if (getRandomInteger(0, 200) == 1) {
//            int msg = getRandomInteger(0, numMailboxes-1);
//            dispatcher.removeMailbox(msg);
//        }
        dispatcher.update();
    } while (diff < duration);

    std::vector<Delay> totalDelays;
    for (const auto&[msg, mailbox]: dispatcher.mailboxes) {
        totalDelays.insert(totalDelays.end(), mailbox->measuredDelays.begin(), mailbox->measuredDelays.end());
    }

    std::string filename = "exp16.csv";
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
    csvFile << "number of unsubscriptions: " << unsubs << "\n";
    csvFile << "latency,delay\n";
    writeToCSV(csvFile, totalDelays);
    csvFile.close();

}

//int main() {
//    MessageDispatcher dispatcher;
//
//    dispatcher.addMailbox(0);
//
//    auto t = std::make_shared<Telegraph>();
//
//    dispatcher.addListener(t, 0, 0);
//
//    std::shared_ptr<void> extraInfo(
//            new int(42),
//            [](void* p) {
//                // Custom deleter function to delete the correct type
//                delete static_cast<int*>(p);
//            }
//    );
//
//    dispatcher.dispatchMessage(0, extraInfo);
//}

