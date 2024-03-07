#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include "Game.h"

#define FILENAME = "test.txt";

struct StateHash {
    std::size_t operator()(const State& state) const {
        // Combine hash values of individual components
        std::size_t hash = 0;
        hash_combine(hash, std::hash<int>{}(static_cast<int>(state.direction)));
        hash_combine(hash, std::hash<int>{}(static_cast<int>(state.foodDirection)));
        hash_combine(hash, std::hash<int>{}(static_cast<int>(state.body)));
        hash_combine(hash, std::hash<int>{}(static_cast<int>(state.wall)));

        //hash_combine(hash, std::hash<int>{}(static_cast<int>(state.foodDirection)));
        //hash_combine(hash, std::hash<int>{}(static_cast<int>(state.bodyInfront)));
        return hash;
    }


    void hash_combine(std::size_t& seed, const int& v) const {
        std::hash<int> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
};

template<typename T>
class QLearningAgent {
public:
    QLearningAgent(int size, double learningRate, double discountFactor, double epsilon) :
        size(size), learningRate(learningRate), discountFactor(discountFactor), epsilon(epsilon), dist_action(0, 3) {
        ReadTrainingData("test.txt");
        ReadStateIndexMap("test2.txt");
        if (qTable.empty()) {
            qTable.resize(size * size * 100, std::vector<double>(4, 0.0)); // Preallocate memory for Q-table
        }
        rng.seed(std::random_device()()); // Seed random number generator
    }

    // Function to choose an action using epsilon-greedy strategy
    T chooseAction(State state) {
        if (dist(rng) < epsilon) {
            // Explore: Choose a random action
            return static_cast<T>(dist_action(rng));
        }
        else {
            // Exploit: Choose the action with the highest Q-value
            return static_cast<T>(std::distance(qTable[stateToIndex(state)].begin(),
                std::max_element(qTable[stateToIndex(state)].begin(), qTable[stateToIndex(state)].end())));
        }
    }

    // Function to update Q-value based on Bellman equation
    void updateQValue(State state, T action, double reward, State newState) {
        if (epsilon > minEpsilon) {
            epsilon -= epsilonDecay;
        }
        int stateIndex = stateToIndex(state);
        int newStateIndex = stateToIndex(newState);

        // Update Q-value
        double oldQValue = qTable[stateIndex][static_cast<int>(action)];
        double maxNextQValue = (newStateIndex != -1) ? *std::max_element(qTable[newStateIndex].begin(), qTable[newStateIndex].end()) : 0.0;
        double newQValue = oldQValue + learningRate * (reward + discountFactor * maxNextQValue - oldQValue);

        qTable[stateIndex][static_cast<int>(action)] = newQValue;
    }

    // Get Q-value for a state-action pair
    double getQValue(const State& state, int action) const {
        return qTable[stateToIndex(state)][action];
    }

    void linearDecay(double initialEpsilon, double minEpsilon, double decayRate, int episode) {
        epsilon = std::max(initialEpsilon - decayRate * episode, minEpsilon);
    }

    void setEpsilon(double val)
    {
        epsilon = val;
    }

    void setLearningRate(double val)
    {
        learningRate = val;
    }

    void SaveTraining(const std::string& fileName)
    {
        std::ofstream file(fileName);
        if (file.is_open()) {
            for (const auto& f : qTable) {
                for (const auto& s : f) {
                    file << s << " ";
                }
                file << "\n";
            }
        }
        file.close();
    }

    void ReadTrainingData(const std::string& fileName)
    {
        std::ifstream file(fileName);
        std::string line;
        while (std::getline(file, line)) {
            std::vector<double> innerVector;
            std::stringstream ss(line);
            double value;
            while (ss >> value) {
                innerVector.push_back(value);
            }
            qTable.push_back(innerVector);
        }

        file.close();
    }

    void SaveStateIndexMap(const std::string& filename)
    {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto& entry : stateIndexMap) {
                file << (int)entry.first.body << " " << (int)entry.first.direction << " " << (int)entry.first.wall << " " << (int)entry.first.foodDirection << " " << entry.second << std::endl;
            }
        }
        file.close();
    }

    void ReadStateIndexMap(const std::string& filename)
    {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                State state;
                int value;
                int dir;
                int fooddir;
                int wall;
                int body;
                if (iss >> body >> dir >> wall >> fooddir >> value) {
                    state.body = (Danger)body;
                    state.wall = (Danger)wall;
                    state.direction = (Direction)dir;
                    state.foodDirection = (Direction)fooddir;
                    stateIndexMap[state] = value;
                }
            }
        }
    }

private:
    int stateToIndex(State state) {
        // Check if state already exists in the map
        auto it = stateIndexMap.find(state);
        if (it != stateIndexMap.end()) {
            // Return the index if state is found
            return it->second;
        }
        else {
            // Otherwise, assign a new index and return
            int newIndex = stateIndexMap.size();
            stateIndexMap[state] = newIndex;
            return newIndex;
        }
    }

    

private:
    std::vector<std::vector<double>> qTable;
    double learningRate;
    double discountFactor;
    double epsilon;
    std::mt19937 rng; // Random number generator
    int size;
    std::uniform_real_distribution<double> dist{ 0.0, 1.0 };
    std::uniform_int_distribution<int> dist_action; // Distribution for choosing action
    std::unordered_map<State, int, StateHash> stateIndexMap;
    double minEpsilon = 0.1;
    double epsilonDecay = 0.000001;
};