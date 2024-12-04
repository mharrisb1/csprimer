import sys
import numpy as np

if __name__ == "__main__":
    data = np.loadtxt(sys.argv[1])
    v1, v2 = np.sort(data[:, 0]), np.sort(data[:, 1])
    d = np.sum(np.abs(v1 - v2))
    assert d == np.float64(675)
    print("ok")
