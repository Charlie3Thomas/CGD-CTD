import re
import matplotlib.pyplot as plt

def parse_log_file(log_file_path):
    results = {}
    current_time_weight = None
    current_result = {}

    with open(log_file_path, 'r') as log_file:
        log_lines = log_file.readlines()

        for line in log_lines:
            if 'Optimizing with time weight: ' in line:
                current_time_weight = float(re.search(r'[-+]?\d*\.\d+|\d+', line).group())
                results[current_time_weight] = []
            elif 'Rendering film with' in line and 'direct samples' in line:
                direct_samples_match = re.search(r'\b\d+\b', line)
                if direct_samples_match:
                    N = int(direct_samples_match.group())
                    current_result['direct_samples'] = N
            elif 'Rendering film with' in line and 'indirect samples' in line:
                indirect_samples_match = re.search(r'\b\d+\b', line)
                if indirect_samples_match:
                    N = int(indirect_samples_match.group())
                    current_result['indirect_samples'] = N
            elif 'Rendering film with' in line and 'recursion depth' in line:
                recursion_depth_samples_match = re.search(r'\b\d+\b', line)
                if recursion_depth_samples_match:
                    N = int(recursion_depth_samples_match.group())
                    current_result['recursion_depth'] = N
            elif 'RenderFilm took' in line:
                render_film_ms_match = re.search(r'(\d+)', line)
                if render_film_ms_match:
                    current_result['RenderFilm_ms'] = int(render_film_ms_match.group(1))
            elif 'L1 Difference' in line:
                l1_difference_match = re.search(r'L1 Difference: ([0-9.]+)', line)
                if l1_difference_match:
                    current_result['L1_difference'] = float(l1_difference_match.group(1))
            elif 'L2 Difference' in line:
                l2_difference_match = re.search(r'L2 Difference: ([0-9.]+)', line)
                if l2_difference_match:
                    current_result['L2_difference'] = float(l2_difference_match.group(1))
            elif 'Objective value' in line:
                objective_value_match = re.search(r'Objective value: ([0-9.]+)', line)
                if objective_value_match:
                    current_result['Objective_value'] = float(objective_value_match.group(1))
                    results[current_time_weight].append(current_result)
                    current_result = {}

    return results


if __name__ == "__main__":
    log_file_path = "overnight.txt"
    extracted_data = parse_log_file(log_file_path)

    for time_weight, test_results in extracted_data.items():
        print(f"Time Weight: {time_weight}")

        for i, data in enumerate(test_results):
            print(f"Test Set {i + 1}:")
            print("Direct Samples:", data.get('direct_samples', None))
            print("Indirect Samples:", data.get('indirect_samples', None))
            print("Recursion Depth:", data.get('recursion_depth', None))
            print("RenderFilm_ms:", data.get('RenderFilm_ms', None))
            print("L1 Difference:", data.get('L1_difference', None))
            print("L2 Difference:", data.get('L2_difference', None))
            print("Objective Value:", data.get('Objective_value', None))
            print()

        ## Plot Direct samples vs L1 difference
        #plt.figure(figsize=(10, 6))
        #if direct_samples_l1:
        #    x, y = zip(*direct_samples_l1)
        #    plt.plot(x, y, marker='o')
        #    plt.xlabel("Direct Samples")
        #    plt.ylabel("L1 Difference")
        #    plt.title(f"Direct Samples vs L1 Difference (Time Weight: {time_weight})")
        #    plt.grid(True)
        #    plt.savefig(f"direct_samples_l1_plot_{time_weight}.png")
        #    plt.close()
#
        ## Plot Indirect samples vs L1 difference
        #plt.figure(figsize=(10, 6))
        #if indirect_samples_l1:
        #    x, y = zip(*indirect_samples_l1)
        #    plt.plot(x, y, marker='o', color='r')
        #    plt.xlabel("Indirect Samples")
        #    plt.ylabel("L1 Difference")
        #    plt.title(f"Indirect Samples vs L1 Difference (Time Weight: {time_weight})")
        #    plt.grid(True)
        #    plt.savefig(f"indirect_samples_l1_plot_{time_weight}.png")
        #    plt.close()
#
        ## Plot Direct samples vs Objective value
        #plt.figure(figsize=(10, 6))
        #if direct_samples_obj:
        #    x, y = zip(*direct_samples_obj)
        #    plt.plot(x, y, marker='o', color='g')
        #    plt.xlabel("Direct Samples")
        #    plt.ylabel("Objective Value")
        #    plt.title(f"Direct Samples vs Objective Value (Time Weight: {time_weight})")
        #    plt.grid(True)
        #    plt.savefig(f"direct_samples_obj_plot_{time_weight}.png")
        #    plt.close()
#
        ## Plot Indirect samples vs Objective value
        #plt.figure(figsize=(10, 6))
        #if indirect_samples_obj:
        #    x, y = zip(*indirect_samples_obj)
        #    plt.plot(x, y, marker='o', color='purple')
        #    plt.xlabel("Indirect Samples")
        #    plt.ylabel("Objective Value")
        #    plt.title(f"Indirect Samples vs Objective Value (Time Weight: {time_weight})")
        #    plt.grid(True)
        #    plt.savefig(f"indirect_samples_obj_plot_{time_weight}.png")
        #    plt.close()
