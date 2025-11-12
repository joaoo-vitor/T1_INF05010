# Ativa o ambiente local e garante dependências
import Pkg
Pkg.activate(@__DIR__)
Pkg.instantiate()

using CSV
using DataFrames
using Dates

# Importa o arquivo com sua função principal
include("fli.jl")

function run_all_instances()
    base_path = "instances"
    time_limit = 300.0
    seeds = 0:9

    # Arquivo CSV de saída
    results_file = "resultados.csv"

    # Cria o DataFrame (se já existir, lê o existente)
    if isfile(results_file)
        results_df = CSV.read(results_file, DataFrame)
    else
        results_df = DataFrame(
            nome_arquivo = String[],
            seed = Int[],
            tempo_total = Float64[],
            solucao_encontrada = Float64[],
        )
    end

    for i in 9:10
        instance_name = lpad(i, 2, '0') * ".txt"
        filename = joinpath(base_path, instance_name)

        println("==============================================")
        println("Executando instância: $filename")
        println("==============================================")

        for seed in seeds
            println(">> Rodando com seed = $seed, time_limit = $time_limit s")

            try
                # Mede o tempo de execução e captura o valor retornado
                tempo_total = @elapsed begin
                    solucao = fli(filename, time_limit, seed)
                end

                # Adiciona linha no DataFrame
                push!(results_df, (
                    nome_arquivo = instance_name,
                    seed = seed,
                    tempo_total = tempo_total,
                    solucao_encontrada = solucao,
                ))

                println("✅ Solução: $solucao (tempo = $(round(tempo_total, digits=2)) s)")

            catch e
                println("❌ Erro ao rodar $filename com seed=$seed:")
                println(e)
                push!(results_df, (
                    nome_arquivo = instance_name,
                    seed = seed,
                    tempo_total = NaN,
                    solucao_encontrada = NaN,
                ))
            end

            println("-------------------------------------------------")
        end

        # Salva o CSV a cada instância (para não perder dados em caso de erro)
        CSV.write(results_file, results_df)
    end

    println("✅ Execuções concluídas! Resultados salvos em $results_file")
end

# Executa automaticamente se rodado diretamente
run_all_instances()
