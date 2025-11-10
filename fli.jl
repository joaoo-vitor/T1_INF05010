# Para garantir a instalação dos pacotes necessários.
import Pkg
Pkg.activate(@__DIR__)
Pkg.instantiate()

using JuMP
using HiGHS

# Pega o nome do arquivo passado na linha de comando
# Uso: julia fli.jl 07.txt
if length(ARGS) < 3
    println("Uso: julia fli.jl <arquivo.txt> <tempo_max_s> <seed>")
    exit()
end
filename = ARGS[1]
time_limit = parse(Float64, ARGS[2])
seed = parse(Int, ARGS[3])

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
conflitos = []
for i in 1:I
    dupla = split(lines[2 + J + i], " ")
    push!(conflitos, (parse(Int, strip(dupla[1])), parse(Int, strip(dupla[2]))))
end

# Valores lidos:
println("Instancia ($filename): ")
println("Budget B = $B")
println("Jogadores J = $J")
println("Conflitos I = $I")

# Cria modelo HIGH com os parâmetors dados
m = Model(HiGHS.Optimizer)
set_optimizer_attribute(m, "time_limit", time_limit)
set_optimizer_attribute(m, "random_seed", seed)

# ---- FORMULAÇÃO LINEAR ----
# - Variáveis

# -- xij significa botar jogador i no time j
# m será o número de times máximo a ser considerado (pior caso)
# Podemos considerar o numero maximo de times como o caso em que só temos um jogador por time (assim é impossivel ter conflitos )
# m = J
# Mas, podemos considerar tbm que criamos um time novo para cada conflito, assim teremos I+1 times
# Para salvar memoria, usamos m como o menor desses dois valores (pode variar dependendo da instância)
c_max_times = min(J, I+1)
println("numero inicial de times = $c_max_times")
@variable(m, x[1:J, 1:c_max_times], Bin)

# -- yj significa que um time está sendo populado (tem pelo menos um jogador)
@variable(m, y[1:c_max_times], Bin)

# -- numero total de times criados
@variable(m, num_times, Int)

# -- função objetivo
@objective(m, Min, num_times)

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
@constraint(m, [j=1:c_max_times], y[j] >= sum(x[1:J, j])/J)

# - restrição para min_times ter o significado certo
# tem que ser a soma de todos os times que foram usados (yj esta ligado)
@constraint(m, num_times == sum(y[1:c_max_times]))

# - restrição para que todo jogador seja assignado
# (poderia fazer o caso para que um jogador não seja assignado mais de uma vez, mas como é minimização não precisa)
@constraint(m, [i=1:J], sum(x[i,1:c_max_times])>=1)

optimize!(m)
@show value(num_times)
@show objective_value(m)

