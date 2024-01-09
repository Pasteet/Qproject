import matplotlib.pyplot as plt

def read_data_from_file(filename):
    with open(filename, 'r') as file:
        data = [float(line.strip()) for line in file.readlines()]

    return data

def display_graph(data):
    num_runs = len(data) - 1
    average_time = data[-1]

    fig, ax1 = plt.subplots()

    # Plot the run times on the left y-axis
    scatter = ax1.scatter(range(1, num_runs + 1), data[:-1], label='Run times', color='blue')
    ax1.set_xlabel('Run Number')
    ax1.set_ylabel('Duration (nanoseconds)', color='blue')
    ax1.tick_params('y', colors='blue')
    ax1.set_xticks(range(1, num_runs + 1))

    # Add vertical dotted lines for each result
    for i in range(1, num_runs + 1):
        ax1.vlines(i, 0, data[i - 1], linestyles='dotted', colors='gray', linewidth=0.8)

    # Plot the average time on the right y-axis
    ax2 = ax1.twinx()
    line = ax2.axhline(y=average_time, color='r', linestyle='-', label='Average Time')
    ax2.set_ylim(ax1.get_ylim())  # Match the y-axis scales
    ax2.tick_params('y', colors='red')
    ax2.set_yticks([average_time])

    # Disable scientific notation for y-axis ticks
    ax1.ticklabel_format(style='plain', axis='y')
    ax2.ticklabel_format(style='plain', axis='y')

    # Create a legend specifying the colors
    legend = plt.legend([scatter, line], ['Run times', 'Average Time'], loc='upper left')
    legend.get_frame().set_edgecolor('black')

    plt.title('Benchmark results')
    plt.show()

if __name__ == "__main__":
    filename = "results.txt"
    data = read_data_from_file(filename)

    if len(data) < 2:
        print("Not enough data to display.")
    else:
        display_graph(data)
