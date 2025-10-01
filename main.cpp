#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <optional>
#include <algorithm> // for shuffle

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

// ================= File Reader ================= //
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

// ================= Helper: check if player fits in a team ================= //
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
vector<Team> construct_initial_solution(const ProblemInstance &instance, mt19937 &rng) {
    vector<int> order(instance.J);
    for (int i = 0; i < instance.J; i++) order[i] = i;

    // Shuffle players order using seed rng
    shuffle(order.begin(), order.end(), rng);

    vector<Team> teams;

    // Place players one by one
    for (int pid : order) {
        const Player &p = instance.players[pid];
        bool placed = false;

        // Try to place in existing teams
        for (auto &team : teams) {
            if (can_add_to_team(team, p)) {
                team.players.push_back(p.id);
                team.remaining_budget -= p.salary;
                placed = true;
                break;
            }
        }

        // If not placed, create a new team
        if (!placed) {
            Team new_team;
            new_team.remaining_budget = instance.B - p.salary;
            new_team.players.push_back(p.id);
            teams.push_back(move(new_team));
        }
    }

    return teams;
}

tuple<vector<Team>, bool, int> local_search_step(std::vector<Team> teams, const ProblemInstance instance, int team_to_dissolve) {
    if (teams.size() <= 1) return {teams, false, 0};
    
    Team &src_team = teams[team_to_dissolve];
    int players_moved=0; // How many players of this team could be moved
    
    if (src_team.players.empty()) return {teams, false, 0};

    std::vector<int> remaining_players; // Players that we fail to move
    bool all_moved = true;

    // Try to move every player
    for (int pid : src_team.players) {
        const Player &p = instance.players[pid];
        bool moved = false;

        for (int i = 0; i < (int)teams.size(); i++) {
            if (i == team_to_dissolve) continue; // skip own team
            if (can_add_to_team(teams[i], p)) {
                teams[i].players.push_back(pid);
                teams[i].remaining_budget -= p.salary;
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

    // Update the source team’s players and budget
    src_team.players = remaining_players;
    src_team.remaining_budget = instance.B;
    for (int pid : src_team.players) {
        src_team.remaining_budget -= instance.players[pid].salary;
    }

    // If we moved everyone, delete the team (improvement)
    if (all_moved) {
        teams.erase(teams.begin() + team_to_dissolve);
        return {teams, true, players_moved};
    }

    return {teams, false, players_moved}; // no full team dissolved
}


vector<Team> local_search(vector<Team> initial, 
                               const ProblemInstance &instance, 
                               int max_iterations) {

    vector<Team> best_solution = initial;
    vector<Team> current_team = best_solution; // This will be current node on the neighbors graph
    vector<Team> current_best_neighbor; 
    int iterations_without_improvement=0; //Stop if nothing gets better after X iterations
    for (int iter = 0; iter < max_iterations; iter++) {
        int current_best_score=0; // The scores of the neighbors (best will be chosen)
        iterations_without_improvement++;

        // EACH ITERATION: node of solutions graph

        // For every available team, considering stepping to its direction
        // Graph step: choose this team to dissolve
        bool dissolved = false;
        for(int team_idx=0; team_idx < current_team.size(); team_idx++){
            vector<Team> neighbor;
            int neighbor_score; // Score is how many players of this team could be moved
            tie(neighbor, dissolved, neighbor_score) = local_search_step(best_solution, instance, team_idx);

            if (dissolved) {
                // If the team was successfully removed (rare), choose this and stop looking for other neighbors
                current_best_neighbor=neighbor;
                best_solution=neighbor; //Only update the best solution output when a team is dissolved (-1 total teams)
                iterations_without_improvement=0;
                break; //go to next iteration
            }

            // Keep track of the best neighbor in case no one actually improves
            if(neighbor_score>current_best_score){
                current_best_neighbor=neighbor;
                current_best_score=neighbor_score;
            }
        }
        if(current_best_score==0){
            cout << "Couldnt move any player anymore.";
            break;
        } 
            
        current_team = current_best_neighbor; // WALK on the solutions graph
        if(iterations_without_improvement>10) {
            break;
        }

    }

    return best_solution;
}



// ================= Main ================= //

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0]
                  << " <instance_file> <max_iterations> <seed> [--perturbation_size N]\n";
        return 1;
    }

    string instance_file = argv[1];
    int max_iterations = stoi(argv[2]);
    int seed = stoi(argv[3]);
    optional<int> perturbation_size;

    // check optional argument
    if (argc == 6) {
        string flag = argv[4];
        if (flag == "--perturbation_size") {
            perturbation_size = stoi(argv[5]);
        } else {
            cerr << "Unknown option: " << flag << "\n";
            return 1;
        }
    }

    try {
        ProblemInstance instance = read_instance(instance_file);

        cout << "Read instance with " << instance.J << " players, "
                  << instance.I << " conflicts, budget " << instance.B << "\n";
        if (perturbation_size) {
            cout << "Perturbation size = " << *perturbation_size << "\n";
        }
        cout << "Max iterations = " << max_iterations << "\n";
        cout << "Seed = " << seed << "\n";

        mt19937 rng(seed);
        // Build initial solution
        auto initial_solution = construct_initial_solution(instance, rng);
        cout << "Initial solution has " << initial_solution.size() << " teams.\n";

        auto best_solution = local_search(initial_solution, instance, max_iterations);

        cout << "Final solution uses " << best_solution.size() << " teams.\n";

    } catch (const exception &ex) {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
