import pandas as pd
import matplotlib.pyplot as plt

# Daten laden
df = pd.read_csv("test.csv")
df["ts"] = df["ts"] / 1000  # ms → Sekunden

print(df)

fig, axes = plt.subplots(3, 1, figsize=(12, 8))
fig.suptitle("IMU Rohdaten", fontsize=14)

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