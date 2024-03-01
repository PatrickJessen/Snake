#pragma once
#include <vector>
#include <random>

template<typename T>
class QLearningAgent {
public:
    QLearningAgent(int size, double learningRate, double discountFactor, double epsilon) :
        size(size), learningRate(learningRate), discountFactor(discountFactor), epsilon(epsilon) {
        // Initialize Q-table with zeros
        qTable.resize(size * size * size, std::vector<double>(4, 0.0));
        // Seed the random number generator
        rng.seed(std::random_device()());
    }

    // Function to choose an action using epsilon-greedy strategy
    T chooseAction(int state) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        if (dist(rng) < epsilon) {
            // Random action with probability epsilon
            return static_cast<T>(dist(rng) * 4);
        }
        else {
            // Greedy action with probability 1 - epsilon
            double maxQValue = qTable[state][0];
            T maxAction = static_cast<T>(0);
            for (int i = 1; i < 4; ++i) {
                if (qTable[state][i] > maxQValue) {
                    maxQValue = qTable[state][i];
                    maxAction = static_cast<T>(i);
                }
            }
            return maxAction;
        }
    }

    // Function to update Q-value based on Bellman equation
    void updateQValue(int state, T action, double reward, int newState) {
        double maxQValue = qTable[newState][0];
        for (int i = 1; i < 4; ++i) {
            if (qTable[newState][i] > maxQValue) {
                maxQValue = qTable[newState][i];
            }
        }
        qTable[state][static_cast<int>(action)] += learningRate * (reward + discountFactor * maxQValue - qTable[state][static_cast<int>(action)]);
    }

private:
    std::vector<std::vector<double>> qTable;
    double learningRate;
    double discountFactor;
    double epsilon;
    std::mt19937 rng; // Random number generator
    int size;
};