import pandas as pd
import matplotlib.pyplot as plt

plt.style.use('seaborn-v0_8-whitegrid')  

def load_apst_greedy(txt_path):
    df = pd.read_csv(
        txt_path,
        sep=r'\s+',      # Accept any whitespace separator
        header=0
    )

    # Auto-name columns if header is not present
    expected_cols = [
        "filename", "constant", "nodes", "delta", "success",
        "apst_colours", "apst_time", "apst_memory_GB",
        "greedy_colours", "greedy_time", "greedy_memory"
    ]

    df.columns = expected_cols[:len(df.columns)]
    if "success" in df.columns:
        df = df[df["success"] == 1]
    return df

def print_results_table(df):
    # Select columns for output
    cols = ["filename", "constant", "nodes", "delta", "apst_colours", "apst_time", "apst_memory_GB"]
    print("\nAPST Results Table (successful runs only):")
    print(df[cols].to_string(
        index=False,
        formatters={
            "apst_time": "{:.4f}".format,
            "apst_memory_GB": "{:.4f}".format,
        }
    ))

def plot_constant_metric(df, apst_metric, ylabel, filename):
    plt.figure(figsize=(9,6))
    # Group by filename for legend
    for graphfile, group in df.groupby("filename"):
        plt.plot(group["constant"], group[apst_metric], marker='o', linewidth=2, label=graphfile)
    min_c = df["constant"].min()
    max_c = df["constant"].max()
    plt.xlim(left=max(1, min_c - 1), right=max_c + 1)
    plt.xlabel("Constant in List Size Equation", fontsize=13)
    plt.ylabel(ylabel, fontsize=13)
    plt.title(f"{ylabel} vs Constant", fontsize=15)
    plt.xticks(range(int(min_c), int(max_c)+1, 2))
    plt.legend(title="Graph Filename", fontsize=10, title_fontsize=11)
    plt.tight_layout()
    plt.savefig(filename, dpi=200)
    plt.close()
    print(f"Saved: {filename}")

if __name__ == "__main__":
    result_txt = "../Plot/results.txt"
    df = load_apst_greedy(result_txt)
    print_results_table(df)
    plot_constant_metric(df, "apst_colours", "Colours Used", "Images/constant_vs_colours.png")
    plot_constant_metric(df, "apst_time", "Time (s)", "Images/constant_vs_time.png")
    plot_constant_metric(df, "apst_memory_GB", "Memory (MB)", "Images/constant_vs_memory.png")
