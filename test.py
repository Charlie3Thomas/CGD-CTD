import json
import matplotlib.pyplot as plt

def scrub_data(json_data):
    scrubbed_data = []
    for entry in json_data:
        scrubbed_entry = {
            "direct_samples": entry["direct_samples"][0],
            "indirect_samples": entry["indirect_samples"][0],
            "recursion_depth": entry["recursion_depth"][0],
            "L1_difference": entry["L1_difference"][0],
            "Objective Value": entry["objective_value"][0]
        }
        scrubbed_data.append(scrubbed_entry)
    return scrubbed_data

    #with open("output_data_20230806204332.json", "r") as file:
if __name__ == "__main__":
    # Load data from the JSON file
    with open("output_data_20230807142758.json", "r") as file:
        json_data = json.load(file)
    scrubbed_data = scrub_data(json_data)
    direct_samples = [entry["direct_samples"] for entry in scrubbed_data]
    indirect_samples = [entry["indirect_samples"] for entry in scrubbed_data]
    recursion_depth = [entry["recursion_depth"] for entry in scrubbed_data]
    objective_value = [entry["Objective Value"] for entry in scrubbed_data]

    fig_width = 16
    fig, ax1 = plt.subplots(figsize=(fig_width, 9), dpi=300)
    ax1.plot(direct_samples, label="Direct Samples", color="tab:blue")
    ax1.plot(indirect_samples, label="Indirect Samples", color="tab:orange")
    ax1.plot(recursion_depth, label="Recursion Depth", color="tab:green")
    ax1.set_xlabel("Iteration")
    ax1.set_ylabel("Direct/Indirect Samples, Recursion Depth", color="black")
    ax1.legend(loc="upper left")
    ax2 = ax1.twinx()
    ax2.plot(objective_value, label="Objective Value", color="tab:red")
    ax2.set_ylabel("Objective Value", color="tab:red")
    ax2.legend(loc="upper right")

    # Find the index of the minimum objective value
    min_objective_value_index = objective_value.index(min(objective_value))
    min_objective_value_iteration = min_objective_value_index + 1
    min_objective_value = min(objective_value)
    min_direct_samples = direct_samples[min_objective_value_index]
    min_indirect_samples = indirect_samples[min_objective_value_index]
    min_recursion_depth = recursion_depth[min_objective_value_index]

    # Annotate graph
    ax2.plot(min_objective_value_index, min_objective_value, marker='o', markersize=8, color='red')
    ax2.annotate(f'Objective Value: {min_objective_value:.8f}', (min_objective_value_index, min_objective_value), textcoords="offset points", xytext=(-15,-10), ha='center')
    ax1.plot(min_objective_value_index, min_direct_samples, marker='o', markersize=8, color='blue')
    ax1.annotate(f'Direct: {min_direct_samples:.2f}', (min_objective_value_index, min_direct_samples), textcoords="offset points", xytext=(0,10), ha='center')    
    ax1.plot(min_objective_value_index, min_indirect_samples, marker='o', markersize=8, color='orange')
    ax1.annotate(f'Indirect: {min_indirect_samples:.2f}', (min_objective_value_index, min_indirect_samples), textcoords="offset points", xytext=(0,10), ha='center')    
    ax1.plot(min_objective_value_index, min_recursion_depth, marker='o', markersize=8, color='green')
    ax1.annotate(f'Recursion: {min_recursion_depth:.2f}', (min_objective_value_index, min_recursion_depth), textcoords="offset points", xytext=(0,10), ha='center')

    plt.title("Scrubbed Data Metrics")
    plt.grid(True)
    plt.savefig("scrubbed_data_metrics.png")