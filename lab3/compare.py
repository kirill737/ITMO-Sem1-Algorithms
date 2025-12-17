import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("../build/result.csv")

N = df["N"]
insert_time = df["insert_ns"]
quick_time = df["quick_ns"]


plt.figure(figsize=(10, 6))
plt.plot(N, insert_time, marker='o', label="Insertion Sort")
plt.plot(N, quick_time, marker='o', label="Quick Sort")

plt.xlabel("Размер массива")
plt.ylabel("Время сортировки (наносекунды)")
plt.title("Сравнение Insertion Sort и Quick Sort")
plt.legend()
plt.grid(True)

plt.show()