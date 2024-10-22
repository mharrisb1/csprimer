from __future__ import annotations

import math
import random
import time
from typing import Any, Callable

import pandas as pd
import matplotlib.pyplot as plt


# no class
VecT = tuple[float, ...]


def vec_add(a: VecT, b: VecT) -> VecT:
    return tuple(a[n] + b[n] for n in range(len(a)))


def vec_sub(a: VecT, b: VecT) -> VecT:
    return tuple(a[n] - b[n] for n in range(len(a)))


def vec_scalar_mul(a: VecT, k: float) -> VecT:
    return tuple(n * k for n in a)


def vec_dot_product(a: VecT, b: VecT) -> VecT:
    return tuple(a[n] * b[n] for n in range(len(a)))


def vec_norm(a: VecT) -> float:
    return math.sqrt(sum(n**2 for n in a))


# class
class Vec:
    def __init__(self, *args: float) -> None:
        self._n = args

    def __repr__(self) -> str:
        return f"Vec({', '.join((str(n) for n in self._n))})"

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Vec):
            return False
        return all((n == other._n[ix] for ix, n in enumerate(self._n)))

    def __add__(self, other: Vec) -> Vec:
        return Vec(*(n + other._n[ix] for ix, n in enumerate(self._n)))

    def __sub__(self, other: Vec) -> Vec:
        return Vec(*(n - other._n[ix] for ix, n in enumerate(self._n)))

    def __mul__(self, other: Vec | float) -> Vec:
        if isinstance(other, Vec):
            return Vec(*(n * other._n[ix] for ix, n in enumerate(self._n)))
        else:
            return Vec(*(n * other for n in self._n))

    def __abs__(self) -> float:
        return math.sqrt(sum((n**2 for n in self._n)))


def profile(fn: Callable[..., Any], *args: Any, **kwargs: Any) -> float:
    t1 = time.time()
    fn(*args, **kwargs)
    t2 = time.time()
    return t2 - t1


def aos_to_soa(aos: list[dict[str, Any]]) -> dict[str, list[Any]]:
    soa: dict[str, list[Any]] = {k: [] for k in aos[0].keys()}
    for row in aos:
        for k, v in row.items():
            soa[k].append(v)
    return soa


if __name__ == "__main__":
    runs: list[dict[str, Any]] = []
    for n in range(1, 1001):

        # non-oo vectors
        a: VecT = tuple(random.randint(0, 9) for _ in range(n))
        b: VecT = tuple(random.randint(0, 9) for _ in range(n))

        # oo vectors
        v1 = Vec(*a)
        v2 = Vec(*b)

        for run in [
            # add
            (False, "add", vec_add, a, b),
            (True, "add", v1.__add__, v2),
            # sub
            (False, "sub", vec_sub, a, b),
            (True, "sub", v1.__sub__, v2),
            # scalar mul
            (False, "smul", vec_scalar_mul, a, 3),
            (True, "smul", v1.__mul__, 3),
            # dot
            (False, "dot", vec_dot_product, a, b),
            (True, "dot", v1.__mul__, v2),
            # norm
            (False, "norm", vec_norm, a),
            (True, "norm", v1.__abs__),
        ]:
            oo, op, fn, *args = run
            runs.append(dict(oo=oo, op=op, n=n, t=profile(fn, *args)))

    df = pd.DataFrame(data=aos_to_soa(runs))

    unique_ops = df["op"].unique()

    num_plots = len(unique_ops)
    num_cols = 2
    num_rows = math.ceil(num_plots / num_cols)

    fig, axes = plt.subplots(nrows=num_rows, ncols=num_cols, figsize=(15, 5 * num_rows))
    axes = axes.flatten()

    for idx, op in enumerate(unique_ops):
        ax = axes[idx]
        df_op = df[df["op"] == op]

        df_oo = df_op[df_op["oo"] == True].sort_values("n")
        df_non_oo = df_op[df_op["oo"] == False].sort_values("n")

        ax.plot(df_non_oo["n"], df_non_oo["t"], label="non-oo", color="blue")
        ax.plot(df_oo["n"], df_oo["t"], label="oo", color="orange")

        ax.set_title(f"Operation: {op}")
        ax.set_xlabel("n")
        ax.set_ylabel("t (seconds)")
        ax.legend()
        ax.grid(True)

    if num_plots < len(axes):
        for j in range(num_plots, len(axes)):
            fig.delaxes(axes[j])

    plt.tight_layout()
    plt.show()
