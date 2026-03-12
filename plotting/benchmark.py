import math
import sys
from fractions import Fraction

import matplotlib.pyplot as plt
import pandas as pd

from colors import colormap


styles = [
    '-',
    '--',
    '-.',
    ':',
    (0, (1, 1)),
    (0, (5, 1)),
    (0, (3, 1, 1, 1)),
    (0, (3, 5, 1, 5)),
    (0, (5, 5)),
    (0, (3, 10, 1, 10)),
]
colours = [
    'tab:blue',
    'tab:orange',
    'tab:green',
    'tab:red',
    'tab:purple',
    'tab:brown',
    'tab:pink',
    'tab:gray',
    'tab:olive',
    'tab:cyan',
    'navy',
    'teal',
    'crimson',
    'mediumseagreen',
    'slateblue',
]
min_scaling_points = 3
single_choice_loglog_algorithms = ['QBL', 'Exp3_heap', 'Exp3LightHeap']


def format_fraction(value):
    if value.denominator == 1:
        return str(value.numerator)
    return f"{value.numerator}/{value.denominator}"


def format_ratio_relation(value):
    if value.numerator == 1:
        return f"m = k/{value.denominator}"
    if value.denominator == 1:
        return f"m = {value.numerator}k"
    return f"m = {value.numerator}k/{value.denominator}"


def load_data(csv_path):
    df = pd.read_csv(csv_path, on_bad_lines='skip')
    df['name'] = df['name'].astype(str).str.strip()
    df['k'] = pd.to_numeric(df['k'], errors='coerce')
    df['m'] = pd.to_numeric(df['m'], errors='coerce')
    df['real_time'] = pd.to_numeric(df['real_time'], errors='coerce')
    df = df.dropna(subset=['name', 'k', 'm', 'real_time'])
    df = df[df['name'] != 'name']
    df = df[(df['k'] > 0) & (df['m'] > 0) & (df['real_time'] > 0)]
    df['k'] = df['k'].astype(int)
    df['m'] = df['m'].astype(int)
    df = df.drop_duplicates(subset=['name', 'k', 'm', 'real_time'])
    df['time_s'] = df['real_time'] / 1000.0
    df['m_over_k'] = [Fraction(m, k).limit_denominator() for m, k in zip(df['m'], df['k'])]
    return df.sort_values(['name', 'k', 'm'])


def get_plot_specs(df):
    specs = []

    used_indices = set()
    single_choice_parts = []

    for _, algo_rows in df.groupby('name'):
        algo_rows = algo_rows.copy()

        explicit_single_choice = algo_rows.loc[algo_rows['m'] == 1].copy()
        if explicit_single_choice['k'].nunique() >= 2:
            single_choice_parts.append(explicit_single_choice)
            used_indices.update(explicit_single_choice.index.tolist())
            continue

        remaining_algo_rows = algo_rows.loc[~algo_rows.index.isin(used_indices)].copy()
        constant_m_candidates = []
        for _, constant_m_rows in remaining_algo_rows.groupby('m'):
            constant_m_rows = (
                constant_m_rows.copy()
                .sort_index()
                .drop_duplicates(subset=['k'], keep='first')
            )
            if constant_m_rows['k'].nunique() < 2:
                continue
            constant_m_candidates.append(constant_m_rows)

        if constant_m_candidates:
            chosen_single_choice = min(
                constant_m_candidates,
                key=lambda rows: rows.index.min(),
            )
            single_choice_parts.append(chosen_single_choice)
            used_indices.update(chosen_single_choice.index.tolist())

    single_choice_rows = pd.concat(single_choice_parts, ignore_index=False) if single_choice_parts else pd.DataFrame()
    if single_choice_rows['k'].nunique() >= 2:
        specs.append({
            'rows': single_choice_rows,
            'x_column': 'k',
            'log_y': False,
            'title': 'Single-choice runtime vs k',
        })
        specs.append({
            'rows': single_choice_rows,
            'x_column': 'k',
            'log_y': True,
            'title': 'Single-choice runtime vs k log-log',
            'preferred_algorithms': single_choice_loglog_algorithms,
        })

    remaining_df = df.loc[~df.index.isin(used_indices)].copy()
    fixed_k_parts = {}
    fixed_k_indices = set()
    for _, algo_rows in remaining_df.groupby('name'):
        for k_value, fixed_rows in algo_rows.groupby('k'):
            fixed_rows = fixed_rows.copy()
            if fixed_rows['m'].nunique() < min_scaling_points:
                continue
            fixed_k_parts.setdefault(k_value, []).append(fixed_rows)
            fixed_k_indices.update(fixed_rows.index.tolist())

    ratio_df = remaining_df.loc[~remaining_df.index.isin(fixed_k_indices)].copy()
    ratio_parts = {}
    for ratio, ratio_rows in ratio_df.groupby('m_over_k'):
        ratio_rows = ratio_rows.copy()
        if ratio_rows['k'].nunique() < 2:
            continue
        ratio_parts[ratio] = ratio_rows

    ratios = sorted(ratio_parts, key=lambda value: (value.numerator / value.denominator, value.numerator, value.denominator))
    for ratio in ratios:
        rows = ratio_parts[ratio].copy()
        ratio_label = format_ratio_relation(ratio)
        specs.append({
            'rows': rows,
            'x_column': 'k',
            'log_y': False,
            'title': ratio_label,
        })
        specs.append({
            'rows': rows,
            'x_column': 'k',
            'log_y': True,
            'title': f'{ratio_label} (log-log)',
        })

    for k_value in sorted(fixed_k_parts):
        rows = pd.concat(fixed_k_parts[k_value], ignore_index=True)
        specs.append({
            'rows': rows,
            'x_column': 'm',
            'log_y': False,
            'title': f'Runtime vs m (k = {k_value})',
        })
        specs.append({
            'rows': rows,
            'x_column': 'm',
            'log_y': True,
            'title': f'Runtime vs m log-log (k = {k_value})',
        })

    if specs:
        return specs

    multi_choice_df = remaining_df.copy()

    if multi_choice_df['k'].nunique() >= 2:
        return [
            {
                'rows': multi_choice_df.copy(),
                'x_column': 'k',
                'log_y': False,
                'title': 'Runtime vs k',
            },
            {
                'rows': multi_choice_df.copy(),
                'x_column': 'k',
                'log_y': True,
                'title': 'Runtime vs k log-log',
            },
        ]

    if multi_choice_df['m'].nunique() >= 2:
        return [
            {
                'rows': multi_choice_df.copy(),
                'x_column': 'm',
                'log_y': False,
                'title': 'Runtime vs m',
            },
            {
                'rows': multi_choice_df.copy(),
                'x_column': 'm',
                'log_y': True,
                'title': 'Runtime vs m log-log',
            },
        ]

    return []


def draw_plot(ax, spec):
    rows = spec['rows']
    rows = rows.loc[
        rows.groupby('name')[spec['x_column']].transform('nunique') >= 2
    ].copy()

    if spec.get('preferred_algorithms'):
        rows = rows.loc[rows['name'].isin(spec['preferred_algorithms'])].copy()

    algorithms = sorted(set(rows['name']))

    for nr, algo in enumerate(algorithms):
        algo_rows = rows.loc[rows['name'] == algo].sort_values(spec['x_column'])
        if algo_rows.empty:
            continue

        ax.plot(
            algo_rows[spec['x_column']],
            algo_rows['time_s'],
            linestyle=styles[nr % len(styles)],
            color=colormap.get(algo, colours[nr % len(colours)]),
            marker='o',
            label=algo,
        )

    ax.set_xlabel(spec['x_column'])
    ax.set_ylabel('Time (s)')
    ax.set_title(spec['title'])
    ax.set_xscale('log')
    if spec['log_y']:
        ax.set_yscale('log')
    ax.grid(True, which='both', linestyle=':')
    if algorithms:
        ax.legend(fontsize=10)


def build_figure(specs):
    columns = 2 if len(specs) > 1 else 1
    rows = math.ceil(len(specs) / columns)
    fig, axes = plt.subplots(rows, columns, figsize=(6 * columns, 4 * rows), squeeze=False)
    flat_axes = axes.flatten()

    for axis, spec in zip(flat_axes, specs):
        draw_plot(axis, spec)

    for axis in flat_axes[len(specs):]:
        axis.set_visible(False)

    fig.tight_layout()
    return fig


def main():
    df = load_data(sys.argv[1])
    specs = get_plot_specs(df)

    if not specs:
        raise ValueError('No plottable benchmark rows were found in the provided CSV file.')

    figure = build_figure(specs)
    if len(sys.argv) >= 3:
        figure.savefig(sys.argv[2])
    else:
        plt.show()


if __name__ == '__main__':
    main()
