import numpy as np
import subprocess
import datetime
import time
import signal
import json
from scipy.optimize import minimize

print("Oingo Boingo!")

# Ranges for each parameter
PARAMETER_RANGES = {
    "samples_pp":       (1, 4),
    "direct_samples":   (1, 32),
    "indirect_samples": (1, 16),
    "recursion_depth":  (1, 3),
}

# Time limit for each render in seconds
RENDER_TIME_LIMIT = 30  # 30 seconds

# Time weight to balance render time and L1 difference in the objective function
#time_weight = 0.0001

def watchdog(func):
    def inner(*args, **kwargs):
        def timout_handler(signum, frame):
            raise RuntimeError("Render time limit exceeded")
        signal.signal(signal.SIGALRM, timout_handler)
        signal.alarm(RENDER_TIME_LIMIT)
        return func(*args, **kwargs)
    return inner

def objective_function(params, render_time_list, time_weight, save_output=False):
    # Ensure that all parameter values are between 0 and 1
    if not all(0 <= p <= 1 for p in params):
        return float('inf')
    print(params[0], params[1], params[2])   

    # Convert parameters from [0, 1] range to their actual values in PARAMETER_RANGES
    actual_params = []
    for param_name, (param_min, param_max) in PARAMETER_RANGES.items():
        param_value = param_min + params[list(PARAMETER_RANGES.keys()).index(param_name)] * (param_max - param_min)
        rounded_param_value = round(param_value)
        actual_params.append(rounded_param_value)

    samples_pp, direct_samples, indirect_samples, recursion_depth = actual_params

    executable_path = './build/release/apps/ray-tracer'
    resolution_param = "-r 1280x720"
    #environment_param = "-e 1"
    #environment_param = "-e 2"
    #environment_param = "-e 3"
    #environment_param = "-e 4"
    #environment_param = "-e 5"
    environment_param = "-e 6"
    #environment_param = "-e 7"
    denoise_param = "-k"
    save_param = "-m"
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    output_file_name = f"output/py/test-cornell-box_{timestamp}-d{direct_samples}-h{indirect_samples}-i{recursion_depth}.exr"
    output_param = f"-o {output_file_name}"
    samples_per_pixel_param = f"-p {samples_pp}"
    direct_samples_param = f"-d {direct_samples}"
    indirect_samples_param = f"-h {indirect_samples}"
    recursion_depth_param = f"-i {recursion_depth}"
    if (save_output):
        command = f"{executable_path} {resolution_param} {environment_param} {samples_per_pixel_param} {direct_samples_param} {indirect_samples_param} {recursion_depth_param} {output_param} {denoise_param} {save_param}"
    else:
        command = f"{executable_path} {resolution_param} {environment_param} {samples_per_pixel_param} {direct_samples_param} {indirect_samples_param} {recursion_depth_param} {output_param} {denoise_param}"

    #@watchdog
    def oingoboingo():
        return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    # Initialize objective_value before the try-except block
    objective_value = float('inf')

    try:
        # Run executable
        process = oingoboingo()

        # Print the captured output for each iteration
        print("Iteration Output:")
        print({ "spp": samples_pp, "direct_samples": direct_samples, "indirect_samples": indirect_samples, "recursion_depth": recursion_depth })
        render_time = 0
        for line in process.stdout:
            line_str = line.decode('utf-8').strip()
            print(line_str)
            if "L1 difference:" in line_str:
                l1_difference = float(line_str.split(":")[1])
            elif "L2 difference:" in line_str:
                l2_difference = float(line_str.split(":")[1])
            elif "RenderFilm took" in line_str:
                render_time = int(line_str.split()[-2])        

        # Use a combination of L1 difference and render time as the objective value
        objective_value = (l1_difference + l2_difference) + time_weight * render_time
        render_time_list.append((render_time, objective_value))
        print(f"Objective value: {objective_value}")

        print("\n")

        return objective_value

    except (subprocess.CalledProcessError, ValueError, RuntimeError) as e:
        # If the executable fails or returns invalid output, return a large value to avoid it
        print(f"Exception: {e}\n")
        objective_value = float('inf')
        return objective_value

def generate_random_initial_guess():
    initial_guess = []
    for param_range in PARAMETER_RANGES.values():
        rand_val = np.random.uniform(0, 1)
        initial_guess.append(rand_val)
    return np.array(initial_guess)

def generate_middle_scaled_initial_guess():
    initial_guess = []
    for param_range in PARAMETER_RANGES.values():
        param_mid = (param_range[0] + param_range[1]) / 2.0
        scaled_val = (param_mid - param_range[0]) / (param_range[1] - param_range[0])
        initial_guess.append(scaled_val)
    return np.array(initial_guess)

def generate_max_initial_guess():
    initial_guess = []
    for param_range in PARAMETER_RANGES.values():
        initial_guess.append(1.0)
    return np.array(initial_guess)

def generate_min_initial_guess():
    initial_guess = []
    for param_range in PARAMETER_RANGES.values():
        initial_guess.append(0.0)
    return np.array(initial_guess)

def test_guess():
    initial_guess = [1.0, 1.0, 0.373387, 0.793112]
    return np.array(initial_guess)

# Nelder-Mead optimization
def nelder_mead_optimization(render_time_list, time_weight, initial_guess, perturbation_scale, max_iterations=1):
    print("Running Nelder-Mead optimization")
    all_results = []
    for _ in range(max_iterations):
        print(f"Iteration {_ + 1} of {max_iterations}")
        # Add random perturbation to the initial guess
        perturbation = np.random.normal(scale=perturbation_scale, size=len(initial_guess))
        perturbed_guess = initial_guess + perturbation
        perturbed_guess = np.clip(perturbed_guess, 0.0, 1.0)
        result = minimize(
            objective_function,
            perturbed_guess,
            args=(render_time_list, time_weight),
            method='Nelder-Mead')
        best_vertex = result.x
        direct_samples, indirect_samples, recursion_depth = best_vertex
        objective_value = result.fun
        l1_difference = result.fun
        iteration_data = (best_vertex, objective_value, direct_samples, indirect_samples, recursion_depth, result.nfev, l1_difference)
        all_results.append(iteration_data)
    return all_results

def cobyla_optimization(render_time_list, time_weight, initial_guess, perturbation_scale, max_iterations=5):
    print("Running COBYLA optimization")
    all_results = []
    for _ in range(max_iterations):
        print(f"Iteration {_ + 1} of {max_iterations}")
        # Add random perturbation to the initial guess
        perturbation    = np.random.normal(scale=perturbation_scale, size=len(initial_guess))
        perturbed_guess = initial_guess + perturbation
        perturbed_guess = [max(min(val, 1.0), 0.0) for val in perturbed_guess]
        result = minimize(
            objective_function,
            perturbed_guess,
            args=(render_time_list, time_weight),
            method='COBYLA')
        best_vertex = result.x
        samples_pp, direct_samples, indirect_samples, recursion_depth = best_vertex
        objective_value = result.fun
        l1_difference = result.fun
        iteration_data = (best_vertex, objective_value, samples_pp, direct_samples, indirect_samples, recursion_depth, result.nfev, l1_difference)
        all_results.append(iteration_data)
    return all_results

# Trust-constr optimization
def trust_constr_optimization(render_time_list, time_weight, initial_guess, max_iterations=5):
    result = minimize(
        objective_function,
        initial_guess,
        args=(render_time_list, time_weight),
        method='trust-constr',
        options={'maxiter': max_iterations})
    return [(result.x, result.fun)]

def shrink_simplex(vertices, best_vertex):
    # Replace all vertices except the best with the new points obtained by shrinking towards the best point.
    sigma = 0.5
    for i in range(len(vertices)):
        if not np.array_equal(vertices[i], best_vertex):
            vertices[i] = best_vertex + sigma * (vertices[i] - best_vertex)
    return vertices

def main():
    cob = True
    dt = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    output_file = f"output_data_{dt}.json"
    all_json_data = []
    absolute_best_result = []
    num_attempts = 100

    # Iterate over different time weight values
    #time_weights = [0, 0.0001, 0.001, 0.01, 0.1, 0.2, 0.5, 0.75, 1]
    # 1/(time * L1/L2)
    time_weights = [0]

    for time_weight in time_weights:
        print(f"Optimizing with time weight: {time_weight}")

        all_results_for_time_weight = []  # List to store all results for the current time weight
        best_result = None  # Initialize best_result to None

        for attempt_num in range(num_attempts):
            print(f"Attempt {attempt_num + 1} of {num_attempts}")

            render_time_list = []  # List to store render times for each optimization iteration

            if attempt_num == 0:
                initial_guess = generate_min_initial_guess()
            elif best_result is not None:
                best_params = best_result[0]
                perturbation_range = 1.0 / (attempt_num + 1)**2
                perturbation = np.random.uniform(-perturbation_range, perturbation_range, size=len(best_params))
                initial_guess = np.clip(best_params + perturbation, 0, 1)

            if (cob):
                results = cobyla_optimization(render_time_list, time_weight, initial_guess, 0.1)
            else:
                results = nelder_mead_optimization(render_time_list, time_weight, initial_guess, 0)
            all_results_for_time_weight.extend(results)  # Store all results for this time weight

            # Find best result of last three sub-iterations
            best_result = min(results, key=lambda x: x[1])

            # Perform the Shrink step
            if (cob):
                best_vertex = best_result[0]
                vertices = [result[0] for result in results]
                vertices = shrink_simplex(vertices, best_vertex)

            # Find the best result overall
            absolute_best_result = min(all_results_for_time_weight, key=lambda x: x[1])

            # Create a new copy of json_data dictionary for this iteration
            json_data = {
                "time_weight": [],
                "attempt": [],
                "iteration": [],
                "samples_pp": [],
                "direct_samples": [],
                "indirect_samples": [],
                "recursion_depth": [],
                "render_time": [],
                "L1_difference": [],
                "objective_value": []
            }

            for iteration, (best_vertex, objective_value, samples_pp, direct_samples, indirect_samples, recursion_depth, nfev, l1_difference) in enumerate(results, 1):
                # Append the data to the json_data dictionary
                matching_render_time = -1
                for render_time, render_time_obj_value in render_time_list:
                    if render_time_obj_value == objective_value:
                        matching_render_time = render_time
                        break
                
                ## TODO: NORMALIZE WEIGHTS AGAINST PARAMETER RANGES                
                json_data["time_weight"].append(time_weight)
                json_data["attempt"].append(attempt_num)
                json_data["iteration"].append(iteration)
                json_data["samples_pp"].append(samples_pp)
                json_data["direct_samples"].append(direct_samples)
                json_data["indirect_samples"].append(indirect_samples)
                json_data["recursion_depth"].append(recursion_depth)
                json_data["render_time"].append(matching_render_time)
                json_data["L1_difference"].append(l1_difference)
                json_data["objective_value"].append(objective_value + time_weight * matching_render_time)

            # Append the json_data for the current iteration to all_json_data
            all_json_data.append(json_data)

        # Run the best result for each time weight one more time and save the output
        best_params = absolute_best_result[0]
        print(f"Running the best result for {time_weight} one more time with params: {best_params}")
        objective_function(best_params, [], time_weight, save_output=True)  

    # Write the json data to the output file
    with open(output_file, 'w') as f:
        json.dump(all_json_data, f, indent=4)

    print(f"JSON data has been written to {output_file}")

if __name__ == "__main__":
    main()
