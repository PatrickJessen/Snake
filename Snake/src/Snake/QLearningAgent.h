#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <unordered_map>
#include "Game.h"

template<typename T>
class QLearningAgent {
public:
    QLearningAgent(int size, double learningRate, double discountFactor, double epsilon) :
        size(size), learningRate(learningRate), discountFactor(discountFactor), epsilon(epsilon), dist_action(0, 3) {
        qTable.resize(size * size * 50, std::vector<double>(4, 0.0)); // Preallocate memory for Q-table
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
        int stateIndex = stateToIndex(state);
        int newStateIndex = stateToIndex(newState);

        // Update Q-value
        double oldQValue = qTable[stateIndex][static_cast<int>(action)];
        double maxNextQValue = (newStateIndex < qTable.size()) ? *std::max_element(qTable[newStateIndex].begin(), qTable[newStateIndex].end()) : 0.0;
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

private:
    int stateToIndex(State state) {
        /*int index = state.headY * size + 4 + state.headX;
        index = index * size + 4 + static_cast<int>(state.direction);*/
        //index = index * size + 4 + state.distanceToApple;
        int index = size + 4 + static_cast<int>(state.foodDirection);
        //index = index * size + 4 + static_cast<int>(state.nearestWall);
        index = index * size + (int)state.wallDown + (int)state.wallUp + (int)state.wallLeft + (int)state.wallRight;
        index = index * size + 1 + (int)state.bodyInfront;
        if (index < 0) {
            return 0;
        }
        return index;
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
};