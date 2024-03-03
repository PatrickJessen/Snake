#include <vector>
#include <cmath>
#include <random>
#include "Game.h"

struct Transition {
    State state;
    Direction action;
    double reward;
    State next_state;
};

// Define a simple neural network for Q-value approximation
class NeuralNetwork {
public:
    NeuralNetwork(int input_size, int hidden_size, int output_size)
        : input_size(input_size), hidden_size(hidden_size), output_size(output_size) {
        // Initialize weights and biases
        // Random initialization
        // For simplicity, let's assume a single hidden layer
        weights_input_hidden = initialize_weights(input_size, hidden_size);
        biases_input_hidden = initialize_biases(hidden_size);
        weights_hidden_output = initialize_weights(hidden_size, output_size);
        biases_hidden_output = initialize_biases(output_size);
    }

    std::vector<double> forward_pass(const std::vector<double>& input) {
        // Perform forward pass
        std::vector<double> hidden_layer(hidden_size);
        std::vector<double> output_layer(output_size);

        // Compute hidden layer activations
        for (int i = 0; i < hidden_size; ++i) {
            hidden_layer[i] = 0.0;
            for (int j = 0; j < input_size; ++j) {
                hidden_layer[i] += input[j] * weights_input_hidden[j][i];
            }
            hidden_layer[i] += biases_input_hidden[i];
            hidden_layer[i] = activation_function(hidden_layer[i]);
        }

        // Compute output layer activations
        for (int i = 0; i < output_size; ++i) {
            output_layer[i] = 0.0;
            for (int j = 0; j < hidden_size; ++j) {
                output_layer[i] += hidden_layer[j] * weights_hidden_output[j][i];
            }
            output_layer[i] += biases_hidden_output[i];
            output_layer[i] = activation_function(output_layer[i]);
        }

        return output_layer;
    }

    void backward_pass(const std::vector<double>& loss_gradient, double learning_rate, const std::vector<double>& input) {
        // Perform backward pass (backpropagation) to update weights and biases
        // Adjust weights and biases using gradient descent

        // Update weights and biases between hidden and output layers
        for (int i = 0; i < hidden_size; ++i) {
            for (int j = 0; j < output_size; ++j) {
                weights_hidden_output[i][j] -= learning_rate * loss_gradient[j] * hidden_layer_activation[i];
            }
        }

        // Update weights and biases between input and hidden layers
        for (int i = 0; i < input_size; ++i) {
            for (int j = 0; j < hidden_size; ++j) {
                double sum_gradient = 0.0;
                for (int k = 0; k < output_size; ++k) {
                    sum_gradient += loss_gradient[k] * weights_hidden_output[j][k];
                }
                // Update weights using the computed gradients and learning rate
                weights_input_hidden[i][j] -= learning_rate * sum_gradient * input[i] * hidden_layer_activation[j] * (1.0 - hidden_layer_activation[j]);
            }
        }
    }

private:
    int input_size; // Input layer size
    int hidden_size; // Hidden layer size
    int output_size; // Output layer size

    std::vector<std::vector<double>> weights_input_hidden; // Weights between input and hidden layers
    std::vector<std::vector<double>> weights_hidden_output; // Weights between hidden and output layers
    std::vector<double> biases_input_hidden; // Biases for hidden layer
    std::vector<double> biases_hidden_output; // Biases for output layer

    std::vector<double> hidden_layer_activation; // Activations of hidden layer neurons

    std::vector<std::vector<double>> initialize_weights(int rows, int cols) {
        // Initialize weights with random values
        std::vector<std::vector<double>> weights(rows, std::vector<double>(cols));
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dis(0.0, 0.1);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                weights[i][j] = dis(gen); // Initialize weights with Gaussian distribution
            }
        }
        return weights;
    }

    std::vector<double> initialize_biases(int size) {
        // Initialize biases with random values
        std::vector<double> biases(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> dis(0.0, 0.1);
        for (int i = 0; i < size; ++i) {
            biases[i] = dis(gen); // Initialize biases with Gaussian distribution
        }
        return biases;
    }

    double activation_function(double x) {
        // Activation function (e.g., sigmoid)
        return 1.0 / (1.0 + exp(-x));
    }
};

// Define the Deep Q-Network (DQN) class
class DQN {
public:
    DQN(int num_states, int num_actions, double learning_rate, double discount_factor, double epsilon, int hidden_size)
        : num_states(num_states), num_actions(num_actions), learning_rate(learning_rate), discount_factor(discount_factor), epsilon(epsilon) {
        // Initialize neural network
        neural_net = new NeuralNetwork(num_states, hidden_size, num_actions);
    }

    Direction choose_action(const State& state) {
        if (rand_uniform(0.0, 1.0) < epsilon) {
            return random_action();
        }
        else {
            std::vector<double> q_values = neural_net->forward_pass(state_to_input(state));
            return argmax(q_values);
        }
    }

    void update(const Transition& transition) {
        // Convert state and next state to neural network input format
        std::vector<double> state_input = state_to_input(transition.state);
        std::vector<double> next_state_input = state_to_input(transition.next_state);

        // Forward pass to get Q-values for current state and next state
        std::vector<double> q_values = neural_net->forward_pass(state_input);
        std::vector<double> next_q_values = neural_net->forward_pass(next_state_input);

        // Calculate target Q-value using the Bellman equation
        double target = transition.reward + discount_factor * max(next_q_values);

        // Calculate loss gradient for the action taken
        std::vector<double> loss_gradient(q_values.size(), 0.0);
        loss_gradient[(int)transition.action] = q_values[(int)transition.action] - target;

        // Perform backward pass (backpropagation) to update neural network parameters
        neural_net->backward_pass(loss_gradient, learning_rate, state_to_input(transition.state));
    }

private:
    int num_states;
    int num_actions;
    double learning_rate;
    double discount_factor;
    double epsilon;

    NeuralNetwork* neural_net;

    Direction random_action() {
        // Generate random action
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(0, num_actions - 1);
        int rand_action_index = dis(gen);
        Direction rand_action;
        rand_action = (Direction)rand_action_index;
        return rand_action;
    }

    double rand_uniform(double min, double max) {
        // Generate random number between min and max
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(min, max);
        return dis(gen);
    }

    std::vector<double> state_to_input(const State& state) {
        // Convert state to neural network input format
        std::vector<double> input;

        // Normalize coordinates and other relevant features if necessary
        double normalized_snake_head_x = state.headX / (double)width;
        double normalized_snake_head_y = state.headY / (double)height;

        // Add normalized coordinates and other features to input vector
        input.push_back(normalized_snake_head_x);
        input.push_back(normalized_snake_head_y);

        // Add additional features as needed
        // For example, you might include the direction of movement, presence of obstacles, etc.

        return input;
    }

    Direction argmax(const std::vector<double>& values) {
        // Find index of maximum value in vector
        double max_value = values[0];
        int max_index = 0;
        for (int i = 1; i < values.size(); ++i) {
            if (values[i] > max_value) {
                max_value = values[i];
                max_index = i;
            }
        }
        Direction max_action;
        max_action = (Direction)max_index;
        return max_action;
    }

    double max(const std::vector<double>& values) {
        // Find maximum value in vector
        double max_value = values[0];
        for (int i = 1; i < values.size(); ++i) {
            if (values[i] > max_value) {
                max_value = values[i];
            }
        }
        return max_value;
    }
};
