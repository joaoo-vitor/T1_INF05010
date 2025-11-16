#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <optional>
#include <algorithm>
#include <iomanip> 
#include <numeric>  
#include <tuple>    
#include <utility>  
#include <chrono> 
#include <set>
#include <stdexcept> 
#include <unordered_set>

using namespace std;

// Player struct 
struct Player {
    int id;
    int salary;
    vector<int> conflicts; // Players id wh have a conflict
};

// Instance struct
struct ProblemInstance {
    int J; // players number
    int I; // Conflicts number
    int B; // Budget per team
    vector<Player> players;
};

// Team struct
struct Team {
    int total_salary = 0;
    unordered_set<int> player_ids_set; // Quickly checks conflicts
    vector<int> player_ids_list;

    // Checks if a player can join a team
    bool CanAddPlayer(const Player& player, int budget_B) const {
        
        // Check the budget
        if (total_salary + player.salary > budget_B) {
            return false;
        }

        // Check the coonflicts
        for (int conflicted_player_id : player.conflicts) {
            if (player_ids_set.count(conflicted_player_id)) { 
                return false; // Returns false if a player with a conflict is already on the team
            }
        }
        
        return true;
    }

    // Clean the team
    void Clear() {
        total_salary = 0;
        player_ids_set.clear();
        player_ids_list.clear();
    }

    // Add a valid player to the team
    void AddPlayer(const Player& player) {
        total_salary += player.salary;
        player_ids_set.insert(player.id);
        player_ids_list.push_back(player.id);
    }
};

// Solution struct
struct Solution {
    vector<Team> teams;
};

ProblemInstance read_instance(const string &filename) {
    ProblemInstance instance;
    
    // Open file
    ifstream in(filename);
    if (!in.is_open()) {
        throw runtime_error("Cannot open the file " + filename);
    }

    // Read budget B
    in >> instance.B;

    // Read J and I
    in >> instance.J >> instance.I;

    //Resize the vector to fit all players
    instance.players.resize(instance.J);

    // Read salaries (lines 3 - 2+J)
    for (int i = 0; i < instance.J; i++) {
        instance.players[i].id = i; // Define o ID (base 0)
        in >> instance.players[i].salary; // Lê o salário da linha
    }

    // Read conflicts (lines 2+J+1 - 2+J+I)
    for (int k = 0; k < instance.I; k++) {
        int u, v;
        in >> u >> v; // Ids based on 1

        // Convert ids to base 0
        u--; 
        v--;

        // Add conflicts to both players lists
        instance.players[u].conflicts.push_back(v);
        instance.players[v].conflicts.push_back(u);
    }

    in.close(); 
    return instance;
}

// Build the initial solution using a first-fit strategy with random order
Solution build_initial_solution(const ProblemInstance& instance, mt19937& g) {
    Solution solution;

    // Create a players id list
    vector<int> player_order(instance.J);
    iota(player_order.begin(), player_order.end(), 0);

    // Shuffle the players randomly
    shuffle(player_order.begin(), player_order.end(), g);

    for (int player_id : player_order) {
        const Player& player = instance.players[player_id];
        bool allocated = false;

        // Try to allocate it to the first valid team
        for (Team& team : solution.teams) {
            if (team.CanAddPlayer(player, instance.B)) {
                team.AddPlayer(player);
                allocated = true;
                break;
            }
        }

        // Create a new team if it cannot be allocated 
        if (!allocated) {
            Team new_team;
            new_team.AddPlayer(player);
            solution.teams.push_back(new_team);
        }
    }

    return solution;
}


void local_search(const ProblemInstance& instance, Solution& solution) {
    bool improved = true;

    // Repeat local search while improving the solution (to dissolve teams)
    while (improved) {
        improved = false;

        // Create a players id list
        vector<int> team_indices(solution.teams.size());
        iota(team_indices.begin(), team_indices.end(), 0);

        // Order in ascending order to dissolve smaller teams first 
        sort(team_indices.begin(), team_indices.end(), [&](int a, int b) {
            return solution.teams[a].player_ids_list.size() < 
                   solution.teams[b].player_ids_list.size();
        });

        // Try to dissolve a team
        for (int team_index_to_empty : team_indices) {
            
            Team& team_to_empty = solution.teams[team_index_to_empty];
            if (team_to_empty.player_ids_list.empty()) {
                continue; // Empty team
            }

            // Identify team players to move
            vector<int> players_to_move = team_to_empty.player_ids_list;
            
            // Stores player movements for new teams
            vector<pair<int, int>> proposed_moves;

            // Verify if the team was dissolved
            bool all_players_can_move = true;

            // Create a new solution with players moved
            vector<Team> hypothetical_teams = solution.teams;
            
            // Try to move each player
            for (int player_id : players_to_move) {
                const Player& player_obj = instance.players[player_id];
                bool found_spot = false;

                // Try to find a new team
                for (int k = 0; k < hypothetical_teams.size(); ++k) {
                    if (k == team_index_to_empty) {
                        continue; // Same team
                    }

                    if (hypothetical_teams[k].CanAddPlayer(player_obj, instance.B)) {
                        hypothetical_teams[k].AddPlayer(player_obj); // Found a new team
                        proposed_moves.push_back({player_id, k});
                        found_spot = true;
                        break; // Stop looking for a new team for this player
                    }
                }

                if (!found_spot) { // The team has a player who cannot be transferred to a new team, so it cannot be dissolved
                    all_players_can_move = false;
                    break;
                }
            }

            // Check if all players can be moved
            if (all_players_can_move) {
                
                // Apply the player movements for his new teams
                for (const auto& move : proposed_moves) {
                    int player_id = move.first;
                    int target_team_index = move.second;
                    const Player& player_obj = instance.players[player_id];
                    
                    solution.teams[target_team_index].AddPlayer(player_obj);
                }

                // clear the team
                solution.teams[team_index_to_empty].Clear();

                // Set the improvement
                improved = true;
                break;
            }
        }
    }

    // Removes all empty teams
    solution.teams.erase(
        remove_if(solution.teams.begin(), solution.teams.end(), 
            [](const Team& t) { return t.player_ids_list.empty(); }
        ),
        solution.teams.end()
    );
}

// Solution disturbance using the destroy and rebuild strategy, selecting randoly k team to destroy
void pertubarion_destroy_and_rebuild(const ProblemInstance& instance, Solution& solution, int k, mt19937& g) {
    int num_teams = solution.teams.size();

    // No teams
    if (num_teams == 0) {
        return; 
    }

    // Check if it is not already greater than the number of teams
    if (k > num_teams) {
        k = num_teams;
    }

    // Create a teams ids list
    vector<int> team_indices(num_teams);
    iota(team_indices.begin(), team_indices.end(), 0);

    //  Shuffle the players randomly
    shuffle(team_indices.begin(), team_indices.end(), g);

    vector<int> destroied_teams_players;

    // Destroy firt k teams
    for (int i = 0; i < k; ++i) {
        int team_index_to_destroy = team_indices[i];
        Team& team = solution.teams[team_index_to_destroy];

        // Add players team to insert again 
        destroied_teams_players.insert(
            destroied_teams_players.end(),
            team.player_ids_list.begin(),
            team.player_ids_list.end()
        );

        // Clear team
        team.Clear();
    }

    //  Shuffle the players randomly
    shuffle(destroied_teams_players.begin(), destroied_teams_players.end(), g);

    // Insert the players without team using first-fit strategy
    for (int player_id : destroied_teams_players) {
        const Player& player = instance.players[player_id];
        bool allocated = false;

        // Try allocating them to any team, including the recently disbanded ones
        for (Team& team : solution.teams) {
            if (team.CanAddPlayer(player, instance.B)) {
                team.AddPlayer(player);
                allocated = true;
                break;
            }
        }

        // Create a new team if need
        if (!allocated) {
            Team new_team;
            new_team.AddPlayer(player);
            solution.teams.push_back(new_team);
        }
    }

    // Removes all empty teams
    solution.teams.erase(
        remove_if(solution.teams.begin(), solution.teams.end(), 
            [](const Team& t) { return t.player_ids_list.empty(); }
        ),
        solution.teams.end()
    );
}

int main(int argc, char* argv[]) {
    auto start = chrono::high_resolution_clock::now();

    // Checks for right amount of arguments
    if (argc < 4) {
        cerr << "Usage: " << argv[0]
        << " <instance_file> <max_iterations> <seed>]\n";
        return 1;
    }

    // Read arguments
    string instance_file = argv[1];
    int max_iterations = stoi(argv[2]);
    int seed = stoi(argv[3]);

    try {
        string file_to_read = "instances/01.txt"; 
        
        ProblemInstance prob = read_instance(file_to_read);

        // Seed
        mt19937 g(seed);

        // --- 2. Parâmetros do ILS ---
        const int MAX_ITERATIONS = max_iterations;
        const int MAX_iter_without_improvement = 20; // Para controle de 'k'
        
        int k_min = 2; // Minimum force of the disturbance
        int k_max = 5; // Maximum force of the disturbance
        int k_current = k_min;
        int iter_without_improvement = 0;
        
        Solution current_solution = build_initial_solution(prob, g);
        Solution best_solution = current_solution; // S*

        cout << "[Iter 0] Initial Solution = " 
             << best_solution.teams.size() << " teams" << endl;

        for (int i = 1; i <= MAX_ITERATIONS; ++i) {
            current_solution = best_solution; 
            local_search(prob, current_solution);
            pertubarion_destroy_and_rebuild(prob, current_solution, k_current, g);

            if (current_solution.teams.size() < best_solution.teams.size()) {
                best_solution = current_solution; // New global optimum
                iter_without_improvement = 0;
                k_current = k_min;

                cout << "[Iter " << i << "] New Solution = " 
                     << best_solution.teams.size() << " teams" << endl;
            } else {
                iter_without_improvement++;
            }

            // It increases the strength of the disturbance
            if (iter_without_improvement >= MAX_iter_without_improvement) {
                k_current++;
                if (k_current > k_max || k_current > prob.J / 10) { 
                    k_current = k_min;
                }
                iter_without_improvement = 0;
            }
        }

        cout << "\n--- ILS Finish ---" << endl;
        cout << "Better Solution = " 
             << best_solution.teams.size() << " teams" << endl;
        
        // Print better solution
        int team_num = 1;
        for (const auto& team : best_solution.teams) {
            cout << "Time " << team_num++ << " (Salário: " << team.total_salary << "): ";
            for (int id : team.player_ids_list) {
                cout << id + 1 << " "; // +1 para mostrar em base 1
            }
            cout << endl;
        }

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
