import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('results.csv')

plt.figure(figsize=(12, 6))

x_values = data['time_index']

plt.plot(x_values, data['exact_mean'],
         label='Точное значение $F_t^0$',
         linewidth=3, marker='o', markersize=8, color='green')

plt.plot(x_values, data['estimate_mean'],
         label='Оценка $N_t$',
         linewidth=3, marker='s', markersize=8, color='blue', linestyle='--')

plt.xlabel('Номер шага (момент времени $t$)')
plt.ylabel('Количество уникальных элементов')
plt.title('Сравнение оценки $N_t$ и $F_t^0$')
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)

if 'prefix_size' in data.columns:
    total_size = data['prefix_size'].iloc[-1]
    labels = []
    for i, size in enumerate(data['prefix_size']):
        percentage = int((size / total_size) * 100)
        labels.append(f'Шаг {i}\n({percentage}%)')
    plt.xticks(x_values, labels)

plt.tight_layout()
plt.savefig('graph1_comparison.png', dpi=300)
plt.show()

plt.figure(figsize=(12, 6))

plt.plot(x_values, data['estimate_mean'],
         label='$\mathbb{E}(N_t)$',
         linewidth=3, color='red')

plt.fill_between(x_values,
                 data['estimate_minus_std'],
                 data['estimate_plus_std'],
                 alpha=0.3, color='red',
                 label='$\mathbb{E}(N_t) \pm \sigma_{N_t}$')

plt.xlabel('Номер шага (момент времени $t$)')
plt.ylabel('Оценка кардинальности')
plt.title('Статистики оценки: $\mathbb{E}(N_t)$ и область неопределенности')
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)

if 'prefix_size' in data.columns:
    plt.xticks(x_values, labels)

plt.tight_layout()
plt.savefig('graph2_statistics.png', dpi=300)
plt.show()