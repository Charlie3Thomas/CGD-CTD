import json
import datetime
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap

PARAMETER_RANGES = {
    "samples_pp":       (1, 4),
    "direct_samples":   (1, 32),
    "indirect_samples": (1, 16),
    "recursion_depth":  (1, 3),
}

def reverse_normalize(normalized_value, param_min, param_max):
    return param_min + normalized_value * (param_max - param_min)

def scrub_data(json_data):
    scrubbed_data = []
    for entry in json_data:
        num_sub_iterations = len(entry["direct_samples"])
        for i in range(num_sub_iterations):
            scrubbed_entry = {
                "samples_pp":       entry["samples_pp"][i], 
                "direct_samples":   entry["direct_samples"][i], 
                "indirect_samples": entry["indirect_samples"][i], 
                "recursion_depth":  entry["recursion_depth"][i],
                "L1_difference":    entry["L1_difference"][i],
                "Objective Value":  entry["objective_value"][i],
                "render_time":      entry["render_time"][i]
            }
            scrubbed_data.append(scrubbed_entry)
    return scrubbed_data

def full_plot(scrubbed_data, scene_name):
    samples_pp       = [entry["samples_pp"] for entry in scrubbed_data]
    direct_samples   = [entry["direct_samples"] for entry in scrubbed_data]
    indirect_samples = [entry["indirect_samples"] for entry in scrubbed_data]
    recursion_depth  = [entry["recursion_depth"] for entry in scrubbed_data]
    objective_value  = [entry["Objective Value"] for entry in scrubbed_data]
    render_time      = [entry["render_time"] for entry in scrubbed_data]
    # Plot the data
    fig_width = 16
    fig, ax1 = plt.subplots(figsize=(fig_width, 9), dpi=300)
    ax1.set_yticks(range(1, 33))
    ax1.plot(samples_pp, label="Samples Per Pixel", color="tab:purple")
    ax1.plot(direct_samples, label="Direct Samples", color="tab:blue")
    ax1.plot(indirect_samples, label="Indirect Samples", color="tab:orange")
    ax1.plot(recursion_depth, label="Recursion Depth", color="tab:green")
    ax1.set_xlabel("Iteration")
    ax1.set_ylabel("Parameter Value", color="black")
    ax1.legend(loc="upper left")
    ax2 = ax1.twinx()
    ax2.plot(objective_value, label="L1 Difference", color="tab:red")
    ax2.set_ylabel("L1 Difference", color="black")
    ax2.legend(loc="upper right")

    # Find the index of the minimum objective value
    min_objective_value_index = objective_value.index(min(objective_value))
    min_objective_value_iteration = min_objective_value_index + 1
    min_objective_value = min(objective_value)
    min_samples_pp = samples_pp[min_objective_value_index]
    min_direct_samples = direct_samples[min_objective_value_index]
    min_indirect_samples = indirect_samples[min_objective_value_index]
    min_recursion_depth = recursion_depth[min_objective_value_index]

    # Annotate graph
    ax2.plot(min_objective_value_index, min_objective_value, marker='o', markersize=8, color='red')
    ax2.annotate(f'L1 Difference: {min_objective_value:.8f}', (min_objective_value_index, min_objective_value), textcoords="offset points", xytext=(-15,-10), ha='center')
    ax1.plot(min_objective_value_index, min_samples_pp, marker='o', markersize=8, color='purple')
    ax1.annotate(f'Samples Per Pixel: {min_samples_pp:.2f}', (min_objective_value_index, min_samples_pp), textcoords="offset points", xytext=(0,10), ha='center')
    ax1.plot(min_objective_value_index, min_direct_samples, marker='o', markersize=8, color='blue')
    ax1.annotate(f'Direct Samples: {min_direct_samples:.2f}', (min_objective_value_index, min_direct_samples), textcoords="offset points", xytext=(0,10), ha='center')    
    ax1.plot(min_objective_value_index, min_indirect_samples, marker='o', markersize=8, color='orange')
    ax1.annotate(f'Indirect Samples: {min_indirect_samples:.2f}', (min_objective_value_index, min_indirect_samples), textcoords="offset points", xytext=(0,10), ha='center')    
    ax1.plot(min_objective_value_index, min_recursion_depth, marker='o', markersize=8, color='green')
    ax1.annotate(f'Recursion Depth: {min_recursion_depth:.2f}', (min_objective_value_index, min_recursion_depth), textcoords="offset points", xytext=(0,10), ha='center')        

    # Save the plot
    plt.title(scene)
    plt.grid(True)

    plt.savefig(f"graphs/TEST-{scene_name}.png")
    plt.close()

def partial_plot(scrubbed_data, scene_name, data_set):
    #sorted_data = sorted(scrubbed_data, key=lambda entry: entry[data_set])
    selected_data = [entry[data_set] for entry in scrubbed_data]
    l1_difference = [entry["L1_difference"] for entry in scrubbed_data]
    correlation_coefficient = np.corrcoef(selected_data, l1_difference)[0, 1]
    colour_mapping = {
        "samples_pp":       "tab:purple",
        "direct_samples":   "tab:blue",
        "indirect_samples": "tab:orange",
        "recursion_depth":  "tab:green"
    }
    label_mapping = {
        "samples_pp":       "Samples Per Pixel",
        "direct_samples":   "Direct Samples",
        "indirect_samples": "Indirect Samples",
        "recursion_depth":  "Recursion Depth"
    }
    # Plot the data
    fig_width = 16
    fig, ax1 = plt.subplots(figsize=(fig_width, 9), dpi=300)
    ax1.set_yticks(range(1, 33))
    ax1.plot(selected_data, label=label_mapping[data_set], color=colour_mapping[data_set])
    ax1.set_xlabel("Ordered Iteration")
    ax1.set_ylabel("Parameter Value", color="black")
    ax1.legend(loc="upper left")
    ax2 = ax1.twinx()
    ax2.plot(l1_difference, label="L1 Difference", color="tab:red")
    ax2.set_ylabel("L1 Difference", color="black")
    ax2.legend(loc="upper right")
    # Find the index of the minimum L1 difference
    min_l1_difference_index = l1_difference.index(min(l1_difference))
    min_l1_difference = min(l1_difference)
    min_selected_data = selected_data[min_l1_difference_index]
    # Annotate graph
    ax2.plot(min_l1_difference_index, min_l1_difference, marker='o', markersize=8, color='red')
    ax2.annotate(f'L1 Difference: {min_l1_difference:.8f}', (min_l1_difference_index, min_l1_difference), textcoords="offset points", xytext=(-15,-10), ha='center')
    ax1.plot(min_l1_difference_index, min_selected_data, marker='o', markersize=8, color=colour_mapping[data_set])
    ax1.annotate(f'{label_mapping[data_set]}: {min_selected_data:.2f}', (min_l1_difference_index, min_selected_data), textcoords="offset points", xytext=(0,10), ha='center')
    #ax1.annotate(f'Correlation: {correlation_coefficient:.2f}', (0.5, 0.9), xycoords='axes fraction', ha='center')
    # Save the plot
    plt.title(f"{scene_name}: {label_mapping[data_set]} vs L1 Difference. Correlation: {correlation_coefficient:.2f}")
    plt.grid(True)
    plt.savefig(f"graphs/TEST-{scene_name}-{data_set}.png")
    plt.close()

def l1_vs_render_time_heatmap(scrubbed_data, scene_name):
    l1_difference = np.array([entry["L1_difference"] for entry in scrubbed_data])
    render_time = np.array([entry["render_time"] / 1000.0 for entry in scrubbed_data])  # Convert to seconds

    # Filter out values more than 2 standard deviations from the mean
    mean_l1 = np.mean(l1_difference)
    std_dev_l1 = np.std(l1_difference)
    filtered_indices = np.where(np.abs(l1_difference - mean_l1) <= 2 * std_dev_l1)
    filtered_l1_difference = l1_difference[filtered_indices]
    filtered_render_time = render_time[filtered_indices]

    # Create a heatmap
    fig, ax = plt.subplots(figsize=(10, 8))
    ax.set_facecolor('white')  # Set background color of the axis
    heatmap = ax.hist2d(filtered_l1_difference, filtered_render_time, bins=(50, 50), cmap='inferno', cmin=1, weights=np.ones(len(filtered_l1_difference)))  # Use inferno colormap for dark background

    ax.set_xlabel("L1 Difference")
    ax.set_ylabel("Render Time (seconds)")
    ax.set_title(f"Heatmap: L1 Difference vs Render Time - {scene_name}")
    cbar = plt.colorbar(heatmap[3], ax=ax)
    cbar.set_label('Frequency')

    # Find the index of the minimum L1 difference
    min_l1_difference_index = np.argmin(filtered_l1_difference)
    min_l1_difference = filtered_l1_difference[min_l1_difference_index]
    min_render_time = filtered_render_time[min_l1_difference_index]

    # Annotate the minimum L1 value on the heatmap
    ax.plot(min_l1_difference, min_render_time, marker='o', markersize=16, color='green')

    # Save the heatmap
    plt.savefig(f"graphs/Heatmap_L1_vs_RenderTime_{scene_name}.png")
    plt.close()

def l1_vs_render_time_scatter(scrubbed_data, scene_name):
    l1_difference = np.array([entry["L1_difference"] for entry in scrubbed_data])
    render_time = np.array([entry["render_time"] / 1000.0 for entry in scrubbed_data])  # Convert to seconds

    # Filter out values more than 2 standard deviations from the mean
    mean_l1 = np.mean(l1_difference)
    std_dev_l1 = np.std(l1_difference)
    filtered_indices = np.where(np.abs(l1_difference - mean_l1) <= 2 * std_dev_l1)
    filtered_l1_difference = l1_difference[filtered_indices]
    filtered_render_time = render_time[filtered_indices]

    # Create a scatter plot
    plt.figure(figsize=(10, 8))
    plt.scatter(filtered_l1_difference, filtered_render_time, color='black', marker='o', alpha=0.7)
    plt.xlabel("L1 Difference")
    plt.ylabel("Render Time (seconds)")
    plt.title(f"Scatter Plot: L1 Difference vs Render Time - {scene_name}")
    plt.grid(True)

    # Find the index of the minimum L1 difference
    min_l1_difference_index = np.argmin(filtered_l1_difference)
    min_l1_difference = filtered_l1_difference[min_l1_difference_index]
    min_render_time = filtered_render_time[min_l1_difference_index]

    # Annotate the point with the minimum L1 difference
    plt.scatter(min_l1_difference, min_render_time, marker='o', color='red', label='Minimum L1 Difference')
    plt.annotate(f'Minimum L1 Difference', (min_l1_difference, min_render_time), textcoords="offset points", xytext=(-15, -10), ha='center', color='red', fontsize=10, fontweight='bold')

    plt.legend()

    # Save the scatter plot
    plt.savefig(f"Scatter_L1_vs_RenderTime_{scene_name}.png")
    plt.close()

def plot_pareto_frontier(Xs, Ys, maxX=True, maxY=True):
    sorted_list = sorted([[Xs[i], Ys[i]] for i in range(len(Xs))], reverse=maxY)
    pareto_front = [sorted_list[0]]
    for pair in sorted_list[1:]:
        if maxY:
            if pair[1] >= pareto_front[-1][1]:
                pareto_front.append(pair)
        else:
            if pair[1] <= pareto_front[-1][1]:
                pareto_front.append(pair)
    
    plt.scatter(Xs, Ys)
    pf_X = [pair[0] for pair in pareto_front]
    pf_Y = [pair[1] for pair in pareto_front]
    plt.plot(pf_X, pf_Y, color='green', marker='o', linestyle='--', linewidth=2, label='Pareto Frontier')
    plt.xlabel("L1 Difference")
    plt.ylabel("Render Time (seconds)")
    plt.title("Scatter Plot with Pareto Frontier")
    plt.legend()
    plt.grid(True)
    plt.show()

def l1_vs_render_time_scatter_with_pareto(scrubbed_data, scene_name):
    l1_difference = np.array([entry["L1_difference"] for entry in scrubbed_data])
    render_time = np.array([entry["render_time"] / 1000.0 for entry in scrubbed_data])  # Convert to seconds
    samples_pp = np.array([entry["samples_pp"] for entry in scrubbed_data])

    # Filter out values more than 2 standard deviations from the mean
    mean_l1 = np.mean(l1_difference)
    std_dev_l1 = np.std(l1_difference)
    filtered_indices = np.where(np.abs(l1_difference - mean_l1) <= 2 * std_dev_l1)
    filtered_l1_difference = l1_difference[filtered_indices]
    filtered_render_time = render_time[filtered_indices]
    filtered_samples_pp = samples_pp[filtered_indices]

    cmap = ListedColormap(['red', 'green', 'blue', 'purple'])

    # Create a scatter plot
    plt.figure(figsize=(10, 8))
    #plt.scatter(filtered_l1_difference, filtered_render_time, color='orange', marker='o', alpha=0.5, label='All Points')
    plt.scatter(filtered_l1_difference, filtered_render_time, c=filtered_samples_pp, cmap=cmap, marker='o', alpha=0.5, label='All Points')

    # Call the plot_pareto_frontier function to draw the Pareto frontier line
    #plot_pareto_frontier(filtered_l1_difference, filtered_render_time, maxX=False, maxY=False)

    # Find the index of the minimum L1 difference
    min_l1_difference_index = np.argmin(filtered_l1_difference)
    min_l1_difference = filtered_l1_difference[min_l1_difference_index]
    min_render_time = filtered_render_time[min_l1_difference_index]

    # Annotate the point with the minimum L1 difference
    #plt.scatter(min_l1_difference, min_render_time, marker='o', color='red', s=100, label='Minimum L1 Difference')
    
    plt.xlabel("L1 Difference")
    plt.ylabel("Render Time (seconds)")
    plt.title(f"Scatter Plot with Pareto Frontier: L1 Difference vs Render Time - {scene_name}")
    plt.grid(True)
    
    plt.legend()

    # Save the scatter plot
    plt.savefig(f"Scatter_Pareto_L1_vs_RenderTime_{scene_name}.png")
    plt.close()

if __name__ == "__main__":
    # Load data from the JSON file
    scene_name = [
        "cornell-box",
        "dragon",
        "split",
        "split-left",
        "split-right",
        "triple",
        "triple-al"
    ]
    path = [ 
        "testdata/cornell_20230822220351.json", 
        "testdata/dragon_20230823042906.json", 
        "testdata/split_20230823134721.json",
        "testdata/split_left_20230825041300.json", 
        "testdata/split_dark_20230824201235.json",
        "testdata/triple_20230823180538.json", 
        "testdata/triple_al_20230823203308.json" 
    ]

    for i, scene in enumerate(scene_name):
        with open(path[i], "r") as file:
            json_data = json.load(file)
        scrubbed_data = scrub_data(json_data)

        l1_vs_render_time_scatter_with_pareto(scrubbed_data, scene)

        #l1_vs_render_time_heatmap(scrubbed_data, scene)

        #full_plot(scrubbed_data, scene)
        #partial_plot(scrubbed_data, scene, "samples_pp")
        #partial_plot(scrubbed_data, scene, "direct_samples")
        #partial_plot(scrubbed_data, scene, "indirect_samples")
        #partial_plot(scrubbed_data, scene, "recursion_depth")
