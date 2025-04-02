# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

__author__ = 'Steven Ward'
__license__ = 'OSL-3.0'
__version__ = '2023-09-08'

import json
import matplotlib.pyplot as plt
import sys

# This is a prng-results.*.json file.
prng_results_file_name = sys.argv[1]

with open(prng_results_file_name) as f:
    prng_results = json.load(f)

prng_results_failure = []
prng_results_nonfailure = []

for prng_result in prng_results:
    # massage the data
    tmp = {
            'prng_name': prng_result['prng_name'],
            'test_ended_at_exp2_bytes': prng_result['test_ended_at_exp2_bytes'],
            'prng_throughput_mebibytes': prng_result['prng_throughput_mebibytes']
            }

    if prng_result['test_ended_with_failure']:
        prng_results_failure.append(tmp)
    else:
        prng_results_nonfailure.append(tmp)

# plot

fig, ax = plt.subplots()

# https://matplotlib.org/stable/gallery/color/named_colors.html
fig.set_facecolor('powderblue')

ax.margins(x=0.01)
ax.set_title("PRNG PractRand endurance vs. speed")

# https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.set_xlabel.html
ax.set_xlabel('Test ended at $2^x$ (bytes)')
# https://matplotlib.org/stable/api/_as_gen/matplotlib.axes.Axes.set_ylabel.html
ax.set_ylabel('Throughput (MiB/s)')

# https://matplotlib.org/stable/api/_as_gen/matplotlib.axis.Axis.set_major_formatter.html
ax.yaxis.set_major_formatter(lambda x, pos: '{:,}'.format(int(x)))

x_ticks = set()

# https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.scatter.html
# https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.annotate.html
# https://matplotlib.org/stable/gallery/text_labels_and_annotations/annotation_demo.html

x = [prng_result['test_ended_at_exp2_bytes'] for prng_result in prng_results_failure]
y = [prng_result['prng_throughput_mebibytes'] for prng_result in prng_results_failure]
plt.scatter(x, y, marker='s', c='red')
x_ticks |= set(range(min(x), max(x) + 1))

for i, label in enumerate(prng_result['prng_name'] for prng_result in prng_results_failure):
    ax.annotate(label, (x[i], y[i]), xycoords='data', xytext=(30, 10), textcoords='offset points', arrowprops=dict(arrowstyle="-"), ha='left')

x = [prng_result['test_ended_at_exp2_bytes'] for prng_result in prng_results_nonfailure]
y = [prng_result['prng_throughput_mebibytes'] for prng_result in prng_results_nonfailure]
plt.scatter(x, y, marker='>', c='green')
x_ticks |= set(range(min(x), max(x) + 1))

for i, label in enumerate(prng_result['prng_name'] for prng_result in prng_results_nonfailure):
    ax.annotate(label, (x[i], y[i]), xycoords='data', xytext=(-30, 10), textcoords='offset points', arrowprops=dict(arrowstyle="-"), ha='right')

plt.xticks(sorted(x_ticks))

plt.ylim(bottom=0)
plt.yticks(minor=True)
plt.grid(visible=True, which='both', axis='y')

plt.tight_layout()
# https://stackoverflow.com/a/4066599
plt.subplots_adjust(left=0.08, right=0.92, top=0.92, bottom=0.08)

plt.show()
