using Random, Dates, Printf

num_teams(teams) = maximum(teams)

function is_valid(teams, salaries, conflicts, B)
    for t in 1:num_teams(teams)
        players = findall(x -> x == t, teams)
        if sum(salaries[p] for p in players) > B
            return false
        end
        for (i, j) in conflicts
            if teams[i] == t && teams[j] == t
                return false
            end
        end
    end
    return true
end

function initial_solution(n, salaries, conflicts, B) # greedy solution
    teams = zeros(Int, n) # player j belongs to teams[j]
    team_count = 0

    for j in 1:n
        placed = false

        for t in 1:team_count
            teams[j] = t
            if is_valid(teams, salaries, conflicts, B) # check if player j can belong to an existing team
                placed = true
                break
            end
        end

        if !placed
            team_count += 1
            teams[j] = team_count
        end
    end

    return teams

end

function local_search(teams, salaries, conflicts, B) # classic heuristic
    improved = true
    best = copy(teams)

    while improved
        improved = false

        for j in 1:length(teams)
            current_team = best[j]

            for t in 1:maximum(best) # try create a new solution by moving player to another existing team
                if t != current_team
                    trial = copy(best)
                    trial[j] = t

                    if is_valid(trial, salaries, conflicts, B) && num_teams(trial) <= num_teams(best) # check if new solutions is valid and at least as good as the current
                        best = trial
                        improved = true
                    end
                end
            end
        end
    end

    return best
end

function perturb(teams, k) # k = number of random changes performed
    new_teams = copy(teams)

    for _ in 1:k
        j = rand(1:length(teams)) # choose a player j
        new_teams[j] = rand(1:maximum(new_teams)+1) # choose another existing team to player j 
    end

    return new_teams
end

function acceptance(s_current, s_candidate, salaries, conflicts, B)
    if !is_valid(s_candidate, salaries, conflicts, B)
        return s_current
    end

    if num_teams(s_candidate) <= num_teams(s_current) 
        return s_candidate
    end

    return s_current
end

function ILS(instance::Dict; max_iter, k=2)
    salaries = instance["salaries"]
    conflicts = instance["conflicts"]
    B = instance["B"]

    n = length(salaries) # players number

    s0 = initial_solution(n, salaries, conflicts, B)
    s_best = local_search(s0, salaries, conflicts, B)

    start_time = time()

    best_value = num_teams(s_best)

    println(@sprintf("%.2f seg | valor=%d | solução=%s", # initial log
                     time() - start_time, best_value, string(s_best)))

    for it in 1:max_iter
        s_perturbed = perturb(s_best, k)
        s_local = local_search(s_perturbed, salaries, conflicts, B)
        s_best_candidate = acceptance(s_best, s_local, salaries, conflicts, B) # choose the best iteration solution 

        if num_teams(s_best_candidate) < best_value # choose the best global solution
            best_value = num_teams(s_best_candidate)
            s_best = s_best_candidate
            println(@sprintf("%.2f seg | valor=%d | solução=%s", # best global solution log
                             time() - start_time, best_value, string(s_best)))
        end

        elapsed = time() - start_time
        if abs(elapsed - 5.0) < 0.01  # iterations around 5s
            println(">> Iteração $(it) alcançada em ~5s")
        elseif abs(elapsed - 300.0) < 0.01  # iterations around 300s
            println(">> Iteração $(it) alcançada em ~300s")
        end
    end

    return s_best
end

function read_file(filename)
    lines = readlines(filename)

    J, B = parse.(Int, split(lines[1]))
    salaries = parse.(Int, split.(lines[2:J+1]))
    I = parse(Int, lines[J+2])
    conflicts = [parse.(Int, split(lines[J+2+i])) for i in 1:I]
    
    return Dict(
        "num_players" => J,
        "B" => B,
        "salaries" => salaries,
        "num_conflicts" => I,
        "conflicts" => conflicts
    )
end

function main()
    if length(ARGS) < 3
        println("Try: julia <program_name>.jl <instance_file> <max_iterations> <seed> [--perturbation_size N]")
        return
    end

    filename = ARGS[1]
    #max_iterations = parse(Int, ARGS[2])
    max_iterations = 10
    #seed = parse(Int, ARGS[3])
    seed = 123

    # optional args
    perturbation_size = 2  # default
    for i in 4:2:length(ARGS)
        if ARGS[i] == "--perturbation_size"
            perturbation_size = parse(Int, ARGS[i+1])
        end
    end

    Random.seed!(seed)

    #instance = read_file(filename)
    instance = Dict(
        "num_players" => 5,
        "B" => 10,  
        "salaries" => [4, 3, 5, 2, 1],
        "num_conflicts" => 2,
        "conflicts" => [[1, 2], [3, 4]] 
    )


    solution = ILS(instance; max_iter=max_iterations, k=perturbation_size)

    println("Final solution: ", solution)
    println("Number of teams: ", num_teams(solution))
end

main()
