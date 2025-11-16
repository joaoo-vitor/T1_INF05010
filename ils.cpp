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

using namespace std;

// ================= Structures ================= //
struct Player {
    int id;
    int salary;
    vector<int> conflicts; // store indices of players it conflicts with
};

struct Team {
    int remaining_budget;
    vector<int> players; // store player ids
};

// ================= Problem Data ================= //
struct ProblemInstance {
    int J; // number of players
    int I; // number of conflicts
    int B; // budget per team
    vector<Player> players;
};


/**
 * @brief Prints the current solution (teams and players) in a formatted table.
 * 
 * @param teams Vector of teams containing players and remaining budgets.
 * 
 * @return void
 */
void print_solution(const vector<Team> &teams) {
    cout << "\n========== Current Solution ==========\n";
    cout << "Total teams: " << teams.size() << "\n\n";

    // Print table header
    cout << left << setw(8) << "Team"
         << setw(15) << "Budget"
         << "Players (id:salary)\n";
    cout << string(50, '-') << "\n";

    // Print each team
    for (int t = 0; t < (int)teams.size(); t++) {
        cout << left << setw(8) << t
             << setw(15) << teams[t].remaining_budget;

        for (int pid : teams[t].players) {
            cout << pid << ", ";
        }
        cout << "\n";
    }
    cout << "======================================\n";
}

/**
 * @brief Reads a problem instance from a file.
 * 
 * @param filename The name of the file containing the instance data.
 * 
 * @return ProblemInstance The populated problem instance with players, conflicts, and budget.
 * @throws runtime_error If the file cannot be opened.
 */
ProblemInstance read_instance(const string &filename) {
    ProblemInstance instance;
    ifstream in(filename);
    if (!in.is_open()) {
        throw runtime_error("Could not open file " + filename);
    }

    // First line: B
    in >> instance.B;

    // Second line: J and I
    in >> instance.J >> instance.I;

    instance.players.resize(instance.J);

    // Next J lines: salaries
    for (int i = 0; i < instance.J; i++) {
        int s;
        in >> s;
        instance.players[i].id = i;
        instance.players[i].salary = s;
    }

    // Next I lines: conflicts
    for (int k = 0; k < instance.I; k++) {
        int u, v;
        in >> u >> v;
        // Convert to 0-based indexing
        u--; v--;
        instance.players[u].conflicts.push_back(v);
        instance.players[v].conflicts.push_back(u);
    }

    return instance;
}

/**
 * @brief Checks if a player can be added to a given team.
 * 
 * @param team The team to insert the player.
 * @param player The player to be tested for insertion.
 * 
 * @return true If the player can be added without exceeding budget or causing conflicts.
 * @return false Otherwise.
 */
bool can_add_to_team(const Team &team, const Player &player) {
    if (player.salary > team.remaining_budget) return false;

    // Check conflicts with players already in the team
    for (int teammate_id : team.players) {
        // If teammate is in player's conflict list, return false
        for (int conflict : player.conflicts) {
            if (conflict == teammate_id) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Constructs an initial solution using First Fit Decreasing (FFD).
 * 
 * @param instance The problem instance with players and constraints.
 * 
 * @return vector<Team> A set of initial teams with assigned players.
 */
vector<Team> construct_initial_solution(const ProblemInstance &instance, mt19937 rng) {

    // Create a player ids list
    vector<int> order(instance.J);
    iota(order.begin(), order.end(), 0); 

    // Shuffle players ids
    std::shuffle(order.begin(), order.end(), rng);

    vector<Team> teams;

    // Place players one by one (First Fit Decreasing)
    for (int pid : order) {
        const Player &p = instance.players[pid];
        bool placed = false;

        // Try to put on the first viable team
        for (auto &team : teams) {
            if (can_add_to_team(team, p)) {
                team.players.push_back(p.id);
                team.remaining_budget -= p.salary;
                placed = true;
                break;
            }
        }

        // If doesnt fit on no one, create a new team
        if (!placed) {
            Team new_team;
            new_team.remaining_budget = instance.B - p.salary;
            new_team.players.push_back(p.id);
            teams.push_back(move(new_team));
        }
    }

    return teams;
}

/**
 * @brief Step in the neighborhood of the localsearch by attempting to dissolve a given team
 *        and redistribute its players.
 * 
 * @param teams Current list of teams.
 * @param instance The problem instance with players and constraints.
 * @param team_to_dissolve Index of the team selected to attempt dissolution.
 * 
 * @return tuple<vector<Team>, bool, int> 
 *         - Updated set of teams.
 *         - Boolean flag indicating whether the team was successfully dissolved.
 *         - Number of players moved during the step (score of neighbor).
 */
tuple<vector<Team>, bool, int> local_search_step(std::vector<Team> teams, const ProblemInstance instance, int team_to_dissolve) {
    if (teams.size() <= 1) return {teams, false, 0};
    
    Team &src_team = teams[team_to_dissolve];
    int players_moved=0; // How many players of this team could be moved
    
    if (src_team.players.empty()) return {teams, false, 0};

    std::vector<int> remaining_players; // Players that we fail to move
    bool all_moved = true;
    int total_economy = 0; // Money saved from the source team from removing players

    // Try to move every player
    for (int pid : src_team.players) {
        const Player &p = instance.players[pid];
        bool moved = false;

        for (int i = 0; i < (int)teams.size(); i++) {
            if (i == team_to_dissolve) continue; // skip own team
            if (can_add_to_team(teams[i], p)) {
                teams[i].players.push_back(pid);
                teams[i].remaining_budget -= p.salary;
                total_economy+=p.salary;
                moved = true;
                players_moved++; //count everytime a player is moved
                break; // stop once we place the player
            }
        }

        if (!moved) {
            remaining_players.push_back(pid);
            all_moved = false;
        }
    }

    // If we moved everyone, delete the team (improvement)
    if (all_moved) {
        teams.erase(teams.begin() + team_to_dissolve);
        return {teams, true, players_moved};
    }

    // Update the source team’s players and budget
    src_team.players = remaining_players;
    src_team.remaining_budget+=total_economy;
    
    return {teams, false, players_moved}; // no full team dissolved
}

vector<Team> local_search(vector<Team> initial, const ProblemInstance &instance) {
    vector<Team> best_solution = initial;
    
    while(true) {
        bool was_dissolved = false;
        vector<Team> next_solution;

        // Create a team ids list 
        std::vector<int> team_indices(best_solution.size());
        std::iota(team_indices.begin(), team_indices.end(), 0);

        // Sort teams to dissolve teams with smaller size first
        std::sort(team_indices.begin(), team_indices.end(), [&](int idx_a, int idx_b) {
            return best_solution[idx_a].players.size() < best_solution[idx_b].players.size();
        });

        for(int team_idx : team_indices){
            vector<Team> neighbor;
            int neighbor_score=0; // Score is how many players of this team could be moved out

            // Get neighbor for testing score
            tie(neighbor, was_dissolved, neighbor_score) = local_search_step(best_solution, instance, team_idx);

            if (was_dissolved) {
                // If the team was successfully removed (rare), choose this and stop looking for other neighbors
                next_solution=neighbor;
                was_dissolved = true;
                break; //go to next iteration
            }
        }

        if(was_dissolved) {
            best_solution = next_solution;
        } else {
            break; // Finish local search
        }

    }

    return best_solution;
    
}

/**
 * @brief Applies a perturbation to the current solution by moving a random percentage 
 *        of players to new teams, creating new teams if moving fails.
 * 
 * @param solution Current solution.
 * @param rng Random number generator for selecting players to move.
 * @param instance The original problem instance with players and constraints.
 * @param perturbation_ratio The percentage of players to perturb (0.0–1.0).
 * 
 * @return vector<Team> The perturbed solution after reassignments.
 */
vector<Team> perturbation(vector<Team> solution, mt19937 &rng, ProblemInstance instance, double perturbation_ratio) {
    // Flatten all players with (team_id, player_id)
    vector<pair<int,int>> all_players;
    for (int tid = 0; tid < (int)solution.size(); tid++) {
        for (int pid : solution[tid].players) {
            all_players.push_back({tid, pid});
        }
    }

    if (all_players.empty()) return solution;

    // Determine how many players to move, ranging from [1, all_players.size()]
    size_t proposed = static_cast<size_t>(all_players.size() * perturbation_ratio);
    size_t num_to_move = std::max<size_t>(1, std::min(all_players.size(), proposed));
    shuffle(all_players.begin(), all_players.end(), rng);
    vector<pair<int,int>> chosen(all_players.begin(), all_players.begin() + static_cast<long>(num_to_move));

    // Remove chosen players from their teams
    for (const auto &tp : chosen) {
        int tid = tp.first;
        int pid = tp.second;
        auto &team = solution[tid];
        team.players.erase(remove(team.players.begin(), team.players.end(), pid), team.players.end());
    }

    // Recompute team budgets after removals
    for (auto &team : solution) {
        team.remaining_budget = instance.B;
        for (int pid : team.players) {
            team.remaining_budget -= instance.players[pid].salary;
        }
    }

    // Try to reassign each chosen player
    for (const auto &tp : chosen) {
        int old_tid = tp.first;
        (void)old_tid; // not used, but kept for clarity
        int pid = tp.second;
        const Player &p = instance.players[pid];
        bool placed = false;

        for (auto &team : solution) {
            if (can_add_to_team(team, p)) {
                team.players.push_back(pid);
                team.remaining_budget -= p.salary;
                placed = true;
                break;
            }
        }

        // If no team can fit, create a new one
        if (!placed) {
            Team new_team;
            new_team.remaining_budget = instance.B - p.salary;
            new_team.players.push_back(pid);
            solution.push_back(move(new_team));
        }
    }

    return solution;
}


int main(int argc, char* argv[]) {
    auto start = chrono::high_resolution_clock::now();

    // Checks for right amount of arguments
    if (argc < 4) {
        cerr << "Usage: " << argv[0]
        << " <instance_file> <max_iterations> <seed> [--perturbation_ratio N]\n";
        return 1;
    }

    // Read arguments
    string instance_file = argv[1];
    int max_iterations = stoi(argv[2]);
    int seed = stoi(argv[3]);
    double pertubation_ratio = 0.15;

    // Check optional argument
    if (argc == 6) {
        string flag = argv[4];
        if (flag == "--perturbation_ratio") {
            pertubation_ratio = stoi(argv[5]);
        } else {
            cerr << "Unknown option: " << flag << "\n";
            return 1;
        }
    }


    try {
        // Read problem instance
        ProblemInstance instance = read_instance(instance_file);

        cout << "Read instance with " << instance.J << " players, "
        << instance.I << " conflicts, budget " << instance.B << "\n";
        cout << "Max iterations = " << max_iterations << "\n";
        cout << "Seed = " << seed << "\n";

        mt19937 rng(seed);
        // Build initial solution
        auto initial_solution = construct_initial_solution(instance, rng);
        cout << "Initial solution has " << initial_solution.size() << " teams.\n";

        vector<Team> best_solution = initial_solution;
        vector<Team> current_solution = best_solution;
        
        // Compute local search with perturbation many times
        for(int i=0; i<max_iterations; i++){
            
            cout << "Local search number " << (1+i) << "\n";
            current_solution = local_search(current_solution, instance);

            if (current_solution.size() < best_solution.size()){
                // Get elapsed time for logging
                auto now = chrono::high_resolution_clock::now();
                auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(now - start).count();
                
                best_solution = current_solution;
                cout << "(" << elapsed_ms/1000.0 << ") New solution found in LS: " << best_solution.size() << " teams.\n";    
                print_solution(best_solution);
            }
            
            current_solution = perturbation(current_solution, rng, instance, pertubation_ratio);
        }

        cout << "Final solution uses " << best_solution.size() << " teams.\n";

    } catch (const exception &ex) {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}