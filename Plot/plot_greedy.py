import pandas as pd
import matplotlib.pyplot as plt
import itertools

plt.style.use('seaborn-v0_8-whitegrid')

def load_apst_greedy(txt_path):
    df = pd.read_csv(
        txt_path, 
        sep=r'\s+',
        header=0
    )
    if "apst_memory" in df.columns:
        df["apst_memory_MB"] = df["apst_memory"] / (1024 * 1024)
    if "greedy_memory" in df.columns:
        df["greedy_memory_MB"] = df["greedy_memory"] / (1024 * 1024)
    df["Delta+1"] = df["delta"] + 1
    df["filename"] = df["filename"].astype(str)
    return df

def plot_vs_deltap1(df, apst_metric, greedy_metric, ylabel, filename):
    plt.figure(figsize=(9,6))

    # Color cycle for all files
    color_cycle = plt.rcParams['axes.prop_cycle'].by_key()['color']
    filelist = sorted(df['filename'].unique())
    color_map = {fname: color_cycle[i % len(color_cycle)] for i, fname in enumerate(filelist)}

    for fname in filelist:
        group = df[(df['filename'] == fname) & (df["constant"] == 40)].sort_values("Delta+1")
        if group.empty: continue
        color = color_map[fname]
        # APST: line with circles
        plt.plot(group["Delta+1"], group[apst_metric], marker='o', linestyle='-', color=color, label=f"{fname} (APST)")
        # Greedy: single square, same color, no line
        plt.scatter(group["Delta+1"].iloc[0], group[greedy_metric].iloc[0], marker='s', color=color, s=80, label=f"{fname} (Greedy)")

    plt.xlabel("Delta + 1", fontsize=13)
    plt.ylabel(ylabel, fontsize=13)
    plt.title(f"{ylabel} vs Delta+1\n(APST constant=40 vs Greedy)", fontsize=15)
    plt.legend(fontsize=9, title="Legend", title_fontsize=10)
    plt.grid(True, alpha=0.4)
    plt.tight_layout()
    plt.savefig(filename, dpi=200)
    plt.close()
    print(f"Saved: {filename}")

if __name__ == "__main__":
    result_txt = "results.txt"
    df = load_apst_greedy(result_txt)

    plot_vs_deltap1(df, "apst_colours", "greedy_colours", "Colours Used", "colours_vs_deltap1.png")
    plot_vs_deltap1(df, "apst_time", "greedy_time", "Time (s)", "time_vs_deltap1.png")
    plot_vs_deltap1(df, "apst_memory_MB", "greedy_memory_MB", "Memory (MB)", "memory_vs_deltap1.png")
