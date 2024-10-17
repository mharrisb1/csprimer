import math
from typing import Union


class Vec:
    """N-dimensional vector"""

    def __init__(self, *args: float) -> None:
        self._n = args

    def __repr__(self) -> str:
        return f"Vec({', '.join((str(n) for n in self._n))})"

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Vec):
            return False
        return all((n == other._n[ix] for ix, n in enumerate(self._n)))

    def __add__(self, other: "Vec") -> "Vec":
        return Vec(*(n + other._n[ix] for ix, n in enumerate(self._n)))

    def __sub__(self, other: "Vec") -> "Vec":
        return Vec(*(n - other._n[ix] for ix, n in enumerate(self._n)))

    def __mul__(self, other: Union["Vec", float]) -> "Vec":
        if isinstance(other, Vec):
            return Vec(*(n * other._n[ix] for ix, n in enumerate(self._n)))
        else:
            return Vec(*(n * other for n in self._n))

    def __abs__(self) -> float:
        return math.sqrt(sum((n**2 for n in self._n)))


if __name__ == "__main__":
    v1 = Vec(3, 2, 1)
    v2 = Vec(1, 1, 1)

    assert v1 + v2 == Vec(4, 3, 2)
    assert v1 - v2 == Vec(2, 1, 0)
    assert v1 * 2 == Vec(6, 4, 2)
    assert v1 * v2 == Vec(3, 2, 1)
    assert 3.74 < abs(v1) < 3.75
