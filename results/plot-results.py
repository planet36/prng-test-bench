# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

__author__ = 'Steven Ward'
__license__ = 'OSL-3.0'
__version__ = '2025-04-02'

import json
import sys

import matplotlib.pyplot as plt
from matplotlib.ticker import AutoMinorLocator

if len(sys.argv) != 2:
    sys.exit(f'Usage: {sys.argv[0]} prng-results.*.json')

prng_results_file_name = sys.argv[1]

with open(prng_results_file_name, encoding='utf-8') as f:
    prng_results = json.load(f)

# plot

fig, ax = plt.subplots()

# https://matplotlib.org/stable/gallery/color/named_colors.html
fig.set_facecolor('powderblue')

ax.margins(x=0.01)
ax.set_title("PRNG PractRand endurance vs. speed")

# https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.set_xlabel.html
ax.set_xlabel('Test ended at $2^x$ (bytes)')
# https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.set_ylabel.html
ax.set_ylabel('Throughput (GiB/s)')

# https://matplotlib.org/stable/api/_as_gen/matplotlib.axis.Axis.set_major_formatter.html
ax.yaxis.set_major_formatter(lambda x, pos: f'{x:,g}')

x_ticks = set()

# https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.scatter.html
# https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.annotate.html
# https://matplotlib.org/stable/gallery/text_labels_and_annotations/annotation_demo.html

groups = (
    (True, {'marker': 's', 'c': 'red'}, (30, 10), 'left'),
    (False, {'marker': '>', 'c': 'green'}, (-30, 10), 'right'),
)

for failed, style, offset, ha in groups:
    rows = [r for r in prng_results if r['test_ended_with_failure'] == failed]
    if not rows:
        continue

    x = [r['test_ended_at_exp2_bytes'] for r in rows]
    y = [r['prng_throughput_gibibytes_per_second'] for r in rows]
    ax.scatter(x, y, **style)
    x_ticks.update(range(min(x), max(x) + 1))

    for r, xi, yi in zip(rows, x, y, strict=True):
        ax.annotate(r['prng_name'], (xi, yi), xycoords='data', xytext=offset,
                    textcoords='offset points', arrowprops={'arrowstyle': '-'}, ha=ha)

ax.set_xticks(sorted(x_ticks))

ax.set_ylim(bottom=0)
ax.yaxis.set_minor_locator(AutoMinorLocator())
ax.grid(visible=True, which='both', axis='y')

# https://stackoverflow.com/a/4066599
fig.subplots_adjust(left=0.08, right=0.92, top=0.92, bottom=0.08)

plt.show()
