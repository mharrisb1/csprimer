import csv
import math
import time

if __name__ == "__main__":
    t = time.perf_counter()

    sum_age, n_users = 0, 0
    with open("users.csv") as f:
        for line in csv.reader(f):
            sum_age += int(line[2])
            n_users += 1

    sum_amt, sq_sum_amt, n_pmts = 0, 0, 0
    with open("payments.csv") as f:
        for line in csv.reader(f):
            amount = int(line[0]) * 1e-2
            sum_amt += amount
            sq_sum_amt += amount * amount
            n_pmts += 1

    print(f"Data loading: {time.perf_counter() - t:.3f}s")

    t = time.perf_counter()
    assert abs((sum_age / n_users) - 59.626) < 0.01
    assert (
        abs(math.sqrt((sq_sum_amt / n_pmts) - (sum_amt / n_pmts) ** 2) - 288684.849)
        < 0.01
    )
    assert abs((sum_amt / n_pmts) - 499850.559) < 0.01
    print(f"Computation {time.perf_counter() - t:.6f}s")
