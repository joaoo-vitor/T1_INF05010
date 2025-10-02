#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <optional>
#include <algorithm> // for shuffle
#include <iomanip>  // for setw, left, right
#include <tuple>
#include <set>


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
    std::set<int> prohibited_players;
};

// ================= Problem Data ================= //
struct ProblemInstance {
    int J; // number of players
    int I; // number of conflicts
    int B; // budget per team
    vector<Player> players;
};


void print_solution(const vector<Team> &teams, const ProblemInstance &instance) {
    cout << "\n========== Current Solution ==========\n";
    cout << "Total teams: " << teams.size() << "\n\n";

    // Print header
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

    for (int prohibited_teammate_id : team.prohibited_players)
    {
        if (prohibited_teammate_id == player.id)
        {
            return false;
        }
    }
    return true;
}

vector<Team> construct_initial_solution(const ProblemInstance &instance, mt19937 &rng) {
    vector<int> order(instance.J);
    iota(order.begin(), order.end(), 0); // gera [0, 1, ..., J-1]

    // Ordena por salário decrescente (FFD)
    sort(order.begin(), order.end(), [&](int a, int b) {
        return instance.players[a].salary > instance.players[b].salary;
    });

    vector<Team> teams;

    // Place players one by one (First Fit Decreasing)
    for (int pid : order) {
        const Player &p = instance.players[pid];
        bool placed = false;

        // Tenta colocar no primeiro time viável
        for (auto &team : teams) {
            if (can_add_to_team(team, p)) {
                team.players.push_back(p.id);
                team.remaining_budget -= p.salary;
                placed = true;
                break;
            }
        }

        // Se não couber em nenhum, cria novo time
        if (!placed) {
            Team new_team;
            new_team.remaining_budget = instance.B - p.salary;
            new_team.players.push_back(p.id);
            teams.push_back(move(new_team));
        }
    }

    return teams;
}

tuple<vector<Team>, int> local_search_step_soft(vector<Team> teams, const ProblemInstance &instance, int team_to_dissolve) {
    if (teams.size() <= 1) return {teams, 0};

    Team &src_team = teams[team_to_dissolve];
    int players_moved = 0;

    if (src_team.players.empty()) return {teams, 0};

    vector<int> remaining_players; // jogadores que não puderam ser movidos

    // Tenta mover cada jogador para outro time
    for (int pid : src_team.players) {
        const Player &p = instance.players[pid];
        bool moved = false;

        for (int i = 0; i < (int)teams.size(); i++) {
            if (i == team_to_dissolve) continue;
            if (can_add_to_team(teams[i], p)) {
                teams[i].players.push_back(pid);
                teams[i].remaining_budget -= p.salary;
                moved = true;
                players_moved++;
                break;
            }
        }

        if (!moved) {
            remaining_players.push_back(pid);
        }
    }

    // Atualiza o time original apenas com os remanescentes
    src_team.players = remaining_players;
    src_team.remaining_budget = instance.B;
    for (int pid : src_team.players)
        src_team.remaining_budget -= instance.players[pid].salary;

    // Se todos os jogadores foram movidos, apaga o time
    if (remaining_players.empty()) {
        teams.erase(teams.begin() + team_to_dissolve);
    }

    return {teams, players_moved};
}


vector<Team> local_search_soft(vector<Team> initial, const ProblemInstance &instance) {
    vector<Team> current_solution = initial;
    int iterations_without_improvement = 0;

    while (true) {
        int best_moved = 0;
        int team_idx_best = -1;
        vector<Team> best_neighbor = current_solution;

        for (int t = 0; t < (int)current_solution.size(); t++) {
            vector<Team> neighbor;
            int moved;
            tie(neighbor, moved) = local_search_step_soft(current_solution, instance, t);

            if (moved > best_moved) {
                best_moved = moved;
                best_neighbor = neighbor;
                team_idx_best = t;
            }
        }

        if (best_moved == 0 || iterations_without_improvement > 3) break;

        current_solution = best_neighbor;
        iterations_without_improvement++;

        cout << "Soft dissolve: moved " << best_moved << " players from team " << team_idx_best << "\n";
    }

    return current_solution;
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
                for(int player_conflict : p.conflicts){
                    teams[i].prohibited_players.insert(player_conflict);
                }
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
                               const ProblemInstance &instance) {

    vector<Team> best_solution = initial;
    vector<Team> current_solution = best_solution; // This will be current node on the neighbors graph
    vector<Team> current_best_neighbor; 
    int iterations_without_improvement=0; //Stop if nothing gets better after X iterations
    while(true) {
        int current_best_score=0; // The scores of the neighbors (best will be chosen for each step)
        iterations_without_improvement++;
        int team_exploded_idx;

        // EACH ITERATION: node of solutions graph

        // For every available team, considering stepping to its direction
        // Graph step: choose this team to dissolve
        bool dissolved = false;
        for(int team_idx=0; team_idx < current_solution.size(); team_idx++){
            vector<Team> neighbor;
            int neighbor_score; // Score is how many players of this team could be moved

            tie(neighbor, dissolved, neighbor_score) = local_search_step(current_solution, instance, team_idx);

            if (dissolved) {
                // If the team was successfully removed (rare), choose this and stop looking for other neighbors
                current_best_neighbor=neighbor;
                best_solution=neighbor; //Only update the best solution output when a team is dissolved (-1 total teams)
                iterations_without_improvement=0;
                team_exploded_idx=team_idx;
                break; //go to next iteration
            }

            // Keep track of the best neighbor in case no one actually improves
            if(neighbor_score>current_best_score){
                current_best_neighbor=neighbor;
                current_best_score=neighbor_score;
                team_exploded_idx=team_idx;
            }
        }
        if(current_best_score==0){
            // Stop if no improvement
            break;
        } 

        current_solution = current_best_neighbor; // WALK on the solutions graph

        // Logs each step
        if(dissolved){
            cout << "Succesffuly dissolved team number " << team_exploded_idx+1 << " moving " << current_best_score << " (all) players. \n";
        }else{
            cout << "Exploded team number " << team_exploded_idx+1 << " moving " << current_best_score << " players. \n";
        }

        if(iterations_without_improvement>3) {
            // After trying a lot and not dissolving any team, stop local search
            break;
        }

    }

    return best_solution;
    
}

vector<Team> perturbation(vector<Team> solution, mt19937 &rng, ProblemInstance instance, double perturbation_ratio) {
    // Flatten all players with (team_id, player_id)
    vector<pair<int,int>> all_players;
    for (int tid = 0; tid < (int)solution.size(); tid++) {
        for (int pid : solution[tid].players) {
            all_players.push_back({tid, pid});
        }
    }

    if (all_players.empty()) return solution;

    // Determine how many players to move, clamped to [1, all_players.size()]
    size_t proposed = static_cast<size_t>(all_players.size() * perturbation_ratio);
    size_t num_to_move = std::max<size_t>(1, std::min(all_players.size(), proposed));
    shuffle(all_players.begin(), all_players.end(), rng);
    vector<pair<int,int>> chosen(all_players.begin(), all_players.begin() + num_to_move);

    // Remove chosen players from their teams
    for (auto [tid, pid] : chosen) {
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
    for (auto [old_tid, pid] : chosen) {
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
    double perturbation_ratio = 0.15; // default ratio (15%)

    // check optional argument
    if (argc == 6) {
        string flag = argv[4];
        if (flag == "--perturbation_size") {
            // Accept either integer percentage (e.g., 15) or float ratio (e.g., 0.15)
            string val = argv[5];
            if (val.find('.') != string::npos) {
                perturbation_ratio = stod(val);
            } else {
                perturbation_ratio = stod(val) / 100.0;
            }
        } else {
            cerr << "Unknown option: " << flag << "\n";
            return 1;
        }
    }

    try {
        ProblemInstance instance = read_instance(instance_file);

        cout << "Read instance with " << instance.J << " players, "
                  << instance.I << " conflicts, budget " << instance.B << "\n";
    cout << "Perturbation ratio = " << fixed << setprecision(2) << (perturbation_ratio * 100.0) << "%\n";
        cout << "Max iterations = " << max_iterations << "\n";
        cout << "Seed = " << seed << "\n";

        mt19937 rng(seed);
        // Build initial solution
        auto initial_solution = construct_initial_solution(instance, rng);
        cout << "Initial solution has " << initial_solution.size() << " teams.\n";
        // cout << "Initial solution:\n";
        // print_solution(initial_solution, instance);

        vector<Team> best_solution = initial_solution;
        vector<Team> current_solution = best_solution;
        
        // Compute local search with perturbation many times
        for(int i=0; i<max_iterations; i++){
            current_solution = perturbation(current_solution, rng, instance, perturbation_size);
            // cout << "Solution after perturbation:\n";
            // print_solution(current_solution, instance);

            current_solution = local_search_soft(current_solution, instance);
            if (current_solution.size() < best_solution.size()){
                best_solution = current_solution;
                cout << "New solution found: " << best_solution.size() << "\n";
                // print_solution(current_solution, instance);

            }
        }

        cout << "Final solution uses " << best_solution.size() << " teams.\n";

    } catch (const exception &ex) {
        cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
