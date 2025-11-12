# Para garantir a instalação dos pacotes necessários.
import Pkg
Pkg.activate(@__DIR__)
Pkg.instantiate()

using JuMP
using HiGHS
using Cbc

struct Player
        id::Int
        salary::Int
    end

mutable struct Team
    players::Vector{Int}
    remaining_budget::Int
end

# (TODO) ATUALIZAR CONSTRUÇÃO CASA HAJA MUDANÇAS NO ils.cpp NESSA PARTE (TODO)
function can_add_to_team(team::Team, pid::Int, salarios::Vector{Int}, conflitos::Vector{Tuple{Int,Int}})
    # Verifica se há conflito entre pid e algum jogador já no time
    for other in team.players
        for (a, b) in conflitos
            if (pid == a && other == b) || (pid == b && other == a)
                return false
            end
        end
    end
    # Verifica orçamento
    return salarios[pid] <= team.remaining_budget
end

function construct_initial_solution(B::Int, salarios::Vector{Int}, conflitos::Vector{Tuple{Int,Int}})
    J = length(salarios)

    # Ordena jogadores por salário decrescente (First Fit Decreasing)
    order = sortperm(salarios; rev=true)

    teams = Team[]

    for pid in order
        placed = false

        # Tenta colocar no primeiro time viável
        for team in teams
            if can_add_to_team(team, pid, salarios, conflitos)
                push!(team.players, pid)
                team.remaining_budget -= salarios[pid]
                placed = true
                break
            end
        end

        # Se não couber em nenhum time existente, cria um novo
        if !placed
            new_team = Team([pid], B - salarios[pid])
            push!(teams, new_team)
        end
    end

    return teams
end

function fli(filename::String, time_limit::Float64, seed::Int)
    # Cria modelo HIGH com os parâmetors dados
    m = Model(Cbc.Optimizer)
    set_optimizer_attribute(m, "time_limit", time_limit)
    set_optimizer_attribute(m, "random_seed", seed)

    # ---- LEITURA do arquivo ----
    lines = strip.(readlines(filename))

    # leitura budget
    B = parse(Int, lines[1])

    # leitura num_jogadores, num_conflitos
    parts = split(lines[2], " ")
    J = parse(Int, strip(parts[1]))
    I = parse(Int, strip(parts[2]))

    # leitura salarios
    salarios = [parse(Int, strip(lines[2 + j])) for j in 1:J]

    # leitura conflitos
    conflitos::Vector{Tuple{Int,Int}}= Vector{Tuple{Int,Int}}()
    for i in 1:I
        dupla = split(lines[2 + J + i], " ")
        push!(conflitos, (parse(Int, strip(dupla[1])), parse(Int, strip(dupla[2]))))
    end

    # Valores lidos:
    println("Instancia ($filename): ")
    println("Budget B = $B")
    println("Jogadores J = $J")
    println("Conflitos I = $I")

    # ---- ENCONTRA SOLUÇÃO INICIAL ----
    # Antes de rodar a formulação, rodamos a mesma construção 
    # de times gulosa implementada na heuristica inicialmente
    # para encontrar um valor menor para c_max_times para rodar a
    # formulação com menos variáveis.
    initial_teams = construct_initial_solution(B, salarios, conflitos)

    # ---- FORMULAÇÃO LINEAR ----
    # - Variáveis

    # m será o número de times máximo a ser considerado
    c_max_times = length(initial_teams)
    
    # -- xij significa botar jogador i no time j
    println("numero inicial de times = $c_max_times")
    @variable(m, x[1:J, 1:c_max_times], Bin, start=0)
    index_team=1
    for team in initial_teams
        for player in team.players
            set_start_value(x[player, index_team], 1)
        end
        index_team+=1
    end

    println("Solução inicial setada:")
    for j in 1:c_max_times
    jogadores_time = [i for i in 1:J if get_start_value(x[i,j]) == 1]
    println("Time $j: jogadores $jogadores_time")
    end

    println("tamanho da variavel x:", size(x))
    # -- yj significa que um time está sendo populado (tem pelo menos um jogador)
    # todos começam com 1, pois na solução inicial todos estão sendo usados
    @variable(m, y[1:c_max_times], Bin, start=1)

    # -- numero total de times criados
    @variable(m, num_times, Int)

    # -- função objetivo
    @objective(m, Min, sum(y[1:c_max_times]))

    # -- constraints
    # - restrição de budget para cada time
    @constraint(m, [j=1:c_max_times], sum(x[1:J, j] .* salarios[1:J]) <= B);

    # - restrição para cada conflito
    # exemplo: jogadores a e b tem conflito
    # o jogador a estar no time j implica que o jogador b não está no time j
    # x[a, j] -> ~x[b, j]
    for conflito in conflitos
        (a,b) = conflito
        @constraint(m, [j=1:c_max_times], x[a, j] <= (1- x[b, j]))
    end

    # - restrição para yj ter o significado certo
    # yj -> x1j v x2j v ... v xij
    # (poderia fazer o caso para limitar superiormente y, mas como é minimização não precisa)
    # @constraint(m, [j=1:c_max_times], y[j] >= sum(x[1:J, j])/J)
    @constraint(m, [i=1:J, j=1:c_max_times], x[i,j] <= y[j])

    # - restrição para que todo jogador seja assignado
    # (poderia fazer o caso para que um jogador não seja assignado mais de uma vez, mas como é minimização não precisa)
    @constraint(m, [i=1:J], sum(x[i,1:c_max_times])==1)

    optimize!(m)
    @show objective_value(m)
end

if abspath(PROGRAM_FILE) == @__FILE__
    # Pega o nome do arquivo passado na linha de comando
    # Uso: julia fli.jl 07.txt
    if length(ARGS) < 3
        println("Uso: julia fli.jl <arquivo.txt> <tempo_max_s> <seed>")
        exit()
    end
    filename = ARGS[1]
    time_limit = parse(Float64, ARGS[2])
    seed = parse(Int, ARGS[3])

    # Chama o método definido para formulação
    fli(filename, time_limit, seed)
end