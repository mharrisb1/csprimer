import math

VecT = tuple[float, ...]


def vec_add(a: VecT, b: VecT) -> VecT:
    return tuple(a[n] + b[n] for n in range(len(a)))


def vec_sub(a: VecT, b: VecT) -> VecT:
    return tuple(a[n] - b[n] for n in range(len(a)))


def vec_scalar_mul(a: VecT, k: float) -> VecT:
    return tuple(n * k for n in a)


def vec_norm(a: VecT) -> float:
    return math.sqrt(sum(n**2 for n in a))


if __name__ == "__main__":
    v1: VecT = (1, 2, 3, 4)
    v2: VecT = (1, 2, 1, 2)

    assert vec_add(v1, v2) == (2, 4, 4, 6)
    assert vec_sub(v1, v2) == (0, 0, 2, 2)
    assert vec_scalar_mul(v1, 3) == (3, 6, 9, 12)
    assert 5.47 < vec_norm(v1) < 5.48

    print("ok")
