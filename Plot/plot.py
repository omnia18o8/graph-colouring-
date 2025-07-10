import pandas as pd
import matplotlib.pyplot as plt
import os

def load_results(csv_path, label):
    df = pd.read_csv(
        csv_path,
        header=None,
        names=[
            "filename", "nodes", "delta", "constant", "colours_used",
            "total_time", "total_memory", "avg_list_size", "conflict_edges"
        ]
    )
    df["memory_MB"] = df["total_memory"] / (1024 * 1024)
    df["Delta+1"] = df["delta"] + 1
    df["label"] = label
    df["shortfile"] = df["filename"].apply(lambda x: os.path.basename(str(x)))
    return df

def plot_vs_deltap1(apst_df, greedy_df, metric, ylabel, filename):
    plt.figure(figsize=(8, 6))

    # Use only constant == 40 for APST
    apst_40 = apst_df[apst_df["constant"] == 40].sort_values("Delta+1")
    greedy_sorted = greedy_df.sort_values("Delta+1")

    plt.plot(apst_40["Delta+1"], apst_40[metric], marker='o', linestyle='-', label='APST (constant=40)')
    plt.plot(greedy_sorted["Delta+1"], greedy_sorted[metric], marker='s', linestyle='--', label='Greedy')

    # Annotate each point with the filename
    for i, row in apst_40.iterrows():
        plt.annotate(row["shortfile"], (row["Delta+1"], row[metric]), textcoords="offset points", xytext=(0,5), ha='center', fontsize=8)
    for i, row in greedy_sorted.iterrows():
        plt.annotate(row["shortfile"], (row["Delta+1"], row[metric]), textcoords="offset points", xytext=(0,-12), ha='center', fontsize=8, color='red')

    plt.xlabel("Delta + 1")
    plt.ylabel(ylabel)
    plt.title(f"{ylabel} vs Delta+1 (APST constant=40 vs Greedy)")
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
    plt.savefig(filename)
    plt.close()
    print(f"Saved: {filename}")

if __name__ == "__main__":
    apst_file = "apst_results.csv"
    greedy_file = "greedy_results.csv"
    apst_df = load_results(apst_file, "APST")
    greedy_df = load_results(greedy_file, "Greedy")

    plot_vs_deltap1(apst_df, greedy_df, "colours_used", "Colours Used", "colours_vs_deltap1.png")
    plot_vs_deltap1(apst_df, greedy_df, "total_time", "Time (s)", "time_vs_deltap1.png")
    plot_vs_deltap1(apst_df, greedy_df, "memory_MB", "Memory (MB)", "memory_vs_deltap1.png")
