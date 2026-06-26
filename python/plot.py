import pandas as pd
import matplotlib.pyplot as plt
import sys
from dataclasses import dataclass
import io

@dataclass(frozen=True)
class Trick:
    start_ms: int
    trick_name: str
    raw_data: pd.DataFrame
    result: str

def plot_trick(trick: Trick):


    fig, axes = plt.subplots(3, 1, figsize=(12, 8))
    fig.suptitle(f"IMU data for {trick.trick_name} - {trick.result}", fontsize=14)

    df = trick.raw_data

    # ── Rotationswinkel berechnen ──────────────────────────────
    # Zeitdifferenz zwischen jedem Sample
    df["deltaTime"] = df["ts"].diff().fillna(0)

    # Integration: Winkel += Gyro * deltaTime
    df["angleX"] = (df["gyr_x"] * df["deltaTime"]).cumsum()
    df["angleY"] = (df["gyr_y"] * df["deltaTime"]).cumsum()
    df["angleZ"] = (df["gyr_z"] * df["deltaTime"]).cumsum()

    # Accelerometer
    axes[0].plot(df["ts"], df["acc_x"], label="X")
    axes[0].plot(df["ts"], df["acc_y"], label="Y")
    axes[0].plot(df["ts"], df["acc_z"], label="Z")
    axes[0].set_title("Accelerometer (g)")
    axes[0].legend()
    axes[0].grid(True)

    # Gyroscope
    axes[1].plot(df["ts"], df["gyr_x"], label="X")
    axes[1].plot(df["ts"], df["gyr_y"], label="Y")
    axes[1].plot(df["ts"], df["gyr_z"], label="Z")
    axes[1].set_title("Gyroscope (°/s)")
    axes[1].legend()
    axes[1].grid(True)

    # Rotationswinkel
    axes[2].plot(df["ts"], df["angleX"], label="Rotation X")
    axes[2].plot(df["ts"], df["angleY"], label="Rotation Y")
    axes[2].plot(df["ts"], df["angleZ"], label="Rotation Z")
    axes[2].set_title("Rotationswinkel durch Gyro-Integration (°)")
    axes[2].legend()
    axes[2].grid(True)

    axes[1].set_xlabel("Zeit (s)")
    plt.tight_layout()
    plt.show()


def read_tricks_from_session(path):
    tricks = []
    trick_buffer = []
    result = ""
    trick_name = ""
    start_ms = 0
    with open(path, 'r') as f:
        for line in f:
            if line.startswith('---'):
                header = line.split(',')
                trick_name = header[1]
                start_ms = header[2]
            elif line.startswith('result'):
                footer = line.split(',')
                result = footer[1]
                df = pd.read_csv(io.StringIO("".join(trick_buffer)))
                df['ts'] = df['ts'].astype(int) - int(start_ms)
                tricks.append(Trick(start_ms, trick_name, df, result))
                trick_buffer = []
            else:
                trick_buffer.append(line)

    return tricks

def main():

    if len(sys.argv) != 2:
        print("Wrong number of args. Please pass a session file.")

    tricks = read_tricks_from_session(sys.argv[1])

    for trick in tricks:
        plot_trick(trick)


if __name__ == '__main__':
    main()