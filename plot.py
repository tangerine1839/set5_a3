import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('results.csv')

total_size = data['prefix_size'].iloc[-1]
labels = []
for i, size in enumerate(data['prefix_size']):
    percentage = int((size / total_size) * 100)
    labels.append(f'Шаг {i}\n({percentage}%)')

plt.figure(figsize=(12, 6))
plt.plot(data['time_index'], data['exact_mean'], label='$F_t^0$', linewidth=3, marker='o', color='green')
plt.plot(data['time_index'], data['estimate_mean'], label='$N_t$', linewidth=3, marker='s', color='blue', linestyle='--')
plt.xlabel('Номер шага (момент времени $t$)')
plt.ylabel('Количество уникальных элементов')
plt.title('Сравнение $F_t^0$ и $N_t$')
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.xticks(data['time_index'], labels)
plt.tight_layout()
plt.savefig('graph1_comparison.png', dpi=300)
plt.show()

plt.figure(figsize=(12, 6))
plt.plot(data['time_index'], data['estimate_mean'], label='$\mathbb{E}(N_t)$', linewidth=3, color='red')
plt.fill_between(data['time_index'], data['estimate_minus_std'], data['estimate_plus_std'], alpha=0.3, color='red', label='$\mathbb{E}(N_t) \pm \sigma_{N_t}$')
plt.xlabel('Номер шага (момент времени $t$)')
plt.ylabel('Количество уникальных элементов')
plt.title('Статистики оценки: $\mathbb{E}(N_t)$ и область неопределенности')
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.xticks(data['time_index'], labels)
plt.tight_layout()
plt.savefig('graph2_statistics.png', dpi=300)
plt.show()