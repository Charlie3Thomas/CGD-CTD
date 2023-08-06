import numpy as np
import subprocess
import datetime
import time
import signal
from scipy.optimize import minimize

print("Oingo Boingo!")

# Ranges for each parameter
PARAMETER_RANGES = {
    "direct_samples":   (1, 8),
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

def objective_function(params, render_time_list, time_weight, calc_unscaled_L1=False):
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

    direct_samples, indirect_samples, recursion_depth = actual_params

    executable_path = './build/release/apps/ray-tracer'
    params_str = f"{direct_samples} {indirect_samples} {recursion_depth}"
    resolution_param = "-r 1280x720"
    environment_param = "-e 3"
    denoise_param = "-k"
    timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
    output_file_name = f"output/py/test-cornell-box_{timestamp}-d{direct_samples}-h{indirect_samples}-i{recursion_depth}.exr"
    output_param = f"-o {output_file_name}"
    direct_samples_param = f"-d {direct_samples}"
    indirect_samples_param = f"-h {indirect_samples}"
    recursion_depth_param = f"-i {recursion_depth}"
    command = f"{executable_path} {resolution_param} {environment_param} {direct_samples_param} {indirect_samples_param} {recursion_depth_param} {output_param} {denoise_param}"

    @watchdog
    def oingoboingo():
        return subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    try:
        # Run executable
        process = oingoboingo()

        # Print the captured output for each iteration
        print("Iteration Output:")
        print({ "direct_samples": direct_samples, "indirect_samples": indirect_samples, "recursion_depth": recursion_depth })
        render_time = 0
        for line in process.stdout:
            line_str = line.decode('utf-8').strip()
            print(line_str)
            if "L1 difference:" in line_str:
                L1_diff = float(line_str.split(":")[1])
            elif "RenderFilm took" in line_str:
                render_time = int(line_str.split()[-2])

        render_time_list.append(render_time)

        # If L1 difference is not found, return a large value to avoid it
        if "L1_diff" not in locals():
            L1_diff = float('inf')

        # Use a combination of L1 difference and render time as the objective value
        objective_value = L1_diff + time_weight * render_time
        print(f"Objective value: {objective_value}")

        print("\n")

        if calc_unscaled_L1:
            return L1_diff  # Return unscaled L1 value
        else:
            return objective_value  # Return the weighted objective value

    except (subprocess.CalledProcessError, ValueError, RuntimeError) as e:
        # If the executable fails or returns invalid output, return a large value to avoid it
        print(f"Exception: {e}\n")
        L1_diff = float('inf')
        return L1_diff

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

# Nelder-Mead optimization
def nelder_mead_optimization(render_time_list, time_weight, initial_guess, max_iterations=10):
    result = minimize(
        objective_function,
        initial_guess,
        args=(render_time_list, time_weight),
        method='Nelder-Mead',
        options={'maxiter': max_iterations})
    return [(result.x, result.fun)]

# COBYLA optimization
def cobyla_optimization(render_time_list, time_weight, initial_guess, max_iterations=5):
    result = minimize(
        objective_function,
        initial_guess,
        args=(render_time_list, time_weight),
        method='COBYLA',
        options={'maxiter': max_iterations})
    return [(result.x, result.fun)]

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
    absolute_best_result = []  # List to store the best result for each time weight

    # Number of optimization attempts
    num_attempts = 10

    # Iterate over different time weight values
    #time_weights = [0, 0.0001, 0.001, 0.01, 0.1, 0.2, 0.5, 0.75, 1]
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
                perturbation_range = 0.25
                perturbation = np.random.uniform(-perturbation_range, perturbation_range, size=len(best_params))
                initial_guess = np.clip(best_params + perturbation, 0, 1)

            results = cobyla_optimization(render_time_list, time_weight, initial_guess)
            all_results_for_time_weight.extend(results)  # Store all results for this time weight

            # Find best result of last three sub-iterations
            best_result = min(results, key=lambda x: x[1])

            # Perform the Shrink step
            best_vertex = best_result[0]
            vertices = [result[0] for result in results]
            vertices = shrink_simplex(vertices, best_vertex)

            # Update the best result after shrinking
            best_result = (best_vertex, objective_function(best_vertex, render_time_list, time_weight))

            # Log results for each attempt - do this by appending to the log file

            # Find the best result overall
            absolute_best_result = min(all_results_for_time_weight, key=lambda x: x[1])

        # Run the best result for each time weight one more time and save the output
        best_params = absolute_best_result[0]
        print(f"Running the best result for {time_weight} one more time with params: {best_params}")
        objective_function(best_params, [], time_weight, calc_unscaled_L1=True)     
        print("//////////////////////////////////////////////////////////////////////////////////////\n")
        print("//////////////////////////////////////////////////////////////////////////////////////\n")
        print("//////////////////////////////////////////////////////////////////////////////////////\n\n")

if __name__ == "__main__":
    main()
