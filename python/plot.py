import pandas as pd
import matplotlib.pyplot as plt

# Daten laden
df = pd.read_csv("record.csv", names=["time","accX","accY","accZ","gyrX","gyrY","gyrZ"], header=None)
df["time"] = df["time"] / 1000  # ms → Sekunden

fig, axes = plt.subplots(3, 1, figsize=(12, 8))
fig.suptitle("IMU Rohdaten", fontsize=14)

# ── Rotationswinkel berechnen ──────────────────────────────
# Zeitdifferenz zwischen jedem Sample
df["deltaTime"] = df["time"].diff().fillna(0)

# Integration: Winkel += Gyro * deltaTime
df["angleX"] = (df["gyrX"] * df["deltaTime"]).cumsum()
df["angleY"] = (df["gyrY"] * df["deltaTime"]).cumsum()
df["angleZ"] = (df["gyrZ"] * df["deltaTime"]).cumsum()

# Accelerometer
axes[0].plot(df["time"], df["accX"], label="X")
axes[0].plot(df["time"], df["accY"], label="Y")
axes[0].plot(df["time"], df["accZ"], label="Z")
axes[0].set_title("Accelerometer (g)")
axes[0].legend()
axes[0].grid(True)

# Gyroscope
axes[1].plot(df["time"], df["gyrX"], label="X")
axes[1].plot(df["time"], df["gyrY"], label="Y")
axes[1].plot(df["time"], df["gyrZ"], label="Z")
axes[1].set_title("Gyroscope (°/s)")
axes[1].legend()
axes[1].grid(True)

# Rotationswinkel
axes[2].plot(df["time"], df["angleX"], label="Rotation X")
axes[2].plot(df["time"], df["angleY"], label="Rotation Y")
axes[2].plot(df["time"], df["angleZ"], label="Rotation Z")
axes[2].set_title("Rotationswinkel durch Gyro-Integration (°)")
axes[2].legend()
axes[2].grid(True)

# # Magnetometer
# axes[2].plot(df["time"], df["magX"], label="X")
# axes[2].plot(df["time"], df["magY"], label="Y")
# axes[2].plot(df["time"], df["magZ"], label="Z")
# axes[2].set_title("Magnetometer (µT)")
# axes[2].legend()
# axes[2].grid(True)

axes[1].set_xlabel("Zeit (s)")
plt.tight_layout()
plt.show()