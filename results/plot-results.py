# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: OSL-3.0

"""Plot PractRand endurance against throughput from a prng-results JSON file."""

__author__ = 'Steven Ward'
__license__ = 'OSL-3.0'
__version__ = '2026-09-16'

import argparse
import json

import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.ticker import AutoMinorLocator


def load_results(path: str) -> list[dict]:
    """Return the records in a prng-results.*.json file."""
    with open(path, encoding='utf-8') as f:
        return json.load(f)


def plot_results(prng_results: list[dict]) -> Figure:
    """Return a scatter plot of test length against throughput."""
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

    x_ticks: set[int] = set()

    # https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.scatter.html
    # https://matplotlib.org/stable/api/_as_gen/matplotlib.pyplot.annotate.html
    # https://matplotlib.org/stable/gallery/text_labels_and_annotations/annotation_demo.html

    groups = (
        (True, 's', 'red', (30, 10), 'left'),
        (False, '>', 'green', (-30, 10), 'right'),
    )

    for failed, marker, color, offset, ha in groups:
        rows = [r for r in prng_results if r['test_ended_with_failure'] == failed]
        if not rows:
            continue

        x = [r['test_ended_at_exp2_bytes'] for r in rows]
        y = [r['prng_throughput_gibibytes_per_second'] for r in rows]
        ax.scatter(x, y, marker=marker, c=color)
        x_ticks.update(range(min(x), max(x) + 1))

        for r in rows:
            xy = (r['test_ended_at_exp2_bytes'], r['prng_throughput_gibibytes_per_second'])
            ax.annotate(r['prng_name'], xy, xycoords='data', xytext=offset,
                        textcoords='offset points', arrowprops={'arrowstyle': '-'}, ha=ha)

    ax.set_xticks(sorted(x_ticks))

    ax.set_ylim(bottom=0)
    ax.yaxis.set_minor_locator(AutoMinorLocator())
    ax.grid(visible=True, which='both', axis='y')

    # https://stackoverflow.com/a/4066599
    fig.subplots_adjust(left=0.08, right=0.92, top=0.92, bottom=0.08)

    return fig


def main() -> None:
    """Parse the options, then show or save the plot."""
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('results_file', help='a prng-results.*.json file')
    parser.add_argument('-o', '--output',
                        help='save the plot to this file (for example, a .png) instead of '
                             'showing it')
    args = parser.parse_args()

    fig = plot_results(load_results(args.results_file))
    if args.output:
        fig.savefig(args.output)
    else:
        plt.show()


if __name__ == '__main__':
    main()
